import QtQuick
import QtQuick.Controls
import RemarkableDraftingPro

/**
 * AreaMeasurementOverlay - Visual area fill visualization
 * Shows measured areas with fill color
 */
Canvas {
    id: areaOverlay
    
    property AdvancedMeasurement advancedMeasurement: null
    
    visible: advancedMeasurement && advancedMeasurement.showAreaFill
    z: 7
    
    onPaint: {
        if (!advancedMeasurement || !advancedMeasurement.showAreaFill)
            return;
        
        var ctx = getContext("2d");
        var areaMeasurements = advancedMeasurement.getAllAreaMeasurements();
        
        for (var i = 0; i < areaMeasurements.length; i++) {
            var area = areaMeasurements[i];
            
            if (area.points.length < 3)
                continue;
            
            // Draw filled area
            if (area.filled) {
                ctx.fillStyle = area.fillColor;
                ctx.beginPath();
                ctx.moveTo(area.points[0].x, area.points[0].y);
                for (var j = 1; j < area.points.length; j++) {
                    ctx.lineTo(area.points[j].x, area.points[j].y);
                }
                ctx.closePath();
                ctx.fill();
            }
            
            // Draw stroke
            ctx.strokeStyle = area.strokeColor;
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(area.points[0].x, area.points[0].y);
            for (var j = 1; j < area.points.length; j++) {
                ctx.lineTo(area.points[j].x, area.points[j].y);
            }
            ctx.closePath();
            ctx.stroke();
            
            // Draw area text
            if (area.points.length > 0) {
                var centerX = 0;
                var centerY = 0;
                for (var j = 0; j < area.points.length; j++) {
                    centerX += area.points[j].x;
                    centerY += area.points[j].y;
                }
                centerX /= area.points.length;
                centerY /= area.points.length;
                
                ctx.fillStyle = area.strokeColor;
                ctx.font = "14px sans-serif";
                ctx.textAlign = "center";
                ctx.fillText(area.area.toFixed(2) + " " + area.unit + "²", centerX, centerY);
            }
        }
    }
    
    Connections {
        target: advancedMeasurement
        function onAreaMeasurementAdded() {
            areaOverlay.requestPaint();
        }
    }
}

