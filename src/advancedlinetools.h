#ifndef ADVANCEDLINETOOLS_H
#define ADVANCEDLINETOOLS_H

#include <QObject>
#include <QPointF>
#include <QList>
#include <QColor>

/**
 * AdvancedLineTools - Advanced line drawing tools similar to AutoCAD/Fusion 360
 * Construction lines, polylines, splines, fillet, chamfer, trim/extend, etc.
 */
class AdvancedLineTools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentTool READ currentTool WRITE setCurrentTool NOTIFY currentToolChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(bool constructionMode READ constructionMode WRITE setConstructionMode NOTIFY constructionModeChanged)

public:
    explicit AdvancedLineTools(QObject *parent = nullptr);
    
    QString currentTool() const { return m_currentTool; }
    qreal lineWidth() const { return m_lineWidth; }
    QColor lineColor() const { return m_lineColor; }
    bool constructionMode() const { return m_constructionMode; }
    
    void setCurrentTool(const QString &tool);
    void setLineWidth(qreal width);
    void setLineColor(const QColor &color);
    void setConstructionMode(bool enabled);
    
    // Line types
    enum LineType {
        LineStandard,      // Standard line (two points)
        LineRay,           // Ray (one point, infinite direction)
        LineConstruction,  // Construction line (infinite both ways)
        LinePolyline,      // Polyline (multiple connected segments)
        LineSpline,        // Spline curve
        LineBezier,        // Bezier curve
        LineTangent,       // Tangent to circle/arc
        LinePerpendicular, // Perpendicular to line
        LineParallel,      // Parallel to line
        LineOffset         // Offset from line
    };
    Q_ENUM(LineType)
    
    // Construction line
    Q_INVOKABLE void startConstructionLine(const QPointF &point, const QPointF &direction);
    Q_INVOKABLE void addConstructionLine(const QPointF &point, const QPointF &direction);
    
    // Ray line
    Q_INVOKABLE void startRay(const QPointF &startPoint, const QPointF &direction);
    Q_INVOKABLE void addRay(const QPointF &startPoint, const QPointF &direction);
    
    // Polyline
    Q_INVOKABLE void startPolyline(const QPointF &startPoint);
    Q_INVOKABLE void addPolylinePoint(const QPointF &point);
    Q_INVOKABLE void finishPolyline();
    Q_INVOKABLE void cancelPolyline();
    
    // Spline
    Q_INVOKABLE void startSpline(const QPointF &startPoint);
    Q_INVOKABLE void addSplinePoint(const QPointF &point);
    Q_INVOKABLE void finishSpline();
    
    // Bezier curve
    Q_INVOKABLE void startBezier(const QPointF &startPoint);
    Q_INVOKABLE void addBezierControlPoint(const QPointF &point, int controlIndex);
    Q_INVOKABLE void finishBezier();
    
    // Tangent line
    Q_INVOKABLE void createTangentLine(const QPointF &point, const QPointF &circleCenter, qreal radius);
    
    // Perpendicular line
    Q_INVOKABLE void createPerpendicularLine(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd);
    
    // Parallel line
    Q_INVOKABLE void createParallelLine(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd, qreal distance);
    
    // Offset line
    Q_INVOKABLE void createOffsetLine(const QPointF &lineStart, const QPointF &lineEnd, qreal offset);
    
    // Fillet (rounded corner)
    Q_INVOKABLE void createFillet(const QPointF &line1Start, const QPointF &line1End,
                                  const QPointF &line2Start, const QPointF &line2End,
                                  qreal radius);
    
    // Chamfer (beveled corner)
    Q_INVOKABLE void createChamfer(const QPointF &line1Start, const QPointF &line1End,
                                   const QPointF &line2Start, const QPointF &line2End,
                                   qreal distance1, qreal distance2);
    
    // Trim/extend
    Q_INVOKABLE void trimLine(const QPointF &trimPoint, const QPointF &lineStart, const QPointF &lineEnd);
    Q_INVOKABLE void extendLine(const QPointF &extendPoint, const QPointF &lineStart, const QPointF &lineEnd);
    
    // Break line
    Q_INVOKABLE void breakLine(const QPointF &breakPoint, const QPointF &lineStart, const QPointF &lineEnd);
    
    // Join lines
    Q_INVOKABLE void joinLines(const QPointF &line1Start, const QPointF &line1End,
                               const QPointF &line2Start, const QPointF &line2End);
    
    // Line data structures
    struct LineSegment {
        QPointF start;
        QPointF end;
        LineType type;
        QColor color;
        qreal width;
        bool construction;
    };
    
    struct PolylineData {
        QList<QPointF> points;
        bool closed;
        QColor color;
        qreal width;
    };
    
    struct SplineData {
        QList<QPointF> controlPoints;
        int degree;
        QColor color;
        qreal width;
    };
    
    struct BezierData {
        QPointF start;
        QPointF control1;
        QPointF control2;
        QPointF end;
        QColor color;
        qreal width;
    };
    
    // Get current drawing data
    Q_INVOKABLE QList<LineSegment> getAllLines() const { return m_lines; }
    Q_INVOKABLE QList<PolylineData> getAllPolylines() const { return m_polylines; }
    Q_INVOKABLE QList<SplineData> getAllSplines() const { return m_splines; }
    Q_INVOKABLE QList<BezierData> getAllBeziers() const { return m_beziers; }
    
    // Current drawing state
    Q_INVOKABLE bool isDrawingPolyline() const { return m_isDrawingPolyline; }
    Q_INVOKABLE bool isDrawingSpline() const { return m_isDrawingSpline; }
    Q_INVOKABLE bool isDrawingBezier() const { return m_isDrawingBezier; }
    Q_INVOKABLE QList<QPointF> getCurrentPolylinePoints() const { return m_currentPolylinePoints; }
    Q_INVOKABLE QList<QPointF> getCurrentSplinePoints() const { return m_currentSplinePoints; }
    Q_INVOKABLE QList<QPointF> getCurrentBezierPoints() const { return m_currentBezierPoints; }

