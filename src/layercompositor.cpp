#include "layercompositor.h"
#include <QtMath>
#include <QDebug>

LayerCompositor::LayerCompositor(QObject *parent)
    : QObject(parent)
{
}

QImage LayerCompositor::compositeLayers(const QList<LayerData> &layers, const QSize &size)
{
    if (layers.isEmpty() || size.isEmpty())
        return QImage();
    
    // Create result image (white background)
    QImage result(size, QImage::Format_ARGB32);
    result.fill(Qt::white);
    
    // Composite each layer
    for (int i = 0; i < layers.size(); ++i) {
        const LayerData &layer = layers[i];
        
        // Skip invisible layers
        if (!layer.visible || layer.opacity <= 0.0)
            continue;
        
        // Skip if image is null or wrong size
        if (layer.image.isNull() || layer.image.size() != size)
            continue;
        
        // Composite this layer onto result
        for (int y = 0; y < size.height(); ++y) {
            for (int x = 0; x < size.width(); ++x) {
                QColor sourceColor = layer.image.pixelColor(x, y);
                QColor destColor = result.pixelColor(x, y);
                
                // Skip transparent pixels
                if (sourceColor.alpha() == 0)
                    continue;
                
                // Apply blend mode
                QColor blended = applyBlendMode(sourceColor, destColor, layer.blendMode);
                
                // Apply layer opacity
                if (layer.opacity < 1.0) {
                    blended = QColor::fromRgbF(
                        lerp(destColor.redF(), blended.redF(), layer.opacity),
                        lerp(destColor.greenF(), blended.greenF(), layer.opacity),
                        lerp(destColor.blueF(), blended.blueF(), layer.opacity),
                        lerp(destColor.alphaF(), blended.alphaF(), layer.opacity)
                    );
                }
                
                result.setPixelColor(x, y, blended);
            }
            
            // Emit progress (every 10% of height)
            if (y % (size.height() / 10) == 0) {
                int percent = ((i * size.height() + y) * 100) / (layers.size() * size.height());
                emit compositionProgress(percent);
            }
        }
    }
    
    emit compositionProgress(100);
    return result;
}

QColor LayerCompositor::applyBlendMode(const QColor &source, const QColor &destination, const QString &blendMode)
{
    QString mode = blendMode.toLower();
    
    if (mode == "normal") {
        return blendNormal(source, destination, 1.0);
    } else if (mode == "multiply") {
        return blendMultiply(source, destination, 1.0);
    } else if (mode == "screen") {
        return blendScreen(source, destination, 1.0);
    } else if (mode == "overlay") {
        return blendOverlay(source, destination, 1.0);
    } else if (mode == "darken") {
        return blendDarken(source, destination, 1.0);
    } else if (mode == "lighten") {
        return blendLighten(source, destination, 1.0);
    }
    
    // Default to normal
    return blendNormal(source, destination, 1.0);
}

QStringList LayerCompositor::supportedBlendModes()
{
    return QStringList() << "normal" << "multiply" << "screen" << "overlay" << "darken" << "lighten";
}

QColor LayerCompositor::blendNormal(const QColor &source, const QColor &destination, qreal opacity)
{
    Q_UNUSED(destination);
    return source;
}

QColor LayerCompositor::blendMultiply(const QColor &source, const QColor &destination, qreal opacity)
{
    // Multiply: result = source * destination
    qreal r = clamp(source.redF() * destination.redF());
    qreal g = clamp(source.greenF() * destination.greenF());
    qreal b = clamp(source.blueF() * destination.blueF());
    qreal a = qMax(source.alphaF(), destination.alphaF());
    
    return QColor::fromRgbF(r, g, b, a);
}

QColor LayerCompositor::blendScreen(const QColor &source, const QColor &destination, qreal opacity)
{
    // Screen: result = 1 - (1 - source) * (1 - destination)
    qreal r = clamp(1.0 - (1.0 - source.redF()) * (1.0 - destination.redF()));
    qreal g = clamp(1.0 - (1.0 - source.greenF()) * (1.0 - destination.greenF()));
    qreal b = clamp(1.0 - (1.0 - source.blueF()) * (1.0 - destination.blueF()));
    qreal a = qMax(source.alphaF(), destination.alphaF());
    
    return QColor::fromRgbF(r, g, b, a);
}

QColor LayerCompositor::blendOverlay(const QColor &source, const QColor &destination, qreal opacity)
{
    // Overlay: combines multiply and screen based on destination
    qreal r, g, b;
    
    if (destination.redF() < 0.5) {
        r = clamp(2.0 * source.redF() * destination.redF());
    } else {
        r = clamp(1.0 - 2.0 * (1.0 - source.redF()) * (1.0 - destination.redF()));
    }
    
    if (destination.greenF() < 0.5) {
        g = clamp(2.0 * source.greenF() * destination.greenF());
    } else {
        g = clamp(1.0 - 2.0 * (1.0 - source.greenF()) * (1.0 - destination.greenF()));
    }
    
    if (destination.blueF() < 0.5) {
        b = clamp(2.0 * source.blueF() * destination.blueF());
    } else {
        b = clamp(1.0 - 2.0 * (1.0 - source.blueF()) * (1.0 - destination.blueF()));
    }
    
    qreal a = qMax(source.alphaF(), destination.alphaF());
    return QColor::fromRgbF(r, g, b, a);
}

QColor LayerCompositor::blendDarken(const QColor &source, const QColor &destination, qreal opacity)
{
    // Darken: takes the darker of the two colors
    qreal r = qMin(source.redF(), destination.redF());
    qreal g = qMin(source.greenF(), destination.greenF());
    qreal b = qMin(source.blueF(), destination.blueF());
    qreal a = qMax(source.alphaF(), destination.alphaF());
    
    return QColor::fromRgbF(r, g, b, a);
}

QColor LayerCompositor::blendLighten(const QColor &source, const QColor &destination, qreal opacity)
{
    // Lighten: takes the lighter of the two colors
    qreal r = qMax(source.redF(), destination.redF());
    qreal g = qMax(source.greenF(), destination.greenF());
    qreal b = qMax(source.blueF(), destination.blueF());
    qreal a = qMax(source.alphaF(), destination.alphaF());
    
    return QColor::fromRgbF(r, g, b, a);
}

qreal LayerCompositor::clamp(qreal value)
{
    return qBound(0.0, value, 1.0);
}

qreal LayerCompositor::lerp(qreal a, qreal b, qreal t)
{
    return a + (b - a) * t;
}

