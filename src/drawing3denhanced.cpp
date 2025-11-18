#include "drawing3denhanced.h"
#include <QtMath>
#include <QVector3D>

Drawing3DEnhanced::Drawing3DEnhanced(QObject *parent)
    : QObject(parent)
    , m_rotationX(0.0)
    , m_rotationY(0.0)
    , m_rotationZ(0.0)
    , m_currentView("isometric")
{
}

void Drawing3DEnhanced::setRotationX(qreal angle)
{
    if (qFuzzyCompare(m_rotationX, angle))
        return;
    
    m_rotationX = angle;
    emit rotationChanged();
}

void Drawing3DEnhanced::setRotationY(qreal angle)
{
    if (qFuzzyCompare(m_rotationY, angle))
        return;
    
    m_rotationY = angle;
    emit rotationChanged();
}

void Drawing3DEnhanced::setRotationZ(qreal angle)
{
    if (qFuzzyCompare(m_rotationZ, angle))
        return;
    
    m_rotationZ = angle;
    emit rotationChanged();
}

void Drawing3DEnhanced::setCurrentView(const QString &view)
{
    if (m_currentView == view)
        return;
    
    m_currentView = view;
    emit currentViewChanged();
}

QList<QPointF> Drawing3DEnhanced::createSphere(qreal radius, const QPointF &center, int segments) const
{
    QList<QPointF> points;
    
    // Create sphere outline (simplified - shows equator and meridians)
    for (int i = 0; i <= segments; ++i) {
        qreal theta = 2.0 * M_PI * i / segments;
        QVector3D point3D(radius * qCos(theta), 0, radius * qSin(theta));
        QVector3D rotated = rotate3D(point3D, m_rotationX, m_rotationY, m_rotationZ);
        QPointF projected = projectIsometric(rotated);
        points.append(projected + center);
    }
    
    return points;
}

QList<QPointF> Drawing3DEnhanced::createCone(qreal radius, qreal height, const QPointF &center, int segments) const
{
    QList<QPointF> points;
    
    // Base circle
    for (int i = 0; i <= segments; ++i) {
        qreal theta = 2.0 * M_PI * i / segments;
        QVector3D point3D(radius * qCos(theta), 0, radius * qSin(theta));
        QVector3D rotated = rotate3D(point3D, m_rotationX, m_rotationY, m_rotationZ);
        QPointF projected = projectIsometric(rotated);
        points.append(projected + center);
    }
    
    // Apex
    QVector3D apex(0, height, 0);
    QVector3D rotatedApex = rotate3D(apex, m_rotationX, m_rotationY, m_rotationZ);
    QPointF projectedApex = projectIsometric(rotatedApex);
    points.append(projectedApex + center);
    
    return points;
}

QList<QPointF> Drawing3DEnhanced::createPyramid(qreal baseSize, qreal height, const QPointF &center, int sides) const
{
    QList<QPointF> points;
    
    // Base polygon
    for (int i = 0; i < sides; ++i) {
        qreal angle = 2.0 * M_PI * i / sides;
        QVector3D point3D(baseSize * qCos(angle), 0, baseSize * qSin(angle));
        QVector3D rotated = rotate3D(point3D, m_rotationX, m_rotationY, m_rotationZ);
        QPointF projected = projectIsometric(rotated);
        points.append(projected + center);
    }
    
    // Apex
    QVector3D apex(0, height, 0);
    QVector3D rotatedApex = rotate3D(apex, m_rotationX, m_rotationY, m_rotationZ);
    QPointF projectedApex = projectIsometric(rotatedApex);
    points.append(projectedApex + center);
    
    return points;
}

QList<QPointF> Drawing3DEnhanced::createTorus(qreal majorRadius, qreal minorRadius, const QPointF &center, int segments) const
{
    QList<QPointF> points;
    
    // Simplified torus - shows main circle
    for (int i = 0; i <= segments; ++i) {
        qreal theta = 2.0 * M_PI * i / segments;
        QVector3D point3D(majorRadius * qCos(theta), 0, majorRadius * qSin(theta));
        QVector3D rotated = rotate3D(point3D, m_rotationX, m_rotationY, m_rotationZ);
        QPointF projected = projectIsometric(rotated);
        points.append(projected + center);
    }
    
    return points;
}

