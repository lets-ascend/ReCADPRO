#include "snaptools.h"
#include "drawingengine.h"
#include "shapetools.h"
#include <QtMath>
#include <QDebug>

SnapTools::SnapTools(QObject *parent)
    : QObject(parent)
    , m_snapEnabled(true)
    , m_snapToEndpoint(true)
    , m_snapToMidpoint(true)
    , m_snapToIntersection(true)
    , m_snapToCenter(true)
    , m_snapToPerpendicular(false)
    , m_snapToTangent(false)
    , m_snapTolerance(10.0) // 10 pixels default
    , m_drawingEngine(nullptr)
    , m_shapeTools(nullptr)
{
}

void SnapTools::setSnapEnabled(bool enabled)
{
    if (m_snapEnabled == enabled)
        return;
    
    m_snapEnabled = enabled;
    emit snapEnabledChanged();
}

void SnapTools::setSnapToEndpoint(bool enabled)
{
    if (m_snapToEndpoint == enabled)
        return;
    
    m_snapToEndpoint = enabled;
    emit snapToEndpointChanged();
}

void SnapTools::setSnapToMidpoint(bool enabled)
{
    if (m_snapToMidpoint == enabled)
        return;
    
    m_snapToMidpoint = enabled;
    emit snapToMidpointChanged();
}

void SnapTools::setSnapToIntersection(bool enabled)
{
    if (m_snapToIntersection == enabled)
        return;
    
    m_snapToIntersection = enabled;
    emit snapToIntersectionChanged();
}

void SnapTools::setSnapToCenter(bool enabled)
{
    if (m_snapToCenter == enabled)
        return;
    
    m_snapToCenter = enabled;
    emit snapToCenterChanged();
}

void SnapTools::setSnapToPerpendicular(bool enabled)
{
    if (m_snapToPerpendicular == enabled)
        return;
    
    m_snapToPerpendicular = enabled;
    emit snapToPerpendicularChanged();
}

void SnapTools::setSnapToTangent(bool enabled)
{
    if (m_snapToTangent == enabled)
        return;
    
    m_snapToTangent = enabled;
    emit snapToTangentChanged();
}

void SnapTools::setSnapTolerance(qreal tolerance)
{
    if (qFuzzyCompare(m_snapTolerance, tolerance))
        return;
    
    m_snapTolerance = tolerance;
    emit snapToleranceChanged();
}

void SnapTools::setDrawingEngine(DrawingEngine *engine)
{
    m_drawingEngine = engine;
}

void SnapTools::setShapeTools(ShapeTools *shapes)
{
    m_shapeTools = shapes;
}

QPointF SnapTools::snapPoint(const QPointF &point) const
{
    if (!m_snapEnabled)
        return point;
    
    QPointF bestSnap = point;
    qreal bestDistance = m_snapTolerance;
    QString bestType;
    
    // Try each enabled snap type
    if (m_snapToEndpoint) {
        QPointF snap = snapToNearestEndpoint(point);
        qreal dist = distance(point, snap);
        if (dist < bestDistance) {
            bestSnap = snap;
            bestDistance = dist;
            bestType = "endpoint";
        }
    }
    
    if (m_snapToMidpoint) {
        QPointF snap = snapToNearestMidpoint(point);
        qreal dist = distance(point, snap);
        if (dist < bestDistance) {
            bestSnap = snap;
            bestDistance = dist;
            bestType = "midpoint";
        }
    }
    
    if (m_snapToIntersection) {
        QPointF snap = snapToNearestIntersection(point);
        qreal dist = distance(point, snap);
        if (dist < bestDistance) {
            bestSnap = snap;
            bestDistance = dist;
            bestType = "intersection";
        }
    }
    
    if (m_snapToCenter) {
        QPointF snap = snapToNearestCenter(point);
        qreal dist = distance(point, snap);
        if (dist < bestDistance) {
            bestSnap = snap;
            bestDistance = dist;
            bestType = "center";
        }
    }
    
    // Perpendicular and tangent need a reference point (from previous point)
    // These are handled separately in drawing operations
    
    if (bestType.isEmpty()) {
        return point; // No snap found
    }
    
    const_cast<SnapTools*>(this)->m_lastSnapPoint = bestSnap;
    const_cast<SnapTools*>(this)->m_lastSnapType = bestType;
    emit const_cast<SnapTools*>(this)->snapPointFound(bestSnap, bestType);
    
    return bestSnap;
}

