#ifndef EXPORTER_H
#define EXPORTER_H

#include <QObject>
#include <QUrl>
#include <QImage>
#include <QString>

/**
 * Exporter - Export drawing to various formats
 * Supports PNG, SVG, PDF export
 */
class Exporter : public QObject
{
    Q_OBJECT

public:
    explicit Exporter(QObject *parent = nullptr);
    
    // Export functions
    Q_INVOKABLE bool exportPNG(const QImage &image, const QUrl &fileUrl);
    Q_INVOKABLE bool exportSVG(const QImage &image, const QUrl &fileUrl, const QSize &size);
    Q_INVOKABLE bool exportPDF(const QImage &image, const QUrl &fileUrl, const QSize &size);
    
    // Export with options
    Q_INVOKABLE bool exportPNGWithOptions(const QImage &image, const QUrl &fileUrl, int quality = 100);
    Q_INVOKABLE bool exportSVGWithSize(const QImage &image, const QUrl &fileUrl, const QSize &size, bool embedImages = true);

signals:
    void exportStarted(const QString &format);
    void exportProgress(int percent);
    void exportFinished(bool success, const QString &filePath);
    void exportError(const QString &error);

private:
    QString urlToLocalPath(const QUrl &url) const;
};

#endif // EXPORTER_H

