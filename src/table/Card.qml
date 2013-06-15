import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

SvgImage{
    id: card
    
    property bool selectable: false

    signal mouseClicked()
    signal mouseEntered()
    signal mouseExited()
    
    //This isn't works. :\
    //onSelectableChanged: { if( selectable && isContainsMouse() ) mouseEntered(); }
    
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
    
    /*MouseArea{
        id: mouseArea
        anchors.fill: card
        hoverEnabled: true
        onClicked: { 
            console.log( "Selectable: "+selectable );
            if( selectable ) mouseClicked(); 
        }
        onEntered: { if( selectable ) mouseEntered(); }
        onExited: { if( selectable ) mouseExited(); }
    }*/
    
    /*function setMoveAnimation( x, y ){
        xAnimation.to = x;
        yAnimation.to = y;
    }
    
    function startMoveAnimation(){
        xAnimation.start();
        yAnimation.start();
    }*/
    
    function startAnimation( endX, endY ){
        xAnimation.to = endX;
        yAnimation.to = endY;
        xAnimation.start();
        yAnimation.start();
    }
    
    /*function isContainsMouse(){
        return mouseArea.containsMouse;
    }*/
    
}
