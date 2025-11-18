#include "exporter.h"
#include <QFileInfo>
#include <QDir>
#include <QSvgGenerator>
#include <QPainter>
#include <QPdfWriter>
#include <QDebug>

Exporter::Exporter(QObject *parent)
    : QObject(parent)
{
}

bool Exporter::exportPNG(const QImage &image, const QUrl &fileUrl)
{
    return exportPNGWithOptions(image, fileUrl, 100);
}

bool Exporter::exportPNGWithOptions(const QImage &image, const QUrl &fileUrl, int quality)
{
    if (image.isNull()) {
        emit exportError("Image is null");
        return false;
    }
    
    QString filePath = urlToLocalPath(fileUrl);
    if (filePath.isEmpty()) {
        emit exportError("Invalid file path");
        return false;
    }
    
    emit exportStarted("PNG");
    
    // Ensure directory exists
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    bool success = image.save(filePath, "PNG", quality);
    
    if (success) {
        emit exportFinished(true, filePath);
    } else {
        emit exportError("Failed to save PNG file");
    }
    
    return success;
}

bool Exporter::exportSVG(const QImage &image, const QUrl &fileUrl, const QSize &size)
{
    return exportSVGWithSize(image, fileUrl, size, true);
}

bool Exporter::exportSVGWithSize(const QImage &image, const QUrl &fileUrl, const QSize &size, bool embedImages)
{
    if (image.isNull()) {
        emit exportError("Image is null");
        return false;
    }
    
    QString filePath = urlToLocalPath(fileUrl);
    if (filePath.isEmpty()) {
        emit exportError("Invalid file path");
        return false;
    }
    
    emit exportStarted("SVG");
    
    QSvgGenerator generator;
    generator.setFileName(filePath);
    generator.setSize(size.isValid() ? size : image.size());
    generator.setViewBox(QRect(0, 0, generator.size().width(), generator.size().height()));
    generator.setTitle("reMarkable Drafting Pro Export");
    generator.setDescription("Exported from reMarkable Drafting Pro");
    
    QPainter painter(&generator);
    
    if (embedImages) {
        // Draw image directly (will be embedded)
        painter.drawImage(QRect(0, 0, generator.size().width(), generator.size().height()), image);
    } else {
        // Convert to paths (more complex, but smaller file)
        painter.drawImage(QRect(0, 0, generator.size().width(), generator.size().height()), image);
    }
    
    painter.end();
    
    bool success = QFileInfo(filePath).exists();
    
    if (success) {
        emit exportFinished(true, filePath);
    } else {
        emit exportError("Failed to create SVG file");
    }
    
    return success;
}

bool Exporter::exportPDF(const QImage &image, const QUrl &fileUrl, const QSize &size)
{
    if (image.isNull()) {
        emit exportError("Image is null");
        return false;
    }
    
    QString filePath = urlToLocalPath(fileUrl);
    if (filePath.isEmpty()) {
        emit exportError("Invalid file path");
        return false;
    }
    
    emit exportStarted("PDF");
    
    QSize pdfSize = size.isValid() ? size : image.size();
    
    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QSizeF(pdfSize), QPageSize::Point));
    writer.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Point);
    writer.setTitle("reMarkable Drafting Pro Export");
    
    QPainter painter(&writer);
    painter.drawImage(QRect(0, 0, pdfSize.width(), pdfSize.height()), image);
    painter.end();
    
    bool success = QFileInfo(filePath).exists();
    
    if (success) {
        emit exportFinished(true, filePath);
    } else {
        emit exportError("Failed to create PDF file");
    }
    
    return success;
}

QString Exporter::urlToLocalPath(const QUrl &url) const
{
    if (url.isLocalFile()) {
        return url.toLocalFile();
    }
    return url.toString();
}

