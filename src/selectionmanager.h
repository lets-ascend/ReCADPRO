#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include <QObject>
#include <QList>
#include <QRectF>
#include <QPointF>

class DrawingObject;

/**
 * SelectionManager - Manages selection of strokes and objects
 * Handles selection UI and operations
 */
class SelectionManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY selectionChanged)
    Q_PROPERTY(QRectF selectionBounds READ selectionBounds NOTIFY selectionChanged)
    Q_PROPERTY(QList<QObject*> selectedObjects READ selectedObjects NOTIFY selectionChanged)

public:
    explicit SelectionManager(QObject *parent = nullptr);
    
    bool hasSelection() const { return !m_selectedStrokeIndices.isEmpty() || !m_selectedObjects.isEmpty(); }
    QRectF selectionBounds() const { return m_selectionBounds; }
    QList<QObject*> selectedObjects() const;
    
    // Stroke selection
    Q_INVOKABLE void selectStroke(int strokeIndex);
    Q_INVOKABLE void selectStrokes(const QList<int> &strokeIndices);
    Q_INVOKABLE void deselectStroke(int strokeIndex);
    Q_INVOKABLE void deselectAll();
    Q_INVOKABLE QList<int> getSelectedStrokes() const { return m_selectedStrokeIndices; }
    
    // Object selection
    Q_INVOKABLE void selectObject(DrawingObject *obj);
    Q_INVOKABLE void selectObjects(const QList<DrawingObject*> &objects);
    Q_INVOKABLE void toggleObjectSelection(DrawingObject *obj);
    
    // Rectangle selection (lasso)
    Q_INVOKABLE void startRectangleSelection(const QPointF &startPoint);
    Q_INVOKABLE void updateRectangleSelection(const QPointF &endPoint);
    Q_INVOKABLE void finishRectangleSelection();
    
    // Hit testing
    Q_INVOKABLE bool isStrokeSelected(int strokeIndex) const;
    Q_INVOKABLE bool isPointInSelection(const QPointF &point) const;
    Q_INVOKABLE QList<int> getStrokesInRect(const QRectF &rect) const;
    
    // Selection operations
    Q_INVOKABLE void deleteSelected();
    Q_INVOKABLE void moveSelected(const QPointF &offset);
    Q_INVOKABLE void copySelected();

signals:
    void selectionChanged();
    void selectionStarted();
    void selectionFinished();

private:
    QList<int> m_selectedStrokeIndices;
    QList<DrawingObject*> m_selectedObjects;
    QRectF m_selectionBounds;
    QRectF m_tempSelectionRect;
    bool m_isSelecting;
    QPointF m_selectionStart;
    
    void updateBounds();
    void clearSelection();
};

#endif // SELECTIONMANAGER_H

