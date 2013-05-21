import QtQuick 1.1
import "globals.js" as Globals

Rectangle{
    id: frame
    color: "white"
    
    Text{
        id: name
        anchors.centerIn: parent
    }
    
    NumberAnimation on y { 
        id: moveAnimation
        running: false
        //from: frame.y
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
    
    function setAnimationPosY( start, distance ){
        moveAnimation.from = start;
        moveAnimation.to = start + distance;
    }
    
    function animationPosYEnd(){
        return moveAnimation.to;
    }
    
    function startAnimation(){
        moveAnimation.start();
        opcityAnimation.start();
    }
    
}
