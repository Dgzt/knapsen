import QtQuick 1.1
import "globals.js" as Globals

Item{
    id: scoreTable
    
    property int rectWidth: 100
    property int rectHeight: 50
    
    property string tricksStr: "Tricks: "
    property string scoresStr: "Scores: "
    
    Rectangle{
        id: tricksRect
        color: "white"
        width: rectWidth
        height: rectHeight / 2
    
        Text{
            id: tricks
            text: tricksStr + 0
            anchors.centerIn: parent
        }
    }
    
    Rectangle{
        id: scoresRect
        color: "white"
        width: rectWidth
        height: rectHeight / 2
        y: rectHeight / 2 
    
        Text{
            id: scores
            text: scoresStr + 0
            anchors.centerIn: parent
        }
    }
    
    NumberAnimation on x {  
        id: moveAnimation
        running: false
        from: scoreTable.x
        duration: Globals.ANIMATION_TIME
    }
    
    NumberAnimation on opacity {
        id: opacityAnimation
        running: false
        from: 0
        to: 1
        duration: Globals.ANIMATION_TIME
    }
    
    /*function setTricks( tricksNum ){
        tricks.text = tricksStr + tricksNum;
    }
    
    function setScores( scoresNum ){
        scores.text = scoresStr + scoresNum;
    }*/
    
    function getWidth(){
        return tricksRect.width;
    }
    
    function getHeight(){
        return rectHeight;
    }
    
    /*function setAnimationEnd( end ){
        moveAnimation.to = end;
    }*/
    
    function setAnimationPosX( start, end ){
        moveAnimation.from = start;
        moveAnimation.to = end;
    }
    
    function startAnimation(){
        moveAnimation.start();
        opacityAnimation.start();
    }
    
}
