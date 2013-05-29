import QtQuick 1.1

Rectangle{
    id: button
    color: "white"
    radius: 5
    
    signal click()
    
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
            onClicked: click()
        }
        onEntered: { 
            console.log( "Enter to button" ); 
            button.border.width = 1;
            button.color = "#DEDEDE"
        }
        onExited: { 
            console.log( "Exit from button" ); 
            button.border.width = 0;
            button.color = "white"
        }
    }
    
}
