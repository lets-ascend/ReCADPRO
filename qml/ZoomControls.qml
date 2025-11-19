import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * ZoomControls - Zoom control panel for Paper Pro
 * Provides zoom in/out buttons, zoom slider, zoom level display, and zoom to fit
 */
Rectangle {
    id: zoomControls
    
    property ZoomPanHandler zoomHandler: null
    property SelectionManager selectionManager: null
    
    width: 200
    height: 48
    color: "#ffffff"
    border.color: "#e0e0e0"
    border.width: 1
    radius: 4
    
    Row {
        anchors.fill: parent
        anchors.margins: 4
        spacing: 8
        
        // Zoom out button
        NativeTouchButton {
            width: 40
            height: 40
            text: "−"
            font.pixelSize: 20
            font.bold: true
            enabled: zoomHandler && zoomHandler.zoomLevel > zoomHandler.minZoom
            onClicked: {
                if (zoomHandler) {
                    var newZoom = Math.max(zoomHandler.minZoom, zoomHandler.zoomLevel - 0.25)
                    zoomHandler.zoomLevel = newZoom
                    zoomHandler.zoomChanged(newZoom)
                }
            }
        }
        
        // Zoom slider
        Slider {
            id: zoomSlider
            width: 80
            height: 40
            from: zoomHandler ? zoomHandler.minZoom : 0.25
            to: zoomHandler ? zoomHandler.maxZoom : 4.0
            value: zoomHandler ? zoomHandler.zoomLevel : 1.0
            enabled: zoomHandler !== null
            
            onValueChanged: {
                if (zoomHandler && Math.abs(zoomHandler.zoomLevel - value) > 0.01) {
                    zoomHandler.zoomLevel = value
                    zoomHandler.zoomChanged(value)
                }
            }
            
            Connections {
                target: zoomHandler
                function onZoomChanged(level) {
                    if (Math.abs(zoomSlider.value - level) > 0.01) {
                        zoomSlider.value = level
                    }
                }
            }
        }
        
        // Zoom level display
        Text {
            width: 50
            height: 40
            text: zoomHandler ? Math.round(zoomHandler.zoomLevel * 100) + "%" : "100%"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        
        // Zoom in button
        NativeTouchButton {
            width: 40
            height: 40
            text: "+"
            font.pixelSize: 20
            font.bold: true
            enabled: zoomHandler && zoomHandler.zoomLevel < zoomHandler.maxZoom
            onClicked: {
                if (zoomHandler) {
                    var newZoom = Math.min(zoomHandler.maxZoom, zoomHandler.zoomLevel + 0.25)
                    zoomHandler.zoomLevel = newZoom
                    zoomHandler.zoomChanged(newZoom)
                }
            }
        }
        
        // Zoom to fit button
        NativeTouchButton {
            width: 40
            height: 40
            text: "⌂"
            font.pixelSize: 16
            enabled: zoomHandler !== null
            onClicked: {
                if (zoomHandler) {
                    // Zoom to fit entire canvas
                    var bounds = Qt.rect(0, 0, parent.parent.width, parent.parent.height)
                    zoomHandler.zoomToFit(bounds)
                }
            }
        }
        
        // Zoom to selection button
        NativeTouchButton {
            width: 40
            height: 40
            text: "⌖"
            font.pixelSize: 16
            enabled: zoomHandler !== null && selectionManager && selectionManager.hasSelection
            onClicked: {
                if (zoomHandler && selectionManager && selectionManager.hasSelection) {
                    // Get selection bounds
                    var bounds = selectionManager.getSelectionBounds()
                    if (bounds.width > 0 && bounds.height > 0) {
                        zoomHandler.zoomToFit(bounds)
                    }
                }
            }
        }
    }
}

