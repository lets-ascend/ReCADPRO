#ifndef DRAWINGENGINE_H
#define DRAWINGENGINE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QPointF>
#include <QColor>
#include <QImage>
#include <QList>
#include <QTouchEvent>
#include <QMouseEvent>

/**
 * DrawingEngine - Core drawing functionality for reMarkable Paper Pro
 * Handles pressure-sensitive drawing optimized for e-paper display
 */
class DrawingEngine : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(bool pressureSensitive READ pressureSensitive WRITE setPressureSensitive NOTIFY pressureSensitiveChanged)

public:
    explicit DrawingEngine(QQuickItem *parent = nullptr);
    
    // Property getters
    qreal lineWidth() const { return m_lineWidth; }
    QColor color() const { return m_color; }
    qreal opacity() const { return m_opacity; }
    bool pressureSensitive() const { return m_pressureSensitive; }
    
    // Property setters
    void setLineWidth(qreal width);
    void setColor(const QColor &color);
    void setOpacity(qreal opacity);
    void setPressureSensitive(bool enabled);

public slots:
    void clear();
    void undo();
    void redo();
    QImage getImage() const;
    void setImage(const QImage &image);

signals:
    void lineWidthChanged();
    void colorChanged();
    void opacityChanged();
    void pressureSensitiveChanged();

protected:
    void paint(QPainter *painter) override;
    
    // Touch/stylus input handling
    void touchEvent(QTouchEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    struct StrokePoint {
        QPointF point;
        qreal pressure;
        qreal width;
    };
    
    struct Stroke {
        QList<StrokePoint> points;
        QColor color;
        qreal opacity;
    };
    
    void addPoint(const QPointF &point, qreal pressure = 1.0);
    void drawStroke(QPainter *painter, const Stroke &stroke);
    void optimizeForEpaper(QPainter *painter);
    
    qreal m_lineWidth;
    QColor m_color;
    qreal m_opacity;
    bool m_pressureSensitive;
    
    bool m_isDrawing;
    QList<StrokePoint> m_currentStroke;
    QList<Stroke> m_strokes;
    QList<QImage> m_undoStack;
    QList<QImage> m_redoStack;
    
    static const int MAX_UNDO_STEPS = 50;
};

#endif // DRAWINGENGINE_H

