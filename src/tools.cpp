#include "tools.h"

Tools::Tools(QObject *parent)
    : QObject(parent)
    , m_currentTool("pen")
    , m_lineWidth(3.0)
    , m_color("#000000")
{
}

void Tools::setCurrentTool(const QString &tool)
{
    if (m_currentTool == tool)
        return;
    
    m_currentTool = tool;
    emit currentToolChanged();
}

void Tools::setLineWidth(qreal width)
{
    if (qFuzzyCompare(m_lineWidth, width))
        return;
    
    m_lineWidth = width;
    emit lineWidthChanged();
}

void Tools::setColor(const QString &color)
{
    if (m_color == color)
        return;
    
    m_color = color;
    emit colorChanged();
}

