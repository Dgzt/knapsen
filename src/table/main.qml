import QtQuick 1.0

Item{
    id: main
    width: 600
    height: 600
    //anchors.fill: parent

    //Opponent's name
    Rectangle{
        id: opponentNameRect
        color: "white"
        width: 100
        height: 30
        
        x: ( parent.width - width ) / 2
        y: 10
        
        Text{
            id: opponentNameText
            text: "Opponent name"
            anchors.centerIn: parent
        }
    }
    
    //Player's name
    Rectangle{
        id: playerNameRect
        color: "white"
        width: 100
        height: 30
        
        x: ( parent.width - width ) / 2
        y: parent.height - height - 10
        
        Text{
            id: playerNameText
            text: "Player name"
            anchors.centerIn: parent
        }
    }
    
    function resize(){
        console.log( main.width + "x" + main.height );
    }
    
    function initialize( playerName, opponentName ){
        console.log( "initialize, player:\""+playerName+"\", opponent:\""+opponentName+"\"" );
        playerNameText.text = playerName;
        opponentNameText.text = opponentName;
    }
    
    
}
