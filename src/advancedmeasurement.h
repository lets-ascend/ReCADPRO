#ifndef ADVANCEDMEASUREMENT_H
#define ADVANCEDMEASUREMENT_H

#include <QObject>
#include <QPointF>
#include <QList>
#include <QString>
#include <QColor>
#include <QRectF>

/**
 * AdvancedMeasurement - Advanced measurement tools with dimension lines
 * Dimension lines with arrows, area visualization, annotations, export
 */
class AdvancedMeasurement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool showDimensionLines READ showDimensionLines WRITE setShowDimensionLines NOTIFY showDimensionLinesChanged)
    Q_PROPERTY(bool showAreaFill READ showAreaFill WRITE setShowAreaFill NOTIFY showAreaFillChanged)
    Q_PROPERTY(QColor dimensionColor READ dimensionColor WRITE setDimensionColor NOTIFY dimensionColorChanged)

public:
    explicit AdvancedMeasurement(QObject *parent = nullptr);
    
    bool showDimensionLines() const { return m_showDimensionLines; }
    bool showAreaFill() const { return m_showAreaFill; }
    QColor dimensionColor() const { return m_dimensionColor; }
    
    void setShowDimensionLines(bool show);
    void setShowAreaFill(bool show);
    void setDimensionColor(const QColor &color);
    
    // Dimension line structure
    struct DimensionLine {
        QPointF start;
        QPointF end;
        QString measurement;
        QString unit;
        QColor color;
        bool showArrows;
        qreal arrowSize;
    };
    
    // Area measurement structure
    struct AreaMeasurement {
        QList<QPointF> points;
        qreal area;
        QString unit;
        QColor fillColor;
        QColor strokeColor;
        bool filled;
    };
    
    // Measurement annotation
    struct MeasurementAnnotation {
        QPointF position;
        QString text;
        QString unit;
        QColor color;
        QPointF anchorPoint;  // Point being annotated
    };
    
    // Dimension lines
    Q_INVOKABLE DimensionLine createDimensionLine(const QPointF &start, const QPointF &end, 
                                                  const QString &measurement, const QString &unit);
    Q_INVOKABLE QList<QPointF> getDimensionLineArrows(const DimensionLine &dimension) const;
    Q_INVOKABLE void addDimensionLine(const DimensionLine &dimension);
    Q_INVOKABLE QList<DimensionLine> getAllDimensionLines() const { return m_dimensionLines; }
    
    // Area visualization
    Q_INVOKABLE AreaMeasurement createAreaMeasurement(const QList<QPointF> &points, 
                                                       qreal area, const QString &unit);
    Q_INVOKABLE void addAreaMeasurement(const AreaMeasurement &area);
    Q_INVOKABLE QList<AreaMeasurement> getAllAreaMeasurements() const { return m_areaMeasurements; }
    
    // Annotations
    Q_INVOKABLE MeasurementAnnotation createAnnotation(const QPointF &position, 
                                                       const QString &text,
                                                       const QPointF &anchorPoint);
    Q_INVOKABLE void addAnnotation(const MeasurementAnnotation &annotation);
    Q_INVOKABLE QList<MeasurementAnnotation> getAllAnnotations() const { return m_annotations; }
    
    // Export measurements
    Q_INVOKABLE QString exportMeasurementsToText() const;
    Q_INVOKABLE QString exportMeasurementsToJSON() const;
    Q_INVOKABLE void clearAllMeasurements();

signals:
    void showDimensionLinesChanged();
    void showAreaFillChanged();
    void dimensionColorChanged();
    void dimensionLineAdded(const DimensionLine &dimension);
    void areaMeasurementAdded(const AreaMeasurement &area);
    void annotationAdded(const MeasurementAnnotation &annotation);

private:
    bool m_showDimensionLines;
    bool m_showAreaFill;
    QColor m_dimensionColor;
    
    QList<DimensionLine> m_dimensionLines;
    QList<AreaMeasurement> m_areaMeasurements;
    QList<MeasurementAnnotation> m_annotations;
    
    QPointF calculateArrowPoint(const QPointF &lineStart, const QPointF &lineEnd, 
                               qreal arrowSize, bool atStart) const;
};

#endif // ADVANCEDMEASUREMENT_H

