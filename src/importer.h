#ifndef IMPORTER_H
#define IMPORTER_H

#include <QObject>
#include <QUrl>
#include <QImage>
#include <QString>

/**
 * Importer - PDF and image import functionality
 * Handles importing PDFs and images, converting to grayscale
 */
class Importer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList supportedFormats READ supportedFormats CONSTANT)

public:
    explicit Importer(QObject *parent = nullptr);
    
    QStringList supportedFormats() const {
        return QStringList() << "PDF" << "PNG" << "JPEG" << "JPG" << "BMP" << "SVG";
    }
    
    Q_INVOKABLE QImage importImage(const QUrl &fileUrl);
    Q_INVOKABLE QImage importPDF(const QUrl &fileUrl, int pageNumber = 0);
    Q_INVOKABLE QImage convertToGrayscale(const QImage &image);  // Optional, Paper Pro supports color
    Q_INVOKABLE QImage adjustSaturation(const QImage &image, qreal saturationFactor);  // Adjust for muted display
    Q_INVOKABLE QImage scaleImage(const QImage &image, qreal scaleFactor);
    Q_INVOKABLE QImage rotateImage(const QImage &image, qreal angle);

signals:
    void importStarted(const QString &filename);
    void importProgress(int percent);
    void importFinished(const QImage &image);
    void importError(const QString &error);

private:
    QImage convertColorToGrayscale(const QImage &colorImage);
};

#endif // IMPORTER_H

