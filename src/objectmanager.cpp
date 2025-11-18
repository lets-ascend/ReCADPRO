#include "objectmanager.h"
#include <QDebug>

ObjectManager::ObjectManager(QObject *parent)
    : QObject(parent)
{
}

QList<QObject*> ObjectManager::objects() const
{
    QList<QObject*> result;
    for (DrawingObject *obj : m_objects) {
        result.append(obj);
    }
    return result;
}

DrawingObject* ObjectManager::createObject(const QString &name)
{
    DrawingObject *obj = new DrawingObject(this);
    if (!name.isEmpty()) {
        obj->setName(name);
    }
    
    m_objects.append(obj);
    emit objectCreated(obj);
    emit objectsChanged();
    
    return obj;
}

DrawingObject* ObjectManager::getObject(const QString &id) const
{
    for (DrawingObject *obj : m_objects) {
        if (obj->id() == id) {
            return obj;
        }
    }
    return nullptr;
}

void ObjectManager::deleteObject(const QString &id)
{
    int index = findObjectIndex(id);
    if (index >= 0) {
        DrawingObject *obj = m_objects.takeAt(index);
        QString objId = obj->id();
        obj->deleteLater();
        emit objectDeleted(objId);
        emit objectsChanged();
    }
}

void ObjectManager::deleteObject(DrawingObject *obj)
{
    if (obj) {
        deleteObject(obj->id());
    }
}

void ObjectManager::selectObject(const QString &id)
{
    DrawingObject *obj = getObject(id);
    if (obj) {
        selectObject(obj);
    }
}

void ObjectManager::selectObject(DrawingObject *obj)
{
    if (!obj || obj->locked())
        return;
    
    deselectAll();
    obj->setSelected(true);
    emit selectionChanged();
}

void ObjectManager::deselectAll()
{
    bool changed = false;
    for (DrawingObject *obj : m_objects) {
        if (obj->selected()) {
            obj->setSelected(false);
            changed = true;
        }
    }
    if (changed) {
        emit selectionChanged();
    }
}

void ObjectManager::selectAll()
{
    bool changed = false;
    for (DrawingObject *obj : m_objects) {
        if (!obj->locked() && !obj->selected()) {
            obj->setSelected(true);
            changed = true;
        }
    }
    if (changed) {
        emit selectionChanged();
    }
}

QList<DrawingObject*> ObjectManager::getSelectedObjects() const
{
    QList<DrawingObject*> selected;
    for (DrawingObject *obj : m_objects) {
        if (obj->selected()) {
            selected.append(obj);
        }
    }
    return selected;
}

void ObjectManager::lockObject(const QString &id)
{
    DrawingObject *obj = getObject(id);
    if (obj) {
        obj->setLocked(true);
    }
}

void ObjectManager::unlockObject(const QString &id)
{
    DrawingObject *obj = getObject(id);
    if (obj) {
        obj->setLocked(false);
    }
}

void ObjectManager::lockAll()
{
    for (DrawingObject *obj : m_objects) {
        obj->setLocked(true);
    }
}

void ObjectManager::unlockAll()
{
    for (DrawingObject *obj : m_objects) {
        obj->setLocked(false);
    }
}

void ObjectManager::groupObjects(const QList<QString> &objectIds)
{
    if (objectIds.size() < 2)
        return;
    
    DrawingObject *group = createObject("Group");
    
    for (const QString &id : objectIds) {
        DrawingObject *obj = getObject(id);
        if (obj) {
            // Add strokes from object to group
            // This would need access to stroke indices
            // Implementation depends on integration with DrawingEngine
        }
    }
    
    // Delete original objects or mark them as part of group
    // Implementation depends on design choice
}

DrawingObject* ObjectManager::groupSelected()
{
    QList<DrawingObject*> selected = getSelectedObjects();
    if (selected.size() < 2)
        return nullptr;
    
    QList<QString> ids;
    for (DrawingObject *obj : selected) {
        ids.append(obj->id());
    }
    
    groupObjects(ids);
    return getObject(ids.first());  // Return group object
}

void ObjectManager::ungroupObject(const QString &groupId)
{
    DrawingObject *group = getObject(groupId);
    if (!group)
        return;
    
    // Extract objects from group
    // Implementation depends on group structure
    // For now, just delete the group
    deleteObject(groupId);
}

void ObjectManager::bringToFront(const QString &id)
{
    int index = findObjectIndex(id);
    if (index >= 0 && index < m_objects.size() - 1) {
        m_objects.move(index, m_objects.size() - 1);
        emit objectsChanged();
    }
}

void ObjectManager::sendToBack(const QString &id)
{
    int index = findObjectIndex(id);
    if (index > 0) {
        m_objects.move(index, 0);
        emit objectsChanged();
    }
}

void ObjectManager::bringForward(const QString &id)
{
    int index = findObjectIndex(id);
    if (index >= 0 && index < m_objects.size() - 1) {
        m_objects.move(index, index + 1);
        emit objectsChanged();
    }
}

void ObjectManager::sendBackward(const QString &id)
{
    int index = findObjectIndex(id);
    if (index > 0) {
        m_objects.move(index, index - 1);
        emit objectsChanged();
    }
}

int ObjectManager::findObjectIndex(const QString &id) const
{
    for (int i = 0; i < m_objects.size(); ++i) {
        if (m_objects[i]->id() == id) {
            return i;
        }
    }
    return -1;
}

int ObjectManager::findObjectIndex(DrawingObject *obj) const
{
    return m_objects.indexOf(obj);
}

