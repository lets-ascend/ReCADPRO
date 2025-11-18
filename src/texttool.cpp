#include "texttool.h"
#include <QDebug>

TextTool::TextTool(QObject *parent)
    : QObject(parent)
    , m_textColor(Qt::black)
    , m_currentText("")
{
    m_font.setFamily("Arial");
    m_font.setPixelSize(24);
}

void TextTool::setFont(const QFont &font)
{
    if (m_font == font)
        return;
    
    m_font = font;
    emit fontChanged();
}

void TextTool::setTextColor(const QColor &color)
{
    if (m_textColor == color)
        return;
    
    m_textColor = color;
    emit textColorChanged();
}

void TextTool::setCurrentText(const QString &text)
{
    if (m_currentText == text)
        return;
    
    m_currentText = text;
    emit currentTextChanged();
}

void TextTool::placeText(const QPointF &position, const QString &text)
{
    if (text.isEmpty())
        return;
    
    TextData data;
    data.text = text;
    data.position = position;
    data.font = m_font;
    data.color = m_textColor;
    
    m_texts.append(data);
    emit textPlaced(data);
}

void TextTool::editText(int textIndex)
{
    if (textIndex < 0 || textIndex >= m_texts.size())
        return;
    
    emit textUpdated(textIndex);
}

void TextTool::deleteText(int textIndex)
{
    if (textIndex < 0 || textIndex >= m_texts.size())
        return;
    
    m_texts.removeAt(textIndex);
    emit textDeleted(textIndex);
}

TextTool::TextData TextTool::getText(int index) const
{
    if (index < 0 || index >= m_texts.size()) {
        return TextData();
    }
    return m_texts[index];
}

