import QtQuick 2.15
import ReCADPro 1.0

/**
 * SelectionOverlay - Visual selection rectangle and handles
 * Shows selection bounds and provides transformation handles
 */
Rectangle {
    id: selectionOverlay
    
    property SelectionManager selectionManager: null
    
    visible: selectionManager && selectionManager.hasSelection
    color: "transparent"
    border.color: "#2196F3"
    border.width: 2
    radius: 2
    
    // Selection bounds
    x: selectionManager && selectionManager.hasSelection ? selectionManager.selectionBounds.x : 0
    y: selectionManager && selectionManager.hasSelection ? selectionManager.selectionBounds.y : 0
    width: selectionManager && selectionManager.hasSelection ? selectionManager.selectionBounds.width : 0
    height: selectionManager && selectionManager.hasSelection ? selectionManager.selectionBounds.height : 0
    
    // Selection handles (corners and edges)
    Repeater {
        model: 8 // 4 corners + 4 edges
        
        Rectangle {
            property int handleSize: 8
            x: getHandleX(index)
            y: getHandleY(index)
            width: handleSize
            height: handleSize
            radius: handleSize / 2
            color: "#2196F3"
            border.color: "white"
            border.width: 1
            
            MouseArea {
                anchors.fill: parent
                cursorShape: getCursor(index)
                onPressed: {
                    // Start transformation
                }
                onPositionChanged: {
                    // Update transformation
                }
            }
        }
    }
    
    function getHandleX(index) {
        switch(index) {
            case 0: case 3: case 5: return 0 - handleSize/2
            case 1: case 6: return parent.width/2 - handleSize/2
            case 2: case 4: case 7: return parent.width - handleSize/2
            default: return 0
        }
    }
    
    function getHandleY(index) {
        switch(index) {
            case 0: case 1: case 2: return 0 - handleSize/2
            case 3: case 7: return parent.height/2 - handleSize/2
            case 4: case 5: case 6: return parent.height - handleSize/2
            default: return 0
        }
    }
    
    function getCursor(index) {
        switch(index) {
            case 0: return Qt.SizeFDiagCursor
            case 1: return Qt.SizeVerCursor
            case 2: return Qt.SizeBDiagCursor
            case 3: return Qt.SizeHorCursor
            case 4: return Qt.SizeFDiagCursor
            case 5: return Qt.SizeVerCursor
            case 6: return Qt.SizeBDiagCursor
            case 7: return Qt.SizeHorCursor
            default: return Qt.ArrowCursor
        }
    }
}