QVector3D Drawing3DEnhanced::rotate3D(const QVector3D &point, qreal rotX, qreal rotY, qreal rotZ) const
{
    QVector3D rotated = point;
    rotated = rotateX(rotated, rotX);
    rotated = rotateY(rotated, rotY);
    rotated = rotateZ(rotated, rotZ);
    return rotated;
}

void Drawing3DEnhanced::setRotation(qreal rotX, qreal rotY, qreal rotZ)
{
    setRotationX(rotX);
    setRotationY(rotY);
    setRotationZ(rotZ);
}

void Drawing3DEnhanced::resetRotation()
{
    setRotation(0.0, 0.0, 0.0);
}

QList<QPointF> Drawing3DEnhanced::createIsometricView(const QString &viewName, const QList<QVector3D> &points3D) const
{
    QList<QPointF> projected;
    
    for (const QVector3D &point3D : points3D) {
        QVector3D rotated = rotate3D(point3D, m_rotationX, m_rotationY, m_rotationZ);
        QPointF proj = projectIsometric(rotated);
        projected.append(proj);
    }
    
    return projected;
}

QStringList Drawing3DEnhanced::getAvailableViews() const
{
    return QStringList() << "front" << "top" << "side" << "isometric" << "custom";
}

QList<QPointF> Drawing3DEnhanced::project3DTo2D(const QList<QVector3D> &points3D, const QString &projectionType) const
{
    QList<QPointF> projected;
    
    for (const QVector3D &point3D : points3D) {
        QVector3D rotated = rotate3D(point3D, m_rotationX, m_rotationY, m_rotationZ);
        QPointF proj = projectPoint3D(rotated, projectionType);
        projected.append(proj);
    }
    
    return projected;
}

QPointF Drawing3DEnhanced::projectPoint3D(const QVector3D &point3D, const QString &projectionType) const
{
    if (projectionType == "isometric") {
        return projectIsometric(point3D);
    } else {
        return projectOrthographic(point3D, projectionType);
    }
}

QVector3D Drawing3DEnhanced::rotateX(const QVector3D &point, qreal angle) const
{
    qreal rad = angle * M_PI / 180.0;
    qreal c = qCos(rad);
    qreal s = qSin(rad);
    return QVector3D(point.x(),
                    point.y() * c - point.z() * s,
                    point.y() * s + point.z() * c);
}

QVector3D Drawing3DEnhanced::rotateY(const QVector3D &point, qreal angle) const
{
    qreal rad = angle * M_PI / 180.0;
    qreal c = qCos(rad);
    qreal s = qSin(rad);
    return QVector3D(point.x() * c + point.z() * s,
                    point.y(),
                    -point.x() * s + point.z() * c);
}

QVector3D Drawing3DEnhanced::rotateZ(const QVector3D &point, qreal angle) const
{
    qreal rad = angle * M_PI / 180.0;
    qreal c = qCos(rad);
    qreal s = qSin(rad);
    return QVector3D(point.x() * c - point.y() * s,
                    point.x() * s + point.y() * c,
                    point.z());
}

QPointF Drawing3DEnhanced::projectIsometric(const QVector3D &point) const
{
    // Isometric projection (30° standard)
    qreal angle = 30.0 * M_PI / 180.0;
    qreal x = point.x() * qCos(angle) - point.z() * qCos(angle);
    qreal y = point.y() - point.x() * qSin(angle) - point.z() * qSin(angle);
    return QPointF(x, y);
}

QPointF Drawing3DEnhanced::projectOrthographic(const QVector3D &point, const QString &view) const
{
    if (view == "front") {
        return QPointF(point.x(), point.y());
    } else if (view == "top") {
        return QPointF(point.x(), point.z());
    } else if (view == "side") {
        return QPointF(point.z(), point.y());
    }
    return QPointF(point.x(), point.y());
}

