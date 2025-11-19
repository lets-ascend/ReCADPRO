import QtQuick
import QtQuick.Controls

/**
 * ZoomPanHandler - Native zoom and pan gestures for Paper Pro
 * Pinch-to-zoom, two-finger pan, swipe gestures
 */
Item {
    id: zoomPanHandler
    
    property real zoomLevel: 1.0
    property point panOffset: Qt.point(0, 0)
    property real minZoom: 0.25
    property real maxZoom: 4.0
    
    signal zoomChanged(real level)
    signal panChanged(point offset)
    
    // Pinch gesture for zoom
    PinchArea {
        anchors.fill: parent
        enabled: true
        
        onPinchUpdated: (pinch) => {
            var newZoom = zoomLevel * pinch.scale
            newZoom = Math.max(minZoom, Math.min(maxZoom, newZoom))
            
            if (newZoom !== zoomLevel) {
                zoomLevel = newZoom
                zoomChanged(zoomLevel)
            }
        }
        
        onPinchFinished: {
            // Snap to common zoom levels
            var snapLevels = [0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 2.0, 3.0, 4.0]
            var closest = snapLevels[0]
            var minDiff = Math.abs(zoomLevel - closest)
            
            for (var i = 1; i < snapLevels.length; i++) {
                var diff = Math.abs(zoomLevel - snapLevels[i])
                if (diff < minDiff) {
                    minDiff = diff
                    closest = snapLevels[i]
                }
            }
            
            if (minDiff < 0.1) {
                zoomLevel = closest
                zoomChanged(zoomLevel)
            }
        }
    }
    
    // Two-finger pan gesture
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        
        property point lastPanPoint: Qt.point(0, 0)
        property bool isPanning: false
        
        onPressed: (mouse) => {
            if (mouse.buttons === (Qt.LeftButton | Qt.RightButton)) {
                isPanning = true
                lastPanPoint = Qt.point(mouse.x, mouse.y)
            }
        }
        
        onPositionChanged: (mouse) => {
            if (isPanning && mouse.buttons === (Qt.LeftButton | Qt.RightButton)) {
                var dx = mouse.x - lastPanPoint.x
                var dy = mouse.y - lastPanPoint.y
                
                panOffset = Qt.point(panOffset.x + dx, panOffset.y + dy)
                panChanged(panOffset)
                
                lastPanPoint = Qt.point(mouse.x, mouse.y)
            }
        }
        
        onReleased: {
            isPanning = false
        }
    }
    
    // Swipe gestures for undo/redo
    SwipeArea {
        anchors.fill: parent
        enabled: true
        
        onSwipeLeft: {
            // Swipe left = redo
            // Emit signal or call function
        }
        
        onSwipeRight: {
            // Swipe right = undo
            // Emit signal or call function
        }
    }
    
    // Zoom to fit function
    function zoomToFit(bounds) {
        if (!bounds || bounds.width <= 0 || bounds.height <= 0)
            return
        
        // Calculate zoom to fit bounds
        var scaleX = parent.width / bounds.width
        var scaleY = parent.height / bounds.height
        zoomLevel = Math.min(scaleX, scaleY) * 0.9 // 90% to add margin
        zoomLevel = Math.max(minZoom, Math.min(maxZoom, zoomLevel))
        
        // Center the bounds
        var centerX = bounds.x + bounds.width / 2
        var centerY = bounds.y + bounds.height / 2
        var viewCenterX = parent.width / 2
        var viewCenterY = parent.height / 2
        
        panOffset = Qt.point(
            viewCenterX - centerX * zoomLevel,
            viewCenterY - centerY * zoomLevel
        )
        
        zoomChanged(zoomLevel)
        panChanged(panOffset)
    }
    
    // Reset zoom and pan
    function reset() {
        zoomLevel = 1.0
        panOffset = Qt.point(0, 0)
        zoomChanged(zoomLevel)
        panChanged(panOffset)
    }
}

