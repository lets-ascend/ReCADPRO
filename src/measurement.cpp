#include "measurement.h"
#include <QtMath>
#include <QDebug>

Measurement::Measurement(QObject *parent)
    : QObject(parent)
    , m_unit("mm")
    , m_scaleFactor(1.0)
    , m_pixelsPerUnit(3.779527559)  // Default: 1mm = ~3.78 pixels at 96 DPI
{
}

void Measurement::setUnit(const QString &unit)
{
    if (m_unit == unit)
        return;
    
    m_unit = unit;
    
    // Set default pixels per unit based on unit type
    if (unit == "mm") {
        m_pixelsPerUnit = 3.779527559;  // 96 DPI / 25.4 mm per inch
    } else if (unit == "cm") {
        m_pixelsPerUnit = 37.79527559;  // 10x mm
    } else if (unit == "m") {
        m_pixelsPerUnit = 3779.527559;  // 1000x mm
    } else if (unit == "in" || unit == "inch") {
        m_pixelsPerUnit = 96.0;  // 96 DPI
    } else if (unit == "ft" || unit == "feet") {
        m_pixelsPerUnit = 1152.0;  // 12 inches * 96 DPI
    } else {
        m_pixelsPerUnit = 1.0;  // Pixels
    }
    
    emit unitChanged();
    emit pixelsPerUnitChanged();
}

void Measurement::setScaleFactor(qreal factor)
{
    if (qFuzzyCompare(m_scaleFactor, factor))
        return;
    
    m_scaleFactor = factor;
    emit scaleFactorChanged();
}

void Measurement::setPixelsPerUnit(qreal ppu)
{
    if (qFuzzyCompare(m_pixelsPerUnit, ppu))
        return;
    
    m_pixelsPerUnit = ppu;
    emit pixelsPerUnitChanged();
}

qreal Measurement::measureDistance(const QPointF &p1, const QPointF &p2) const
{
    qreal dx = p2.x() - p1.x();
    qreal dy = p2.y() - p1.y();
    qreal pixelDistance = qSqrt(dx * dx + dy * dy);
    return convertToUnit(pixelDistance);
}

qreal Measurement::measureArea(const QList<QPointF> &points) const
{
    if (points.size() < 3)
        return 0.0;
    
    // Shoelace formula for polygon area
    qreal area = 0.0;
    for (int i = 0; i < points.size(); ++i) {
        int j = (i + 1) % points.size();
        area += points[i].x() * points[j].y();
        area -= points[j].x() * points[i].y();
    }
    area = qAbs(area) / 2.0;
    
    // Convert pixels^2 to units^2
    qreal unitArea = area / (m_pixelsPerUnit * m_pixelsPerUnit);
    return unitArea * m_scaleFactor * m_scaleFactor;
}

qreal Measurement::measureAngle(const QPointF &p1, const QPointF &center, const QPointF &p2) const
{
    QPointF v1 = p1 - center;
    QPointF v2 = p2 - center;
    
    qreal dot = v1.x() * v2.x() + v1.y() * v2.y();
    qreal mag1 = qSqrt(v1.x() * v1.x() + v1.y() * v1.y());
    qreal mag2 = qSqrt(v2.x() * v2.x() + v2.y() * v2.y());
    
    if (mag1 == 0 || mag2 == 0)
        return 0.0;
    
    qreal cosAngle = dot / (mag1 * mag2);
    cosAngle = qBound(-1.0, cosAngle, 1.0);
    
    qreal angle = qAcos(cosAngle) * 180.0 / M_PI;
    return angle;
}

QString Measurement::formatMeasurement(qreal value) const
{
    QString symbol = getUnitSymbol();
    return QString::number(value, 'f', 2) + " " + symbol;
}

qreal Measurement::pixelsToUnits(qreal pixels) const
{
    return convertToUnit(pixels);
}

qreal Measurement::unitsToPixels(qreal units) const
{
    return units * m_pixelsPerUnit / m_scaleFactor;
}

qreal Measurement::convertToUnit(qreal pixels) const
{
    return (pixels / m_pixelsPerUnit) * m_scaleFactor;
}

QString Measurement::getUnitSymbol() const
{
    if (m_unit == "mm") return "mm";
    if (m_unit == "cm") return "cm";
    if (m_unit == "m") return "m";
    if (m_unit == "in" || m_unit == "inch") return "in";
    if (m_unit == "ft" || m_unit == "feet") return "ft";
    return "px";
}

