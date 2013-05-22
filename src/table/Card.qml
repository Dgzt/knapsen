import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

SvgImage{
    id: card
    
    NumberAnimation on x { 
        id: xAnimation
        running: false
        duration: Globals.ANIMATION_TIME
    }
    
    NumberAnimation on y { 
        id: yAnimation
        running: false
        duration: Globals.ANIMATION_TIME
    }
    
    function setXAnimation( posX ){
        xAnimation.to = posX;
    }
    
    function setYAnimation( posY ){
        yAnimation.to = posY;
    }
    
    function startXAnimation(){
        xAnimation.start();
    }
    
    function startYAnimation(){
        yAnimation.start();
    }
}
