#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QVariantMap>

/**
 * ProjectFile - Save and load project files
 * Stores layers, strokes, objects, and settings
 */
class ProjectFile : public QObject
{
    Q_OBJECT

public:
    explicit ProjectFile(QObject *parent = nullptr);
    
    // File operations
    Q_INVOKABLE bool saveProject(const QUrl &fileUrl, const QVariantMap &projectData);
    Q_INVOKABLE QVariantMap loadProject(const QUrl &fileUrl);
    
    // Project data structure
    // {
    //   "version": "1.0",
    //   "layers": [...],
    //   "strokes": [...],
    //   "objects": [...],
    //   "settings": {...}
    // }
    
    Q_INVOKABLE QVariantMap createProjectData();
    Q_INVOKABLE bool validateProjectData(const QVariantMap &data);

signals:
    void saveStarted();
    void saveProgress(int percent);
    void saveFinished(bool success, const QString &filePath);
    void loadStarted();
    void loadProgress(int percent);
    void loadFinished(bool success, const QVariantMap &data);
    void error(const QString &error);

private:
    QString urlToLocalPath(const QUrl &url) const;
    QVariantMap serializeStrokes(const QVariantList &strokes);
    QVariantList deserializeStrokes(const QVariantMap &data);
};

#endif // PROJECTFILE_H

