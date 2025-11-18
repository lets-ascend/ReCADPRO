import QtQuick
import QtQuick.Controls
import RemarkableDraftingPro

/**
 * DimensionLineOverlay - Visual dimension lines with arrows
 * Shows measurement annotations on canvas
 */
Canvas {
    id: dimensionOverlay
    
    property AdvancedMeasurement advancedMeasurement: null
    
    visible: advancedMeasurement && advancedMeasurement.showDimensionLines
    z: 8
    
    onPaint: {
        if (!advancedMeasurement || !advancedMeasurement.showDimensionLines)
            return;
        
        var ctx = getContext("2d");
        var dimensionLines = advancedMeasurement.getAllDimensionLines();
        
        for (var i = 0; i < dimensionLines.length; i++) {
            var dim = dimensionLines[i];
            
            // Draw dimension line
            ctx.strokeStyle = dim.color;
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(dim.start.x, dim.start.y);
            ctx.lineTo(dim.end.x, dim.end.y);
            ctx.stroke();
            
            // Draw arrows
            if (dim.showArrows) {
                var arrows = advancedMeasurement.getDimensionLineArrows(dim);
                drawArrow(ctx, dim.start, dim.end, dim.arrowSize, true);
                drawArrow(ctx, dim.end, dim.start, dim.arrowSize, false);
            }
            
            // Draw measurement text
            var midX = (dim.start.x + dim.end.x) / 2;
            var midY = (dim.start.y + dim.end.y) / 2;
            
            ctx.fillStyle = dim.color;
            ctx.font = "14px sans-serif";
            ctx.textAlign = "center";
            ctx.fillText(dim.measurement + " " + dim.unit, midX, midY - 10);
        }
    }
    
    function drawArrow(ctx, start, end, size, atStart) {
        var angle = Math.atan2(end.y - start.y, end.x - start.x);
        var arrowAngle = Math.PI / 6; // 30 degrees
        
        var base = atStart ? start : end;
        var tipX, tipY;
        
        if (atStart) {
            tipX = base.x + Math.cos(angle) * size;
            tipY = base.y + Math.sin(angle) * size;
        } else {
            tipX = base.x - Math.cos(angle) * size;
            tipY = base.y - Math.sin(angle) * size;
        }
        
        var arrow1X = tipX - Math.cos(angle - arrowAngle) * size;
        var arrow1Y = tipY - Math.sin(angle - arrowAngle) * size;
        var arrow2X = tipX - Math.cos(angle + arrowAngle) * size;
        var arrow2Y = tipY - Math.sin(angle + arrowAngle) * size;
        
        ctx.beginPath();
        ctx.moveTo(tipX, tipY);
        ctx.lineTo(arrow1X, arrow1Y);
        ctx.moveTo(tipX, tipY);
        ctx.lineTo(arrow2X, arrow2Y);
        ctx.stroke();
    }
    
    Connections {
        target: advancedMeasurement
        function onDimensionLineAdded() {
            dimensionOverlay.requestPaint();
        }
    }
}

