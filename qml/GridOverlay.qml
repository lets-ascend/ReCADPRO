import QtQuick

/**
 * GridOverlay - Grid overlay for drafting tools
 * Optimized for reMarkable Paper Pro e-paper display
 */
Canvas {
    id: gridCanvas
    
    property string gridType: "lines"
    property real gridSize: 20.0
    
    anchors.fill: parent
    
    onGridTypeChanged: requestPaint()
    onGridSizeChanged: requestPaint()
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()
    
    onPaint: {
        var ctx = getContext("2d");
        ctx.strokeStyle = "#e0e0e0";
        ctx.lineWidth = 1;
        
        if (gridType === "lines") {
            // Draw line grid
            for (var x = 0; x <= width; x += gridSize) {
                ctx.beginPath();
                ctx.moveTo(x, 0);
                ctx.lineTo(x, height);
                ctx.stroke();
            }
            
            for (var y = 0; y <= height; y += gridSize) {
                ctx.beginPath();
                ctx.moveTo(0, y);
                ctx.lineTo(width, y);
                ctx.stroke();
            }
        } else if (gridType === "dots") {
            // Draw dot grid
            ctx.fillStyle = "#e0e0e0";
            for (var x = 0; x <= width; x += gridSize) {
                for (var y = 0; y <= height; y += gridSize) {
                    ctx.beginPath();
                    ctx.arc(x, y, 1, 0, Math.PI * 2);
                    ctx.fill();
                }
            }
        } else if (gridType === "isometric") {
            // Draw isometric grid
            var angle = Math.PI / 6; // 30 degrees
            
            // Diagonal lines
            for (var i = -height; i <= width; i += gridSize) {
                ctx.beginPath();
                ctx.moveTo(i, 0);
                ctx.lineTo(i + height * Math.tan(angle), height);
                ctx.stroke();
            }
            
            // Reverse diagonal lines
            for (var j = 0; j <= width + height; j += gridSize) {
                ctx.beginPath();
                ctx.moveTo(j, 0);
                ctx.lineTo(j - height * Math.tan(angle), height);
                ctx.stroke();
            }
            
            // Horizontal lines
            for (var k = 0; k <= height; k += gridSize) {
                ctx.beginPath();
                ctx.moveTo(0, k);
                ctx.lineTo(width, k);
                ctx.stroke();
            }
        }
    }
}

