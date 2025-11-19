#ifndef SHAPETOOLS_H
#define SHAPETOOLS_H

#include <QObject>
#include <QPointF>
#include <QColor>
#include <QList>
#include <QPolygonF>

class QPainter;

/**
 * ShapeTools - Shape drawing tools (rectangle, circle, arrow, polygon)
 * Handles shape creation and rendering
 */
class ShapeTools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentShape READ currentShape WRITE setCurrentShape NOTIFY currentShapeChanged)
    Q_PROPERTY(bool fillEnabled READ fillEnabled WRITE setFillEnabled NOTIFY fillEnabledChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)

public:
    explicit ShapeTools(QObject *parent = nullptr);
    
    QString currentShape() const { return m_currentShape; }
    bool fillEnabled() const { return m_fillEnabled; }
    QColor fillColor() const { return m_fillColor; }
    
    void setCurrentShape(const QString &shape);
    void setFillEnabled(bool enabled);
    void setFillColor(const QColor &color);
    
    // Shape creation
    Q_INVOKABLE void startShape(const QPointF &startPoint);
    Q_INVOKABLE void updateShape(const QPointF &currentPoint);
    Q_INVOKABLE void finishShape();
    
    // Arc creation methods
    Q_PROPERTY(QString arcMode READ arcMode WRITE setArcMode NOTIFY arcModeChanged)
    QString arcMode() const { return m_arcMode; }
    void setArcMode(const QString &mode); // "3point", "center-radius", "start-end-radius"
    
    // Arc-specific methods
    Q_INVOKABLE void startArc3Point(const QPointF &startPoint);
    Q_INVOKABLE void setArc3PointMiddle(const QPointF &middlePoint);
    Q_INVOKABLE void finishArc3Point(const QPointF &endPoint);
    
    Q_INVOKABLE void startArcCenterRadius(const QPointF &center);
    Q_INVOKABLE void setArcRadius(const QPointF &radiusPoint);
    Q_INVOKABLE void setArcStartAngle(const QPointF &startAnglePoint);
    Q_INVOKABLE void finishArcCenterRadius(const QPointF &endAnglePoint);
    
    Q_INVOKABLE void startArcStartEndRadius(const QPointF &startPoint);
    Q_INVOKABLE void setArcEndPoint(const QPointF &endPoint);
    Q_INVOKABLE void finishArcStartEndRadius(const QPointF &radiusPoint);
    
    // Shape data for rendering
    struct ShapeData {
        QString type;
        QPointF startPoint;
        QPointF endPoint;
        QPointF centerPoint;      // For arcs/circles
        QPointF middlePoint;      // For 3-point arc
        qreal radius;            // For arcs/circles
        qreal startAngle;        // For arcs (degrees)
        qreal endAngle;          // For arcs (degrees)
        QColor strokeColor;
        QColor fillColor;
        qreal lineWidth;
        bool filled;
    };
    
    Q_INVOKABLE ShapeData getCurrentShape() const { return m_currentShapeData; }
    Q_INVOKABLE QList<ShapeData> getAllShapes() const { return m_shapes; }

signals:
    void currentShapeChanged();
    void arcModeChanged();
    void fillEnabledChanged();
    void fillColorChanged();
    void shapeStarted();
    void shapeUpdated();
    void shapeFinished(const ShapeData &shape);
    void arcStepChanged(int step); // For multi-step arc creation

private:
    QString m_currentShape; // "rectangle", "circle", "arrow", "polygon", "line", "arc"
    QString m_arcMode; // "3point", "center-radius", "start-end-radius"
    bool m_fillEnabled;
    QColor m_fillColor;
    
    ShapeData m_currentShapeData;
    QList<ShapeData> m_shapes;
    bool m_isDrawing;
    int m_arcStep; // Current step in arc creation (0, 1, 2, etc.)
    
    void drawRectangle(QPainter *painter, const QPointF &start, const QPointF &end, bool filled);
    void drawCircle(QPainter *painter, const QPointF &center, qreal radius, bool filled);
    void drawArc(QPainter *painter, const QPointF &center, qreal radius, qreal startAngle, qreal endAngle);
    void drawArrow(QPainter *painter, const QPointF &start, const QPointF &end);
    void drawLine(QPainter *painter, const QPointF &start, const QPointF &end);
    
    // Arc calculation helpers
    QPointF calculateArcCenter3Point(const QPointF &p1, const QPointF &p2, const QPointF &p3) const;
    qreal calculateAngle(const QPointF &center, const QPointF &point) const;
    qreal normalizeAngle(qreal angle) const;
};

#endif // SHAPETOOLS_H

