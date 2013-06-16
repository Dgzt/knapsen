import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

SvgImage{
    id: card
    
    property bool selectable: false
    property string newElementId: Globals.CARD_BACK
    property bool containsMouse: false
    
    signal mouseClicked()
    signal mouseEntered()
    signal mouseExited()
    
    //This isn't works. :\
    //onSelectableChanged: { if( selectable && mouseArea.containsMouse ) console.log( "highligt" ); }
    
    NumberAnimation on x { 
        id: xAnimation
        running: false
        duration: Globals.ANIMATION_TIME
        
        onRunningChanged: { if( !running ) card.elementId = card.newElementId; }
    }
    
    NumberAnimation on y { 
        id: yAnimation
        running: false
        duration: Globals.ANIMATION_TIME
        
        onRunningChanged: { if( !running ) card.elementId = card.newElementId; }
    }
    
    MouseArea{
        id: mouseArea
        anchors.fill: card
        hoverEnabled: true
        onClicked: { 
            console.log( "Selectable: "+selectable );
            if( selectable ) mouseClicked(); 
        }
        onEntered: { if( selectable ) mouseEntered(); }
        onExited: { if( selectable ) mouseExited(); }
        
        //
        onContainsMouseChanged: card.containsMouse = mouseArea.containsMouse;
    }
    
    function startAnimation( endX, endY ){
        xAnimation.to = endX;
        yAnimation.to = endY;
        xAnimation.start();
        yAnimation.start();
    }
    
}
