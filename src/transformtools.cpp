#include "transformtools.h"
#include <QtMath>
#include <QPolygonF>

TransformTools::TransformTools(QObject *parent)
    : QObject(parent)
    , m_currentTransform("free")
{
}

void TransformTools::setCurrentTransform(const QString &transform)
{
    if (m_currentTransform == transform)
        return;
    
    m_currentTransform = transform;
    emit currentTransformChanged();
}

QTransform TransformTools::createFreeTransform(const QRectF &bounds,
                                               const QPointF &topLeft,
                                               const QPointF &topRight,
                                               const QPointF &bottomLeft,
                                               const QPointF &bottomRight) const
{
    // Free transform: map corners to new positions
    QPointF src1 = bounds.topLeft();
    QPointF src2 = bounds.topRight();
    QPointF src3 = bounds.bottomLeft();
    QPointF src4 = bounds.bottomRight();
    
    return calculatePerspectiveMatrix(src1, src2, src3, src4,
                                    topLeft, topRight, bottomLeft, bottomRight);
}

QTransform TransformTools::createSkewTransform(const QRectF &bounds, qreal skewX, qreal skewY) const
{
    QTransform transform;
    transform.translate(bounds.center().x(), bounds.center().y());
    transform.shear(skewX, skewY);
    transform.translate(-bounds.center().x(), -bounds.center().y());
    return transform;
}

QTransform TransformTools::createDistortTransform(const QRectF &bounds,
                                                   const QPointF &topLeftOffset,
                                                   const QPointF &topRightOffset,
                                                   const QPointF &bottomLeftOffset,
                                                   const QPointF &bottomRightOffset) const
{
    QPointF topLeft = bounds.topLeft() + topLeftOffset;
    QPointF topRight = bounds.topRight() + topRightOffset;
    QPointF bottomLeft = bounds.bottomLeft() + bottomLeftOffset;
    QPointF bottomRight = bounds.bottomRight() + bottomRightOffset;
    
    QPointF src1 = bounds.topLeft();
    QPointF src2 = bounds.topRight();
    QPointF src3 = bounds.bottomLeft();
    QPointF src4 = bounds.bottomRight();
    
    return calculatePerspectiveMatrix(src1, src2, src3, src4,
                                    topLeft, topRight, bottomLeft, bottomRight);
}

QTransform TransformTools::createPerspectiveTransform(const QRectF &bounds,
                                                      const QPointF &topLeft,
                                                      const QPointF &topRight,
                                                      const QPointF &bottomLeft,
                                                      const QPointF &bottomRight) const
{
    QPointF src1 = bounds.topLeft();
    QPointF src2 = bounds.topRight();
    QPointF src3 = bounds.bottomLeft();
    QPointF src4 = bounds.bottomRight();
    
    return calculatePerspectiveMatrix(src1, src2, src3, src4,
                                    topLeft, topRight, bottomLeft, bottomRight);
}

QTransform TransformTools::createFlipHorizontal(const QRectF &bounds) const
{
    QTransform transform;
    transform.translate(bounds.center().x(), 0);
    transform.scale(-1, 1);
    transform.translate(-bounds.center().x(), 0);
    return transform;
}

QTransform TransformTools::createFlipVertical(const QRectF &bounds) const
{
    QTransform transform;
    transform.translate(0, bounds.center().y());
    transform.scale(1, -1);
    transform.translate(0, -bounds.center().y());
    return transform;
}

QPointF TransformTools::applyTransform(const QTransform &transform, const QPointF &point) const
{
    return transform.map(point);
}

QRectF TransformTools::applyTransformToRect(const QTransform &transform, const QRectF &rect) const
{
    QPolygonF poly;
    poly << rect.topLeft() << rect.topRight() << rect.bottomRight() << rect.bottomLeft();
    QPolygonF transformed = transform.map(poly);
    return transformed.boundingRect();
}

QTransform TransformTools::calculatePerspectiveMatrix(const QPointF &src1, const QPointF &src2,
                                                      const QPointF &src3, const QPointF &src4,
                                                      const QPointF &dst1, const QPointF &dst2,
                                                      const QPointF &dst3, const QPointF &dst4) const
{
    // Perspective transform using 4-point mapping
    // Uses homography calculation (simplified version)
    
    // For a full implementation, would solve the homography matrix
    // This is a simplified version using QTransform's perspective capabilities
    
    // Calculate scale and rotation from corners
    QPointF srcCenter = (src1 + src2 + src3 + src4) / 4.0;
    QPointF dstCenter = (dst1 + dst2 + dst3 + dst4) / 4.0;
    
    QTransform transform;
    transform.translate(dstCenter.x(), dstCenter.y());
    
    // Calculate average scale
    qreal srcWidth = qSqrt(qPow(src2.x() - src1.x(), 2) + qPow(src2.y() - src1.y(), 2));
    qreal srcHeight = qSqrt(qPow(src3.x() - src1.x(), 2) + qPow(src3.y() - src1.y(), 2));
    qreal dstWidth = qSqrt(qPow(dst2.x() - dst1.x(), 2) + qPow(dst2.y() - dst1.y(), 2));
    qreal dstHeight = qSqrt(qPow(dst3.x() - dst1.x(), 2) + qPow(dst3.y() - dst1.y(), 2));
    
    if (srcWidth > 0 && srcHeight > 0) {
        qreal scaleX = dstWidth / srcWidth;
        qreal scaleY = dstHeight / srcHeight;
        transform.scale(scaleX, scaleY);
    }
    
    transform.translate(-srcCenter.x(), -srcCenter.y());
    
    return transform;
}

