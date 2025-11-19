import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * DrawingCanvas - Main drawing surface optimized for reMarkable Paper Pro
 * Handles touch and stylus input with pressure sensitivity
 * Integrated with selection, shapes, and text tools
 */
DrawingEngine {
    id: canvas
    
    property SelectionManager selectionManager: null
    property ShapeTools shapeTools: null
    property TextTool textTool: null
    property DraftingTools draftingTools: null
    property Tools tools: null
    property AdvancedLineTools advancedLineTools: null
    property Measurement measurement: null
    property TransformTools transformTools: null
    property BrushTools brushTools: null
    property AdvancedMeasurement advancedMeasurement: null
    property Drawing3DEnhanced drawing3DEnhanced: null
    property SnapTools snapTools: null
    
    // Canvas properties
    clip: true
    
    // Drawing state
    property bool isDrawingShape: false
    property point shapeStartPoint: Qt.point(0, 0)
    property point shapeCurrentPoint: Qt.point(0, 0)
    property point m_rayStartPoint: null
    property point m_constructionStartPoint: null
    property point m_rulerStartPoint: null
    property point m_protractorCenter: null
    property point m_protractorPoint1: null
    
    // Optimize for e-paper refresh
    // Note: Full optimization requires -platform epaper flag when running
    
    // Visual feedback
    Rectangle {
        anchors.fill: parent
        color: "white"
        border.color: "#e0e0e0"
        border.width: 1
        z: -1
    }
    
    // Selection overlay
    SelectionOverlay {
        id: selectionOverlay
        anchors.fill: parent
        selectionManager: canvas.selectionManager
        visible: selectionManager && selectionManager.hasSelection
        z: 10
    }
    
    // Ruler overlay
    RulerOverlay {
        id: rulerOverlay
        anchors.fill: parent
        measurement: canvas.measurement
        active: tools && tools.currentTool === "ruler"
        z: 9
    }
    
    // Protractor overlay
    ProtractorOverlay {
        id: protractorOverlay
        anchors.fill: parent
        measurement: canvas.measurement
        active: tools && tools.currentTool === "protractor"
        z: 9
    }
    
    // Dimension lines overlay
    DimensionLineOverlay {
        id: dimensionLineOverlay
        anchors.fill: parent
        advancedMeasurement: canvas.advancedMeasurement
        z: 8
    }
    
    // Area measurement overlay
    AreaMeasurementOverlay {
        id: areaMeasurementOverlay
        anchors.fill: parent
        advancedMeasurement: canvas.advancedMeasurement
        z: 7
    }
    
    // Snap indicator overlay
    SnapIndicatorOverlay {
        id: snapIndicatorOverlay
        anchors.fill: parent
        snapTools: canvas.snapTools
    }
    
    // Text rendering overlay
    Repeater {
        model: textTool ? textTool.getAllTexts() : []
        
        Text {
            x: modelData.position.x
            y: modelData.position.y
            text: modelData.text
            font: modelData.font
            color: modelData.color
            z: 5
        }
    }
    
    // Shape preview (while drawing)
    Canvas {
        id: shapePreviewCanvas
        anchors.fill: parent
        z: 8
        visible: isDrawingShape
        
        onPaint: {
            var ctx = getContext("2d");
            ctx.strokeStyle = canvas.color;
            ctx.lineWidth = canvas.lineWidth;
            
            if (shapeTools && isDrawingShape) {
                var shape = shapeTools.getCurrentShape();
                drawShapePreview(ctx, shape);
            }
        }
        
        function drawShapePreview(ctx, shape) {
            var start = shape.startPoint;
            var end = shape.endPoint;
            
            switch(shape.type) {
                case "rectangle":
                    ctx.strokeRect(start.x, start.y, end.x - start.x, end.y - start.y);
                    if (shape.filled) {
                        ctx.fillStyle = shape.fillColor;
                        ctx.fillRect(start.x, start.y, end.x - start.x, end.y - start.y);
                    }
                    break;
                case "circle":
                    var radius = Math.sqrt(Math.pow(end.x - start.x, 2) + Math.pow(end.y - start.y, 2));
                    ctx.beginPath();
                    ctx.arc(start.x, start.y, radius, 0, Math.PI * 2);
                    ctx.stroke();
                    if (shape.filled) {
                        ctx.fill();
                    }
                    break;
                case "arc":
                    // Draw arc preview based on current step
                    if (shape.centerPoint && shape.radius > 0) {
                        // Arc is calculated, draw it
                        var startAngleRad = shape.startAngle * Math.PI / 180;
                        var endAngleRad = shape.endAngle * Math.PI / 180;
                        var center = shape.centerPoint;
                        var radius = shape.radius;
                        
                        ctx.beginPath();
                        ctx.arc(center.x, center.y, radius, startAngleRad, endAngleRad);
                        ctx.stroke();
                    } else if (shape.middlePoint && shape.middlePoint.x !== start.x && shape.middlePoint.y !== start.y) {
                        // 3-point arc: show preview through 3 points
                        ctx.beginPath();
                        ctx.moveTo(start.x, start.y);
                        ctx.quadraticCurveTo(shape.middlePoint.x, shape.middlePoint.y, end.x, end.y);
                        ctx.stroke();
                    } else {
                        // Just show line from start to end
                        ctx.beginPath();
                        ctx.moveTo(start.x, start.y);
                        ctx.lineTo(end.x, end.y);
                        ctx.stroke();
                    }
                    break;
                case "line":
                    ctx.beginPath();
                    ctx.moveTo(start.x, start.y);
                    ctx.lineTo(end.x, end.y);
                    ctx.stroke();
                    break;
                case "arrow":
                    ctx.beginPath();
                    ctx.moveTo(start.x, start.y);
                    ctx.lineTo(end.x, end.y);
                    ctx.stroke();
                    // Arrow head would be drawn here
                    break;
            }
        }
    }
    
    // Zoom and pan handler
    ZoomPanHandler {
        id: zoomPanHandler
        anchors.fill: parent
        enabled: tools && tools.currentTool !== "select" && tools.currentTool !== "pan"  // Disable during selection, enable pan tool
        
        undoCallback: function() {
            if (window && window.drawingEngine) {
                window.drawingEngine.undo()
            }
        }
        
        redoCallback: function() {
            if (window && window.drawingEngine) {
                window.drawingEngine.redo()
            }
        }
        
        onZoomChanged: (level) => {
            // Apply zoom transform to canvas
            canvas.scale = level
        }
        
        onPanChanged: (offset) => {
            // Apply pan transform to canvas
            canvas.x = offset.x
            canvas.y = offset.y
        }
    }
    
    // Zoom controls overlay
    ZoomControls {
        id: zoomControlsOverlay
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        zoomHandler: zoomPanHandler
        selectionManager: canvas.selectionManager
        z: 100
    }
    
    // Mouse/touch handling for shapes
    MouseArea {
        anchors.fill: parent
        enabled: tools && tools.currentTool && (tools.currentTool === "rectangle" || 
                                       tools.currentTool === "circle" || 
                                       tools.currentTool === "arc" ||
                                       tools.currentTool === "line" || 
                                       tools.currentTool === "arrow" ||
                                       tools.currentTool === "text" ||
                                       tools.currentTool === "select" ||
                                       tools.currentTool === "ruler" ||
                                       tools.currentTool === "protractor" ||
                                       tools.currentTool === "ray" ||
                                       tools.currentTool === "construction" ||
                                       tools.currentTool === "polyline" ||
                                       tools.currentTool === "spline" ||
                                       tools.currentTool === "bezier" ||
                                       tools.currentTool === "tangent" ||
                                       tools.currentTool === "perpendicular" ||
                                       tools.currentTool === "parallel" ||
                                       tools.currentTool === "offset" ||
                                       tools.currentTool === "fillet" ||
                                       tools.currentTool === "chamfer" ||
                                       tools.currentTool === "trim" ||
                                       tools.currentTool === "extend" ||
                                       tools.currentTool === "break" ||
                                       tools.currentTool === "join")
        
        onPressed: (mouse) => {
            var point = Qt.point(mouse.x, mouse.y);
            
            // Snap to grid if enabled
            if (draftingTools && draftingTools.snapToGrid) {
                var snapped = draftingTools.snapPoint(point.x, point.y);
                point = Qt.point(snapped.x, snapped.y);
            }
            
            // Apply object snap if enabled
            if (snapTools && snapTools.snapEnabled) {
                var snapInfo = snapTools.getSnapInfo(point);
                if (snapInfo.type !== "") {
                    point = snapInfo.point;
                }
            }
            
            if (tools && tools.currentTool === "text") {
                // Open text input dialog
                textInputDialog.position = point;
                textInputDialog.textTool = canvas.textTool;
                textInputDialog.open();
            } else if (tools && tools.currentTool === "select") {
                // Start selection
                if (selectionManager) {
                    selectionManager.startRectangleSelection(point);
                }
            } else if (tools && tools.currentTool === "polyline" && advancedLineTools) {
                // Start polyline
                if (!advancedLineTools.isDrawingPolyline()) {
                    advancedLineTools.startPolyline(point);
                } else {
                    advancedLineTools.addPolylinePoint(point);
                }
            } else if (tools && tools.currentTool === "spline" && advancedLineTools) {
                // Start spline
                if (!advancedLineTools.isDrawingSpline()) {
                    advancedLineTools.startSpline(point);
                } else {
                    advancedLineTools.addSplinePoint(point);
                }
            } else if (tools && tools.currentTool === "bezier" && advancedLineTools) {
                // Start bezier
                if (!advancedLineTools.isDrawingBezier()) {
                    advancedLineTools.startBezier(point);
                } else {
                    var controlIndex = advancedLineTools.getCurrentBezierPoints().length;
                    if (controlIndex < 4) {
                        advancedLineTools.addBezierControlPoint(point, controlIndex);
                    }
                }
            } else if (tools && tools.currentTool === "ray" && advancedLineTools) {
                // Create ray - need direction point
                if (!m_rayStartPoint) {
                    m_rayStartPoint = point;
                } else {
                    var direction = Qt.point(point.x - m_rayStartPoint.x, point.y - m_rayStartPoint.y);
                    advancedLineTools.addRay(m_rayStartPoint, direction);
                    m_rayStartPoint = null;
                }
            } else if (tools && tools.currentTool === "construction" && advancedLineTools) {
                // Create construction line - need direction point
                if (!m_constructionStartPoint) {
                    m_constructionStartPoint = point;
                } else {
                    var direction = Qt.point(point.x - m_constructionStartPoint.x, point.y - m_constructionStartPoint.y);
                    advancedLineTools.addConstructionLine(m_constructionStartPoint, direction);
                    m_constructionStartPoint = null;
                }
            } else if (tools && tools.currentTool === "ruler" && measurement) {
                // Ruler tool - measure distance
                if (!m_rulerStartPoint) {
                    m_rulerStartPoint = point;
                    rulerOverlay.active = true;
                    rulerOverlay.startPoint = point;
                    rulerOverlay.endPoint = point;
                } else {
                    rulerOverlay.endPoint = point;
                }
            } else if (tools && tools.currentTool === "protractor" && measurement) {
                // Protractor tool - measure angle
                if (!m_protractorCenter) {
                    m_protractorCenter = point;
                    protractorOverlay.active = true;
                    protractorOverlay.centerPoint = point;
                    protractorOverlay.point1 = point;
                    protractorOverlay.point2 = point;
                } else if (!m_protractorPoint1) {
                    m_protractorPoint1 = point;
                    protractorOverlay.point1 = point;
                    protractorOverlay.point2 = point;
                } else {
                    protractorOverlay.point2 = point;
                }
            } else if (shapeTools) {
                // Start shape drawing
                isDrawingShape = true;
                shapeStartPoint = point;
                shapeCurrentPoint = point;
                shapeTools.startShape(point);
                shapePreviewCanvas.requestPaint();
            }
        }
        
        onPositionChanged: (mouse) => {
            var point = Qt.point(mouse.x, mouse.y);
            
            // Snap to grid if enabled
            if (draftingTools && draftingTools.snapToGrid) {
                var snapped = draftingTools.snapPoint(point.x, point.y);
                point = Qt.point(snapped.x, snapped.y);
            }
            
            // Apply object snap if enabled
            if (snapTools && snapTools.snapEnabled) {
                var snapInfo = snapTools.getSnapInfo(point);
                if (snapInfo.type !== "") {
                    snapIndicatorOverlay.snapPoint = snapInfo.point;
                    snapIndicatorOverlay.snapType = snapInfo.type;
                    point = snapInfo.point;
                } else {
                    snapIndicatorOverlay.snapType = "";
                }
            } else {
                snapIndicatorOverlay.snapType = "";
            }
            
            if (tools && tools.currentTool === "select" && selectionManager) {
                selectionManager.updateRectangleSelection(point);
            } else if (tools && tools.currentTool === "ruler" && rulerOverlay.active) {
                rulerOverlay.endPoint = point;
            } else if (tools && tools.currentTool === "protractor" && protractorOverlay.active) {
                if (m_protractorPoint1) {
                    protractorOverlay.point2 = point;
                } else {
                    protractorOverlay.point1 = point;
                }
            } else if (isDrawingShape && shapeTools) {
                shapeCurrentPoint = point;
                shapeTools.updateShape(point);
                shapePreviewCanvas.requestPaint();
            }
        }
        
        onReleased: (mouse) => {
            if (tools && tools.currentTool === "select" && selectionManager) {
                selectionManager.finishRectangleSelection();
            } else if (tools && tools.currentTool === "ruler" && m_rulerStartPoint) {
                // Ruler measurement complete - keep overlay visible
                // Can click again to start new measurement
            } else if (tools && tools.currentTool === "protractor" && m_protractorCenter) {
                // Protractor measurement complete - keep overlay visible
                // Can click again to start new measurement
            } else if (tools && tools.currentTool === "polyline" && advancedLineTools) {
                // Double-click or right-click to finish polyline
                // For now, finish on release (can be changed to double-click)
            } else if (tools && tools.currentTool === "spline" && advancedLineTools && advancedLineTools.isDrawingSpline()) {
                // Right-click or button to finish spline
            } else if (tools && tools.currentTool === "bezier" && advancedLineTools && advancedLineTools.isDrawingBezier()) {
                // Finish when all 4 control points are set
                if (advancedLineTools.getCurrentBezierPoints().length >= 4) {
                    advancedLineTools.finishBezier();
                }
            } else if (isDrawingShape && shapeTools) {
                shapeTools.finishShape();
                isDrawingShape = false;
                shapePreviewCanvas.requestPaint();
            }
        }
        
        // Double-click to finish polyline/spline
        onDoubleClicked: (mouse) => {
            if (tools && tools.currentTool === "polyline" && advancedLineTools && advancedLineTools.isDrawingPolyline()) {
                advancedLineTools.finishPolyline();
            } else if (tools && tools.currentTool === "spline" && advancedLineTools && advancedLineTools.isDrawingSpline()) {
                advancedLineTools.finishSpline();
            }
        }
    }
    
    // Text input dialog
    TextInputDialog {
        id: textInputDialog
        anchors.centerIn: parent
    }
    
    // Coordinate display (optional, for debugging)
    Text {
        id: coordDisplay
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 10
        text: "X: 0, Y: 0"
        font.pixelSize: 12
        color: "#666666"
        visible: false // Set to true for debugging
    }
}
