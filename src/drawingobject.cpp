#include "drawingobject.h"
#include <QUuid>
#include <QtMath>

DrawingObject::DrawingObject(QObject *parent)
    : QObject(parent)
    , m_id(generateId())
    , m_name("Object")
    , m_locked(false)
    , m_visible(true)
    , m_selected(false)
    , m_rotation(0.0)
    , m_position(0, 0)
    , m_scaleX(1.0)
    , m_scaleY(1.0)
{
}

QString DrawingObject::generateId() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void DrawingObject::setName(const QString &name)
{
    if (m_name == name)
        return;
    
    m_name = name;
    emit nameChanged();
}

void DrawingObject::setLocked(bool locked)
{
    if (m_locked == locked)
        return;
    
    m_locked = locked;
    
    // When locked, deselect
    if (locked && m_selected) {
        m_selected = false;
        emit selectedChanged();
    }
    
    emit lockedChanged();
}

void DrawingObject::setVisible(bool visible)
{
    if (m_visible == visible)
        return;
    
    m_visible = visible;
    emit visibleChanged();
}

void DrawingObject::setSelected(bool selected)
{
    if (m_selected == selected)
        return;
    
    // Can't select locked objects
    if (selected && m_locked)
        return;
    
    m_selected = selected;
    emit selectedChanged();
}

void DrawingObject::setRotation(qreal rotation)
{
    if (m_locked)
        return;
    
    if (qFuzzyCompare(m_rotation, rotation))
        return;
    
    m_rotation = rotation;
    emit rotationChanged();
    emit transformChanged();
}

void DrawingObject::setPosition(const QPointF &position)
{
    if (m_locked)
        return;
    
    if (m_position == position)
        return;
    
    m_position = position;
    emit positionChanged();
    emit transformChanged();
}

void DrawingObject::setScaleX(qreal scaleX)
{
    if (m_locked)
        return;
    
    if (qFuzzyCompare(m_scaleX, scaleX))
        return;
    
    m_scaleX = scaleX;
    emit scaleChanged();
    emit transformChanged();
}

void DrawingObject::setScaleY(qreal scaleY)
{
    if (m_locked)
        return;
    
    if (qFuzzyCompare(m_scaleY, scaleY))
        return;
    
    m_scaleY = scaleY;
    emit scaleChanged();
    emit transformChanged();
}

void DrawingObject::addStroke(int strokeIndex)
{
    if (!m_strokeIndices.contains(strokeIndex)) {
        m_strokeIndices.append(strokeIndex);
        updateBounds();
    }
}

void DrawingObject::removeStroke(int strokeIndex)
{
    if (m_strokeIndices.removeOne(strokeIndex)) {
        updateBounds();
    }
}

void DrawingObject::updateBounds()
{
    // Bounds will be calculated by parent drawing engine
    // based on stroke indices
    emit boundsChanged();
}

void DrawingObject::translate(const QPointF &offset)
{
    if (m_locked)
        return;
    
    setPosition(m_position + offset);
}

void DrawingObject::rotate(qreal angle)
{
    if (m_locked)
        return;
    
    setRotation(m_rotation + angle);
}

void DrawingObject::scale(qreal factor)
{
    if (m_locked)
        return;
    
    setScaleX(m_scaleX * factor);
    setScaleY(m_scaleY * factor);
}

void DrawingObject::resetTransform()
{
    if (m_locked)
        return;
    
    m_rotation = 0.0;
    m_position = QPointF(0, 0);
    m_scaleX = 1.0;
    m_scaleY = 1.0;
    
    emit rotationChanged();
    emit positionChanged();
    emit scaleChanged();
    emit transformChanged();
}

bool DrawingObject::containsPoint(const QPointF &point) const
{
    return m_bounds.contains(point);
}

bool DrawingObject::intersectsRect(const QRectF &rect) const
{
    return m_bounds.intersects(rect);
}

