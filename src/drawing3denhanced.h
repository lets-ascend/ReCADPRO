#ifndef DRAWING3DENHANCED_H
#define DRAWING3DENHANCED_H

#include <QObject>
#include <QPointF>
#include <QString>
#include <QList>
#include <QVector3D>

/**
 * Drawing3DEnhanced - Enhanced 3D tools
 * More primitives, rotation controls, multiple views, projection preview
 */
class Drawing3DEnhanced : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal rotationX READ rotationX WRITE setRotationX NOTIFY rotationChanged)
    Q_PROPERTY(qreal rotationY READ rotationY WRITE setRotationY NOTIFY rotationChanged)
    Q_PROPERTY(qreal rotationZ READ rotationZ WRITE setRotationZ NOTIFY rotationChanged)
    Q_PROPERTY(QString currentView READ currentView WRITE setCurrentView NOTIFY currentViewChanged)

public:
    explicit Drawing3DEnhanced(QObject *parent = nullptr);
    
    qreal rotationX() const { return m_rotationX; }
    qreal rotationY() const { return m_rotationY; }
    qreal rotationZ() const { return m_rotationZ; }
    QString currentView() const { return m_currentView; }
    
    void setRotationX(qreal angle);
    void setRotationY(qreal angle);
    void setRotationZ(qreal angle);
    void setCurrentView(const QString &view);
    
    // 3D primitives
    Q_INVOKABLE QList<QPointF> createSphere(qreal radius, const QPointF &center, int segments = 16) const;
    Q_INVOKABLE QList<QPointF> createCone(qreal radius, qreal height, const QPointF &center, int segments = 16) const;
    Q_INVOKABLE QList<QPointF> createPyramid(qreal baseSize, qreal height, const QPointF &center, int sides = 4) const;
    Q_INVOKABLE QList<QPointF> createTorus(qreal majorRadius, qreal minorRadius, const QPointF &center, int segments = 16) const;
    
    // 3D rotation
    Q_INVOKABLE QVector3D rotate3D(const QVector3D &point, qreal rotX, qreal rotY, qreal rotZ) const;
    Q_INVOKABLE void setRotation(qreal rotX, qreal rotY, qreal rotZ);
    Q_INVOKABLE void resetRotation();
    
    // Multiple isometric views
    Q_INVOKABLE QList<QPointF> createIsometricView(const QString &viewName, const QList<QVector3D> &points3D) const;
    Q_INVOKABLE QStringList getAvailableViews() const;
    
    // 3D to 2D projection preview
    Q_INVOKABLE QList<QPointF> project3DTo2D(const QList<QVector3D> &points3D, const QString &projectionType) const;
    Q_INVOKABLE QPointF projectPoint3D(const QVector3D &point3D, const QString &projectionType) const;

signals:
    void rotationChanged();
    void currentViewChanged();
    void primitiveCreated(const QString &type, const QList<QPointF> &points);

private:
    qreal m_rotationX;
    qreal m_rotationY;
    qreal m_rotationZ;
    QString m_currentView; // "front", "top", "side", "isometric", "custom"
    
    QVector3D rotateX(const QVector3D &point, qreal angle) const;
    QVector3D rotateY(const QVector3D &point, qreal angle) const;
    QVector3D rotateZ(const QVector3D &point, qreal angle) const;
    QPointF projectIsometric(const QVector3D &point) const;
    QPointF projectOrthographic(const QVector3D &point, const QString &view) const;
};

#endif // DRAWING3DENHANCED_H

