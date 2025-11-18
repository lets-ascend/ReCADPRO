#ifndef LAYERCOMPOSITOR_H
#define LAYERCOMPOSITOR_H

#include <QObject>
#include <QImage>
#include <QList>
#include <QString>

/**
 * LayerCompositor - Composites multiple layers with blend modes and opacity
 * Implements true color blending for reMarkable Paper Pro
 */
class LayerCompositor : public QObject
{
    Q_OBJECT

public:
    explicit LayerCompositor(QObject *parent = nullptr);
    
    struct LayerData {
        QImage image;
        bool visible;
        qreal opacity;
        QString blendMode;
    };
    
    // Composite multiple layers into final image
    Q_INVOKABLE QImage compositeLayers(const QList<LayerData> &layers, const QSize &size);
    
    // Apply blend mode to two colors
    static QColor applyBlendMode(const QColor &source, const QColor &destination, const QString &blendMode);
    
    // Blend modes supported
    static QStringList supportedBlendModes();

signals:
    void compositionProgress(int percent);

private:
    // Individual blend mode implementations
    static QColor blendNormal(const QColor &source, const QColor &destination, qreal opacity);
    static QColor blendMultiply(const QColor &source, const QColor &destination, qreal opacity);
    static QColor blendScreen(const QColor &source, const QColor &destination, qreal opacity);
    static QColor blendOverlay(const QColor &source, const QColor &destination, qreal opacity);
    static QColor blendDarken(const QColor &source, const QColor &destination, qreal opacity);
    static QColor blendLighten(const QColor &source, const QColor &destination, qreal opacity);
    
    // Helper functions
    static qreal clamp(qreal value);
    static qreal lerp(qreal a, qreal b, qreal t);
};

#endif // LAYERCOMPOSITOR_H

