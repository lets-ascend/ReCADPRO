#ifndef SNAPTOOLS_H
#define SNAPTOOLS_H

#include <QObject>
#include <QPointF>
#include <QList>
#include <QColor>

class DrawingEngine;
class ShapeTools;

/**
 * SnapTools - Object snap system for precision drafting
 * Snaps to endpoints, midpoints, intersections, centers, perpendicular, tangent points
 */
class SnapTools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool snapEnabled READ snapEnabled WRITE setSnapEnabled NOTIFY snapEnabledChanged)
    Q_PROPERTY(bool snapToEndpoint READ snapToEndpoint WRITE setSnapToEndpoint NOTIFY snapToEndpointChanged)
    Q_PROPERTY(bool snapToMidpoint READ snapToMidpoint WRITE setSnapToMidpoint NOTIFY snapToMidpointChanged)
    Q_PROPERTY(bool snapToIntersection READ snapToIntersection WRITE setSnapToIntersection NOTIFY snapToIntersectionChanged)
    Q_PROPERTY(bool snapToCenter READ snapToCenter WRITE setSnapToCenter NOTIFY snapToCenterChanged)
    Q_PROPERTY(bool snapToPerpendicular READ snapToPerpendicular WRITE setSnapToPerpendicular NOTIFY snapToPerpendicularChanged)
    Q_PROPERTY(bool snapToTangent READ snapToTangent WRITE setSnapToTangent NOTIFY snapToTangentChanged)
    Q_PROPERTY(qreal snapTolerance READ snapTolerance WRITE setSnapTolerance NOTIFY snapToleranceChanged)
    Q_PROPERTY(QPointF lastSnapPoint READ lastSnapPoint NOTIFY snapPointFound)

public:
    explicit SnapTools(QObject *parent = nullptr);
    
    bool snapEnabled() const { return m_snapEnabled; }
    bool snapToEndpoint() const { return m_snapToEndpoint; }
    bool snapToMidpoint() const { return m_snapToMidpoint; }
    bool snapToIntersection() const { return m_snapToIntersection; }
    bool snapToCenter() const { return m_snapToCenter; }
    bool snapToPerpendicular() const { return m_snapToPerpendicular; }
    bool snapToTangent() const { return m_snapToTangent; }
    qreal snapTolerance() const { return m_snapTolerance; }
    QPointF lastSnapPoint() const { return m_lastSnapPoint; }
    
    void setSnapEnabled(bool enabled);
    void setSnapToEndpoint(bool enabled);
    void setSnapToMidpoint(bool enabled);
    void setSnapToIntersection(bool enabled);
    void setSnapToCenter(bool enabled);
    void setSnapToPerpendicular(bool enabled);
    void setSnapToTangent(bool enabled);
    void setSnapTolerance(qreal tolerance);
    
    // Set references to drawing data sources
    void setDrawingEngine(DrawingEngine *engine);
    void setShapeTools(ShapeTools *shapes);
    
    // Main snap function - finds nearest snap point
    Q_INVOKABLE QPointF snapPoint(const QPointF &point) const;
    
    // Individual snap type functions
    Q_INVOKABLE QPointF snapToNearestEndpoint(const QPointF &point) const;
    Q_INVOKABLE QPointF snapToNearestMidpoint(const QPointF &point) const;
    Q_INVOKABLE QPointF snapToNearestIntersection(const QPointF &point) const;
    Q_INVOKABLE QPointF snapToNearestCenter(const QPointF &point) const;
    Q_INVOKABLE QPointF snapToNearestPerpendicular(const QPointF &point, const QPointF &fromPoint) const;
    Q_INVOKABLE QPointF snapToNearestTangent(const QPointF &point, const QPointF &fromPoint) const;
    
    // Get snap point info
    struct SnapInfo {
        QPointF point;
        QString type; // "endpoint", "midpoint", "intersection", "center", "perpendicular", "tangent"
        QColor color; // Visual indicator color
    };
    
    Q_INVOKABLE SnapInfo getSnapInfo(const QPointF &point) const;

signals:
    void snapEnabledChanged();
    void snapToEndpointChanged();
    void snapToMidpointChanged();
    void snapToIntersectionChanged();
    void snapToCenterChanged();
    void snapToPerpendicularChanged();
    void snapToTangentChanged();
    void snapToleranceChanged();
    void snapPointFound(const QPointF &point, const QString &type);

private:
    bool m_snapEnabled;
    bool m_snapToEndpoint;
    bool m_snapToMidpoint;
    bool m_snapToIntersection;
    bool m_snapToCenter;
    bool m_snapToPerpendicular;
    bool m_snapToTangent;
    qreal m_snapTolerance; // Pixel distance for snap detection
    
    QPointF m_lastSnapPoint;
    QString m_lastSnapType;
    
    DrawingEngine *m_drawingEngine;
    ShapeTools *m_shapeTools;
    
    // Helper functions
    qreal distance(const QPointF &p1, const QPointF &p2) const;
    QPointF findNearestPoint(const QPointF &point, const QList<QPointF> &candidates) const;
    QList<QPointF> getAllEndpoints() const;
    QList<QPointF> getAllMidpoints() const;
    QList<QPointF> getAllIntersections() const;
    QList<QPointF> getAllCenters() const;
    QPointF findPerpendicularPoint(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd) const;
    QPointF findTangentPoint(const QPointF &point, const QPointF &center, qreal radius) const;
    QList<QPointF> getLineSegments() const; // Get all line segments from strokes/shapes
    QList<QPointF> getCircleArcs() const; // Get all circle/arc centers
};

#endif // SNAPTOOLS_H

