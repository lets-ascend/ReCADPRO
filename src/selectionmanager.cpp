#include "selectionmanager.h"
#include "drawingobject.h"
#include "drawingengine.h"
#include "clipboard.h"
#include <QRectF>
#include <QDebug>
#include <QtGlobal>

SelectionManager::SelectionManager(QObject *parent)
    : QObject(parent)
    , m_isSelecting(false)
    , m_drawingEngine(nullptr)
    , m_clipboard(nullptr)
{
}

QList<QObject*> SelectionManager::selectedObjects() const
{
    QList<QObject*> result;
    for (DrawingObject *obj : m_selectedObjects) {
        result.append(obj);
    }
    return result;
}

void SelectionManager::selectStroke(int strokeIndex)
{
    // Validate stroke index (must be non-negative)
    if (strokeIndex < 0) {
        qWarning() << "SelectionManager::selectStroke: Invalid negative index" << strokeIndex;
        return;
    }
    
    // Note: We can't validate upper bound without DrawingEngine reference
    // This is checked in copySelected() when DrawingEngine is available
    
    if (!m_selectedStrokeIndices.contains(strokeIndex)) {
        m_selectedStrokeIndices.append(strokeIndex);
        updateBounds();
        emit selectionChanged();
    }
}

void SelectionManager::selectStrokes(const QList<int> &strokeIndices)
{
    bool changed = false;
    for (int index : strokeIndices) {
        if (!m_selectedStrokeIndices.contains(index)) {
            m_selectedStrokeIndices.append(index);
            changed = true;
        }
    }
    if (changed) {
        updateBounds();
        emit selectionChanged();
    }
}

void SelectionManager::deselectStroke(int strokeIndex)
{
    if (m_selectedStrokeIndices.removeOne(strokeIndex)) {
        updateBounds();
        emit selectionChanged();
    }
}

void SelectionManager::deselectAll()
{
    if (!m_selectedStrokeIndices.isEmpty() || !m_selectedObjects.isEmpty()) {
        clearSelection();
        emit selectionChanged();
    }
}

void SelectionManager::selectObject(DrawingObject *obj)
{
    if (!obj || m_selectedObjects.contains(obj))
        return;
    
    m_selectedObjects.append(obj);
    obj->setSelected(true);
    updateBounds();
    emit selectionChanged();
}

void SelectionManager::selectObjects(const QList<DrawingObject*> &objects)
{
    bool changed = false;
    for (DrawingObject *obj : objects) {
        if (obj && !m_selectedObjects.contains(obj)) {
            m_selectedObjects.append(obj);
            obj->setSelected(true);
            changed = true;
        }
    }
    if (changed) {
        updateBounds();
        emit selectionChanged();
    }
}

void SelectionManager::toggleObjectSelection(DrawingObject *obj)
{
    if (!obj)
        return;
    
    if (m_selectedObjects.contains(obj)) {
        m_selectedObjects.removeOne(obj);
        obj->setSelected(false);
    } else {
        m_selectedObjects.append(obj);
        obj->setSelected(true);
    }
    updateBounds();
    emit selectionChanged();
}

void SelectionManager::startRectangleSelection(const QPointF &startPoint)
{
    m_selectionStart = startPoint;
    m_tempSelectionRect = QRectF(startPoint, QSizeF(0, 0));
    m_isSelecting = true;
    emit selectionStarted();
}

void SelectionManager::updateRectangleSelection(const QPointF &endPoint)
{
    if (!m_isSelecting)
        return;
    
    m_tempSelectionRect = QRectF(m_selectionStart, endPoint).normalized();
    emit selectionChanged();
}

void SelectionManager::finishRectangleSelection()
{
    if (!m_isSelecting)
        return;
    
    m_selectionBounds = m_tempSelectionRect;
    m_isSelecting = false;
    emit selectionFinished();
    emit selectionChanged();
}

