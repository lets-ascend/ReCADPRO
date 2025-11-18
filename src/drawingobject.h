#ifndef DRAWINGOBJECT_H
#define DRAWINGOBJECT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QRectF>
#include <QPointF>
#include <QColor>

/**
 * DrawingObject - Represents a group of strokes/shapes as a single object
 * Supports locking, grouping, and transformation
 */
class DrawingObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool locked READ locked WRITE setLocked NOTIFY lockedChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QRectF bounds READ bounds NOTIFY boundsChanged)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qreal scaleX READ scaleX WRITE setScaleX NOTIFY scaleChanged)
    Q_PROPERTY(qreal scaleY READ scaleY WRITE setScaleY NOTIFY scaleChanged)

public:
    explicit DrawingObject(QObject *parent = nullptr);
    
    // Properties
    QString id() const { return m_id; }
    QString name() const { return m_name; }
    bool locked() const { return m_locked; }
    bool visible() const { return m_visible; }
    bool selected() const { return m_selected; }
    QRectF bounds() const { return m_bounds; }
    qreal rotation() const { return m_rotation; }
    QPointF position() const { return m_position; }
    qreal scaleX() const { return m_scaleX; }
    qreal scaleY() const { return m_scaleY; }
    
    void setName(const QString &name);
    void setLocked(bool locked);
    void setVisible(bool visible);
    void setSelected(bool selected);
    void setRotation(qreal rotation);
    void setPosition(const QPointF &position);
    void setScaleX(qreal scaleX);
    void setScaleY(qreal scaleY);
    
    // Object management
    Q_INVOKABLE void addStroke(int strokeIndex);
    Q_INVOKABLE void removeStroke(int strokeIndex);
    Q_INVOKABLE QList<int> getStrokeIndices() const { return m_strokeIndices; }
    Q_INVOKABLE void updateBounds();
    
    // Transformation
    Q_INVOKABLE void translate(const QPointF &offset);
    Q_INVOKABLE void rotate(qreal angle);
    Q_INVOKABLE void scale(qreal factor);
    Q_INVOKABLE void resetTransform();
    
    // Hit testing
    Q_INVOKABLE bool containsPoint(const QPointF &point) const;
    Q_INVOKABLE bool intersectsRect(const QRectF &rect) const;

signals:
    void nameChanged();
    void lockedChanged();
    void visibleChanged();
    void selectedChanged();
    void boundsChanged();
    void rotationChanged();
    void positionChanged();
    void scaleChanged();
    void transformChanged();

private:
    QString m_id;
    QString m_name;
    bool m_locked;
    bool m_visible;
    bool m_selected;
    QRectF m_bounds;
    qreal m_rotation;
    QPointF m_position;
    qreal m_scaleX;
    qreal m_scaleY;
    
    QList<int> m_strokeIndices;  // Indices of strokes in parent drawing engine
    
    QString generateId() const;
};

#endif // DRAWINGOBJECT_H

