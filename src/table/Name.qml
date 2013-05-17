import QtQuick 1.1

Rectangle{
    //id: textRect
    color: "white"
        
    Text{
        id: name
        text: "Player name"
        anchors.centerIn: parent
    }
    
    function setText( text ){
        name.text = text;
    }
    
}
