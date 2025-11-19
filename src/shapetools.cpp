#include "shapetools.h"
#include <QPainter>
#include <QtMath>

ShapeTools::ShapeTools(QObject *parent)
    : QObject(parent)
    , m_currentShape("rectangle")
    , m_arcMode("3point")
    , m_fillEnabled(false)
    , m_fillColor(Qt::white)
    , m_isDrawing(false)
    , m_arcStep(0)
{
}

void ShapeTools::setCurrentShape(const QString &shape)
{
    if (m_currentShape == shape)
        return;
    
    m_currentShape = shape;
    emit currentShapeChanged();
}

void ShapeTools::setFillEnabled(bool enabled)
{
    if (m_fillEnabled == enabled)
        return;
    
    m_fillEnabled = enabled;
    emit fillEnabledChanged();
}

void ShapeTools::setFillColor(const QColor &color)
{
    if (m_fillColor == color)
        return;
    
    m_fillColor = color;
    emit fillColorChanged();
}

void ShapeTools::startShape(const QPointF &startPoint)
{
    m_currentShapeData.type = m_currentShape;
    m_currentShapeData.startPoint = startPoint;
    m_currentShapeData.endPoint = startPoint;
    m_currentShapeData.filled = m_fillEnabled;
    m_currentShapeData.fillColor = m_fillColor;
    m_isDrawing = true;
    emit shapeStarted();
}

void ShapeTools::updateShape(const QPointF &currentPoint)
{
    if (!m_isDrawing)
        return;
    
    // Handle arc-specific updates
    if (m_currentShape == "arc") {
        if (m_arcMode == "3point") {
            if (m_arcStep == 1) {
                setArc3PointMiddle(currentPoint);
            } else if (m_arcStep == 2) {
                // Preview end point
                m_currentShapeData.endPoint = currentPoint;
                emit shapeUpdated();
            }
        } else if (m_arcMode == "center-radius") {
            if (m_arcStep == 1) {
                setArcRadius(currentPoint);
            } else if (m_arcStep == 2) {
                m_currentShapeData.startAngle = calculateAngle(m_currentShapeData.centerPoint, currentPoint);
                emit shapeUpdated();
            } else if (m_arcStep == 3) {
                m_currentShapeData.endAngle = calculateAngle(m_currentShapeData.centerPoint, currentPoint);
                emit shapeUpdated();
            }
        } else if (m_arcMode == "start-end-radius") {
            if (m_arcStep == 1) {
                setArcEndPoint(currentPoint);
            } else if (m_arcStep == 2) {
                // Preview radius
                emit shapeUpdated();
            }
        }
        return;
    }
    
    m_currentShapeData.endPoint = currentPoint;
    emit shapeUpdated();
}

void ShapeTools::finishShape()
{
    if (!m_isDrawing)
        return;
    
    // Handle arc-specific finish
    if (m_currentShape == "arc") {
        if (m_arcMode == "3point" && m_arcStep == 2) {
            finishArc3Point(m_currentShapeData.endPoint);
        } else if (m_arcMode == "center-radius" && m_arcStep == 3) {
            finishArcCenterRadius(m_currentShapeData.endPoint);
        } else if (m_arcMode == "start-end-radius" && m_arcStep == 2) {
            finishArcStartEndRadius(m_currentShapeData.endPoint);
        }
        return;
    }
    
    m_shapes.append(m_currentShapeData);
    emit shapeFinished(m_currentShapeData);
    m_isDrawing = false;
    m_arcStep = 0;
}

void ShapeTools::drawRectangle(QPainter *painter, const QPointF &start, const QPointF &end, bool filled)
{
    QRectF rect(start, end);
    
    if (filled) {
        painter->fillRect(rect, m_fillColor);
    }
    painter->drawRect(rect);
}

void ShapeTools::drawCircle(QPainter *painter, const QPointF &center, qreal radius, bool filled)
{
    QRectF rect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
    
    if (filled) {
        painter->fillEllipse(rect, m_fillColor);
    }
    painter->drawEllipse(rect);
}

void ShapeTools::drawArrow(QPainter *painter, const QPointF &start, const QPointF &end)
{
    // Draw line
    painter->drawLine(start, end);
    
    // Calculate arrow head
    qreal angle = qAtan2(end.y() - start.y(), end.x() - start.x());
    qreal arrowLength = 20.0;
    qreal arrowAngle = M_PI / 6.0; // 30 degrees
    
    QPointF arrowPoint1(
        end.x() - arrowLength * qCos(angle - arrowAngle),
        end.y() - arrowLength * qSin(angle - arrowAngle)
    );
    QPointF arrowPoint2(
        end.x() - arrowLength * qCos(angle + arrowAngle),
        end.y() - arrowLength * qSin(angle + arrowAngle)
    );
    
    // Draw arrow head
    QPolygonF arrowHead;
    arrowHead << end << arrowPoint1 << arrowPoint2;
    painter->drawPolygon(arrowHead);
}

void ShapeTools::setArcMode(const QString &mode)
{
    if (m_arcMode == mode)
        return;
    
    m_arcMode = mode;
    emit arcModeChanged();
}

