#ifndef DRAWING3D_H
#define DRAWING3D_H

#include <QObject>
#include <QPointF>
#include <QString>

/**
 * Drawing3D - 3D drawing tools using isometric/orthographic projection
 * Provides 3D-like drawing on 2D e-paper display
 */
class Drawing3D : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString projectionType READ projectionType WRITE setProjectionType NOTIFY projectionTypeChanged)
    Q_PROPERTY(qreal isometricAngle READ isometricAngle WRITE setIsometricAngle NOTIFY isometricAngleChanged)

public:
    explicit Drawing3D(QObject *parent = nullptr);
    
    QString projectionType() const { return m_projectionType; }
    qreal isometricAngle() const { return m_isometricAngle; }
    
    void setProjectionType(const QString &type);
    void setIsometricAngle(qreal angle);
    
    // Isometric projection (30° standard)
    Q_INVOKABLE QPointF projectIsometric(qreal x, qreal y, qreal z) const;
    Q_INVOKABLE QList<QPointF> createIsometricBox(qreal width, qreal height, qreal depth, const QPointF &center) const;
    Q_INVOKABLE QList<QPointF> createIsometricCylinder(qreal radius, qreal height, const QPointF &center, int segments = 16) const;
    
    // Orthographic projection (front, top, side views)
    Q_INVOKABLE QPointF projectOrthographic(qreal x, qreal y, qreal z, const QString &view) const;
    Q_INVOKABLE QList<QPointF> createOrthographicBox(const QString &view, qreal width, qreal height, qreal depth, const QPointF &center) const;
    
    // Perspective helpers
    Q_INVOKABLE QPointF calculateVanishingPoint(const QPointF &p1, const QPointF &p2, qreal distance) const;
    Q_INVOKABLE QList<QPointF> createPerspectiveGrid(int divisions, const QRectF &bounds) const;

signals:
    void projectionTypeChanged();
    void isometricAngleChanged();

private:
    QString m_projectionType;  // "isometric", "orthographic", "perspective"
    qreal m_isometricAngle;    // Angle in degrees (typically 30°)
    
    QPointF rotate2D(const QPointF &point, qreal angle) const;
};

#endif // DRAWING3D_H

