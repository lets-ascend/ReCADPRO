#include "importer.h"
#include <QFileInfo>
#include <QDebug>
#include <QImageReader>
#include <QImageWriter>
#include <QPainter>
#include <QTransform>

Importer::Importer(QObject *parent)
    : QObject(parent)
{
}

QImage Importer::importImage(const QUrl &fileUrl)
{
    QString localPath = fileUrl.toLocalFile();
    if (localPath.isEmpty()) {
        emit importError("Invalid file URL");
        return QImage();
    }
    
    // Check file exists
    QFileInfo fileInfo(localPath);
    if (!fileInfo.exists()) {
        emit importError("File does not exist: " + localPath);
        return QImage();
    }
    
    // Check file size (limit to 50MB)
    static const qint64 MAX_FILE_SIZE = 50 * 1024 * 1024; // 50MB
    if (fileInfo.size() > MAX_FILE_SIZE) {
        emit importError("File too large (max 50MB): " + QString::number(fileInfo.size() / 1024 / 1024) + "MB");
        return QImage();
    }
    
    emit importStarted(fileInfo.fileName());
    
    QImage image(localPath);
    if (image.isNull()) {
        emit importError("Failed to load image: " + localPath);
        return QImage();
    }
    
    // Limit image dimensions (scale down if too large)
    static const int MAX_DIMENSION = 4000;
    if (image.width() > MAX_DIMENSION || image.height() > MAX_DIMENSION) {
        QSize newSize = image.size();
        newSize.scale(MAX_DIMENSION, MAX_DIMENSION, Qt::KeepAspectRatio);
        image = image.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        emit importProgress(50); // Indicate scaling occurred
    }
    
    // Note: reMarkable Paper Pro supports color (Canvas Color display)
    // Colors will appear muted/pastel, but we preserve original colors
    // Optional: Apply slight saturation adjustment if needed for visibility
    emit importFinished(image);
    
    return image;
}

QImage Importer::importPDF(const QUrl &fileUrl, int pageNumber)
{
    QString localPath = fileUrl.toLocalFile();
    if (localPath.isEmpty()) {
        emit importError("Invalid PDF URL");
        return QImage();
    }
    
    emit importStarted(QFileInfo(localPath).fileName());
    
    // Note: PDF import requires Poppler or Qt PDF support
    // This is a placeholder - actual implementation depends on available libraries
    // For now, return empty image with error
    
    emit importError("PDF import requires Poppler library. Not yet implemented.");
    return QImage();
    
    // TODO: Implement PDF import using Poppler or Qt PDF
    // QPdfDocument pdf;
    // pdf.load(localPath);
    // QImage image = pdf.render(pageNumber, size);
    // return convertToGrayscale(image);
}

QImage Importer::convertToGrayscale(const QImage &image)
{
    // Optional grayscale conversion (not required for Paper Pro)
    // Paper Pro supports color, but grayscale may be desired for some use cases
    return convertColorToGrayscale(image);
}

QImage Importer::scaleImage(const QImage &image, qreal scaleFactor)
{
    if (image.isNull() || scaleFactor <= 0)
        return QImage();
    
    QSize newSize = image.size() * scaleFactor;
    return image.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QImage Importer::rotateImage(const QImage &image, qreal angle)
{
    if (image.isNull())
        return QImage();
    
    QTransform transform;
    transform.rotate(angle);
    
    QImage rotated = image.transformed(transform, Qt::SmoothTransformation);
    return rotated;
}

QImage Importer::adjustSaturation(const QImage &image, qreal saturationFactor)
{
    if (image.isNull())
        return QImage();
    
    // Adjust saturation for Canvas Color display (colors are naturally muted)
    // saturationFactor: 1.0 = no change, >1.0 = increase, <1.0 = decrease
    QImage adjusted = image;
    
    // Apply saturation adjustment
    // Note: This is a simplified version - full implementation would use HSL color space
    for (int y = 0; y < adjusted.height(); ++y) {
        for (int x = 0; x < adjusted.width(); ++x) {
            QColor color = adjusted.pixelColor(x, y);
            
            // Convert to HSL, adjust saturation, convert back
            qreal h, s, l;
            color.getHslF(&h, &s, &l);
            s = qBound(0.0, s * saturationFactor, 1.0);
            color.setHslF(h, s, l);
            adjusted.setPixelColor(x, y, color);
        }
    }
    
    return adjusted;
}

QImage Importer::convertColorToGrayscale(const QImage &colorImage)
{
    if (colorImage.isNull())
        return QImage();
    
    // Convert to grayscale using luminance formula
    QImage grayscale = colorImage.convertToFormat(QImage::Format_Grayscale8);
    
    // If conversion didn't work, manually convert
    if (grayscale.isNull() || grayscale.format() != QImage::Format_Grayscale8) {
        grayscale = QImage(colorImage.size(), QImage::Format_Grayscale8);
        
        for (int y = 0; y < colorImage.height(); ++y) {
            for (int x = 0; x < colorImage.width(); ++x) {
                QColor color = colorImage.pixelColor(x, y);
                // Luminance formula: 0.299*R + 0.587*G + 0.114*B
                int gray = qRound(0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue());
                grayscale.setPixel(x, y, qRgb(gray, gray, gray));
            }
        }
    }
    
    return grayscale;
}