signals:
    void currentToolChanged();
    void lineWidthChanged();
    void lineColorChanged();
    void constructionModeChanged();
    void lineCreated(const LineSegment &line);
    void polylineCreated(const PolylineData &polyline);
    void splineCreated(const SplineData &spline);
    void bezierCreated(const BezierData &bezier);
    void drawingStarted();
    void drawingUpdated();
    void drawingFinished();

private:
    QString m_currentTool; // "line", "ray", "construction", "polyline", "spline", "bezier", 
                           // "tangent", "perpendicular", "parallel", "offset", "fillet", "chamfer",
                           // "trim", "extend", "break", "join"
    qreal m_lineWidth;
    QColor m_lineColor;
    bool m_constructionMode;
    
    QList<LineSegment> m_lines;
    QList<PolylineData> m_polylines;
    QList<SplineData> m_splines;
    QList<BezierData> m_beziers;
    
    // Current drawing state
    bool m_isDrawingPolyline;
    bool m_isDrawingSpline;
    bool m_isDrawingBezier;
    QList<QPointF> m_currentPolylinePoints;
    QList<QPointF> m_currentSplinePoints;
    QList<QPointF> m_currentBezierPoints;
    int m_currentBezierControlIndex;
    
    // Helper functions
    QPointF calculateIntersection(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);
    qreal calculateDistance(const QPointF &p1, const QPointF &p2);
    QPointF calculatePerpendicularPoint(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd);
    QPointF calculateTangentPoint(const QPointF &point, const QPointF &circleCenter, qreal radius);
    QPointF calculateOffsetPoint(const QPointF &point, const QPointF &lineStart, const QPointF &lineEnd, qreal offset);
    QPointF calculateFilletArcCenter(const QPointF &line1Start, const QPointF &line1End,
                                     const QPointF &line2Start, const QPointF &line2End,
                                     qreal radius);
};

#endif // ADVANCEDLINETOOLS_H

