#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include <QObject>
#include <QPointF>
#include <QColor>
#include <QFont>
#include <QList>

/**
 * TextTool - Text input and rendering tool
 * Handles text placement and editing on canvas
 */
class TextTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString currentText READ currentText WRITE setCurrentText NOTIFY currentTextChanged)

public:
    explicit TextTool(QObject *parent = nullptr);
    
    QFont font() const { return m_font; }
    QColor textColor() const { return m_textColor; }
    QString currentText() const { return m_currentText; }
    
    void setFont(const QFont &font);
    void setTextColor(const QColor &color);
    void setCurrentText(const QString &text);
    
    // Text placement
    Q_INVOKABLE void placeText(const QPointF &position, const QString &text);
    Q_INVOKABLE void editText(int textIndex);
    Q_INVOKABLE void deleteText(int textIndex);
    
    // Text data
    struct TextData {
        QString text;
        QPointF position;
        QFont font;
        QColor color;
    };
    
    Q_INVOKABLE QList<TextData> getAllTexts() const { return m_texts; }
    Q_INVOKABLE TextData getText(int index) const;

signals:
    void fontChanged();
    void textColorChanged();
    void currentTextChanged();
    void textPlaced(const TextData &text);
    void textUpdated(int index);
    void textDeleted(int index);

private:
    QFont m_font;
    QColor m_textColor;
    QString m_currentText;
    QList<TextData> m_texts;
};

#endif // TEXTTOOL_H