void ShapeTools::startArc3Point(const QPointF &startPoint)
{
    m_currentShapeData.type = "arc";
    m_currentShapeData.startPoint = startPoint;
    m_currentShapeData.middlePoint = startPoint;
    m_currentShapeData.endPoint = startPoint;
    m_currentShapeData.filled = false; // Arcs are never filled
    m_isDrawing = true;
    m_arcStep = 1; // Next: set middle point
    emit shapeStarted();
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::setArc3PointMiddle(const QPointF &middlePoint)
{
    if (!m_isDrawing || m_arcStep != 1)
        return;
    
    m_currentShapeData.middlePoint = middlePoint;
    m_arcStep = 2; // Next: set end point
    emit shapeUpdated();
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::finishArc3Point(const QPointF &endPoint)
{
    if (!m_isDrawing || m_arcStep != 2)
        return;
    
    m_currentShapeData.endPoint = endPoint;
    
    // Calculate center and radius from 3 points
    m_currentShapeData.centerPoint = calculateArcCenter3Point(
        m_currentShapeData.startPoint,
        m_currentShapeData.middlePoint,
        m_currentShapeData.endPoint
    );
    
    // Calculate radius
    qreal dx = m_currentShapeData.startPoint.x() - m_currentShapeData.centerPoint.x();
    qreal dy = m_currentShapeData.startPoint.y() - m_currentShapeData.centerPoint.y();
    m_currentShapeData.radius = qSqrt(dx * dx + dy * dy);
    
    // Calculate angles
    m_currentShapeData.startAngle = calculateAngle(m_currentShapeData.centerPoint, m_currentShapeData.startPoint);
    qreal middleAngle = calculateAngle(m_currentShapeData.centerPoint, m_currentShapeData.middlePoint);
    m_currentShapeData.endAngle = calculateAngle(m_currentShapeData.centerPoint, endPoint);
    
    // Ensure arc goes through middle point (adjust end angle if needed)
    if ((middleAngle > m_currentShapeData.startAngle && middleAngle < m_currentShapeData.endAngle) ||
        (m_currentShapeData.endAngle < m_currentShapeData.startAngle && 
         (middleAngle > m_currentShapeData.startAngle || middleAngle < m_currentShapeData.endAngle))) {
        // Middle point is between start and end, arc is correct
    } else {
        // Swap angles if needed to ensure arc goes through middle
        if (m_currentShapeData.endAngle < m_currentShapeData.startAngle) {
            m_currentShapeData.endAngle += 360.0;
        }
    }
    
    m_shapes.append(m_currentShapeData);
    emit shapeFinished(m_currentShapeData);
    m_isDrawing = false;
    m_arcStep = 0;
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::startArcCenterRadius(const QPointF &center)
{
    m_currentShapeData.type = "arc";
    m_currentShapeData.centerPoint = center;
    m_currentShapeData.radius = 0;
    m_currentShapeData.filled = false;
    m_isDrawing = true;
    m_arcStep = 1; // Next: set radius
    emit shapeStarted();
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::setArcRadius(const QPointF &radiusPoint)
{
    if (!m_isDrawing || m_arcStep != 1)
        return;
    
    qreal dx = radiusPoint.x() - m_currentShapeData.centerPoint.x();
    qreal dy = radiusPoint.y() - m_currentShapeData.centerPoint.y();
    m_currentShapeData.radius = qSqrt(dx * dx + dy * dy);
    m_arcStep = 2; // Next: set start angle
    emit shapeUpdated();
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::setArcStartAngle(const QPointF &startAnglePoint)
{
    if (!m_isDrawing || m_arcStep != 2)
        return;
    
    m_currentShapeData.startAngle = calculateAngle(m_currentShapeData.centerPoint, startAnglePoint);
    m_arcStep = 3; // Next: set end angle
    emit shapeUpdated();
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::finishArcCenterRadius(const QPointF &endAnglePoint)
{
    if (!m_isDrawing || m_arcStep != 3)
        return;
    
    m_currentShapeData.endAngle = calculateAngle(m_currentShapeData.centerPoint, endAnglePoint);
    
    // Calculate start and end points
    qreal startRad = qDegreesToRadians(m_currentShapeData.startAngle);
    qreal endRad = qDegreesToRadians(m_currentShapeData.endAngle);
    m_currentShapeData.startPoint = QPointF(
        m_currentShapeData.centerPoint.x() + m_currentShapeData.radius * qCos(startRad),
        m_currentShapeData.centerPoint.y() + m_currentShapeData.radius * qSin(startRad)
    );
    m_currentShapeData.endPoint = QPointF(
        m_currentShapeData.centerPoint.x() + m_currentShapeData.radius * qCos(endRad),
        m_currentShapeData.centerPoint.y() + m_currentShapeData.radius * qSin(endRad)
    );
    
    m_shapes.append(m_currentShapeData);
    emit shapeFinished(m_currentShapeData);
    m_isDrawing = false;
    m_arcStep = 0;
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::startArcStartEndRadius(const QPointF &startPoint)
{
    m_currentShapeData.type = "arc";
    m_currentShapeData.startPoint = startPoint;
    m_currentShapeData.endPoint = startPoint;
    m_currentShapeData.filled = false;
    m_isDrawing = true;
    m_arcStep = 1; // Next: set end point
    emit shapeStarted();
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::setArcEndPoint(const QPointF &endPoint)
{
    if (!m_isDrawing || m_arcStep != 1)
        return;
    
    m_currentShapeData.endPoint = endPoint;
    m_arcStep = 2; // Next: set radius
    emit shapeUpdated();
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::finishArcStartEndRadius(const QPointF &radiusPoint)
{
    if (!m_isDrawing || m_arcStep != 2)
        return;
    
    // Calculate center from start, end, and radius point
    QPointF midPoint = (m_currentShapeData.startPoint + m_currentShapeData.endPoint) / 2.0;
    QPointF dir = m_currentShapeData.endPoint - m_currentShapeData.startPoint;
    qreal dist = qSqrt(dir.x() * dir.x() + dir.y() * dir.y());
    
    if (dist < 0.001) {
        // Start and end are same point, can't create arc
        m_isDrawing = false;
        m_arcStep = 0;
        return;
    }
    
    // Perpendicular direction
    QPointF perp(-dir.y() / dist, dir.x() / dist);
    
    // Distance from midpoint to center
    qreal dx = radiusPoint.x() - midPoint.x();
    qreal dy = radiusPoint.y() - midPoint.y();
    qreal dot = dx * perp.x() + dy * perp.y();
    
    // Center is on perpendicular line through midpoint
    m_currentShapeData.centerPoint = midPoint + perp * dot;
    
    // Calculate radius
    dx = m_currentShapeData.startPoint.x() - m_currentShapeData.centerPoint.x();
    dy = m_currentShapeData.startPoint.y() - m_currentShapeData.centerPoint.y();
    m_currentShapeData.radius = qSqrt(dx * dx + dy * dy);
    
    // Calculate angles
    m_currentShapeData.startAngle = calculateAngle(m_currentShapeData.centerPoint, m_currentShapeData.startPoint);
    m_currentShapeData.endAngle = calculateAngle(m_currentShapeData.centerPoint, m_currentShapeData.endPoint);
    
    m_shapes.append(m_currentShapeData);
    emit shapeFinished(m_currentShapeData);
    m_isDrawing = false;
    m_arcStep = 0;
    emit arcStepChanged(m_arcStep);
}

void ShapeTools::drawArc(QPainter *painter, const QPointF &center, qreal radius, qreal startAngle, qreal endAngle)
{
    QRectF rect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
    
    // Normalize angles
    qreal normalizedStart = normalizeAngle(startAngle);
    qreal normalizedEnd = normalizeAngle(endAngle);
    
    // Convert to radians
    qreal startRad = qDegreesToRadians(normalizedStart);
    qreal endRad = qDegreesToRadians(normalizedEnd);
    
    // Calculate span
    qreal span = normalizedEnd - normalizedStart;
    if (span < 0) span += 360.0;
    qreal spanRad = qDegreesToRadians(span);
    
    // Draw arc
    painter->drawArc(rect, static_cast<int>(normalizedStart * 16), static_cast<int>(span * 16));
}

QPointF ShapeTools::calculateArcCenter3Point(const QPointF &p1, const QPointF &p2, const QPointF &p3) const
{
    // Calculate center of circle through 3 points using perpendicular bisectors
    qreal x1 = p1.x(), y1 = p1.y();
    qreal x2 = p2.x(), y2 = p2.y();
    qreal x3 = p3.x(), y3 = p3.y();
    
    qreal d = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    if (qAbs(d) < 0.001) {
        // Points are collinear, return midpoint
        return (p1 + p2 + p3) / 3.0;
    }
    
    qreal ux = ((x1 * x1 + y1 * y1) * (y2 - y3) + 
                (x2 * x2 + y2 * y2) * (y3 - y1) + 
                (x3 * x3 + y3 * y3) * (y1 - y2)) / d;
    qreal uy = ((x1 * x1 + y1 * y1) * (x3 - x2) + 
                (x2 * x2 + y2 * y2) * (x1 - x3) + 
                (x3 * x3 + y3 * y3) * (x2 - x1)) / d;
    
    return QPointF(ux, uy);
}

qreal ShapeTools::calculateAngle(const QPointF &center, const QPointF &point) const
{
    qreal dx = point.x() - center.x();
    qreal dy = point.y() - center.y();
    qreal angle = qRadiansToDegrees(qAtan2(dy, dx));
    return normalizeAngle(angle);
}

qreal ShapeTools::normalizeAngle(qreal angle) const
{
    while (angle < 0) angle += 360.0;
    while (angle >= 360.0) angle -= 360.0;
    return angle;
}

void ShapeTools::drawLine(QPainter *painter, const QPointF &start, const QPointF &end)
{
    painter->drawLine(start, end);
}

