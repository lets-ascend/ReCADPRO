#include "stylusinput.h"

StylusInput::StylusInput(QObject *parent)
    : QObject(parent)
    , m_pressure(0.0)
    , m_isStylusDown(false)
{
}

void StylusInput::handleStylusEvent(const QPointF &pos, qreal pressure, bool isDown)
{
    bool changed = false;
    
    if (!qFuzzyCompare(m_pressure, pressure)) {
        m_pressure = pressure;
        emit pressureChanged();
        changed = true;
    }
    
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged();
        changed = true;
    }
    
    if (m_isStylusDown != isDown) {
        m_isStylusDown = isDown;
        emit stylusDownChanged();
        changed = true;
    }
    
    if (changed && isDown) {
        emit stylusMoved(pos, pressure);
    }
}

