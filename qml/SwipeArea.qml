import QtQuick

/**
 * SwipeArea - Swipe gesture detection for Paper Pro
 * Detects left/right/up/down swipes
 */
MouseArea {
    id: swipeArea
    
    property real minimumSwipeDistance: 50
    property real maximumSwipeTime: 300
    
    signal swipeLeft()
    signal swipeRight()
    signal swipeUp()
    signal swipeDown()
    
    property point startPoint: Qt.point(0, 0)
    property int startTime: 0
    
    onPressed: (mouse) => {
        startPoint = Qt.point(mouse.x, mouse.y)
        startTime = Date.now()
    }
    
    onReleased: (mouse) => {
        var endPoint = Qt.point(mouse.x, mouse.y)
        var endTime = Date.now()
        var duration = endTime - startTime
        
        if (duration > maximumSwipeTime) {
            return // Too slow, not a swipe
        }
        
        var dx = endPoint.x - startPoint.x
        var dy = endPoint.y - startPoint.y
        var distance = Math.sqrt(dx * dx + dy * dy)
        
        if (distance < minimumSwipeDistance) {
            return // Too short, not a swipe
        }
        
        // Determine swipe direction
        if (Math.abs(dx) > Math.abs(dy)) {
            // Horizontal swipe
            if (dx > 0) {
                swipeRight()
            } else {
                swipeLeft()
            }
        } else {
            // Vertical swipe
            if (dy > 0) {
                swipeDown()
            } else {
                swipeUp()
            }
        }
    }
}

