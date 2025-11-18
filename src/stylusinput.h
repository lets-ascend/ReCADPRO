#ifndef STYLUSINPUT_H
#define STYLUSINPUT_H

#include <QObject>
#include <QPointF>

/**
 * StylusInput - reMarkable marker/stylus input handling
 * Handles pressure-sensitive stylus input for Paper Pro
 */
class StylusInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal pressure READ pressure NOTIFY pressureChanged)
    Q_PROPERTY(QPointF position READ position NOTIFY positionChanged)
    Q_PROPERTY(bool isStylusDown READ isStylusDown NOTIFY stylusDownChanged)

public:
    explicit StylusInput(QObject *parent = nullptr);
    
    qreal pressure() const { return m_pressure; }
    QPointF position() const { return m_position; }
    bool isStylusDown() const { return m_isStylusDown; }
    
    Q_INVOKABLE void handleStylusEvent(const QPointF &pos, qreal pressure, bool isDown);

signals:
    void pressureChanged();
    void positionChanged();
    void stylusDownChanged();
    void stylusMoved(const QPointF &position, qreal pressure);

private:
    qreal m_pressure;
    QPointF m_position;
    bool m_isStylusDown;
};

#endif // STYLUSINPUT_H

