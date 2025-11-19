#include "layermanager.h"
#include <QDebug>
#include <QtGlobal>

LayerManager::LayerManager(QObject *parent)
    : QObject(parent)
    , m_layers(nullptr)
{
    m_compositor = new LayerCompositor(this);
    
    connect(m_compositor, &LayerCompositor::compositionProgress,
            this, &LayerManager::compositionProgress);
}

void LayerManager::setLayers(Layers *layers)
{
    if (m_layers == layers)
        return;
    
    if (m_layers) {
        disconnect(m_layers, nullptr, this, nullptr);
    }
    
    m_layers = layers;
    
    if (m_layers) {
        connect(m_layers, &Layers::layersChanged, this, [this]() {
            // Resize layer images list when layers change
            int count = m_layers->layerNames().size();
            while (m_layerImages.size() < count) {
                m_layerImages.append(QImage());
            }
            while (m_layerImages.size() > count) {
                m_layerImages.removeLast();
            }
        });
    }
    
    emit layersChanged();
}

void LayerManager::registerLayerCanvas(int layerIndex, QImage layerImage)
{
    // Validate layer index
    if (layerIndex < 0) {
        qWarning() << "LayerManager::registerLayerCanvas: Invalid negative index" << layerIndex;
        return;
    }
    
    // Limit maximum layers to prevent memory issues
    static const int MAX_LAYERS = 100;
    if (layerIndex >= MAX_LAYERS) {
        qWarning() << "LayerManager::registerLayerCanvas: Layer index exceeds maximum" << layerIndex;
        return;
    }
    
    // Limit image size to prevent memory issues
    static const int MAX_IMAGE_DIMENSION = 8000;
    if (layerImage.width() > MAX_IMAGE_DIMENSION || layerImage.height() > MAX_IMAGE_DIMENSION) {
        qWarning() << "LayerManager::registerLayerCanvas: Image too large, scaling down";
        QSize newSize = layerImage.size();
        newSize.scale(MAX_IMAGE_DIMENSION, MAX_IMAGE_DIMENSION, Qt::KeepAspectRatio);
        layerImage = layerImage.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    
    // Resize if needed
    if (layerIndex >= m_layerImages.size()) {
        while (m_layerImages.size() <= layerIndex) {
            m_layerImages.append(QImage());
        }
    }
    
    m_layerImages[layerIndex] = layerImage;
}

void LayerManager::updatePreview(const QSize &size)
{
    rebuildPreview(size);
}

void LayerManager::updatePreview()
{
    // Use default canvas size (1872x1404 for Paper Pro)
    rebuildPreview(QSize(1872, 1404));
}

QImage LayerManager::getCompositedImage(const QSize &size)
{
    rebuildPreview(size);
    return m_previewImage;
}

void LayerManager::rebuildPreview(const QSize &size)
{
    if (!m_layers || size.isEmpty())
        return;
    
    // Build layer data list
    QList<LayerCompositor::LayerData> layerDataList;
    
    int layerCount = m_layers->layerNames().size();
    for (int i = 0; i < layerCount; ++i) {
        LayerCompositor::LayerData data;
        
        // Get layer image (resize if needed)
        if (i < m_layerImages.size() && !m_layerImages[i].isNull()) {
            if (m_layerImages[i].size() != size) {
                data.image = m_layerImages[i].scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            } else {
                data.image = m_layerImages[i];
            }
        } else {
            // Create empty image for this layer
            data.image = QImage(size, QImage::Format_ARGB32);
            data.image.fill(Qt::transparent);
        }
        
        // Get layer properties
        data.visible = m_layers->isLayerVisible(i);
        data.opacity = m_layers->getLayerOpacity(i);
        data.blendMode = m_layers->getLayerBlendMode(i);
        
        layerDataList.append(data);
    }
    
    // Composite layers
    m_previewImage = m_compositor->compositeLayers(layerDataList, size);
    emit previewUpdated();
}

