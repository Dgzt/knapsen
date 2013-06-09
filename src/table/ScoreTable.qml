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
}
