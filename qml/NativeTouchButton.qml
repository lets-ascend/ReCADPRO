import QtQuick
import QtQuick.Controls

/**
 * NativeTouchButton - Touch-optimized button for Paper Pro
 * Minimum 44x44px touch target, proper visual feedback
 */
Rectangle {
    id: touchButton
    
    property alias text: label.text
    property alias icon: iconItem.source
    property bool checked: false
    property bool enabled: true
    
    signal clicked()
    
    // Minimum touch target size for Paper Pro (44x44px recommended)
    width: Math.max(44, implicitWidth)
    height: Math.max(44, implicitHeight)
    
    color: {
        if (!enabled) return "#f5f5f5"
        if (checked) return "#2196F3"
        if (pressed) return "#e0e0e0"
        return "#ffffff"
    }
    
    border.color: {
        if (!enabled) return "#cccccc"
        if (checked) return "#1976D2"
        return "#e0e0e0"
    }
    border.width: checked ? 2 : 1
    
    // Visual feedback for touch
    Behavior on color {
        ColorAnimation { duration: 100 }
    }
    
    Behavior on border.color {
        ColorAnimation { duration: 100 }
    }
    
    // Scale feedback on press
    scale: pressed ? 0.95 : 1.0
    Behavior on scale {
        NumberAnimation { duration: 100; easing.type: Easing.OutQuad }
    }
    
    // Content
    Row {
        anchors.centerIn: parent
        spacing: 8
        
        Image {
            id: iconItem
            visible: source !== ""
            width: 20
            height: 20
            anchors.verticalCenter: parent.verticalCenter
        }
        
        Text {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 14
            color: {
                if (!enabled) return "#999999"
                if (checked) return "#ffffff"
                return "#000000"
            }
        }
    }
    
    MouseArea {
        anchors.fill: parent
        enabled: touchButton.enabled
        onClicked: {
            if (touchButton.enabled) {
                touchButton.clicked()
            }
        }
    }
}

