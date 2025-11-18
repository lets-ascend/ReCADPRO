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
    
    // Shape data for rendering
    struct ShapeData {
        QString type;
        QPointF startPoint;
        QPointF endPoint;
        QColor strokeColor;
        QColor fillColor;
        qreal lineWidth;
        bool filled;
    };
    
    Q_INVOKABLE ShapeData getCurrentShape() const { return m_currentShapeData; }
    Q_INVOKABLE QList<ShapeData> getAllShapes() const { return m_shapes; }

signals:
    void currentShapeChanged();
    void fillEnabledChanged();
    void fillColorChanged();
    void shapeStarted();
    void shapeUpdated();
    void shapeFinished(const ShapeData &shape);

private:
    QString m_currentShape; // "rectangle", "circle", "arrow", "polygon", "line"
    bool m_fillEnabled;
    QColor m_fillColor;
    
    ShapeData m_currentShapeData;
    QList<ShapeData> m_shapes;
    bool m_isDrawing;
    
    void drawRectangle(QPainter *painter, const QPointF &start, const QPointF &end, bool filled);
    void drawCircle(QPainter *painter, const QPointF &center, qreal radius, bool filled);
    void drawArrow(QPainter *painter, const QPointF &start, const QPointF &end);
    void drawLine(QPainter *painter, const QPointF &start, const QPointF &end);
};

#endif // SHAPETOOLS_H

