import QtQuick 2.15
import QtQuick.Controls 2.15

/**
 * TextInputDialog - Dialog for text input
 * Used by TextTool for placing text on canvas
 */
Dialog {
    id: textDialog
    
    property TextTool textTool: null
    property point position: Qt.point(0, 0)
    
    title: "Enter Text"
    modal: true
    width: 400
    height: 200
    
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    
    Column {
        anchors.fill: parent
        spacing: 16
        
        TextField {
            id: textField
            width: parent.width
            placeholderText: "Type your text here..."
            focus: true
            
            onAccepted: {
                if (textTool && text.length > 0) {
                    textTool.placeText(position, text);
                    textDialog.close();
                }
            }
        }
        
        Row {
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            
            Button {
                text: "Cancel"
                onClicked: textDialog.close()
            }
            
            Button {
                text: "Place"
                onClicked: {
                    if (textTool && textField.text.length > 0) {
                        textTool.placeText(position, textField.text);
                        textDialog.close();
                    }
                }
            }
        }
    }
}

