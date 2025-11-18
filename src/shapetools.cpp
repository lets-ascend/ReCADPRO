#include "shapetools.h"
#include <QPainter>
#include <QtMath>

ShapeTools::ShapeTools(QObject *parent)
    : QObject(parent)
    , m_currentShape("rectangle")
    , m_fillEnabled(false)
    , m_fillColor(Qt::white)
    , m_isDrawing(false)
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
    
    m_currentShapeData.endPoint = currentPoint;
    emit shapeUpdated();
}

void ShapeTools::finishShape()
{
    if (!m_isDrawing)
        return;
    
    m_shapes.append(m_currentShapeData);
    emit shapeFinished(m_currentShapeData);
    m_isDrawing = false;
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

void ShapeTools::drawLine(QPainter *painter, const QPointF &start, const QPointF &end)
{
    painter->drawLine(start, end);
}