QPointF SnapTools::snapToNearestEndpoint(const QPointF &point) const
{
    QList<QPointF> endpoints = getAllEndpoints();
    if (endpoints.isEmpty())
        return point;
    
    return findNearestPoint(point, endpoints);
}

QPointF SnapTools::snapToNearestMidpoint(const QPointF &point) const
{
    QList<QPointF> midpoints = getAllMidpoints();
    if (midpoints.isEmpty())
        return point;
    
    return findNearestPoint(point, midpoints);
}

QPointF SnapTools::snapToNearestIntersection(const QPointF &point) const
{
    QList<QPointF> intersections = getAllIntersections();
    if (intersections.isEmpty())
        return point;
    
    return findNearestPoint(point, intersections);
}

QPointF SnapTools::snapToNearestCenter(const QPointF &point) const
{
    QList<QPointF> centers = getAllCenters();
    if (centers.isEmpty())
        return point;
    
    return findNearestPoint(point, centers);
}

QPointF SnapTools::snapToNearestPerpendicular(const QPointF &point, const QPointF &fromPoint) const
{
    QList<QPointF> segments = getLineSegments();
    if (segments.size() < 2) // Need at least start and end for a segment
        return point;
    
    QPointF bestSnap = point;
    qreal bestDistance = m_snapTolerance;
    
    // Check each line segment
    for (int i = 0; i < segments.size() - 1; i += 2) {
        QPointF lineStart = segments[i];
        QPointF lineEnd = segments[i + 1];
        
        QPointF perpPoint = findPerpendicularPoint(point, lineStart, lineEnd);
        qreal dist = distance(point, perpPoint);
        
        if (dist < bestDistance) {
            bestSnap = perpPoint;
            bestDistance = dist;
        }
    }
    
    return bestSnap;
}

QPointF SnapTools::snapToNearestTangent(const QPointF &point, const QPointF &fromPoint) const
{
    QList<QPointF> circles = getCircleArcs();
    if (circles.isEmpty())
        return point;
    
    QPointF bestSnap = point;
    qreal bestDistance = m_snapTolerance;
    
    // For each circle/arc, find tangent point
    for (int i = 0; i < circles.size(); i += 3) {
        if (i + 2 >= circles.size())
            break;
        
        QPointF center = circles[i];
        qreal radius = circles[i + 1].x(); // Store radius in x coordinate
        
        QPointF tangentPoint = findTangentPoint(point, center, radius);
        qreal dist = distance(point, tangentPoint);
        
        if (dist < bestDistance) {
            bestSnap = tangentPoint;
            bestDistance = dist;
        }
    }
    
    return bestSnap;
}

SnapTools::SnapInfo SnapTools::getSnapInfo(const QPointF &point) const
{
    SnapInfo info;
    info.point = snapPoint(point);
    info.type = m_lastSnapType;
    
    // Color coding for different snap types
    if (info.type == "endpoint") {
        info.color = QColor(255, 0, 0); // Red
    } else if (info.type == "midpoint") {
        info.color = QColor(0, 255, 0); // Green
    } else if (info.type == "intersection") {
        info.color = QColor(0, 0, 255); // Blue
    } else if (info.type == "center") {
        info.color = QColor(255, 255, 0); // Yellow
    } else if (info.type == "perpendicular") {
        info.color = QColor(255, 0, 255); // Magenta
    } else if (info.type == "tangent") {
        info.color = QColor(0, 255, 255); // Cyan
    } else {
        info.color = QColor(128, 128, 128); // Gray
    }
    
    return info;
}

qreal SnapTools::distance(const QPointF &p1, const QPointF &p2) const
{
    qreal dx = p2.x() - p1.x();
    qreal dy = p2.y() - p1.y();
    return qSqrt(dx * dx + dy * dy);
}

QPointF SnapTools::findNearestPoint(const QPointF &point, const QList<QPointF> &candidates) const
{
    if (candidates.isEmpty())
        return point;
    
    QPointF nearest = candidates[0];
    qreal minDist = distance(point, nearest);
    
    for (const QPointF &candidate : candidates) {
        qreal dist = distance(point, candidate);
        if (dist < minDist) {
            minDist = dist;
            nearest = candidate;
        }
    }
    
    // Only return snap if within tolerance
    if (minDist <= m_snapTolerance) {
        return nearest;
    }
    
    return point;
}

