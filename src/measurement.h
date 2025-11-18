#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QObject>
#include <QPointF>
#include <QString>

/**
 * Measurement - Measurement tools with scale support
 * Handles measurements in real-world units with scale factors
 */
class Measurement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(qreal scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(qreal pixelsPerUnit READ pixelsPerUnit WRITE setPixelsPerUnit NOTIFY pixelsPerUnitChanged)

public:
    explicit Measurement(QObject *parent = nullptr);
    
    enum Unit {
        Pixels,
        Millimeters,
        Centimeters,
        Meters,
        Inches,
        Feet
    };
    Q_ENUM(Unit)
    
    QString unit() const { return m_unit; }
    qreal scaleFactor() const { return m_scaleFactor; }
    qreal pixelsPerUnit() const { return m_pixelsPerUnit; }
    
    void setUnit(const QString &unit);
    void setScaleFactor(qreal factor);
    void setPixelsPerUnit(qreal ppu);
    
    Q_INVOKABLE qreal measureDistance(const QPointF &p1, const QPointF &p2) const;
    Q_INVOKABLE qreal measureArea(const QList<QPointF> &points) const;
    Q_INVOKABLE qreal measureAngle(const QPointF &p1, const QPointF &center, const QPointF &p2) const;
    Q_INVOKABLE QString formatMeasurement(qreal value) const;
    Q_INVOKABLE qreal pixelsToUnits(qreal pixels) const;
    Q_INVOKABLE qreal unitsToPixels(qreal units) const;

signals:
    void unitChanged();
    void scaleFactorChanged();
    void pixelsPerUnitChanged();

private:
    QString m_unit;
    qreal m_scaleFactor;      // e.g., 1:100 means 1 unit = 100 units
    qreal m_pixelsPerUnit;    // Pixels per unit (e.g., pixels per mm)
    
    qreal convertToUnit(qreal pixels) const;
    QString getUnitSymbol() const;
};

#endif // MEASUREMENT_H

