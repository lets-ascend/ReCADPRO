#ifndef DRAFTINGTOOLS_H
#define DRAFTINGTOOLS_H

#include <QObject>
#include <QString>

/**
 * DraftingTools - Drafting utilities for reMarkable Paper Pro
 * Grid, snap-to-grid, ruler, protractor
 */
class DraftingTools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool gridEnabled READ gridEnabled WRITE setGridEnabled NOTIFY gridEnabledChanged)
    Q_PROPERTY(bool snapToGrid READ snapToGrid WRITE setSnapToGrid NOTIFY snapToGridChanged)
    Q_PROPERTY(QString gridType READ gridType WRITE setGridType NOTIFY gridTypeChanged)
    Q_PROPERTY(qreal gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)

public:
    explicit DraftingTools(QObject *parent = nullptr);
    
    bool gridEnabled() const { return m_gridEnabled; }
    bool snapToGrid() const { return m_snapToGrid; }
    QString gridType() const { return m_gridType; }
    qreal gridSize() const { return m_gridSize; }
    
    void setGridEnabled(bool enabled);
    void setSnapToGrid(bool enabled);
    void setGridType(const QString &type);
    void setGridSize(qreal size);
    
    Q_INVOKABLE QPointF snapPoint(qreal x, qreal y) const;

signals:
    void gridEnabledChanged();
    void snapToGridChanged();
    void gridTypeChanged();
    void gridSizeChanged();

private:
    bool m_gridEnabled;
    bool m_snapToGrid;
    QString m_gridType; // "lines", "dots", "isometric"
    qreal m_gridSize;
};

#endif // DRAFTINGTOOLS_H

