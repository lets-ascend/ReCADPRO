#ifndef TRANSFORMTOOLS_H
#define TRANSFORMTOOLS_H

#include <QObject>
#include <QPointF>
#include <QTransform>
#include <QRectF>

/**
 * TransformTools - Advanced transformation tools
 * Free transform, skew, perspective, flip
 */
class TransformTools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentTransform READ currentTransform WRITE setCurrentTransform NOTIFY currentTransformChanged)

public:
    explicit TransformTools(QObject *parent = nullptr);
    
    QString currentTransform() const { return m_currentTransform; }
    void setCurrentTransform(const QString &transform);
    
    // Free transform (corner handles)
    Q_INVOKABLE QTransform createFreeTransform(const QRectF &bounds, 
                                               const QPointF &topLeft, 
                                               const QPointF &topRight,
                                               const QPointF &bottomLeft,
                                               const QPointF &bottomRight) const;
    
    // Skew/distort
    Q_INVOKABLE QTransform createSkewTransform(const QRectF &bounds, qreal skewX, qreal skewY) const;
    Q_INVOKABLE QTransform createDistortTransform(const QRectF &bounds,
                                                  const QPointF &topLeftOffset,
                                                  const QPointF &topRightOffset,
                                                  const QPointF &bottomLeftOffset,
                                                  const QPointF &bottomRightOffset) const;
    
    // Perspective transform
    Q_INVOKABLE QTransform createPerspectiveTransform(const QRectF &bounds,
                                                      const QPointF &topLeft,
                                                      const QPointF &topRight,
                                                      const QPointF &bottomLeft,
                                                      const QPointF &bottomRight) const;
    
    // Flip
    Q_INVOKABLE QTransform createFlipHorizontal(const QRectF &bounds) const;
    Q_INVOKABLE QTransform createFlipVertical(const QRectF &bounds) const;
    
    // Apply transform to point
    Q_INVOKABLE QPointF applyTransform(const QTransform &transform, const QPointF &point) const;
    
    // Apply transform to rect
    Q_INVOKABLE QRectF applyTransformToRect(const QTransform &transform, const QRectF &rect) const;

signals:
    void currentTransformChanged();
    void transformApplied(const QTransform &transform);

private:
    QString m_currentTransform; // "free", "skew", "distort", "perspective", "flip-h", "flip-v"
    
    // Helper: Calculate perspective transform matrix
    QTransform calculatePerspectiveMatrix(const QPointF &src1, const QPointF &src2, 
                                         const QPointF &src3, const QPointF &src4,
                                         const QPointF &dst1, const QPointF &dst2,
                                         const QPointF &dst3, const QPointF &dst4) const;
};

#endif // TRANSFORMTOOLS_H

