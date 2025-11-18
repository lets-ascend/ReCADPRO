#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>
#include <QPointF>
#include <QList>
#include <QColor>

/**
 * Clipboard - Copy/paste functionality for reMarkable Paper Pro
 * Handles object-based clipboard operations
 */
class Clipboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasContent READ hasContent NOTIFY contentChanged)

public:
    explicit Clipboard(QObject *parent = nullptr);
    
    bool hasContent() const { return !m_clipboardStrokes.isEmpty(); }
    
    // Stroke structure (simplified for clipboard)
    struct ClipboardStroke {
        QList<QPointF> points;
        QColor color;
        qreal lineWidth;
        qreal opacity;
    };
    
    Q_INVOKABLE void copy(const QList<ClipboardStroke> &strokes, const QRectF &bounds);
    Q_INVOKABLE QList<ClipboardStroke> paste(const QPointF &position);
    Q_INVOKABLE void clear();
    
    Q_INVOKABLE QPointF getPasteOffset() const { return m_pasteOffset; }

signals:
    void contentChanged();

private:
    QList<ClipboardStroke> m_clipboardStrokes;
    QRectF m_clipboardBounds;
    QPointF m_pasteOffset;
    int m_pasteCount;
    
    static const qreal PASTE_OFFSET_STEP;
};

#endif // CLIPBOARD_H

