#include "brushtools.h"
#include <QDebug>

BrushTools::BrushTools(QObject *parent)
    : QObject(parent)
    , m_currentBrush("default")
    , m_brushSpacing(0.1)
    , m_brushOpacity(1.0)
{
    createDefaultBrushes();
}

void BrushTools::setCurrentBrush(const QString &brushName)
{
    if (m_currentBrush == brushName)
        return;
    
    m_currentBrush = brushName;
    emit currentBrushChanged();
}

void BrushTools::setBrushSpacing(qreal spacing)
{
    if (qFuzzyCompare(m_brushSpacing, spacing))
        return;
    
    m_brushSpacing = qBound(0.0, spacing, 2.0);
    emit brushSpacingChanged();
}

void BrushTools::setBrushOpacity(qreal opacity)
{
    if (qFuzzyCompare(m_brushOpacity, opacity))
        return;
    
    m_brushOpacity = qBound(0.0, opacity, 1.0);
    emit brushOpacityChanged();
}

void BrushTools::addBrushPreset(const BrushPreset &preset)
{
    // Check if brush already exists
    int index = findBrushIndex(preset.id);
    if (index >= 0) {
        m_brushPresets[index] = preset;
        emit brushPresetUpdated(preset.id);
    } else {
        m_brushPresets.append(preset);
        emit brushPresetAdded(preset.id);
    }
}

void BrushTools::removeBrushPreset(const QString &brushId)
{
    int index = findBrushIndex(brushId);
    if (index >= 0) {
        m_brushPresets.removeAt(index);
        emit brushPresetRemoved(brushId);
    }
}

BrushTools::BrushPreset BrushTools::getBrushPreset(const QString &brushId) const
{
    int index = findBrushIndex(brushId);
    if (index >= 0) {
        return m_brushPresets[index];
    }
    return BrushPreset(); // Return empty preset
}

QStringList BrushTools::getBrushPresetNames() const
{
    QStringList names;
    for (const BrushPreset &preset : m_brushPresets) {
        names.append(preset.name);
    }
    return names;
}

void BrushTools::setBrushTexture(const QString &brushId, const QImage &texture)
{
    int index = findBrushIndex(brushId);
    if (index >= 0) {
        m_brushPresets[index].texture = texture;
        emit brushPresetUpdated(brushId);
    }
}

QImage BrushTools::getBrushTexture(const QString &brushId) const
{
    int index = findBrushIndex(brushId);
    if (index >= 0) {
        return m_brushPresets[index].texture;
    }
    return QImage();
}

void BrushTools::setOpacityCurve(const QString &brushId, const QList<qreal> &curve)
{
    int index = findBrushIndex(brushId);
    if (index >= 0) {
        m_brushPresets[index].opacityCurve = curve;
        emit brushPresetUpdated(brushId);
    }
}

QList<qreal> BrushTools::getOpacityCurve(const QString &brushId) const
{
    int index = findBrushIndex(brushId);
    if (index >= 0) {
        return m_brushPresets[index].opacityCurve;
    }
    return QList<qreal>();
}

qreal BrushTools::getOpacityForPressure(const QString &brushId, qreal pressure) const
{
    int index = findBrushIndex(brushId);
    if (index < 0)
        return m_brushOpacity;
    
    const BrushPreset &preset = m_brushPresets[index];
    
    if (preset.opacityCurve.isEmpty()) {
        // Linear mapping if no curve
        return m_brushOpacity * pressure;
    }
    
    return interpolateOpacityCurve(preset.opacityCurve, pressure) * preset.opacity;
}

qreal BrushTools::calculateSpacing(qreal brushSize, qreal spacingFactor) const
{
    return brushSize * spacingFactor;
}

void BrushTools::createDefaultBrushes()
{
    // Default brush
    BrushPreset defaultBrush;
    defaultBrush.name = "Default";
    defaultBrush.id = "default";
    defaultBrush.size = 3.0;
    defaultBrush.spacing = 0.1;
    defaultBrush.opacity = 1.0;
    defaultBrush.opacityCurve = QList<qreal>() << 0.0 << 0.5 << 1.0; // Linear curve
    defaultBrush.color = Qt::black;
    defaultBrush.pressureSensitive = true;
    m_brushPresets.append(defaultBrush);
    
    // Soft brush
    BrushPreset softBrush;
    softBrush.name = "Soft";
    softBrush.id = "soft";
    softBrush.size = 5.0;
    softBrush.spacing = 0.05;
    softBrush.opacity = 0.7;
    softBrush.opacityCurve = QList<qreal>() << 0.0 << 0.3 << 0.7 << 1.0; // Soft curve
    softBrush.color = Qt::black;
    softBrush.pressureSensitive = true;
    m_brushPresets.append(softBrush);
    
    // Hard brush
    BrushPreset hardBrush;
    hardBrush.name = "Hard";
    hardBrush.id = "hard";
    hardBrush.size = 2.0;
    hardBrush.spacing = 0.2;
    hardBrush.opacity = 1.0;
    hardBrush.opacityCurve = QList<qreal>() << 0.0 << 1.0; // Hard curve
    hardBrush.color = Qt::black;
    hardBrush.pressureSensitive = true;
    m_brushPresets.append(hardBrush);
    
    m_currentBrush = "default";
}

int BrushTools::findBrushIndex(const QString &brushId) const
{
    for (int i = 0; i < m_brushPresets.size(); ++i) {
        if (m_brushPresets[i].id == brushId) {
            return i;
        }
    }
    return -1;
}

qreal BrushTools::interpolateOpacityCurve(const QList<qreal> &curve, qreal pressure) const
{
    if (curve.isEmpty())
        return pressure;
    
    if (curve.size() == 1)
        return curve[0];
    
    // Linear interpolation through curve points
    // Curve is array of opacity values at evenly spaced pressure points
    int numPoints = curve.size();
    qreal step = 1.0 / (numPoints - 1);
    
    int index = qBound(0, static_cast<int>(pressure / step), numPoints - 2);
    qreal t = (pressure - index * step) / step;
    
    return curve[index] * (1.0 - t) + curve[index + 1] * t;
}

