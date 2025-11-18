#include "advancedlinetools.h"
#include <QtMath>
#include <QDebug>

AdvancedLineTools::AdvancedLineTools(QObject *parent)
    : QObject(parent)
    , m_currentTool("line")
    , m_lineWidth(1.0)
    , m_lineColor(Qt::black)
    , m_constructionMode(false)
    , m_isDrawingPolyline(false)
    , m_isDrawingSpline(false)
    , m_isDrawingBezier(false)
    , m_currentBezierControlIndex(0)
{
}

void AdvancedLineTools::setCurrentTool(const QString &tool)
{
    if (m_currentTool == tool)
        return;
    
    // Cancel any active drawing
    if (m_isDrawingPolyline) {
        cancelPolyline();
    }
    if (m_isDrawingSpline) {
        m_isDrawingSpline = false;
        m_currentSplinePoints.clear();
    }
    if (m_isDrawingBezier) {
        m_isDrawingBezier = false;
        m_currentBezierPoints.clear();
        m_currentBezierControlIndex = 0;
    }
    
    m_currentTool = tool;
    emit currentToolChanged();
}

void AdvancedLineTools::setLineWidth(qreal width)
{
    if (qFuzzyCompare(m_lineWidth, width))
        return;
    
    m_lineWidth = width;
    emit lineWidthChanged();
}

void AdvancedLineTools::setLineColor(const QColor &color)
{
    if (m_lineColor == color)
        return;
    
    m_lineColor = color;
    emit lineColorChanged();
}

void AdvancedLineTools::setConstructionMode(bool enabled)
{
    if (m_constructionMode == enabled)
        return;
    
    m_constructionMode = enabled;
    emit constructionModeChanged();
}

void AdvancedLineTools::startConstructionLine(const QPointF &point, const QPointF &direction)
{
    addConstructionLine(point, direction);
}