QList<QPointF> SnapTools::getAllEndpoints() const
{
    QList<QPointF> endpoints;
    
    // Get endpoints from strokes
    if (m_drawingEngine) {
        int strokeCount = m_drawingEngine->strokeCount();
        for (int i = 0; i < strokeCount; ++i) {
            auto strokes = m_drawingEngine->getStrokes({i});
            if (!strokes.isEmpty()) {
                const auto &stroke = strokes[0];
                if (!stroke.points.isEmpty()) {
                    endpoints.append(stroke.points.first());
                    endpoints.append(stroke.points.last());
                }
            }
        }
    }
    
    // Get endpoints from shapes
    if (m_shapeTools) {
        auto shapes = m_shapeTools->getAllShapes();
        for (const auto &shape : shapes) {
            if (shape.type == "line" || shape.type == "arrow" || shape.type == "arc") {
                endpoints.append(shape.startPoint);
                endpoints.append(shape.endPoint);
            } else if (shape.type == "rectangle") {
                endpoints.append(shape.startPoint);
                endpoints.append(shape.endPoint);
                // Also add other corners
                endpoints.append(QPointF(shape.startPoint.x(), shape.endPoint.y()));
                endpoints.append(QPointF(shape.endPoint.x(), shape.startPoint.y()));
            }
        }
    }
    
    return endpoints;
}

QList<QPointF> SnapTools::getAllMidpoints() const
{
    QList<QPointF> midpoints;
    
    // Get midpoints from strokes
    if (m_drawingEngine) {
        int strokeCount = m_drawingEngine->strokeCount();
        for (int i = 0; i < strokeCount; ++i) {
            auto strokes = m_drawingEngine->getStrokes({i});
            if (!strokes.isEmpty()) {
                const auto &stroke = strokes[0];
                if (stroke.points.size() >= 2) {
                    int midIndex = stroke.points.size() / 2;
                    midpoints.append(stroke.points[midIndex]);
                }
            }
        }
    }
    
    // Get midpoints from shapes
    if (m_shapeTools) {
        auto shapes = m_shapeTools->getAllShapes();
        for (const auto &shape : shapes) {
            if (shape.type == "line" || shape.type == "arrow") {
                QPointF mid = (shape.startPoint + shape.endPoint) / 2.0;
                midpoints.append(mid);
            } else if (shape.type == "rectangle") {
                QPointF mid = (shape.startPoint + shape.endPoint) / 2.0;
                midpoints.append(mid);
            }
        }
    }
    
    return midpoints;
}

QList<QPointF> SnapTools::getAllIntersections() const
{
    QList<QPointF> intersections;
    
    QList<QPointF> segments = getLineSegments();
    
    // Check all pairs of line segments for intersections
    for (int i = 0; i < segments.size() - 1; i += 2) {
        for (int j = i + 2; j < segments.size() - 1; j += 2) {
            QPointF p1 = segments[i];
            QPointF p2 = segments[i + 1];
            QPointF p3 = segments[j];
            QPointF p4 = segments[j + 1];
            
            // Calculate line intersection
            qreal denom = (p1.x() - p2.x()) * (p3.y() - p4.y()) - (p1.y() - p2.y()) * (p3.x() - p4.x());
            if (qAbs(denom) < 0.001)
                continue; // Lines are parallel
            
            qreal t = ((p1.x() - p3.x()) * (p3.y() - p4.y()) - (p1.y() - p3.y()) * (p3.x() - p4.x())) / denom;
            qreal u = -((p1.x() - p2.x()) * (p1.y() - p3.y()) - (p1.y() - p2.y()) * (p1.x() - p3.x())) / denom;
            
            // Check if intersection is within both segments
            if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
                QPointF intersect(
                    p1.x() + t * (p2.x() - p1.x()),
                    p1.y() + t * (p2.y() - p1.y())
                );
                intersections.append(intersect);
            }
        }
    }
    
    return intersections;
}

QList<QPointF> SnapTools::getAllCenters() const
{
    QList<QPointF> centers;
    
    // Get centers from shapes (circles, arcs)
    if (m_shapeTools) {
        auto shapes = m_shapeTools->getAllShapes();
        for (const auto &shape : shapes) {
            if (shape.type == "circle" || shape.type == "arc") {
                if (shape.centerPoint.x() != 0 || shape.centerPoint.y() != 0) {
                    centers.append(shape.centerPoint);
                } else {
                    // For circles, center is startPoint
                    centers.append(shape.startPoint);
                }
            }
        }
    }
    
    return centers;
}

