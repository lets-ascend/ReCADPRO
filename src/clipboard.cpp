#include "clipboard.h"
#include <QRectF>

const qreal Clipboard::PASTE_OFFSET_STEP = 20.0;

Clipboard::Clipboard(QObject *parent)
    : QObject(parent)
    , m_pasteCount(0)
{
}

void Clipboard::copy(const QList<ClipboardStroke> &strokes, const QRectF &bounds)
{
    m_clipboardStrokes = strokes;
    m_clipboardBounds = bounds;
    m_pasteCount = 0;
    m_pasteOffset = QPointF(0, 0);
    emit contentChanged();
}

QList<Clipboard::ClipboardStroke> Clipboard::paste(const QPointF &position)
{
    if (m_clipboardStrokes.isEmpty())
        return QList<ClipboardStroke>();
    
    // Calculate offset for paste (stagger pastes)
    m_pasteCount++;
    m_pasteOffset = QPointF(
        m_pasteCount * PASTE_OFFSET_STEP,
        m_pasteCount * PASTE_OFFSET_STEP
    );
    
    // Create offset strokes
    QList<ClipboardStroke> offsetStrokes;
    QPointF baseOffset = position - m_clipboardBounds.topLeft() + m_pasteOffset;
    
    for (const ClipboardStroke &stroke : m_clipboardStrokes) {
        ClipboardStroke offsetStroke = stroke;
        for (QPointF &point : offsetStroke.points) {
            point += baseOffset;
        }
        offsetStrokes.append(offsetStroke);
    }
    
    return offsetStrokes;
}

void Clipboard::clear()
{
    m_clipboardStrokes.clear();
    m_pasteCount = 0;
    m_pasteOffset = QPointF(0, 0);
    emit contentChanged();
}

