#include "projectfile.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>
#include <QVariantList>
#include <QStorageInfo>
#include <QThread>
#include <QDebug>

ProjectFile::ProjectFile(QObject *parent)
    : QObject(parent)
{
}

bool ProjectFile::saveProject(const QUrl &fileUrl, const QVariantMap &projectData)
{
    QString filePath = urlToLocalPath(fileUrl);
    if (filePath.isEmpty()) {
        emit error("Invalid file path");
        return false;
    }
    
    emit saveStarted();
    
    // Ensure directory exists
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            emit error("Cannot create directory: " + dir.absolutePath());
            return false;
        }
    }
    
    // Check available disk space (rough estimate - need at least 10MB)
    QStorageInfo storage(dir);
    if (storage.isValid() && storage.bytesAvailable() < 10 * 1024 * 1024) {
        emit error("Insufficient disk space");
        return false;
    }
    
    // Convert to JSON
    QJsonObject jsonObject = QJsonObject::fromVariantMap(projectData);
    QJsonDocument doc(jsonObject);
    QByteArray jsonData = doc.toJson();
    
    // Limit JSON size (100MB max)
    static const qint64 MAX_JSON_SIZE = 100 * 1024 * 1024;
    if (jsonData.size() > MAX_JSON_SIZE) {
        emit error("Project data too large (max 100MB)");
        return false;
    }
    
    // Retry file write (up to 3 attempts)
    QFile file(filePath);
    int retries = 3;
    bool opened = false;
    
    while (retries > 0 && !opened) {
        if (file.open(QIODevice::WriteOnly)) {
            opened = true;
            break;
        }
        retries--;
        if (retries > 0) {
            QThread::msleep(100); // Wait 100ms before retry
        }
    }
    
    if (!opened) {
        emit error("Cannot open file for writing after retries: " + filePath);
        return false;
    }
    
    // Write data
    qint64 bytesWritten = file.write(jsonData);
    file.close();
    
    if (bytesWritten != jsonData.size()) {
        emit error("Failed to write all data to file");
        return false;
    }
    
    emit saveProgress(100);
    emit saveFinished(true, filePath);
    
    return true;
}

QVariantMap ProjectFile::loadProject(const QUrl &fileUrl)
{
    QString filePath = urlToLocalPath(fileUrl);
    if (filePath.isEmpty()) {
        emit error("Invalid file path");
        return QVariantMap();
    }
    
    emit loadStarted();
    
    QFile file(filePath);
    if (!file.exists()) {
        emit error("File does not exist: " + filePath);
        return QVariantMap();
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        emit error("Cannot open file for reading: " + filePath);
        return QVariantMap();
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        emit error("Invalid JSON format");
        return QVariantMap();
    }
    
    QVariantMap projectData = doc.object().toVariantMap();
    
    if (!validateProjectData(projectData)) {
        emit error("Invalid project file format");
        return QVariantMap();
    }
    
    emit loadProgress(100);
    emit loadFinished(true, projectData);
    
    return projectData;
}

QVariantMap ProjectFile::createProjectData()
{
    QVariantMap data;
    data["version"] = "1.0";
    data["layers"] = QVariantList();
    data["strokes"] = QVariantList();
    data["objects"] = QVariantList();
    
    QVariantMap settings;
    settings["canvasWidth"] = 1872;
    settings["canvasHeight"] = 1404;
    data["settings"] = settings;
    
    return data;
}

bool ProjectFile::validateProjectData(const QVariantMap &data)
{
    if (!data.contains("version")) {
        return false;
    }
    
    if (!data.contains("layers") || !data["layers"].canConvert<QVariantList>()) {
        return false;
    }
    
    if (!data.contains("strokes") || !data["strokes"].canConvert<QVariantList>()) {
        return false;
    }
    
    return true;
}

QString ProjectFile::urlToLocalPath(const QUrl &url) const
{
    if (url.isLocalFile()) {
        return url.toLocalFile();
    }
    return url.toString();
}

QVariantMap ProjectFile::serializeStrokes(const QVariantList &strokes)
{
    Q_UNUSED(strokes);
    // Implementation would serialize stroke data
    return QVariantMap();
}

QVariantList ProjectFile::deserializeStrokes(const QVariantMap &data)
{
    Q_UNUSED(data);
    // Implementation would deserialize stroke data
    return QVariantList();
}

