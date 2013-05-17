import QtQuick 1.0

Item{
    id: window
    width: 1000
    height: 1000

    //Background
    Image{
        id: backgroundImage
        anchors.fill: parent
        source: "../pics/background.png"
        fillMode: Image.Tile
    }
    
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
    
    function initialize( playerName, opponentName ){
        console.log( "initialize, player:\""+playerName+"\", opponent:\""+opponentName+"\"" );
        playerNameText.text = playerName;
        opponentNameText.text = opponentName;
    }
    
    
}
