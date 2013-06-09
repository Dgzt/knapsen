import QtQuick 1.1
import "globals.js" as Globals
import "logic.js" as Logic

Item{
    id: main

    onWidthChanged: resized()
    onHeightChanged: resized()
    
    property real rate: 1
    
    //property string cardSource: "/usr/local/share/apps/knapsen/pics/william-tell.svgz"
    property string cardSource: ""
    
    Image{
        id: backgroundImage
        fillMode: Image.Tile
        anchors.fill: parent
        source: "../pics/background.png"
    }
    
    Item{
        id: gameArea
        width: 900
        height: 700
        anchors.centerIn: parent
        scale: rate
        
        Name{
            id: opponentName
            text: "Opponent"
            x: ( parent.width - width ) / 2
            //y: Globals.NAME_DISTANCE
            visible: false
        }
        
        Name{
            id: playerName
            text: "Player"
            x: ( parent.width - width ) / 2
            //y: parent.height - height - Globals.NAME_DISTANCE
            visible: false
        }
        
        Card{
            id: deck
            source: main.cardSource
            elementId: "back"
            x: Globals.DECK_DISTANCE
            y: ( parent.height - height ) / 2
        }
        
        /*Card{
            id: trump
            source: main.cardSource
            elementId: "back"
            y: deck.y
            
        }*/
    }
    
    function resized(){
        console.log( width+"x"+height );
        
        //var widthRate = width / gameArea.constWidth;
        var widthRate = width / gameArea.width;
        //var heightRate = height / gameArea.constHeight;
        var heightRate = height / gameArea.height;
        
        if( widthRate < heightRate ){
            rate = widthRate;
        }else{
            rate = heightRate;
        }
    }
    
    function clear(){
        opponentName.visible = false;
        playerName.visible = false;
    }
    
    function initialize( playerNameStr, opponentNameStr, picsPath, picScale ){
        console.log( "Initialize" );
        
        cardSource = picsPath;
        
        opponentName.text = opponentNameStr;
        playerName.text = playerNameStr;
        
        var mapOpponent = main.mapToItem( gameArea, 0, 0 );
        opponentName.startYAnimation( mapOpponent.y - opponentName.height, Globals.NAME_DISTANCE );
        opponentName.visible = true;
        
        var mapPlayer = main.mapToItem( gameArea, width, height );
        playerName.startYAnimation( mapPlayer.y, gameArea.height - Globals.NAME_DISTANCE - playerName.height );
        playerName.visible = true;
        
    }
    
}
