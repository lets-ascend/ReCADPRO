import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * RulerOverlay - Visual ruler tool overlay
 * Shows measurement line with distance display
 */
Canvas {
    id: rulerOverlay
    
    property Measurement measurement: null
    property bool active: false
    property point startPoint: Qt.point(0, 0)
    property point endPoint: Qt.point(0, 0)
    
    visible: active
    z: 9
    
    onPaint: {
        if (!active || !measurement) return;
        
        var ctx = getContext("2d");
        
        // Draw ruler line
        ctx.strokeStyle = "#0066cc";
        ctx.lineWidth = 2;
        ctx.setLineDash([5, 5]);
        ctx.beginPath();
        ctx.moveTo(startPoint.x, startPoint.y);
        ctx.lineTo(endPoint.x, endPoint.y);
        ctx.stroke();
        ctx.setLineDash([]);
        
        // Calculate distance
        var distance = measurement.measureDistance(startPoint, endPoint);
        var formatted = measurement.formatMeasurement(distance);
        
        // Draw measurement text
        var midX = (startPoint.x + endPoint.x) / 2;
        var midY = (startPoint.y + endPoint.y) / 2;
        
        ctx.fillStyle = "#0066cc";
        ctx.font = "14px sans-serif";
        ctx.textAlign = "center";
        ctx.fillText(formatted, midX, midY - 10);
        
        // Draw start marker
        drawMarker(ctx, startPoint.x, startPoint.y);
        
        // Draw end marker
        drawMarker(ctx, endPoint.x, endPoint.y);
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
    
    function updateRuler(start, end) {
        startPoint = start;
        endPoint = end;
        requestPaint();
    }
    
    function clearRuler() {
        active = false;
        requestPaint();
    }
}

