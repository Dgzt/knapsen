import QtQuick 1.1
import "globals.js" as Globals

Rectangle{
    id: frame
    color: "white"
    opacity: 0
    
    Text{
        id: name
        anchors.centerIn: parent
    }
    
    NumberAnimation on y { 
        id: moveAnimation
        running: false
        from: frame.y
        duration: Globals.ANIMATION_TIME 
    }
    
    NumberAnimation on opacity { 
        id: opcityAnimation
        running: false
        from: 0
        to: 1 
        duration: Globals.ANIMATION_TIME 
    }
    
    function setText( text ){
        name.text = text;
    }
    
    function setAnimationEnd( end ){
        moveAnimation.to = end;
    }
    
    function startAnimation(){
        moveAnimation.start();
        opcityAnimation.start();
    }
    
}
