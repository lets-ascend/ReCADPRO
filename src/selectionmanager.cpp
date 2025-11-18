#include "selectionmanager.h"
#include "drawingobject.h"
#include <QRectF>
#include <QDebug>

SelectionManager::SelectionManager(QObject *parent)
    : QObject(parent)
    , m_isSelecting(false)
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
    // This would trigger clipboard copy
    emit selectionChanged();
}

void SelectionManager::updateBounds()
{
    QRectF bounds;
    
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

void SelectionManager::clearSelection()
{
    m_selectedStrokeIndices.clear();
    
    for (DrawingObject *obj : m_selectedObjects) {
        obj->setSelected(false);
    }
    m_selectedObjects.clear();
    
    m_selectionBounds = QRectF();
}

