#include "draftingtools.h"
#include <QtMath>

DraftingTools::DraftingTools(QObject *parent)
    : QObject(parent)
    , m_gridEnabled(false)
    , m_snapToGrid(false)
    , m_gridType("lines")
    , m_gridSize(20.0)
{
}

void DraftingTools::setGridEnabled(bool enabled)
{
    if (m_gridEnabled == enabled)
        return;
    
    m_gridEnabled = enabled;
    emit gridEnabledChanged();
}

void DraftingTools::setSnapToGrid(bool enabled)
{
    if (m_snapToGrid == enabled)
        return;
    
    m_snapToGrid = enabled;
    emit snapToGridChanged();
}

void DraftingTools::setGridType(const QString &type)
{
    if (m_gridType == type)
        return;
    
    m_gridType = type;
    emit gridTypeChanged();
}

void DraftingTools::setGridSize(qreal size)
{
    if (qFuzzyCompare(m_gridSize, size))
        return;
    
    m_gridSize = size;
    emit gridSizeChanged();
}

QPointF DraftingTools::snapPoint(qreal x, qreal y) const
{
    if (!m_snapToGrid)
        return QPointF(x, y);
    
    qreal snappedX = qRound(x / m_gridSize) * m_gridSize;
    qreal snappedY = qRound(y / m_gridSize) * m_gridSize;
    
    return QPointF(snappedX, snappedY);
}