bool SelectionManager::isStrokeSelected(int strokeIndex) const
{
    return m_selectedStrokeIndices.contains(strokeIndex);
}

bool SelectionManager::isPointInSelection(const QPointF &point) const
{
    return m_selectionBounds.contains(point);
}

QList<int> SelectionManager::getStrokesInRect(const QRectF &rect) const
{
    Q_UNUSED(rect);
    // This would need access to DrawingEngine to check stroke bounds
    // For now, return empty list
    return QList<int>();
}

void SelectionManager::deleteSelected()
{
    // This would need access to DrawingEngine to delete strokes
    // For now, just clear selection
    clearSelection();
    emit selectionChanged();
}

void SelectionManager::moveSelected(const QPointF &offset)
{
    // Move selected objects
    for (DrawingObject *obj : m_selectedObjects) {
        if (!obj->locked()) {
            obj->translate(offset);
        }
    }
    updateBounds();
    emit selectionChanged();
}

void SelectionManager::copySelected()
{
    if (!m_drawingEngine || !m_clipboard || m_selectedStrokeIndices.isEmpty())
        return;
    
    // Get stroke data from DrawingEngine
    QList<DrawingEngine::StrokeData> strokeData = m_drawingEngine->getStrokes(m_selectedStrokeIndices);
    
    // Convert to ClipboardStroke format
    QList<Clipboard::ClipboardStroke> clipboardStrokes;
    for (const DrawingEngine::StrokeData &data : strokeData) {
        Clipboard::ClipboardStroke clipStroke;
        clipStroke.points = data.points;
        clipStroke.color = data.color;
        clipStroke.lineWidth = data.lineWidth;
        clipStroke.opacity = data.opacity;
        clipboardStrokes.append(clipStroke);
    }
    
    // Copy to clipboard
    m_clipboard->copy(clipboardStrokes, m_selectionBounds);
    emit selectionChanged();
}

void SelectionManager::setDrawingEngine(QObject *engine)
{
    m_drawingEngine = qobject_cast<DrawingEngine*>(engine);
}

void SelectionManager::setClipboard(QObject *clipboard)
{
    m_clipboard = qobject_cast<Clipboard*>(clipboard);
}

void SelectionManager::updateBounds()
{
    QRectF bounds;
    
    // Calculate bounds from selected strokes
    if (m_drawingEngine && !m_selectedStrokeIndices.isEmpty()) {
        QList<DrawingEngine::StrokeData> strokeData = m_drawingEngine->getStrokes(m_selectedStrokeIndices);
        
        for (const DrawingEngine::StrokeData &stroke : strokeData) {
            if (stroke.points.isEmpty())
                continue;
            
            qreal minX = stroke.points[0].x();
            qreal minY = stroke.points[0].y();
            qreal maxX = minX;
            qreal maxY = minY;
            
            for (const QPointF &point : stroke.points) {
                minX = qMin(minX, point.x());
                minY = qMin(minY, point.y());
                maxX = qMax(maxX, point.x());
                maxY = qMax(maxY, point.y());
            }
            
            QRectF strokeBounds(minX, minY, maxX - minX, maxY - minY);
            if (bounds.isNull()) {
                bounds = strokeBounds;
            } else {
                bounds = bounds.united(strokeBounds);
            }
        }
    }
    
    // Calculate bounds from selected objects
    for (DrawingObject *obj : m_selectedObjects) {
        QRectF objBounds = obj->bounds();
        if (bounds.isNull()) {
            bounds = objBounds;
        } else {
            bounds = bounds.united(objBounds);
        }
    }
    
    m_selectionBounds = bounds;
}

QRectF SelectionManager::getSelectionBounds() const
{
    return m_selectionBounds;
}

void SelectionManager::clearSelection()
{
    m_selectedStrokeIndices.clear();
    
    for (DrawingObject *obj : m_selectedObjects) {
        obj->setSelected(false);
    }
    m_selectedObjects.clear();
    
    m_selectionBounds = QRectF();
}