QPointF SnapTools::findPerpendicularPoint(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd) const
{
    QPointF lineDir = lineEnd - lineStart;
    qreal lineLen = qSqrt(lineDir.x() * lineDir.x() + lineDir.y() * lineDir.y());
    
    if (lineLen < 0.001)
        return point; // Degenerate line
    
    lineDir /= lineLen; // Normalize
    
    QPointF toPoint = point - lineStart;
    qreal projection = QPointF::dotProduct(toPoint, lineDir);
    
    // Clamp projection to line segment
    projection = qBound(0.0, projection, lineLen);
    
    QPointF projPoint = lineStart + lineDir * projection;
    
    // Perpendicular point is the projection
    return projPoint;
}

QPointF SnapTools::findTangentPoint(const QPointF &point, const QPointF &center, qreal radius) const
{
    qreal distToCenter = distance(point, center);
    
    if (distToCenter < radius)
        return point; // Point is inside circle, no tangent
    
    // Calculate angle from center to point
    qreal angle = qAtan2(point.y() - center.y(), point.x() - center.x());
    
    // Tangent point is at angle ± 90 degrees from line to point
    qreal tangentAngle1 = angle + M_PI / 2.0;
    qreal tangentAngle2 = angle - M_PI / 2.0;
    
    QPointF tangent1(
        center.x() + radius * qCos(tangentAngle1),
        center.y() + radius * qSin(tangentAngle1)
    );
    QPointF tangent2(
        center.x() + radius * qCos(tangentAngle2),
        center.y() + radius * qSin(tangentAngle2)
    );
    
    // Return nearest tangent point
    if (distance(point, tangent1) < distance(point, tangent2)) {
        return tangent1;
    }
    return tangent2;
}

QList<QPointF> SnapTools::getLineSegments() const
{
    QList<QPointF> segments;
    
    // Get line segments from strokes
    if (m_drawingEngine) {
        int strokeCount = m_drawingEngine->strokeCount();
        for (int i = 0; i < strokeCount; ++i) {
            auto strokes = m_drawingEngine->getStrokes({i});
            if (!strokes.isEmpty()) {
                const auto &stroke = strokes[0];
                for (int j = 0; j < stroke.points.size() - 1; ++j) {
                    segments.append(stroke.points[j]);
                    segments.append(stroke.points[j + 1]);
                }
            }
        }
    }
    
    // Get line segments from shapes
    if (m_shapeTools) {
        auto shapes = m_shapeTools->getAllShapes();
        for (const auto &shape : shapes) {
            if (shape.type == "line" || shape.type == "arrow") {
                segments.append(shape.startPoint);
                segments.append(shape.endPoint);
            } else if (shape.type == "rectangle") {
                // Add all 4 edges
                QPointF topLeft = shape.startPoint;
                QPointF bottomRight = shape.endPoint;
                QPointF topRight(bottomRight.x(), topLeft.y());
                QPointF bottomLeft(topLeft.x(), bottomRight.y());
                
                segments.append(topLeft);
                segments.append(topRight);
                segments.append(topRight);
                segments.append(bottomRight);
                segments.append(bottomRight);
                segments.append(bottomLeft);
                segments.append(bottomLeft);
                segments.append(topLeft);
            }
        }
    }
    
    return segments;
}

QList<QPointF> SnapTools::getCircleArcs() const
{
    QList<QPointF> circles;
    
    // Get circles/arcs from shapes
    if (m_shapeTools) {
        auto shapes = m_shapeTools->getAllShapes();
        for (const auto &shape : shapes) {
            if (shape.type == "circle") {
                QPointF center = shape.startPoint;
                qreal radius = distance(center, shape.endPoint);
                circles.append(center);
                circles.append(QPointF(radius, 0)); // Store radius in x coordinate
                circles.append(QPointF(0, 0)); // Placeholder
            } else if (shape.type == "arc") {
                if (shape.centerPoint.x() != 0 || shape.centerPoint.y() != 0) {
                    circles.append(shape.centerPoint);
                    circles.append(QPointF(shape.radius, 0)); // Store radius
                    circles.append(QPointF(0, 0)); // Placeholder
                }
            }
        }
    }
    
    return circles;
}

