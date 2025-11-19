import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * SnapIndicatorOverlay - Visual indicator for snap points
 * Shows snap point location and type with color-coded markers
 */
Canvas {
    id: snapOverlay
    
    property SnapTools snapTools: null
    property point snapPoint: Qt.point(0, 0)
    property string snapType: ""
    
    visible: snapTools && snapTools.snapEnabled && snapType !== ""
    z: 11
    
    onPaint: {
        if (!snapTools || snapType === "")
            return;
        
        var ctx = getContext("2d");
        ctx.clearRect(0, 0, width, height);
        
        // Get color based on snap type
        var color = "#808080"; // Gray default
        if (snapType === "endpoint") {
            color = "#FF0000"; // Red
        } else if (snapType === "midpoint") {
            color = "#00FF00"; // Green
        } else if (snapType === "intersection") {
            color = "#0000FF"; // Blue
        } else if (snapType === "center") {
            color = "#FFFF00"; // Yellow
        } else if (snapType === "perpendicular") {
            color = "#FF00FF"; // Magenta
        } else if (snapType === "tangent") {
            color = "#00FFFF"; // Cyan
        }
        
        // Draw snap indicator (crosshair with circle)
        ctx.strokeStyle = color;
        ctx.lineWidth = 2;
        
        var x = snapPoint.x;
        var y = snapPoint.y;
        var size = 12;
        
        // Draw circle
        ctx.beginPath();
        ctx.arc(x, y, size, 0, Math.PI * 2);
        ctx.stroke();
        
        // Draw crosshair
        ctx.beginPath();
        ctx.moveTo(x - size - 4, y);
        ctx.lineTo(x - size, y);
        ctx.moveTo(x + size, y);
        ctx.lineTo(x + size + 4, y);
        ctx.moveTo(x, y - size - 4);
        ctx.lineTo(x, y - size);
        ctx.moveTo(x, y + size);
        ctx.lineTo(x, y + size + 4);
        ctx.stroke();
    }
    
    Connections {
        target: snapTools
        function onSnapPointFound(point, type) {
            snapOverlay.snapPoint = point;
            snapOverlay.snapType = type;
            snapOverlay.requestPaint();
        }
    }
}

