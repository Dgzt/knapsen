import QtQuick 1.1
import "globals.js" as Globals
import "logic.js" as Logic

Item{
    id: main

    onWidthChanged: resized()
    onHeightChanged: resized()
    
    //property real rate: 1
    
    property string cardSource: ""
    
    signal signalAnimationEnd()
    signal signalSelectedCard( int id, int delay )
    signal signalCloseButtonClicked()
    signal signalSchnapsenButtonClicked()
    signal signalTwentyButtonClicked()
    signal signalFortyButtonClicked()
    signal signalTrumpCardClicked()
    //
    signal signalDestroyTimer()
    //
    
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
        
        //
        MouseArea{
            id: gameAreaMouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
        //
        
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
            //height: schnapsenButton.height + twentyButton.height + fortyButton.height + playerScoreTable.height + 3*Globals.BUTTON_DISTANCE
            
            property real oldHeight: 0
            
            onHeightChanged: {
                y -=  height - oldHeight;
                oldHeight = height;
            }
            
            Button{
                id: schnapsenButton
                text: "Schnapsen"
                width: 100
                height: Globals.BUTTON_HEIGHT
                visible: false
                
                onClick: signalSchnapsenButtonClicked()
            }
            
            Button{
                id: twentyButton
                text: "Twenty"
                width: 100
                height: Globals.BUTTON_HEIGHT
                visible: false
                
                onClick: signalTwentyButtonClicked()
            }
            
            Button{
                id: fortyButton
                text: "Forty"
                width: 100
                height: Globals.BUTTON_HEIGHT
                visible: false
                
                onClick: signalFortyButtonClicked()
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
        
        /*Card{
            id: deck
            source: main.cardSource
            elementId: "back"
            y: ( parent.height - height ) / 2
            visible: false
        }*/
        
        Column{
            id: deckColumn
            width: deck.width
            y: ( parent.height - deck.height ) / 2
            spacing: Globals.BUTTON_DISTANCE
            
            Card{
                id: deck
                source: main.cardSource
                elementId: "back"
                visible: false
            }
            
            Button{
                id: closeButton
                text: "Close"
                width: deck.width
                height: Globals.BUTTON_HEIGHT
                visible: false
                
                onClick: signalCloseButtonClicked()
            }
            
            NumberAnimation on x{
                id: deckColumnXAnimation
                duration: Globals.ANIMATION_TIME
            }
        }
        
        
        
        Card{
            id: trump
            source: main.cardSource
            //y: deck.y
            y: deckColumn.y
            visible: false
            
            onMouseClicked: signalTrumpCardClicked()
        }
        
        CardsGroup{
            id: opponentCardsGroup
            x: ( parent.width - width ) / 2
            y: opponentName.y + opponentName.height + Globals.NAME_DISTANCE
            
            //
            onXChanged: {
                opponentControlPanelXAnimation.from = opponentControlPanel.x;
                opponentControlPanelXAnimation.to = opponentCardsGroup.x + opponentCardsGroup.width + Globals.SCORE_TABLE_DISTANCE; 
                opponentControlPanelXAnimation.restart();
            }
            
            onSelectedCard: { centralCardsGroup.addCard( card, true ); }
        }
        
        CardsGroup{
            id: centralCardsGroup
            x: ( parent.width - width ) / 2
            y: ( parent.height - height ) / 2
            
        }
        
        CardsGroup{
            id: playerCardsGroup
            x: ( parent.width - width ) / 2
            y: playerName.y - Globals.NAME_DISTANCE - height
            
            //
            onXChanged: { 
                playerControlPanelXAnimation.from = playerControlPanel.x;
                playerControlPanelXAnimation.to = playerCardsGroup.x + playerCardsGroup.width + Globals.SCORE_TABLE_DISTANCE; 
                playerControlPanelXAnimation.restart();            
            }
            //
            
            onSelectedCardId: { signalSelectedCard( id, Globals.ANIMATION_TIME ); }
            onSelectedCard: { centralCardsGroup.addCard( card, true ); }
        }
        
    }
    
    function resized(){
        console.log( width+"x"+height );
        
        var widthRate = width / gameArea.width;
        var heightRate = height / gameArea.height;
        
        if( widthRate < heightRate ){
            gameArea.scale = widthRate;
        }else{
            gameArea.scale = heightRate;
        }
    }
    
    function singleShot( triggeredSlot, time ){
        var tmpSingleShot = Logic.createSingleShot( main, time );
        tmpSingleShot.triggered.connect( triggeredSlot );
        signalDestroyTimer.connect( tmpSingleShot.kill );
        tmpSingleShot.start();
    }
    
    function clear(){
        signalDestroyTimer();
        
        opponentName.visible = false;
        playerName.visible = false;
        
        opponentScoreTable.visible = false;
        playerScoreTable.visible = false;
        
        deck.visible = false;
        closeButton.visible = false;
        trump.visible = false;
        
        opponentCardsGroup.clear();
        centralCardsGroup.clear();
        playerCardsGroup.clear();
        
        schnapsenButton.visible = false;
        twentyButton.visible = false;
        fortyButton.visible = false;
    }
    
    function initialize( playerNameStr, opponentNameStr, picsPath, picScale ){
        console.log( "Initialize" );
        
        cardSource = picsPath;
        
        opponentName.text = opponentNameStr;
        playerName.text = playerNameStr;
    }
    
    function newGame(){
        console.log( "New game." );
        
        singleShot( startNewGameAnimation, 500 );
    }
    
    function startNewGameAnimation(){
        var opponentNameStartPosY = main.mapToItem( gameArea, 0, 0 ).y - opponentName.height;
        console.log( "opponentNameStartPosY:"+ opponentNameStartPosY);
        var opponentNameEndPosY = Globals.NAME_DISTANCE;
        console.log( "opponentNameEndPosY:"+ opponentNameEndPosY);
        opponentName.startYAnimation( opponentNameStartPosY, opponentNameEndPosY );
        opponentName.visible = true;
        
        var playerNameStartPosY = main.mapToItem( gameArea, 0, height ).y;
        console.log( "playerNameStartPosY:"+ playerNameStartPosY);
        var playerNameEndPosY = gameArea.height - Globals.NAME_DISTANCE - playerName.height;
        console.log( "playerNameEndPosY:"+ playerNameEndPosY);
        playerName.startYAnimation( playerNameStartPosY, playerNameEndPosY );
        playerName.visible = true;
        
        var playerControlPanelStartPosX = main.mapToItem( gameArea, width, 0 ).x;
        console.log( "playerControlPanelStartPosX:"+ playerControlPanelStartPosX);
        var playerControlPanelEndPosX = gameArea.width / 2;
        console.log( "playerControlPanelEndPosX:"+ playerControlPanelEndPosX);
        var playerControlPanelPosY = playerNameEndPosY - Globals.NAME_DISTANCE - playerControlPanel.height;
        console.log( "playerControlPanelPosY:"+ playerControlPanelPosY);
        playerControlPanel.y = playerControlPanelPosY;
        playerControlPanelXAnimation.from = playerControlPanelStartPosX;
        playerControlPanelXAnimation.to = playerControlPanelEndPosX;
        playerControlPanelXAnimation.start();
        playerScoreTable.visible = true;
        
        var opponentControlPanelStartPosX = playerControlPanelStartPosX;
        console.log( "opponentControlPanelStartPosX:"+ opponentControlPanelStartPosX);
        var opponentControlPanelEndPosX = playerControlPanelEndPosX;
        console.log( "opponentControlPanelEndPosX:"+ opponentControlPanelEndPosX);
        var opponentControlPanelPosY = opponentNameEndPosY + opponentName.height + Globals.NAME_DISTANCE;
        console.log( "opponentControlPanelPosY:"+ opponentControlPanelPosY);
        opponentControlPanel.y = opponentControlPanelPosY;
        opponentControlPanelXAnimation.from = opponentControlPanelStartPosX;
        opponentControlPanelXAnimation.to = opponentControlPanelEndPosX;
        opponentControlPanelXAnimation.start();
        opponentScoreTable.visible = true;
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function newRound(){
        console.log( "New round" );
        
        /*var deckStartPosX = main.mapToItem( gameArea, 0, 0 ).x - deck.width;
        var deckEndPosX = Globals.DECK_DISTANCE;
        deck.x = deckStartPosX;
        deck.startAnimation( deckEndPosX, deck.y );
        deck.visible = true;*/
        
        //Clear table
        trump.visible = false;
        opponentCardsGroup.clear();
        playerCardsGroup.clear();
        centralCardsGroup.clear();
        
        var deckColumnStartPosX = main.mapToItem( gameArea, 0, 0 ).x - deckColumn.width;
        var deckColumnEndPosX = Globals.DECK_DISTANCE;
        deckColumnXAnimation.from = deckColumnStartPosX;
        deckColumnXAnimation.to = deckColumnEndPosX;
        deckColumnXAnimation.start();
        deck.visible = true;
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function newPlayerCard( lastCard, cardElementId ){
        console.log( "New player card." );
        
        //var card = Logic.createCard( gameArea, cardSource, cardElementId, 1, deck.x, deck.y );
        var card = Logic.createCard( gameArea, cardSource, Globals.CARD_BACK, cardElementId, 1, deckColumn.x, deckColumn.y );
        
        if( lastCard ){
            deck.visible = false;
        }
        
        playerCardsGroup.addCard( card, true );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function newPlayerCardTrumpCard(){
        var card = Logic.createCard( gameArea, cardSource, trump.elementId, trump.elementId, 1, trump.x, trump.y );
        trump.visible = false;
        
        playerCardsGroup.addCard( card, true );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function newOpponentCard( lastCard ){
        console.log( "New opponent card." );
        
        //var card = Logic.createCard( gameArea, cardSource, Globals.CARD_BACK, 1, deck.x, deck.y );
        var card = Logic.createCard( gameArea, cardSource, Globals.CARD_BACK, Globals.CARD_BACK, 1, deckColumn.x, deckColumn.y );
        
        if( lastCard ){
            deck.visible = false;
        }
        
        opponentCardsGroup.addCard( card, true );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function newOpponentCardTrumpCard(){
        var card = Logic.createCard( gameArea, cardSource, Globals.CARD_BACK, Globals.CARD_BACK, 1, trump.x, trump.y );
        
        trump.visible = false;
        
        opponentCardsGroup.addCard( card, true );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function newTrumpCard( elementId ){
        console.log( "New trump card." );
        
        //var trumpStartPosX = deck.x;
        //var trumpEndPosX = deck.x + deck.width + Globals.DECK_TRUMP_DISTANCE;
        var trumpStartPosX = deckColumn.x;
        var trumpEndPosX = deckColumn.x + deckColumn.width + Globals.DECK_TRUMP_DISTANCE;
        
        trump.elementId = Globals.CARD_BACK;
        trump.newElementId = elementId;
        trump.x = trumpStartPosX;
        trump.startAnimation( trumpEndPosX, trump.y );
        trump.visible = true;
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function playerCardSelectableChanged( id, selectable ){
        //playerCardsGroup.setSelectable( id, selectable );
        playerCardsGroup.setSelectable( id, selectable, gameAreaMouseArea.mouseX, gameAreaMouseArea.mouseY );
    }
    
    function opponentSelectedCard( id, cardElementId ){
        console.log( "Opponent selected "+id+". card: "+cardElementId );
        
        opponentCardsGroup.selectCard( id, cardElementId );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function opponentGetCentralCards(){
        console.log( "Opponent gets central cards." );
        
        centralCardsGroup.sendCardsAndDestroy( gameArea.width, -centralCardsGroup.height );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function playerGetCentralCards(){
        console.log( "Player gets central cards." );
        
        centralCardsGroup.sendCardsAndDestroy( gameArea.width, gameArea.height );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function opponentScoresChanged( scores ){
        opponentScoreTable.scores = scores;
    }
    
    function playerScoresChanged( scores ){
        playerScoreTable.scores = scores;
    }
    
    function opponentTricksChanged( tricks ){
        opponentScoreTable.tricks = tricks;
    }
    
    function playerTricksChanged( tricks ){
        playerScoreTable.tricks = tricks;
    }
    
    function closeButtonVisibleChanged( visible ){
        closeButton.visible = visible;
    }
    
    function closeDeck(){
        trump.elementId = Globals.CARD_BACK;
    }
    
    function schnapsenButtonVisibleChanged( visible ){
        schnapsenButton.visible = visible;
    }
    
    function twentyButtonVisibleChanged( visible ){
        twentyButton.visible = visible;
    }
    
    function fortyButtonVisibleChanged( visible ){
        fortyButton.visible = visible;
    }
    
    function trumpCardSelectableChanged( selectable ){
        console.log( "Trump card selectable changed: "+selectable );
        trump.selectable = selectable;
    }
    
    function playerChangeTrumpCard( id ){
        console.log( "Player change trump card with "+id+". card." );
        
        var newCard = Logic.createCard( gameArea, cardSource, trump.elementId, trump.elementId, 1, trump.x, trump.y );
        newCard.selectable = true;
        
        trump.visible = false;
        trump.selectable = false;
        trump.willVisible( Globals.ANIMATION_TIME );
        
        var card = playerCardsGroup.takeAt( id );
        card.startAnimation( trump.x, trump.y );
        card.destroy( Globals.ANIMATION_TIME + 10 );
        
        trump.elementId = card.elementId;
        
        playerCardsGroup.addCard( newCard, false );
    }
    
    function opponentChangeTrumpCard( id, elementId ){
        console.log( "Opponent change trump card with "+id+". card ("+elementId+")." );
        
        var newCard = Logic.createCard( gameArea, cardSource, Globals.CARD_BACK, Globals.CARD_BACK, 1, trump.x, trump.y );
        
        trump.visible = false;
        trump.elementId = elementId;
        trump.willVisible( Globals.ANIMATION_TIME );
        
        var card = opponentCardsGroup.takeAt( id );
        card.startAnimation( trump.x, trump.y );
        card.destroy( Globals.ANIMATION_TIME + 10 );
        
        opponentCardsGroup.addCard( newCard, true );
        
        singleShot( signalAnimationEnd, Globals.ANIMATION_TIME );
    }
    
    function showOpponentCards( card1Id, card1ElementId, card2Id, card2ElementId ){
        opponentCardsGroup.showCards( card1Id, card1ElementId, card2Id, card2ElementId );
        
        singleShot( signalAnimationEnd, Globals.SHOW_OPPONENT_CARDS_INTERVAL );
    }
}
