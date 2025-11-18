#include "advancedmeasurement.h"
#include <QtMath>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AdvancedMeasurement::AdvancedMeasurement(QObject *parent)
    : QObject(parent)
    , m_showDimensionLines(true)
    , m_showAreaFill(true)
    , m_dimensionColor(Qt::blue)
{
}

void AdvancedMeasurement::setShowDimensionLines(bool show)
{
    if (m_showDimensionLines == show)
        return;
    
    m_showDimensionLines = show;
    emit showDimensionLinesChanged();
}

void AdvancedMeasurement::setShowAreaFill(bool show)
{
    if (m_showAreaFill == show)
        return;
    
    m_showAreaFill = show;
    emit showAreaFillChanged();
}

void AdvancedMeasurement::setDimensionColor(const QColor &color)
{
    if (m_dimensionColor == color)
        return;
    
    m_dimensionColor = color;
    emit dimensionColorChanged();
}

AdvancedMeasurement::DimensionLine AdvancedMeasurement::createDimensionLine(
    const QPointF &start, const QPointF &end, 
    const QString &measurement, const QString &unit)
{
    DimensionLine dim;
    dim.start = start;
    dim.end = end;
    dim.measurement = measurement;
    dim.unit = unit;
    dim.color = m_dimensionColor;
    dim.showArrows = true;
    dim.arrowSize = 10.0;
    return dim;
}

QList<QPointF> AdvancedMeasurement::getDimensionLineArrows(const DimensionLine &dimension) const
{
    QList<QPointF> arrows;
    
    if (!dimension.showArrows)
        return arrows;
    
    // Start arrow
    QPointF startArrow = calculateArrowPoint(dimension.start, dimension.end, 
                                             dimension.arrowSize, true);
    arrows.append(startArrow);
    
    // End arrow
    QPointF endArrow = calculateArrowPoint(dimension.end, dimension.start, 
                                          dimension.arrowSize, false);
    arrows.append(endArrow);
    
    return arrows;
}

void AdvancedMeasurement::addDimensionLine(const DimensionLine &dimension)
{
    m_dimensionLines.append(dimension);
    emit dimensionLineAdded(dimension);
}

AdvancedMeasurement::AreaMeasurement AdvancedMeasurement::createAreaMeasurement(
    const QList<QPointF> &points, qreal area, const QString &unit)
{
    AreaMeasurement areaMeas;
    areaMeas.points = points;
    areaMeas.area = area;
    areaMeas.unit = unit;
    areaMeas.fillColor = QColor(m_dimensionColor.red(), m_dimensionColor.green(), 
                               m_dimensionColor.blue(), 50); // Semi-transparent
    areaMeas.strokeColor = m_dimensionColor;
    areaMeas.filled = m_showAreaFill;
    return areaMeas;
}

void AdvancedMeasurement::addAreaMeasurement(const AreaMeasurement &area)
{
    m_areaMeasurements.append(area);
    emit areaMeasurementAdded(area);
}

AdvancedMeasurement::MeasurementAnnotation AdvancedMeasurement::createAnnotation(
    const QPointF &position, const QString &text, const QPointF &anchorPoint)
{
    MeasurementAnnotation annotation;
    annotation.position = position;
    annotation.text = text;
    annotation.color = m_dimensionColor;
    annotation.anchorPoint = anchorPoint;
    return annotation;
}

void AdvancedMeasurement::addAnnotation(const MeasurementAnnotation &annotation)
{
    m_annotations.append(annotation);
    emit annotationAdded(annotation);
}

QString AdvancedMeasurement::exportMeasurementsToText() const
{
    QString text;
    text += "=== Measurements ===\n\n";
    
    text += "Dimension Lines:\n";
    for (const DimensionLine &dim : m_dimensionLines) {
        text += QString("  %1 %2\n").arg(dim.measurement).arg(dim.unit);
    }
    
    text += "\nArea Measurements:\n";
    for (const AreaMeasurement &area : m_areaMeasurements) {
        text += QString("  %1 %2²\n").arg(area.area).arg(area.unit);
    }
    
    text += "\nAnnotations:\n";
    for (const MeasurementAnnotation &ann : m_annotations) {
        text += QString("  %1\n").arg(ann.text);
    }
    
    return text;
}

QString AdvancedMeasurement::exportMeasurementsToJSON() const
{
    QJsonObject root;
    
    QJsonArray dimensionLinesArray;
    for (const DimensionLine &dim : m_dimensionLines) {
        QJsonObject dimObj;
        dimObj["startX"] = dim.start.x();
        dimObj["startY"] = dim.start.y();
        dimObj["endX"] = dim.end.x();
        dimObj["endY"] = dim.end.y();
        dimObj["measurement"] = dim.measurement;
        dimObj["unit"] = dim.unit;
        dimensionLinesArray.append(dimObj);
    }
    root["dimensionLines"] = dimensionLinesArray;
    
    QJsonArray areaArray;
    for (const AreaMeasurement &area : m_areaMeasurements) {
        QJsonObject areaObj;
        areaObj["area"] = area.area;
        areaObj["unit"] = area.unit;
        QJsonArray pointsArray;
        for (const QPointF &point : area.points) {
            QJsonObject pointObj;
            pointObj["x"] = point.x();
            pointObj["y"] = point.y();
            pointsArray.append(pointObj);
        }
        areaObj["points"] = pointsArray;
        areaArray.append(areaObj);
    }
    root["areaMeasurements"] = areaArray;
    
    QJsonArray annotationsArray;
    for (const MeasurementAnnotation &ann : m_annotations) {
        QJsonObject annObj;
        annObj["x"] = ann.position.x();
        annObj["y"] = ann.position.y();
        annObj["text"] = ann.text;
        annotationsArray.append(annObj);
    }
    root["annotations"] = annotationsArray;
    
    QJsonDocument doc(root);
    return doc.toJson();
}

void AdvancedMeasurement::clearAllMeasurements()
{
    m_dimensionLines.clear();
    m_areaMeasurements.clear();
    m_annotations.clear();
}

QPointF AdvancedMeasurement::calculateArrowPoint(const QPointF &lineStart, const QPointF &lineEnd,
                                                 qreal arrowSize, bool atStart) const
{
    QPointF direction = lineEnd - lineStart;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    
    if (length < 1e-10)
        return lineStart;
    
    direction = direction / length;
    
    // Perpendicular direction for arrow
    QPointF perp(-direction.y(), direction.x());
    
    QPointF base = atStart ? lineStart : lineEnd;
    QPointF tip = base + direction * arrowSize;
    QPointF arrow1 = tip + perp * arrowSize * 0.5;
    QPointF arrow2 = tip - perp * arrowSize * 0.5;
    
    return tip; // Return tip point (would need to return all 3 points for full arrow)
}

