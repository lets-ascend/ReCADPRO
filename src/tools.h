#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QString>

/**
 * Tools - Drawing tool management for reMarkable Paper Pro
 * Manages tool selection and properties
 */
class Tools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentTool READ currentTool WRITE setCurrentTool NOTIFY currentToolChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit Tools(QObject *parent = nullptr);
    
    QString currentTool() const { return m_currentTool; }
    qreal lineWidth() const { return m_lineWidth; }
    QString color() const { return m_color; }
    
    void setCurrentTool(const QString &tool);
    void setLineWidth(qreal width);
    void setColor(const QString &color);

signals:
    void currentToolChanged();
    void lineWidthChanged();
    void colorChanged();

private:
    QString m_currentTool;
    qreal m_lineWidth;
    QString m_color;
};

#endif // TOOLS_H

