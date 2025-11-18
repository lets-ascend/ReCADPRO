#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include "drawingobject.h"

/**
 * ObjectManager - Manages drawing objects (groups of strokes)
 * Handles object creation, locking, selection, and grouping
 */
class ObjectManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int objectCount READ objectCount NOTIFY objectsChanged)
    Q_PROPERTY(QList<QObject*> objects READ objects NOTIFY objectsChanged)

public:
    explicit ObjectManager(QObject *parent = nullptr);
    
    int objectCount() const { return m_objects.size(); }
    QList<QObject*> objects() const;
    
    Q_INVOKABLE DrawingObject* createObject(const QString &name = QString());
    Q_INVOKABLE DrawingObject* getObject(const QString &id) const;
    Q_INVOKABLE void deleteObject(const QString &id);
    Q_INVOKABLE void deleteObject(DrawingObject *obj);
    
    Q_INVOKABLE void selectObject(const QString &id);
    Q_INVOKABLE void selectObject(DrawingObject *obj);
    Q_INVOKABLE void deselectAll();
    Q_INVOKABLE void selectAll();
    Q_INVOKABLE QList<DrawingObject*> getSelectedObjects() const;
    
    Q_INVOKABLE void lockObject(const QString &id);
    Q_INVOKABLE void unlockObject(const QString &id);
    Q_INVOKABLE void lockAll();
    Q_INVOKABLE void unlockAll();
    
    Q_INVOKABLE void groupObjects(const QList<QString> &objectIds);
    Q_INVOKABLE DrawingObject* groupSelected();
    Q_INVOKABLE void ungroupObject(const QString &groupId);
    
    Q_INVOKABLE void bringToFront(const QString &id);
    Q_INVOKABLE void sendToBack(const QString &id);
    Q_INVOKABLE void bringForward(const QString &id);
    Q_INVOKABLE void sendBackward(const QString &id);

signals:
    void objectsChanged();
    void objectCreated(DrawingObject *obj);
    void objectDeleted(const QString &id);
    void selectionChanged();

private:
    QList<DrawingObject*> m_objects;
    
    int findObjectIndex(const QString &id) const;
    int findObjectIndex(DrawingObject *obj) const;
};

#endif // OBJECTMANAGER_H

