import QtQuick
import QtQuick.Controls

/**
 * ToolIcon - Reusable icon component for tools
 * Optimized for e-paper display with high contrast
 */
Rectangle {
    id: iconRoot
    
    property string iconType: "pen"
    property bool checked: false
    property color iconColor: checked ? "#2196F3" : "#666666"
    property color backgroundColor: checked ? "#e3f2fd" : "transparent"
    
    width: 48
    height: 48
    radius: 4
    color: backgroundColor
    border.color: checked ? "#2196F3" : "#e0e0e0"
    border.width: checked ? 2 : 1
    
    Canvas {
        id: iconCanvas
        anchors.fill: parent
        anchors.margins: 8
        
        onPaint: {
            var ctx = getContext("2d");
            ctx.strokeStyle = iconRoot.iconColor;
            ctx.fillStyle = iconRoot.iconColor;
            ctx.lineWidth = 2;
            ctx.lineCap = "round";
            ctx.lineJoin = "round";
            
            drawIcon(ctx);
        }
        
        function drawIcon(ctx) {
            var w = width;
            var h = height;
            var centerX = w / 2;
            var centerY = h / 2;
            
            switch(iconType) {
                case "pen":
                    // Pen icon - simple line
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY + 4);
                    ctx.lineTo(centerX + 8, centerY - 4);
                    ctx.stroke();
                    // Pen tip
                    ctx.beginPath();
                    ctx.moveTo(centerX + 6, centerY - 3);
                    ctx.lineTo(centerX + 8, centerY - 4);
                    ctx.lineTo(centerX + 7, centerY - 2);
                    ctx.fill();
                    break;
                    
                case "pencil":
                    // Pencil icon - angled rectangle
                    ctx.beginPath();
                    ctx.moveTo(centerX - 6, centerY + 6);
                    ctx.lineTo(centerX + 6, centerY - 6);
                    ctx.lineTo(centerX + 8, centerY - 4);
                    ctx.lineTo(centerX - 4, centerY + 8);
                    ctx.closePath();
                    ctx.stroke();
                    break;
                    
                case "brush":
                    // Brush icon - rounded rectangle with bristles
                    ctx.beginPath();
                    ctx.rect(centerX - 4, centerY - 8, 8, 12);
                    ctx.stroke();
                    // Bristles
                    ctx.beginPath();
                    for (var i = -3; i <= 3; i++) {
                        ctx.moveTo(centerX + i, centerY + 4);
                        ctx.lineTo(centerX + i, centerY + 8);
                    }
                    ctx.stroke();
                    break;
                    
                case "eraser":
                    // Eraser icon - rectangle
                    ctx.beginPath();
                    ctx.rect(centerX - 8, centerY - 4, 16, 8);
                    ctx.stroke();
                    // Eraser lines
                    ctx.beginPath();
                    ctx.moveTo(centerX - 6, centerY - 2);
                    ctx.lineTo(centerX + 6, centerY - 2);
                    ctx.moveTo(centerX - 6, centerY + 2);
                    ctx.lineTo(centerX + 6, centerY + 2);
                    ctx.stroke();
                    break;
                    
                case "line":
                    // Line icon
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.stroke();
                    break;
                    
                case "rectangle":
                    // Rectangle icon
                    ctx.beginPath();
                    ctx.rect(centerX - 8, centerY - 6, 16, 12);
                    ctx.stroke();
                    break;
                    
                case "circle":
                    // Circle icon
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, 8, 0, Math.PI * 2);
                    ctx.stroke();
                    break;
                    
                case "arrow":
                    // Arrow icon
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX + 6, centerY);
                    ctx.moveTo(centerX + 4, centerY - 2);
                    ctx.lineTo(centerX + 6, centerY);
                    ctx.lineTo(centerX + 4, centerY + 2);
                    ctx.stroke();
                    break;
                    
                case "text":
                    // Text icon - letter A
                    ctx.font = "bold 16px sans-serif";
                    ctx.textAlign = "center";
                    ctx.textBaseline = "middle";
                    ctx.fillText("A", centerX, centerY);
                    break;
                    
                case "select":
                    // Selection icon - cursor/arrow
                    ctx.beginPath();
                    ctx.moveTo(centerX - 4, centerY - 6);
                    ctx.lineTo(centerX + 6, centerY + 4);
                    ctx.moveTo(centerX + 2, centerY);
                    ctx.lineTo(centerX + 6, centerY + 4);
                    ctx.lineTo(centerX + 4, centerY + 2);
                    ctx.stroke();
                    break;
                    
                case "ruler":
                    // Ruler icon - rectangle with marks
                    ctx.beginPath();
                    ctx.rect(centerX - 10, centerY - 3, 20, 6);
                    ctx.stroke();
                    // Ruler marks
                    for (var i = -8; i <= 8; i += 4) {
                        ctx.beginPath();
                        ctx.moveTo(centerX + i, centerY - 3);
                        ctx.lineTo(centerX + i, centerY - 1);
                        ctx.stroke();
                    }
                    break;
                    
                case "protractor":
                    // Protractor icon - arc
                    ctx.beginPath();
                    ctx.arc(centerX, centerY + 4, 10, Math.PI, 0, false);
                    ctx.stroke();
                    // Center point
                    ctx.beginPath();
                    ctx.arc(centerX, centerY + 4, 1, 0, Math.PI * 2);
                    ctx.fill();
                    break;
                    
                case "ray":
                    // Ray icon - line with arrow at one end
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.moveTo(centerX + 6, centerY - 2);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.lineTo(centerX + 6, centerY + 2);
                    ctx.stroke();
                    break;
                    
                case "construction":
                    // Construction line icon - dashed line
                    ctx.setLineDash([3, 3]);
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.stroke();
                    ctx.setLineDash([]);
                    break;
                    
                case "polyline":
                    // Polyline icon - connected segments
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY + 4);
                    ctx.lineTo(centerX - 2, centerY - 2);
                    ctx.lineTo(centerX + 4, centerY + 2);
                    ctx.lineTo(centerX + 8, centerY - 4);
                    ctx.stroke();
                    break;
                    
                case "spline":
                    // Spline icon - curved line
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY + 4);
                    ctx.quadraticCurveTo(centerX, centerY - 4, centerX + 8, centerY + 4);
                    ctx.stroke();
                    break;
                    
                case "bezier":
                    // Bezier icon - S curve
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY + 4);
                    ctx.bezierCurveTo(centerX - 4, centerY - 4, centerX + 4, centerY + 4, centerX + 8, centerY - 4);
                    ctx.stroke();
                    break;
                    
                case "tangent":
                    // Tangent icon - line touching circle
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, 6, 0, Math.PI * 2);
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY - 6);
                    ctx.lineTo(centerX + 8, centerY + 6);
                    ctx.stroke();
                    break;
                    
                case "perpendicular":
                    // Perpendicular icon - L shape
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX, centerY);
                    ctx.lineTo(centerX, centerY + 8);
                    ctx.stroke();
                    break;
                    
                case "parallel":
                    // Parallel icon - two parallel lines
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY - 4);
                    ctx.lineTo(centerX + 8, centerY - 4);
                    ctx.moveTo(centerX - 8, centerY + 4);
                    ctx.lineTo(centerX + 8, centerY + 4);
                    ctx.stroke();
                    break;
                    
                case "offset":
                    // Offset icon - parallel lines with arrow
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY - 4);
                    ctx.lineTo(centerX + 8, centerY - 4);
                    ctx.moveTo(centerX - 8, centerY + 4);
                    ctx.lineTo(centerX + 8, centerY + 4);
                    ctx.moveTo(centerX, centerY - 4);
                    ctx.lineTo(centerX, centerY + 4);
                    ctx.stroke();
                    break;
                    
                case "fillet":
                    // Fillet icon - rounded corner
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX, centerY);
                    ctx.arc(centerX, centerY, 4, Math.PI, Math.PI / 2, false);
                    ctx.lineTo(centerX, centerY + 8);
                    ctx.stroke();
                    break;
                    
                case "chamfer":
                    // Chamfer icon - angled corner
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX - 2, centerY);
                    ctx.lineTo(centerX, centerY + 2);
                    ctx.lineTo(centerX, centerY + 8);
                    ctx.stroke();
                    break;
                    
                case "trim":
                    // Trim icon - line with cut mark
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.moveTo(centerX - 2, centerY - 4);
                    ctx.lineTo(centerX + 2, centerY + 4);
                    ctx.stroke();
                    break;
                    
                case "extend":
                    // Extend icon - line with arrow extending
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX + 6, centerY);
                    ctx.moveTo(centerX + 4, centerY - 2);
                    ctx.lineTo(centerX + 6, centerY);
                    ctx.lineTo(centerX + 4, centerY + 2);
                    ctx.stroke();
                    break;
                    
                case "break":
                    // Break icon - line with gap
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX - 2, centerY);
                    ctx.moveTo(centerX + 2, centerY);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.stroke();
                    break;
                    
                case "join":
                    // Join icon - two lines joining
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY - 4);
                    ctx.lineTo(centerX, centerY);
                    ctx.moveTo(centerX - 8, centerY + 4);
                    ctx.lineTo(centerX, centerY);
                    ctx.stroke();
                    break;
                    
                case "sphere":
                    // Sphere icon - circle with shading
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, 8, 0, Math.PI * 2);
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.arc(centerX - 2, centerY - 2, 3, 0, Math.PI * 2);
                    ctx.stroke();
                    break;
                    
                case "cone":
                    // Cone icon - triangle on circle
                    ctx.beginPath();
                    ctx.arc(centerX, centerY + 4, 6, 0, Math.PI * 2);
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.moveTo(centerX, centerY - 6);
                    ctx.lineTo(centerX - 6, centerY + 4);
                    ctx.lineTo(centerX + 6, centerY + 4);
                    ctx.closePath();
                    ctx.stroke();
                    break;
                    
                case "pyramid":
                    // Pyramid icon - triangle
                    ctx.beginPath();
                    ctx.moveTo(centerX, centerY - 6);
                    ctx.lineTo(centerX - 6, centerY + 6);
                    ctx.lineTo(centerX + 6, centerY + 6);
                    ctx.closePath();
                    ctx.stroke();
                    break;
                    
                case "torus":
                    // Torus icon - two circles
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, 8, 0, Math.PI * 2);
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, 4, 0, Math.PI * 2);
                    ctx.stroke();
                    break;
                    
                case "free":
                    // Free transform icon - corner handles
                    ctx.beginPath();
                    ctx.rect(centerX - 6, centerY - 4, 12, 8);
                    ctx.stroke();
                    // Corner handles
                    ctx.fillRect(centerX - 6, centerY - 4, 3, 3);
                    ctx.fillRect(centerX + 3, centerY - 4, 3, 3);
                    ctx.fillRect(centerX - 6, centerY + 1, 3, 3);
                    ctx.fillRect(centerX + 3, centerY + 1, 3, 3);
                    break;
                    
                case "skew":
                    // Skew icon - parallelogram
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY - 4);
                    ctx.lineTo(centerX + 2, centerY - 4);
                    ctx.lineTo(centerX + 8, centerY + 4);
                    ctx.lineTo(centerX - 2, centerY + 4);
                    ctx.closePath();
                    ctx.stroke();
                    break;
                    
                case "perspective":
                    // Perspective icon - trapezoid
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY - 6);
                    ctx.lineTo(centerX + 8, centerY - 2);
                    ctx.lineTo(centerX + 6, centerY + 6);
                    ctx.lineTo(centerX - 6, centerY + 2);
                    ctx.closePath();
                    ctx.stroke();
                    break;
                    
                case "flip-h":
                    // Flip horizontal icon - arrows
                    ctx.beginPath();
                    ctx.moveTo(centerX - 8, centerY);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.moveTo(centerX - 6, centerY - 2);
                    ctx.lineTo(centerX - 8, centerY);
                    ctx.lineTo(centerX - 6, centerY + 2);
                    ctx.moveTo(centerX + 6, centerY - 2);
                    ctx.lineTo(centerX + 8, centerY);
                    ctx.lineTo(centerX + 6, centerY + 2);
                    ctx.stroke();
                    break;
                    
                case "flip-v":
                    // Flip vertical icon - arrows
                    ctx.beginPath();
                    ctx.moveTo(centerX, centerY - 8);
                    ctx.lineTo(centerX, centerY + 8);
                    ctx.moveTo(centerX - 2, centerY - 6);
                    ctx.lineTo(centerX, centerY - 8);
                    ctx.lineTo(centerX + 2, centerY - 6);
                    ctx.moveTo(centerX - 2, centerY + 6);
                    ctx.lineTo(centerX, centerY + 8);
                    ctx.lineTo(centerX + 2, centerY + 6);
                    ctx.stroke();
                    break;
                    
                default:
                    // Default icon - square
                    ctx.beginPath();
                    ctx.rect(centerX - 6, centerY - 6, 12, 12);
                    ctx.stroke();
                    break;
            }
        }
    }
    
    Component.onCompleted: {
        iconCanvas.requestPaint();
    }
    
    onIconTypeChanged: {
        iconCanvas.requestPaint();
    }
    
    onCheckedChanged: {
        iconCanvas.requestPaint();
    }
}

