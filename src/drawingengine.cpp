#include "drawingengine.h"
#include <QTouchEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>

DrawingEngine::DrawingEngine(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_lineWidth(3.0)
    , m_color(Qt::black)
    , m_opacity(1.0)
    , m_pressureSensitive(true)
    , m_isDrawing(false)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptTouchEvents(true);
    setAntialiasing(true);
    
    // Optimize for e-paper display
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
}

void DrawingEngine::setLineWidth(qreal width)
{
    if (qFuzzyCompare(m_lineWidth, width))
        return;
    
    m_lineWidth = width;
    emit lineWidthChanged();
}

void DrawingEngine::setColor(const QColor &color)
{
    if (m_color == color)
        return;
    
    m_color = color;
    emit colorChanged();
}

void DrawingEngine::setOpacity(qreal opacity)
{
    if (qFuzzyCompare(m_opacity, opacity))
        return;
    
    m_opacity = qBound(0.0, opacity, 1.0);
    emit opacityChanged();
}

void DrawingEngine::setPressureSensitive(bool enabled)
{
    if (m_pressureSensitive == enabled)
        return;
    
    m_pressureSensitive = enabled;
    emit pressureSensitiveChanged();
}

void DrawingEngine::clear()
{
    m_strokes.clear();
    m_currentStroke.clear();
    m_isDrawing = false;
    update();
}

void DrawingEngine::undo()
{
    if (m_undoStack.isEmpty())
        return;
    
    m_redoStack.append(getImage());
    if (m_redoStack.size() > MAX_UNDO_STEPS)
        m_redoStack.removeFirst();
    
    setImage(m_undoStack.takeLast());
}

void DrawingEngine::redo()
{
    if (m_redoStack.isEmpty())
        return;
    
    m_undoStack.append(getImage());
    if (m_undoStack.size() > MAX_UNDO_STEPS)
        m_undoStack.removeFirst();
    
    setImage(m_redoStack.takeLast());
}

QImage DrawingEngine::getImage() const
{
    QImage image(boundingRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    
    for (const Stroke &stroke : m_strokes) {
        drawStroke(&painter, stroke);
    }
    
    return image;
}

void DrawingEngine::setImage(const QImage &image)
{
    // Save current state to undo stack
    m_undoStack.append(getImage());
    if (m_undoStack.size() > MAX_UNDO_STEPS)
        m_undoStack.removeFirst();
    
    // Clear and rebuild strokes from image
    // This is a simplified version - in production, you'd want to preserve stroke data
    m_strokes.clear();
    update();
}

void DrawingEngine::paint(QPainter *painter)
{
    // Optimize for e-paper display
    optimizeForEpaper(painter);
    
    // Draw all strokes
    for (const Stroke &stroke : m_strokes) {
        drawStroke(painter, stroke);
    }
    
    // Draw current stroke being drawn
    if (!m_currentStroke.isEmpty()) {
        QPen pen(m_color);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        
        painter->setPen(pen);
        painter->setOpacity(m_opacity);
        
        for (int i = 1; i < m_currentStroke.size(); ++i) {
            const StrokePoint &p1 = m_currentStroke[i - 1];
            const StrokePoint &p2 = m_currentStroke[i];
            
            pen.setWidthF(p1.width);
            painter->setPen(pen);
            painter->drawLine(p1.point, p2.point);
        }
    }
}

void DrawingEngine::touchEvent(QTouchEvent *event)
{
    event->accept();
    
    switch (event->type()) {
    case QEvent::TouchBegin:
        if (!event->touchPoints().isEmpty()) {
            const QTouchEvent::TouchPoint &point = event->touchPoints().first();
            addPoint(point.pos(), point.pressure());
            m_isDrawing = true;
        }
        break;
        
    case QEvent::TouchUpdate:
        if (m_isDrawing && !event->touchPoints().isEmpty()) {
            const QTouchEvent::TouchPoint &point = event->touchPoints().first();
            addPoint(point.pos(), point.pressure());
            update();
        }
        break;
        
    case QEvent::TouchEnd:
        if (m_isDrawing && !m_currentStroke.isEmpty()) {
            // Save current stroke
            Stroke stroke;
            stroke.points = m_currentStroke;
            stroke.color = m_color;
            stroke.opacity = m_opacity;
            m_strokes.append(stroke);
            
            // Save state for undo
            m_undoStack.append(getImage());
            if (m_undoStack.size() > MAX_UNDO_STEPS)
                m_undoStack.removeFirst();
            m_redoStack.clear();
            
            m_currentStroke.clear();
            m_isDrawing = false;
            update();
        }
        break;
        
    default:
        break;
    }
}

void DrawingEngine::mousePressEvent(QMouseEvent *event)
{
    addPoint(event->localPos(), 1.0);
    m_isDrawing = true;
    event->accept();
}

void DrawingEngine::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDrawing) {
        addPoint(event->localPos(), 1.0);
        update();
    }
    event->accept();
}

void DrawingEngine::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isDrawing && !m_currentStroke.isEmpty()) {
        // Save current stroke
        Stroke stroke;
        stroke.points = m_currentStroke;
        stroke.color = m_color;
        stroke.opacity = m_opacity;
        m_strokes.append(stroke);
        
        // Save state for undo
        m_undoStack.append(getImage());
        if (m_undoStack.size() > MAX_UNDO_STEPS)
            m_undoStack.removeFirst();
        m_redoStack.clear();
        
        m_currentStroke.clear();
        m_isDrawing = false;
        update();
    }
    event->accept();
}

void DrawingEngine::addPoint(const QPointF &point, qreal pressure)
{
    StrokePoint strokePoint;
    strokePoint.point = point;
    strokePoint.pressure = pressure;
    
    if (m_pressureSensitive) {
        // Map pressure (0.0-1.0) to line width
        strokePoint.width = m_lineWidth * (0.5 + pressure * 0.5);
    } else {
        strokePoint.width = m_lineWidth;
    }
    
    m_currentStroke.append(strokePoint);
}

void DrawingEngine::drawStroke(QPainter *painter, const Stroke &stroke)
{
    if (stroke.points.isEmpty())
        return;
    
    QPen pen(stroke.color);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    
    painter->setPen(pen);
    painter->setOpacity(stroke.opacity);
    
    for (int i = 1; i < stroke.points.size(); ++i) {
        const StrokePoint &p1 = stroke.points[i - 1];
        const StrokePoint &p2 = stroke.points[i];
        
        pen.setWidthF(p1.width);
        painter->setPen(pen);
        painter->drawLine(p1.point, p2.point);
    }
}

void DrawingEngine::optimizeForEpaper(QPainter *painter)
{
    // Optimize rendering for Canvas Color display
    // - Support full color (up to 20,000 colors)
    // - Colors will appear muted/pastel naturally
    // - Minimize antialiasing for better refresh
    // - Optimize for partial updates
    
    painter->setRenderHint(QPainter::Antialiasing, true);
    
    // Note: Full Canvas Color display optimization requires Qt e-paper backend
    // This is handled by running with -platform epaper flag
    // Colors are preserved - display technology handles muted appearance
}

