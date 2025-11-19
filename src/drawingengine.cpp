#include "drawingengine.h"
#include <QTouchEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtGlobal>
#include <QtMath>
#include <QDateTime>
#include <QRectF>

DrawingEngine::DrawingEngine(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_lineWidth(3.0)
    , m_color(Qt::black)
    , m_opacity(1.0)
    , m_pressureSensitive(true)
    , m_isDrawing(false)
    , m_lastUpdateTime(0)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptTouchEvents(true);
    setAntialiasing(true);
    
    // Optimize for e-paper display
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    
    // Initialize dirty region
    m_dirtyRegion = QRectF();
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
    m_dirtyRegion = QRectF();
    m_lastPoint = QPointF();
    update(); // Full refresh on clear
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
    
    // Store dirty region before clearing (for clipping)
    QRectF dirtyRect = m_dirtyRegion;
    bool isPartialRefresh = dirtyRect.isValid();
    
    // Clear dirty region after storing (will be set again if needed)
    m_dirtyRegion = QRectF();
    
    // E-paper optimization: Only draw strokes in dirty region (if partial refresh)
    // For full refresh, draw everything
    if (isPartialRefresh) {
        // Partial refresh: Set clip region and only draw intersecting strokes
        painter->setClipRect(dirtyRect);
        
        for (const Stroke &stroke : m_strokes) {
            // Skip strokes outside dirty region
            if (!stroke.bounds.isValid() || !stroke.bounds.intersects(dirtyRect)) {
                continue;
            }
            drawStroke(painter, stroke);
        }
        
        // Reset clip region for current stroke
        painter->setClipping(false);
    } else {
        // Full refresh: Draw all strokes
        for (const Stroke &stroke : m_strokes) {
            drawStroke(painter, stroke);
        }
    }
    
    // Draw current stroke being drawn (always draw, outside clip region)
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
            // Update is called inside addPoint() with refresh rate limiting
        }
        break;
        
    case QEvent::TouchEnd:
        if (m_isDrawing && !m_currentStroke.isEmpty()) {
            // Save current stroke
            Stroke stroke;
            stroke.points = m_currentStroke;
            stroke.color = m_color;
            stroke.opacity = m_opacity;
            // Calculate and cache stroke bounds for dirty region tracking
            stroke.bounds = calculateStrokeBounds(stroke);
            m_strokes.append(stroke);
            int strokeIndex = m_strokes.size() - 1;
            
            // Expand dirty region to include completed stroke
            if (m_dirtyRegion.isValid()) {
                m_dirtyRegion = m_dirtyRegion.united(stroke.bounds);
            } else {
                m_dirtyRegion = stroke.bounds;
            }
            
            // Save state for undo
            m_undoStack.append(getImage());
            if (m_undoStack.size() > MAX_UNDO_STEPS)
                m_undoStack.removeFirst();
            m_redoStack.clear();
            
            m_currentStroke.clear();
            m_isDrawing = false;
            m_lastPoint = QPointF(); // Reset smoothing
            
            // Emit stroke completed signal for layer management
            emit strokeCompleted(strokeIndex);
            
            // Final update with dirty region (partial refresh)
            update(m_dirtyRegion.toRect());
            m_dirtyRegion = QRectF(); // Clear after update
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
        // Update is called inside addPoint() with refresh rate limiting
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
    // Validate point (prevent NaN/Inf and extreme values)
    if (qIsNaN(point.x()) || qIsNaN(point.y()) ||
        qIsInf(point.x()) || qIsInf(point.y())) {
        qWarning() << "DrawingEngine::addPoint: Invalid point (NaN/Inf)";
        return;
    }
    
    // Clamp point to reasonable canvas bounds (with margin for off-screen drawing)
    QPointF clampedPoint(
        qBound(-1000.0, point.x(), boundingRect().width() + 1000.0),
        qBound(-1000.0, point.y(), boundingRect().height() + 1000.0)
    );
    
    // Input smoothing: Smooth the point using moving average
    QPointF smoothedPoint = smoothPoint(clampedPoint);
    
    // Validate pressure (should be 0.0-1.0, but allow slight overflow)
    qreal clampedPressure = qBound(0.0, pressure, 2.0);
    
    // Limit stroke point count to prevent memory issues
    static const int MAX_STROKE_POINTS = 10000;
    if (m_currentStroke.size() >= MAX_STROKE_POINTS) {
        qWarning() << "DrawingEngine::addPoint: Stroke point limit reached";
        return;
    }
    
    StrokePoint strokePoint;
    strokePoint.point = smoothedPoint;
    strokePoint.pressure = clampedPressure;
    
    if (m_pressureSensitive) {
        // Map pressure (0.0-1.0) to line width
        strokePoint.width = m_lineWidth * (0.5 + clampedPressure * 0.5);
    } else {
        strokePoint.width = m_lineWidth;
    }
    
    m_currentStroke.append(strokePoint);
    
    // E-paper optimization: Track dirty region for partial refresh
    qreal maxWidth = strokePoint.width;
    QRectF pointRect(smoothedPoint.x() - maxWidth, smoothedPoint.y() - maxWidth,
                     maxWidth * 2, maxWidth * 2);
    if (m_dirtyRegion.isValid()) {
        m_dirtyRegion = m_dirtyRegion.united(pointRect);
    } else {
        m_dirtyRegion = pointRect;
    }
    
    // Refresh rate limiting: Only update if enough time has passed
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastUpdateTime >= MIN_REFRESH_INTERVAL_MS) {
        // Partial update: Only refresh dirty region
        update(m_dirtyRegion.toRect());
        m_lastUpdateTime = currentTime;
    }
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

