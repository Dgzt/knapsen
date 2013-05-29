import QtQuick 1.1

Rectangle{
    id: button
    color: "white"
    radius: 5
    
    Text{
        id: buttonText
        anchors.centerIn: parent
        text: "A button"
    }
    
    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked: { 
            console.log( "Clicked to button." ); 
            //button.color = "grey"
        }
        onEntered: { 
            console.log( "Enter to button" ); 
            button.border.width = 1;
        }
        onExited: { 
            console.log( "Exit from button" ); 
            button.border.width = 0;
        }
    }
    
}
