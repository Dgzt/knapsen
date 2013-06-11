import QtQuick 1.1
import "globals.js" as Globals
import "logic.js" as Logic

Item{
    id: main

    onWidthChanged: resized()
    onHeightChanged: resized()
    
    property real rate: 1
    
    property string cardSource: ""
    
    signal signalAnimationEnd()
    
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
        
        Column{
            id: playerControlPanel
            spacing: Globals.BUTTON_DISTANCE
            height: schnapsenButton.height + twentyButton.height + fortyButton.height + playerScoreTable.height + 3*Globals.BUTTON_DISTANCE
            
            Button{
                id: schnapsenButton
                text: "Schnapsen"
                width: 100
                height: Globals.BUTTON_HEIGHT
            }
            
            Button{
                id: twentyButton
                text: "Twenty"
                width: 100
                height: Globals.BUTTON_HEIGHT
            }
            
            Button{
                id: fortyButton
                text: "Forty"
                width: 100
                height: Globals.BUTTON_HEIGHT
            }
            
            ScoreTable{
                id: playerScoreTable
                visible: false
            }
            
            NumberAnimation on x{
                id: playerControlPanelXAnimation
                duration: Globals.ANIMATION_TIME
            }
        }
        
        Column{
            id: opponentControlPanel
            
            ScoreTable{
                id: opponentScoreTable
                visible: false
            }
            
            NumberAnimation on x{
                id: opponentControlPanelXAnimation
                duration: Globals.ANIMATION_TIME
            }
        }
        
        Card{
            id: deck
            source: main.cardSource
            elementId: "back"
            //x: Globals.DECK_DISTANCE
            y: ( parent.height - height ) / 2
            visible: false
        }
        
        CardsGroup{
            id: opponentCardsGroup
            x: ( parent.width - width ) / 2
            y: opponentName.y + opponentName.height + Globals.NAME_DISTANCE
            
            //
            onWidthChanged: {
                opponentControlPanelXAnimation.from = opponentControlPanel.x;
                opponentControlPanelXAnimation.to = opponentCardsGroup.x + opponentCardsGroup.width /*+ Globals.SCORE_TABLE_DISTANCE*/; 
                opponentControlPanelXAnimation.start();
            }
        }
        
        CardsGroup{
            id: playerCardsGroup
            x: ( parent.width - width ) / 2
            y: playerName.y - Globals.NAME_DISTANCE - height
            
            //
            onWidthChanged: { 
                playerControlPanelXAnimation.from = playerControlPanel.x;
                playerControlPanelXAnimation.to = playerCardsGroup.x + playerCardsGroup.width /*+ Globals.SCORE_TABLE_DISTANCE*/; 
                playerControlPanelXAnimation.start();            
            }
        }
        
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
        
        opponentScoreTable.visible = false;
        playerScoreTable.visible = false;
        
        deck.visible = false;
        
        opponentCardsGroup.clear();
        playerCardsGroup.clear();
    }
    
    function initialize( playerNameStr, opponentNameStr, picsPath, picScale ){
        console.log( "Initialize" );
        
        cardSource = picsPath;
        
        opponentName.text = opponentNameStr;
        playerName.text = playerNameStr;
    }
    
    function newGame(){
        console.log( "New game." );
        
        var opponentNameStartPosY = main.mapToItem( gameArea, 0, 0 ).y - opponentName.height;
        var opponentNameEndPosY = Globals.NAME_DISTANCE;
        opponentName.startYAnimation( opponentNameStartPosY, opponentNameEndPosY );
        opponentName.visible = true;
        
        var playerNameStartPosY = main.mapToItem( gameArea, 0, height ).y;
        var playerNameEndPosY = gameArea.height - Globals.NAME_DISTANCE - playerName.height;
        playerName.startYAnimation( playerNameStartPosY, playerNameEndPosY );
        playerName.visible = true;
        
        var playerControlPanelStartPosX = main.mapToItem( gameArea, width, 0 ).x;
        var playerControlPanelEndPosX = gameArea.width / 2;
        var playerControlPanelPosY = playerNameEndPosY - Globals.NAME_DISTANCE - playerControlPanel.height;
        playerControlPanel.y = playerControlPanelPosY;
        playerControlPanelXAnimation.from = playerControlPanelStartPosX;
        playerControlPanelXAnimation.to = playerControlPanelEndPosX;
        playerControlPanelXAnimation.start();
        playerScoreTable.visible = true;
        
        var opponentControlPanelStartPosX = playerControlPanelStartPosX;
        var opponentControlPanelEndPosX = playerControlPanelEndPosX;
        var opponentControlPanelPosY = opponentNameEndPosY + opponentName.height + Globals.NAME_DISTANCE;
        opponentControlPanel.y = opponentControlPanelPosY;
        opponentControlPanelXAnimation.from = opponentControlPanelStartPosX;
        opponentControlPanelXAnimation.to = opponentControlPanelEndPosX;
        opponentControlPanelXAnimation.start();
        opponentScoreTable.visible = true;
        
        var singleShot = Logic.createSingleShot( main, Globals.ANIMATION_TIME );
        singleShot.triggered.connect( signalAnimationEnd );
        singleShot.start();
    }
    
    function newRound(){
        console.log( "New round" );
        
        var deckStartPosX = main.mapToItem( gameArea, 0, 0 ).x - deck.width;
        var deckEndPosX = Globals.DECK_DISTANCE;
        deck.x = deckStartPosX;
        deck.startAnimation( deckEndPosX, deck.y );
        deck.visible = true;
        
        var singleShot = Logic.createSingleShot( main, Globals.ANIMATION_TIME );
        singleShot.triggered.connect( signalAnimationEnd );
        singleShot.start();
    }
    
    function newPlayerCard( lastCard, cardElementId ){
        console.log( "New player card." );
        
        var card = Logic.createCard( gameArea, cardSource, cardElementId, 1, deck.x, deck.y );
        
        if( lastCard ){
            deck.visible = false;
        }
        
        playerCardsGroup.addCard( card );
        
        var singleShot = Logic.createSingleShot( main, Globals.ANIMATION_TIME );
        singleShot.triggered.connect( signalAnimationEnd );
        singleShot.start();
    }
    
    function newOpponentCard( lastCard ){
        console.log( "New opponent card." );
        
        var card = Logic.createCard( gameArea, cardSource, Globals.CARD_BACK, 1, deck.x, deck.y );
        
        if( lastCard ){
            deck.visible = false;
        }
        
        opponentCardsGroup.addCard( card );
        
        var singleShot = Logic.createSingleShot( main, Globals.ANIMATION_TIME );
        singleShot.triggered.connect( signalAnimationEnd );
        singleShot.start();
    }
}