void DrawingEngine::addImage(const QImage &image, const QPointF &position)
{
    if (image.isNull())
        return;
    
    // Convert image to strokes (simplified - creates a single stroke per row)
    // In production, you might want to add image as a background layer instead
    QColor imageColor;
    QList<StrokePoint> imageStroke;
    
    // Sample image and create strokes
    // For now, add image as a background by drawing it directly
    // Save current state
    m_undoStack.append(getImage());
    if (m_undoStack.size() > MAX_UNDO_STEPS)
        m_undoStack.removeFirst();
    
    // Create a stroke that represents the image bounds
    // In a full implementation, you'd add the image to a background layer
    // For now, we'll add it as a filled rectangle stroke
    Stroke imageStrokeData;
    imageStrokeData.color = Qt::black; // Default
    imageStrokeData.opacity = 1.0;
    
    // Add corner points to represent image
    QPointF topLeft = position;
    QPointF topRight = position + QPointF(image.width(), 0);
    QPointF bottomRight = position + QPointF(image.width(), image.height());
    QPointF bottomLeft = position + QPointF(0, image.height());
    
    StrokePoint pt;
    pt.pressure = 1.0;
    pt.width = 1.0;
    
    pt.point = topLeft;
    imageStrokeData.points.append(pt);
    pt.point = topRight;
    imageStrokeData.points.append(pt);
    pt.point = bottomRight;
    imageStrokeData.points.append(pt);
    pt.point = bottomLeft;
    imageStrokeData.points.append(pt);
    pt.point = topLeft; // Close
    imageStrokeData.points.append(pt);
    
    // Calculate and cache bounds
    imageStrokeData.bounds = calculateStrokeBounds(imageStrokeData);
    
    m_strokes.append(imageStrokeData);
    m_redoStack.clear();
    
    // Partial update with bounds
    if (imageStrokeData.bounds.isValid()) {
        update(imageStrokeData.bounds.toRect());
    } else {
        update();
    }
}

