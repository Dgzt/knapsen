import QtQuick 1.1

Item{
    id: scoreTable
    
    property int rectWidth: 100
    property int rectHeight: 50
    
    property string tricksStr: "Tricks: "
    property string scoresStr: "Scores: "
    
    Name{
        id: pointsText
        width: rectWidth
        height: rectHeight / 2
    }
    
    Name{
        id: scoresText
        width: rectWidth
        height: rectHeight / 2
        y: rectHeight / 2 
    }
    
    function setTricks( tricks ){
        pointsText.setText( tricksStr + tricks );
    }
    
    function setScores( scores ){
        scoresText.setText( scoresStr + scores );
    }
    
}
