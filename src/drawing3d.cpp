#include "drawing3d.h"
#include <QtMath>
#include <QRectF>

Drawing3D::Drawing3D(QObject *parent)
    : QObject(parent)
    , m_projectionType("isometric")
    , m_isometricAngle(30.0)
{
}

void Drawing3D::setProjectionType(const QString &type)
{
    if (m_projectionType == type)
        return;
    
    m_projectionType = type;
    emit projectionTypeChanged();
}

void Drawing3D::setIsometricAngle(qreal angle)
{
    if (qFuzzyCompare(m_isometricAngle, angle))
        return;
    
    m_isometricAngle = angle;
    emit projectionTypeChanged();
}

QPointF Drawing3D::projectIsometric(qreal x, qreal y, qreal z) const
{
    // Isometric projection: 30° rotation
    qreal angleRad = m_isometricAngle * M_PI / 180.0;
    qreal cosAngle = qCos(angleRad);
    qreal sinAngle = qSin(angleRad);
    
    // Project 3D to 2D isometric
    qreal isoX = (x - z) * cosAngle;
    qreal isoY = y + (x + z) * sinAngle;
    
    return QPointF(isoX, isoY);
}

QList<QPointF> Drawing3D::createIsometricBox(qreal width, qreal height, qreal depth, const QPointF &center) const
{
    QList<QPointF> points;
    
    // Define 8 corners of the box
    qreal w = width / 2.0;
    qreal h = height / 2.0;
    qreal d = depth / 2.0;
    
    QList<QPointF> corners3D = {
        QPointF(-w, -h, -d),  // 0: bottom-left-back
        QPointF(w, -h, -d),   // 1: bottom-right-back
        QPointF(w, h, -d),    // 2: top-right-back
        QPointF(-w, h, -d),   // 3: top-left-back
        QPointF(-w, -h, d),   // 4: bottom-left-front
        QPointF(w, -h, d),    // 5: bottom-right-front
        QPointF(w, h, d),     // 6: top-right-front
        QPointF(-w, h, d)     // 7: top-left-front
    };
    
    // Project to isometric
    for (const QPointF &corner : corners3D) {
        QPointF projected = projectIsometric(corner.x(), corner.y(), corner.z());
        points.append(projected + center);
    }
    
    return points;
}

QList<QPointF> Drawing3D::createIsometricCylinder(qreal radius, qreal height, const QPointF &center, int segments) const
{
    QList<QPointF> points;
    
    // Create top and bottom circles
    for (int i = 0; i < segments; ++i) {
        qreal angle = 2.0 * M_PI * i / segments;
        qreal x = radius * qCos(angle);
        qreal z = radius * qSin(angle);
        
        // Bottom circle
        QPointF bottom = projectIsometric(x, -height / 2.0, z);
        points.append(bottom + center);
        
        // Top circle
        QPointF top = projectIsometric(x, height / 2.0, z);
        points.append(top + center);
    }
    
    return points;
}

QPointF Drawing3D::projectOrthographic(qreal x, qreal y, qreal z, const QString &view) const
{
    if (view == "front" || view == "front view") {
        return QPointF(x, y);
    } else if (view == "top" || view == "top view") {
        return QPointF(x, -z);
    } else if (view == "side" || view == "side view" || view == "right") {
        return QPointF(z, y);
    } else if (view == "left") {
        return QPointF(-z, y);
    }
    
    return QPointF(x, y);
}

QList<QPointF> Drawing3D::createOrthographicBox(const QString &view, qreal width, qreal height, qreal depth, const QPointF &center) const
{
    QList<QPointF> points;
    
    qreal w = width / 2.0;
    qreal h = height / 2.0;
    qreal d = depth / 2.0;
    
    QList<QPointF> corners3D = {
        QPointF(-w, -h, -d),
        QPointF(w, -h, -d),
        QPointF(w, h, -d),
        QPointF(-w, h, -d),
        QPointF(-w, -h, d),
        QPointF(w, -h, d),
        QPointF(w, h, d),
        QPointF(-w, h, d)
    };
    
    for (const QPointF &corner : corners3D) {
        QPointF projected = projectOrthographic(corner.x(), corner.y(), corner.z(), view);
        points.append(projected + center);
    }
    
    return points;
}

QPointF Drawing3D::calculateVanishingPoint(const QPointF &p1, const QPointF &p2, qreal distance) const
{
    // Calculate vanishing point for perspective projection
    QPointF direction = p2 - p1;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    
    if (length == 0)
        return p1;
    
    direction /= length;
    return p1 + direction * distance;
}

QList<QPointF> Drawing3D::createPerspectiveGrid(int divisions, const QRectF &bounds) const
{
    QList<QPointF> gridPoints;
    
    // Create perspective grid points
    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            qreal x = bounds.left() + (bounds.width() * i / divisions);
            qreal y = bounds.top() + (bounds.height() * j / divisions);
            gridPoints.append(QPointF(x, y));
        }
    }
    
    return gridPoints;
}

QPointF Drawing3D::rotate2D(const QPointF &point, qreal angle) const
{
    qreal angleRad = angle * M_PI / 180.0;
    qreal cosAngle = qCos(angleRad);
    qreal sinAngle = qSin(angleRad);
    
    return QPointF(
        point.x() * cosAngle - point.y() * sinAngle,
        point.x() * sinAngle + point.y() * cosAngle
    );
}

