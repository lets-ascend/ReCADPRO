#include "layers.h"

Layers::Layers(QObject *parent)
    : QObject(parent)
    , m_activeLayerIndex(0)
{
    // Create initial layer
    addLayer("Layer 1");
}

void Layers::addLayer(const QString &name)
{
    QString layerName = name.isEmpty() ? generateLayerName() : name;
    m_layerNames.append(layerName);
    m_layerVisibility.append(true);
    m_layerOpacity.append(1.0);  // Full opacity
    m_layerBlendModes.append("normal");  // Default blend mode
    emit layersChanged();
}

void Layers::removeLayer(int index)
{
    if (index < 0 || index >= m_layerNames.size() || m_layerNames.size() <= 1)
        return;
    
    m_layerNames.removeAt(index);
    m_layerVisibility.removeAt(index);
    m_layerOpacity.removeAt(index);
    m_layerBlendModes.removeAt(index);
    
    if (m_activeLayerIndex >= m_layerNames.size()) {
        m_activeLayerIndex = m_layerNames.size() - 1;
    }
    
    emit layersChanged();
    emit activeLayerChanged();
}

void Layers::renameLayer(int index, const QString &name)
{
    if (index < 0 || index >= m_layerNames.size())
        return;
    
    m_layerNames[index] = name;
    emit layersChanged();
}

void Layers::setLayerVisible(int index, bool visible)
{
    if (index < 0 || index >= m_layerVisibility.size())
        return;
    
    m_layerVisibility[index] = visible;
    emit layersChanged();
}

bool Layers::isLayerVisible(int index) const
{
    if (index < 0 || index >= m_layerVisibility.size())
        return false;
    
    return m_layerVisibility[index];
}

void Layers::setActiveLayerIndex(int index)
{
    if (index < 0 || index >= m_layerNames.size())
        return;
    
    if (m_activeLayerIndex == index)
        return;
    
    m_activeLayerIndex = index;
    emit activeLayerChanged();
}

void Layers::setLayerOpacity(int index, qreal opacity)
{
    if (index < 0 || index >= m_layerOpacity.size())
        return;
    
    qreal clampedOpacity = qBound(0.0, opacity, 1.0);
    if (qFuzzyCompare(m_layerOpacity[index], clampedOpacity))
        return;
    
    m_layerOpacity[index] = clampedOpacity;
    emit layersChanged();
}

qreal Layers::getLayerOpacity(int index) const
{
    if (index < 0 || index >= m_layerOpacity.size())
        return 1.0;
    
    return m_layerOpacity[index];
}

void Layers::setLayerBlendMode(int index, const QString &blendMode)
{
    if (index < 0 || index >= m_layerBlendModes.size())
        return;
    
    // Valid blend modes: normal, multiply, screen, overlay, darken, lighten, color-dodge, color-burn, hard-light, soft-light, difference, exclusion
    QStringList validModes = {"normal", "multiply", "screen", "overlay", "darken", "lighten"};
    
    QString mode = blendMode.toLower();
    if (!validModes.contains(mode)) {
        mode = "normal";
    }
    
    if (m_layerBlendModes[index] == mode)
        return;
    
    m_layerBlendModes[index] = mode;
    emit layersChanged();
}

QString Layers::getLayerBlendMode(int index) const
{
    if (index < 0 || index >= m_layerBlendModes.size())
        return "normal";
    
    return m_layerBlendModes[index];
}

QString Layers::generateLayerName() const
{
    return QString("Layer %1").arg(m_layerNames.size() + 1);
}