void AdvancedLineTools::addConstructionLine(const QPointF &point, const QPointF &direction)
{
    LineSegment line;
    line.start = point;
    line.end = point + direction * 10000.0; // Very long line
    line.type = LineConstruction;
    line.color = m_constructionMode ? Qt::gray : m_lineColor;
    line.width = m_lineWidth;
    line.construction = true;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::startRay(const QPointF &startPoint, const QPointF &direction)
{
    addRay(startPoint, direction);
}

void AdvancedLineTools::addRay(const QPointF &startPoint, const QPointF &direction)
{
    LineSegment line;
    line.start = startPoint;
    line.end = startPoint + direction * 10000.0; // Very long line
    line.type = LineRay;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::startPolyline(const QPointF &startPoint)
{
    m_isDrawingPolyline = true;
    m_currentPolylinePoints.clear();
    m_currentPolylinePoints.append(startPoint);
    emit drawingStarted();
}

void AdvancedLineTools::addPolylinePoint(const QPointF &point)
{
    if (!m_isDrawingPolyline)
        return;
    
    m_currentPolylinePoints.append(point);
    emit drawingUpdated();
}

void AdvancedLineTools::finishPolyline()
{
    if (!m_isDrawingPolyline || m_currentPolylinePoints.size() < 2)
        return;
    
    PolylineData polyline;
    polyline.points = m_currentPolylinePoints;
    polyline.closed = false; // Can be made configurable
    polyline.color = m_lineColor;
    polyline.width = m_lineWidth;
    
    m_polylines.append(polyline);
    m_isDrawingPolyline = false;
    m_currentPolylinePoints.clear();
    
    emit polylineCreated(polyline);
    emit drawingFinished();
}

void AdvancedLineTools::cancelPolyline()
{
    m_isDrawingPolyline = false;
    m_currentPolylinePoints.clear();
    emit drawingFinished();
}

void AdvancedLineTools::startSpline(const QPointF &startPoint)
{
    m_isDrawingSpline = true;
    m_currentSplinePoints.clear();
    m_currentSplinePoints.append(startPoint);
    emit drawingStarted();
}

void AdvancedLineTools::addSplinePoint(const QPointF &point)
{
    if (!m_isDrawingSpline)
        return;
    
    m_currentSplinePoints.append(point);
    emit drawingUpdated();
}

void AdvancedLineTools::finishSpline()
{
    if (!m_isDrawingSpline || m_currentSplinePoints.size() < 3)
        return;
    
    SplineData spline;
    spline.controlPoints = m_currentSplinePoints;
    spline.degree = 3; // Cubic spline
    spline.color = m_lineColor;
    spline.width = m_lineWidth;
    
    m_splines.append(spline);
    m_isDrawingSpline = false;
    m_currentSplinePoints.clear();
    
    emit splineCreated(spline);
    emit drawingFinished();
}

void AdvancedLineTools::startBezier(const QPointF &startPoint)
{
    m_isDrawingBezier = true;
    m_currentBezierPoints.clear();
    m_currentBezierPoints.append(startPoint);
    m_currentBezierControlIndex = 1;
    emit drawingStarted();
}

void AdvancedLineTools::addBezierControlPoint(const QPointF &point, int controlIndex)
{
    if (!m_isDrawingBezier)
        return;
    
    while (m_currentBezierPoints.size() <= controlIndex) {
        m_currentBezierPoints.append(QPointF());
    }
    m_currentBezierPoints[controlIndex] = point;
    emit drawingUpdated();
}

void AdvancedLineTools::finishBezier()
{
    if (!m_isDrawingBezier || m_currentBezierPoints.size() < 4)
        return;
    
    BezierData bezier;
    bezier.start = m_currentBezierPoints[0];
    bezier.control1 = m_currentBezierPoints[1];
    bezier.control2 = m_currentBezierPoints[2];
    bezier.end = m_currentBezierPoints[3];
    bezier.color = m_lineColor;
    bezier.width = m_lineWidth;
    
    m_beziers.append(bezier);
    m_isDrawingBezier = false;
    m_currentBezierPoints.clear();
    m_currentBezierControlIndex = 0;
    
    emit bezierCreated(bezier);
    emit drawingFinished();
}

void AdvancedLineTools::createTangentLine(const QPointF &point, const QPointF &circleCenter, qreal radius)
{
    QPointF tangentPoint = calculateTangentPoint(point, circleCenter, radius);
    
    LineSegment line;
    line.start = point;
    line.end = tangentPoint;
    line.type = LineTangent;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::createPerpendicularLine(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd)
{
    QPointF perpPoint = calculatePerpendicularPoint(point, lineStart, lineEnd);
    
    LineSegment line;
    line.start = point;
    line.end = perpPoint;
    line.type = LinePerpendicular;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::createParallelLine(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd, qreal distance)
{
    QPointF offsetPoint = calculateOffsetPoint(point, lineStart, lineEnd, distance);
    QPointF direction = lineEnd - lineStart;
    qreal length = calculateDistance(lineStart, lineEnd);
    direction = direction / length;
    
    LineSegment line;
    line.start = offsetPoint;
    line.end = offsetPoint + direction * length;
    line.type = LineParallel;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::createOffsetLine(const QPointF &lineStart, const QPointF &lineEnd, qreal offset)
{
    QPointF direction = lineEnd - lineStart;
    QPointF perp = QPointF(-direction.y(), direction.x());
    qreal length = calculateDistance(QPointF(0, 0), perp);
    perp = perp / length * offset;
    
    LineSegment line;
    line.start = lineStart + perp;
    line.end = lineEnd + perp;
    line.type = LineOffset;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::createFillet(const QPointF &line1Start, const QPointF &line1End,
                                     const QPointF &line2Start, const QPointF &line2End,
                                     qreal radius)
{
    QPointF arcCenter = calculateFilletArcCenter(line1Start, line1End, line2Start, line2End, radius);
    
    // Create trimmed lines and arc
    // This is a simplified version - full implementation would create actual arc segments
    Q_UNUSED(arcCenter);
    // Implementation would create trimmed lines and arc
}

void AdvancedLineTools::createChamfer(const QPointF &line1Start, const QPointF &line1End,
                                      const QPointF &line2Start, const QPointF &line2End,
                                      qreal distance1, qreal distance2)
{
    // Calculate chamfer points
    QPointF dir1 = line1End - line1Start;
    QPointF dir2 = line2End - line2Start;
    qreal len1 = calculateDistance(QPointF(0, 0), dir1);
    qreal len2 = calculateDistance(QPointF(0, 0), dir2);
    dir1 = dir1 / len1;
    dir2 = dir2 / len2;
    
    QPointF intersection = calculateIntersection(line1Start, line1End, line2Start, line2End);
    QPointF chamferStart = intersection - dir1 * distance1;
    QPointF chamferEnd = intersection - dir2 * distance2;
    
    // Create chamfer line
    LineSegment line;
    line.start = chamferStart;
    line.end = chamferEnd;
    line.type = LineStandard;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::trimLine(const QPointF &trimPoint, const QPointF &lineStart, const QPointF &lineEnd)
{
    // Find closest point on line to trim point
    QPointF perpPoint = calculatePerpendicularPoint(trimPoint, lineStart, lineEnd);
    
    // Create trimmed line segments
    // Simplified - would need to track which segment to keep
    Q_UNUSED(perpPoint);
}

void AdvancedLineTools::extendLine(const QPointF &extendPoint, const QPointF &lineStart, const QPointF &lineEnd)
{
    QPointF direction = lineEnd - lineStart;
    qreal length = calculateDistance(lineStart, lineEnd);
    direction = direction / length;
    
    // Extend line towards extend point
    QPointF newEnd = lineEnd + direction * calculateDistance(lineEnd, extendPoint);
    
    LineSegment line;
    line.start = lineStart;
    line.end = newEnd;
    line.type = LineStandard;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

void AdvancedLineTools::breakLine(const QPointF &breakPoint, const QPointF &lineStart, const QPointF &lineEnd)
{
    // Create two line segments
    LineSegment line1;
    line1.start = lineStart;
    line1.end = breakPoint;
    line1.type = LineStandard;
    line1.color = m_lineColor;
    line1.width = m_lineWidth;
    line1.construction = m_constructionMode;
    
    LineSegment line2;
    line2.start = breakPoint;
    line2.end = lineEnd;
    line2.type = LineStandard;
    line2.color = m_lineColor;
    line2.width = m_lineWidth;
    line2.construction = m_constructionMode;
    
    m_lines.append(line1);
    m_lines.append(line2);
    emit lineCreated(line1);
    emit lineCreated(line2);
}

void AdvancedLineTools::joinLines(const QPointF &line1Start, const QPointF &line1End,
                                  const QPointF &line2Start, const QPointF &line2End)
{
    // Find closest endpoints and join
    qreal dist1 = calculateDistance(line1End, line2Start);
    qreal dist2 = calculateDistance(line1End, line2End);
    qreal dist3 = calculateDistance(line1Start, line2Start);
    qreal dist4 = calculateDistance(line1Start, line2End);
    
    QPointF start, end;
    if (dist1 <= dist2 && dist1 <= dist3 && dist1 <= dist4) {
        start = line1Start;
        end = line2End;
    } else if (dist2 <= dist3 && dist2 <= dist4) {
        start = line1Start;
        end = line2Start;
    } else if (dist3 <= dist4) {
        start = line1End;
        end = line2End;
    } else {
        start = line1End;
        end = line2Start;
    }
    
    LineSegment line;
    line.start = start;
    line.end = end;
    line.type = LineStandard;
    line.color = m_lineColor;
    line.width = m_lineWidth;
    line.construction = m_constructionMode;
    
    m_lines.append(line);
    emit lineCreated(line);
}

// Helper functions
QPointF AdvancedLineTools::calculateIntersection(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    qreal denom = (p1.x() - p2.x()) * (p3.y() - p4.y()) - (p1.y() - p2.y()) * (p3.x() - p4.x());
    if (qAbs(denom) < 1e-10)
        return QPointF(); // Lines are parallel
    
    qreal t = ((p1.x() - p3.x()) * (p3.y() - p4.y()) - (p1.y() - p3.y()) * (p3.x() - p4.x())) / denom;
    return p1 + t * (p2 - p1);
}

qreal AdvancedLineTools::calculateDistance(const QPointF &p1, const QPointF &p2)
{
    QPointF diff = p2 - p1;
    return qSqrt(diff.x() * diff.x() + diff.y() * diff.y());
}

QPointF AdvancedLineTools::calculatePerpendicularPoint(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd)
{
    QPointF direction = lineEnd - lineStart;
    qreal length = calculateDistance(lineStart, lineEnd);
    if (length < 1e-10)
        return lineStart;
    
    direction = direction / length;
    QPointF toPoint = point - lineStart;
    qreal projection = QPointF::dotProduct(toPoint, direction);
    return lineStart + direction * projection;
}

QPointF AdvancedLineTools::calculateTangentPoint(const QPointF &point, const QPointF &circleCenter, qreal radius)
{
    QPointF toPoint = point - circleCenter;
    qreal dist = calculateDistance(QPointF(0, 0), toPoint);
    if (dist < radius)
        return circleCenter; // Point inside circle
    
    qreal angle = qAtan2(toPoint.y(), toPoint.x());
    qreal tangentAngle = angle + qAsin(radius / dist);
    
    return circleCenter + QPointF(qCos(tangentAngle), qSin(tangentAngle)) * radius;
}

QPointF AdvancedLineTools::calculateOffsetPoint(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd, qreal offset)
{
    QPointF direction = lineEnd - lineStart;
    QPointF perp = QPointF(-direction.y(), direction.x());
    qreal length = calculateDistance(QPointF(0, 0), perp);
    if (length < 1e-10)
        return point;
    
    perp = perp / length * offset;
    return point + perp;
}

QPointF AdvancedLineTools::calculateFilletArcCenter(const QPointF &line1Start, const QPointF &line1End,
                                                    const QPointF &line2Start, const QPointF &line2End,
                                                    qreal radius)
{
    QPointF intersection = calculateIntersection(line1Start, line1End, line2Start, line2End);
    
    // Calculate angle bisector
    QPointF dir1 = line1End - line1Start;
    QPointF dir2 = line2End - line2Start;
    qreal len1 = calculateDistance(QPointF(0, 0), dir1);
    qreal len2 = calculateDistance(QPointF(0, 0), dir2);
    dir1 = dir1 / len1;
    dir2 = dir2 / len2;
    
    QPointF bisector = dir1 + dir2;
    qreal bisectorLen = calculateDistance(QPointF(0, 0), bisector);
    if (bisectorLen < 1e-10)
        return intersection;
    
    bisector = bisector / bisectorLen;
    
    // Distance from intersection to arc center
    qreal angle = qAcos(QPointF::dotProduct(dir1, dir2));
    qreal dist = radius / qSin(angle / 2.0);
    
    return intersection + bisector * dist;
}

