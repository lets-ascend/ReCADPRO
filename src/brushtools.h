#ifndef BRUSHTOOLS_H
#define BRUSHTOOLS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QImage>
#include <QColor>

/**
 * BrushTools - Advanced brush system with presets, textures, and curves
 * Custom brush library and brush properties
 */
class BrushTools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentBrush READ currentBrush WRITE setCurrentBrush NOTIFY currentBrushChanged)
    Q_PROPERTY(qreal brushSpacing READ brushSpacing WRITE setBrushSpacing NOTIFY brushSpacingChanged)
    Q_PROPERTY(qreal brushOpacity READ brushOpacity WRITE setBrushOpacity NOTIFY brushOpacityChanged)

public:
    explicit BrushTools(QObject *parent = nullptr);
    
    QString currentBrush() const { return m_currentBrush; }
    qreal brushSpacing() const { return m_brushSpacing; }
    qreal brushOpacity() const { return m_brushOpacity; }
    
    void setCurrentBrush(const QString &brushName);
    void setBrushSpacing(qreal spacing);
    void setBrushOpacity(qreal opacity);
    
    // Brush preset structure
    struct BrushPreset {
        QString name;
        QString id;
        QImage texture;  // Brush texture/pattern
        qreal size;
        qreal spacing;
        qreal opacity;
        QList<qreal> opacityCurve;  // Opacity curve (pressure → opacity mapping)
        QColor color;
        bool pressureSensitive;
    };
    
    // Brush library management
    Q_INVOKABLE void addBrushPreset(const BrushPreset &preset);
    Q_INVOKABLE void removeBrushPreset(const QString &brushId);
    Q_INVOKABLE BrushPreset getBrushPreset(const QString &brushId) const;
    Q_INVOKABLE QList<BrushPreset> getAllBrushPresets() const { return m_brushPresets; }
    Q_INVOKABLE QStringList getBrushPresetNames() const;
    
    // Brush texture
    Q_INVOKABLE void setBrushTexture(const QString &brushId, const QImage &texture);
    Q_INVOKABLE QImage getBrushTexture(const QString &brushId) const;
    
    // Opacity curve (pressure → opacity mapping)
    Q_INVOKABLE void setOpacityCurve(const QString &brushId, const QList<qreal> &curve);
    Q_INVOKABLE QList<qreal> getOpacityCurve(const QString &brushId) const;
    Q_INVOKABLE qreal getOpacityForPressure(const QString &brushId, qreal pressure) const;
    
    // Brush spacing control
    Q_INVOKABLE qreal calculateSpacing(qreal brushSize, qreal spacingFactor) const;
    
    // Create default brushes
    Q_INVOKABLE void createDefaultBrushes();

signals:
    void currentBrushChanged();
    void brushSpacingChanged();
    void brushOpacityChanged();
    void brushPresetAdded(const QString &brushId);
    void brushPresetRemoved(const QString &brushId);
    void brushPresetUpdated(const QString &brushId);

private:
    QString m_currentBrush;
    qreal m_brushSpacing;  // Spacing factor (0.0-2.0)
    qreal m_brushOpacity;  // Base opacity (0.0-1.0)
    
    QList<BrushPreset> m_brushPresets;
    
    int findBrushIndex(const QString &brushId) const;
    qreal interpolateOpacityCurve(const QList<qreal> &curve, qreal pressure) const;
};

#endif // BRUSHTOOLS_H

