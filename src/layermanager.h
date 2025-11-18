#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <QList>
#include <QImage>
#include <QSize>
#include "layers.h"
#include "layercompositor.h"

/**
 * LayerManager - Manages layer rendering and composition
 * Handles true color blending with opacity and blend modes
 */
class LayerManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Layers* layers READ layers WRITE setLayers NOTIFY layersChanged)
    Q_PROPERTY(QImage previewImage READ previewImage NOTIFY previewUpdated)

public:
    explicit LayerManager(QObject *parent = nullptr);
    
    Layers* layers() const { return m_layers; }
    void setLayers(Layers *layers);
    
    QImage previewImage() const { return m_previewImage; }
    
    // Register a layer canvas (each layer has its own DrawingEngine)
    Q_INVOKABLE void registerLayerCanvas(int layerIndex, QImage layerImage);
    
    // Update preview with current layer settings
    Q_INVOKABLE void updatePreview(const QSize &size);
    
    // Get composited result
    Q_INVOKABLE QImage getCompositedImage(const QSize &size);

signals:
    void layersChanged();
    void previewUpdated();
    void compositionProgress(int percent);

private:
    Layers *m_layers;
    LayerCompositor *m_compositor;
    QList<QImage> m_layerImages;  // One image per layer
    QImage m_previewImage;
    
    void rebuildPreview(const QSize &size);
};

#endif // LAYERMANAGER_H

