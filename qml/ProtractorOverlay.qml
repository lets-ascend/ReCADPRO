import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * ProtractorOverlay - Visual protractor tool overlay
 * Shows angle measurement with arc and angle display
 */
Canvas {
    id: protractorOverlay
    
    property Measurement measurement: null
    property bool active: false
    property point centerPoint: Qt.point(0, 0)
    property point point1: Qt.point(0, 0)
    property point point2: Qt.point(0, 0)
    
    property qreal radius: {
        if (!active) return 100;
        var dist1 = Math.sqrt(Math.pow(point1.x - centerPoint.x, 2) + Math.pow(point1.y - centerPoint.y, 2));
        var dist2 = Math.sqrt(Math.pow(point2.x - centerPoint.x, 2) + Math.pow(point2.y - centerPoint.y, 2));
        return Math.max(dist1, dist2, 50); // Minimum 50px radius
    }
    
    visible: active
    z: 9
    
    onPaint: {
        if (!active || !measurement) return;
        
        var ctx = getContext("2d");
        
        // Draw protractor arc
        ctx.strokeStyle = "#0066cc";
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.arc(centerPoint.x, centerPoint.y, radius, 0, Math.PI * 2);
        ctx.stroke();
        
        // Draw angle lines
        ctx.strokeStyle = "#0066cc";
        ctx.lineWidth = 1;
        ctx.setLineDash([3, 3]);
        ctx.beginPath();
        ctx.moveTo(centerPoint.x, centerPoint.y);
        ctx.lineTo(point1.x, point1.y);
        ctx.stroke();
        
        ctx.beginPath();
        ctx.moveTo(centerPoint.x, centerPoint.y);
        ctx.lineTo(point2.x, point2.y);
        ctx.stroke();
        ctx.setLineDash([]);
        
        // Calculate angle
        var angle = measurement.measureAngle(point1, centerPoint, point2);
        var angleDegrees = angle * 180 / Math.PI;
        
        // Draw angle arc
        var angle1 = Math.atan2(point1.y - centerPoint.y, point1.x - centerPoint.x);
        var angle2 = Math.atan2(point2.y - centerPoint.y, point2.x - centerPoint.x);
        
        ctx.strokeStyle = "#ff6600";
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.arc(centerPoint.x, centerPoint.y, radius * 0.7, angle1, angle2);
        ctx.stroke();
        
        // Draw angle text
        var midAngle = (angle1 + angle2) / 2;
        var textX = centerPoint.x + Math.cos(midAngle) * radius * 0.5;
        var textY = centerPoint.y + Math.sin(midAngle) * radius * 0.5;
        
        ctx.fillStyle = "#0066cc";
        ctx.font = "14px sans-serif";
        ctx.textAlign = "center";
        ctx.fillText(angleDegrees.toFixed(1) + "°", textX, textY);
        
        // Draw center marker
        drawMarker(ctx, centerPoint.x, centerPoint.y);
    }
    
    function drawMarker(ctx, x, y) {
        ctx.fillStyle = "#0066cc";
        ctx.beginPath();
        ctx.arc(x, y, 5, 0, Math.PI * 2);
        ctx.fill();
        
        // Draw crosshair
        ctx.strokeStyle = "#0066cc";
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.moveTo(x - 8, y);
        ctx.lineTo(x + 8, y);
        ctx.moveTo(x, y - 8);
        ctx.lineTo(x, y + 8);
        ctx.stroke();
    }
    
    function updateProtractor(center, p1, p2) {
        centerPoint = center;
        point1 = p1;
        point2 = p2;
        requestPaint();
    }
    
    function clearProtractor() {
        active = false;
        requestPaint();
    }
}

