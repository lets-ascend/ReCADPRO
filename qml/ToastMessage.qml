import QtQuick
import QtQuick.Controls

/**
 * ToastMessage - User feedback messages for Paper Pro
 * Non-intrusive notifications optimized for e-paper
 */
Rectangle {
    id: toast
    
    property string message: ""
    property string type: "info" // "info", "success", "warning", "error"
    property int duration: 2000 // milliseconds
    
    width: Math.min(400, parent.width - 40)
    height: 60
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    
    color: {
        if (type === "error") return "#f44336"
        if (type === "warning") return "#ff9800"
        if (type === "success") return "#4caf50"
        return "#2196F3" // info/default
    }
    
    border.color: "#ffffff"
    border.width: 2
    radius: 8
    
    opacity: 0
    visible: message !== ""
    
    // Slide up animation
    y: parent.height
    Behavior on y {
        NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
    }
    
    Behavior on opacity {
        NumberAnimation { duration: 300 }
    }
    
    Text {
        anchors.centerIn: parent
        text: toast.message
        color: "#ffffff"
        font.pixelSize: 16
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        width: parent.width - 20
    }
    
    // Auto-hide timer
    Timer {
        id: hideTimer
        interval: toast.duration
        onTriggered: toast.hide()
    }
    
    function show(msg, msgType, msgDuration) {
        message = msg
        type = msgType || "info"
        duration = msgDuration || 2000
        
        opacity = 1
        y = parent.height - height - 20
        
        hideTimer.restart()
    }
    
    function hide() {
        opacity = 0
        y = parent.height
        
        Timer {
            interval: 300
            onTriggered: toast.message = ""
        }
    }
}

