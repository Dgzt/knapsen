import QtQuick 1.1
import "globals.js" as Globals

Rectangle{
    id: scoreTable
    color: "white"
    width: text.width + Globals.TEXT_RECT_DISTANCE_WIDTH;
    height: text.height + Globals.TEXT_RECT_DISTANCE_HEIGHT;
    
    property int tricks: 0
    property int scores: 0
    
    Text{
        id: text
        anchors.centerIn: parent
        text: "Tricks: " + tricks + "\nScores: " + scores;
    }
    
    NumberAnimation on x {  
        id: moveAnimation
        running: false
        duration: Globals.ANIMATION_TIME
    }
    
    NumberAnimation on opacity {
        id: opacityAnimation
        running: false
        from: 0
        to: 1
        duration: Globals.ANIMATION_TIME
    }
    
    function setAnimationPosX( start, end ){
        moveAnimation.from = start;
        moveAnimation.to = end;
    }
    
    function startAnimation(){
        moveAnimation.start();
        opacityAnimation.start();
    }
}
