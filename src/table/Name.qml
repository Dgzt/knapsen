import QtQuick 1.1
import "globals.js" as Globals

Rectangle{
    id: frame
    color: "white"
    width: name.width + Globals.TEXT_RECT_DISTANCE_WIDTH;
    height: name.height + Globals.TEXT_RECT_DISTANCE_HEIGHT;
    
    property string text: ""
    
    Text{
        id: name
        text: parent.text
        anchors.centerIn: parent
    }
    
    NumberAnimation on y{
        id: moveYAnimation
        duration: Globals.ANIMATION_TIME
    }
    
    //
    function startYAnimation( from, to ){
        moveYAnimation.from = from;
        moveYAnimation.to = to;
        moveYAnimation.start();
    }
}
