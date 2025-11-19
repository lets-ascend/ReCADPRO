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
    void addImage(const QImage &image, const QPointF &position = QPointF(0, 0));
    
    // Stroke access for copy/paste
    struct StrokeData {
        QList<QPointF> points;
        QColor color;
        qreal opacity;
        qreal lineWidth;
    };
    Q_INVOKABLE QList<StrokeData> getStrokes(const QList<int> &indices) const;
    Q_INVOKABLE void addStrokes(const QList<StrokeData> &strokes);
    Q_INVOKABLE int strokeCount() const { return m_strokes.size(); }

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
        QRectF bounds; // Cached bounds for dirty region tracking
    };
    
    void addPoint(const QPointF &point, qreal pressure = 1.0);
    void drawStroke(QPainter *painter, const Stroke &stroke);
    void optimizeForEpaper(QPainter *painter);
    QRectF calculateStrokeBounds(const Stroke &stroke) const;
    QPointF smoothPoint(const QPointF &newPoint); // Input smoothing
    
    qreal m_lineWidth;
    QColor m_color;
    qreal m_opacity;
    bool m_pressureSensitive;
    
    bool m_isDrawing;
    QList<StrokePoint> m_currentStroke;
    QList<Stroke> m_strokes;
    QList<QImage> m_undoStack;
    QList<QImage> m_redoStack;
    
    // E-paper optimization: Dirty region tracking
    QRectF m_dirtyRegion;
    QPointF m_lastPoint; // For input smoothing
    qint64 m_lastUpdateTime; // For refresh rate limiting
    
    static const int MAX_UNDO_STEPS = 50;
    static const qint64 MIN_REFRESH_INTERVAL_MS = 33; // ~30fps max for e-paper
    static const qreal SMOOTHING_FACTOR = 0.3; // Input smoothing factor
};

#endif // DRAWINGENGINE_H