QList<DrawingEngine::StrokeData> DrawingEngine::getStrokes(const QList<int> &indices) const
{
    QList<StrokeData> result;
    
    // Limit number of strokes to prevent memory issues
    static const int MAX_STROKES_TO_COPY = 1000;
    int strokeCount = qMin(indices.size(), MAX_STROKES_TO_COPY);
    
    for (int i = 0; i < strokeCount; ++i) {
        int index = indices[i];
        
        // Validate index bounds
        if (index < 0 || index >= m_strokes.size()) {
            qWarning() << "DrawingEngine::getStrokes: Invalid index" << index << "out of" << m_strokes.size();
            continue;
        }
        
        const Stroke &stroke = m_strokes[index];
        StrokeData data;
        data.color = stroke.color;
        data.opacity = qBound(0.0, stroke.opacity, 1.0); // Clamp opacity
        
        // Get average line width from stroke points
        qreal avgWidth = 0;
        int pointCount = qMin(stroke.points.size(), 10000); // Limit point processing
        
        for (int j = 0; j < pointCount; ++j) {
            const StrokePoint &pt = stroke.points[j];
            data.points.append(pt.point);
            avgWidth += pt.width;
        }
        
        if (pointCount > 0) {
            data.lineWidth = avgWidth / pointCount;
        } else {
            data.lineWidth = m_lineWidth;
        }
        
        // Validate line width
        data.lineWidth = qBound(0.1, data.lineWidth, 100.0);
        
        result.append(data);
    }
    
    return result;
}

void DrawingEngine::addStrokes(const QList<StrokeData> &strokes)
{
    if (strokes.isEmpty())
        return;
    
    // Save current state
    m_undoStack.append(getImage());
    if (m_undoStack.size() > MAX_UNDO_STEPS)
        m_undoStack.removeFirst();
    
    // Track dirty region for all added strokes
    QRectF addedBounds;
    
    for (const StrokeData &data : strokes) {
        Stroke stroke;
        stroke.color = data.color;
        stroke.opacity = data.opacity;
        
        for (const QPointF &point : data.points) {
            StrokePoint strokePoint;
            strokePoint.point = point;
            strokePoint.pressure = 1.0;
            strokePoint.width = data.lineWidth > 0 ? data.lineWidth : m_lineWidth;
            stroke.points.append(strokePoint);
        }
        
        // Calculate and cache bounds
        stroke.bounds = calculateStrokeBounds(stroke);
        
        // Accumulate dirty region
        if (addedBounds.isValid()) {
            addedBounds = addedBounds.united(stroke.bounds);
        } else {
            addedBounds = stroke.bounds;
        }
        
        m_strokes.append(stroke);
    }
    
    m_redoStack.clear();
    
    // Partial update with dirty region
    if (addedBounds.isValid()) {
        update(addedBounds.toRect());
    } else {
        update();
    }
}

QRectF DrawingEngine::calculateStrokeBounds(const Stroke &stroke) const
{
    if (stroke.points.isEmpty())
        return QRectF();
    
    qreal minX = stroke.points[0].point.x();
    qreal minY = stroke.points[0].point.y();
    qreal maxX = minX;
    qreal maxY = minY;
    qreal maxWidth = stroke.points[0].width;
    
    for (const StrokePoint &pt : stroke.points) {
        minX = qMin(minX, pt.point.x());
        minY = qMin(minY, pt.point.y());
        maxX = qMax(maxX, pt.point.x());
        maxY = qMax(maxY, pt.point.y());
        maxWidth = qMax(maxWidth, pt.width);
    }
    
    // Add padding for line width
    qreal padding = maxWidth / 2.0;
    return QRectF(minX - padding, minY - padding,
                  maxX - minX + padding * 2,
                  maxY - minY + padding * 2);
}

QPointF DrawingEngine::smoothPoint(const QPointF &newPoint)
{
    // Input smoothing using exponential moving average
    if (m_lastPoint.isNull()) {
        m_lastPoint = newPoint;
        return newPoint;
    }
    
    // Smooth the point
    QPointF smoothed(
        m_lastPoint.x() + (newPoint.x() - m_lastPoint.x()) * SMOOTHING_FACTOR,
        m_lastPoint.y() + (newPoint.y() - m_lastPoint.y()) * SMOOTHING_FACTOR
    );
    
    m_lastPoint = smoothed;
    return smoothed;
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
    
    // Clip region is set in paint() function for partial refresh optimization
}

