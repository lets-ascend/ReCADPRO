#ifndef LAYERS_H
#define LAYERS_H

#include <QObject>
#include <QStringList>
#include <QList>

/**
 * Layers - Layer management for reMarkable Paper Pro
 * Handles multiple drawing layers
 */
class Layers : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList layerNames READ layerNames NOTIFY layersChanged)
    Q_PROPERTY(int activeLayerIndex READ activeLayerIndex WRITE setActiveLayerIndex NOTIFY activeLayerChanged)

public:
    explicit Layers(QObject *parent = nullptr);
    
    QStringList layerNames() const { return m_layerNames; }
    int activeLayerIndex() const { return m_activeLayerIndex; }
    
    Q_INVOKABLE void addLayer(const QString &name = QString());
    Q_INVOKABLE void removeLayer(int index);
    Q_INVOKABLE void renameLayer(int index, const QString &name);
    Q_INVOKABLE void setLayerVisible(int index, bool visible);
    Q_INVOKABLE bool isLayerVisible(int index) const;
    Q_INVOKABLE void setLayerOpacity(int index, qreal opacity);
    Q_INVOKABLE qreal getLayerOpacity(int index) const;
    Q_INVOKABLE void setLayerBlendMode(int index, const QString &blendMode);
    Q_INVOKABLE QString getLayerBlendMode(int index) const;
    void setActiveLayerIndex(int index);

signals:
    void layersChanged();
    void activeLayerChanged();

private:
    QStringList m_layerNames;
    QList<bool> m_layerVisibility;
    QList<qreal> m_layerOpacity;
    QStringList m_layerBlendModes;
    int m_activeLayerIndex;
    
    QString generateLayerName() const;
};

#endif // LAYERS_H

