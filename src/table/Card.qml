import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

SvgImage{
    id: card
    
    property bool selectable: false
    
    //signal mouseEntered()
    //signal mouseLeaved()
    
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
    
    MouseArea{
        anchors.fill: card
        onClicked: { console.log( "Click" ); }
    }
    
    function setMoveAnimation( x, y ){
        xAnimation.to = x;
        yAnimation.to = y;
    }
    
    function startMoveAnimation(){
        xAnimation.start();
        yAnimation.start();
    }
    
}
