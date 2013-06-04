import QtQuick 1.1
import "globals.js" as Globals
import "logic.js" as Logic

//import "logic.js" as Logic

Item{
    id: main
    width: Globals.MAIN_WIDTH
    height: Globals.MAIN_HEIGHT
    
    property string cardSource: ""
    property real cardScale: 0
    property variant cardSize: Qt.size( 0, 0 )
    
    signal signalAnimationEnd()
    signal signalSelectedCard( int id, int delay )
    signal signalCloseButtonClicked()
    signal signalFortyButtonClicked()
    signal signalTwentyButtonClicked()
    signal signalSchnapsenButtonClicked()
    //
    signal signalTrumpCardClicked()
    //
    
    //Opponent's name
    Name{
        id: opponentName
        visible: false
        x: ( parent.width - width ) / 2
    }
    
    //Player's name
    Name{
        id: playerName
        visible: false
        x: ( parent.width - width ) / 2
    }
    
    //Opponent's scoretable
    ScoreTable{
        id: opponentScoreTable
        visible: false
    }
    
    //Player's scoretable
    ScoreTable{
        id: playerScoreTable
        visible: false
    }

    Card{
        id: deck
        visible: false
        source: cardSource
        elementId: Globals.CARD_BACK
        y: ( main.height - deck.height ) / 2
        //
        scale: cardScale
        //
        
        NumberAnimation on opacity {
            id: opacityAnimation
            running: false
            from: 0
            to: 1
            duration: Globals.ANIMATION_TIME
        }
        
        function startOpacityAnimation(){
            opacityAnimation.start();
        }
    }
    
    Button{
        id: closeButton
        text: "Close"
        x: deck.x
        y: deck.y + deck.height + Globals.BUTTON_DISTANCE
        width: deck.width
        height: Globals.BUTTON_HEIGHT
        visible: false
        
        onClick: signalCloseButtonClicked()
    }
    
    Card{
        id: trump
        visible: false
        y: deck.y
        //source: "/usr/local/share/apps/knapsen/pics/william-tell.svgz"
        source: cardSource
        elementId: Globals.CARD_BACK
        scale: cardScale
        
        onMouseClicked: signalTrumpCardClicked()
        
        //
        Timer{
            id: visibleTimer
            interval: Globals.ANIMATION_TIME
            
            onTriggered: { trump.visible = true; }
        }
        
        function startVisibleTimer(){
            visibleTimer.start();
        }
        
    }
    
    CardsGroup{
        id: opponentCardsGroup
        
        //onWidthChanged: setOpponentCardsGroupPos(), setOpponentScoreTablePos()
        onSizeChanged: setOpponentCardsGroupPos(), setOpponentScoreTablePos()
        
        //
        onSelectedCard: centralCardsGroup.addCard( card )
    }
    
    CardsGroup{
        id: playerCardsGroup
        
        //onWidthChanged: setPlayerCardsGroupPos(), setPlayerScoreTablePos()
        onSizeChanged: setPlayerCardsGroupPos(), setPlayerScoreTablePos()
        
        //
        onSelectedCardId: signalSelectedCard( id, Globals.ANIMATION_TIME )
        onSelectedCard: centralCardsGroup.addCard( card )
    }
    
    CardsGroup{
        id: centralCardsGroup
        
        //x: ( main.width - centralCardsGroup.width ) / 2
        //y: ( main.height - centralCardsGroup.height ) / 2
        //onWidthChanged: setCentralCardsGroupPos()
        onSizeChanged: setCentralCardsGroupPos()
    
    }
    
    Button{
        id: fortyButton
        text: "Forty"
        x: playerScoreTable.x
        y: playerScoreTable.y - Globals.BUTTON_DISTANCE - fortyButton.height
        width: deck.width
        height: Globals.BUTTON_HEIGHT
        visible: false
        
        onClick: signalFortyButtonClicked()
    }
    
    Button{
        id: twentyButton
        text: "Twenty"
        x: fortyButton.x
        y: fortyButton.y - Globals.BUTTON_DISTANCE - twentyButton.height
        width: deck.width
        height: Globals.BUTTON_HEIGHT
        visible: false
        
        onClick: signalTwentyButtonClicked()
    }
    
    Button{
        id: schnapsenButton
        text: "Schnapsen"
        x: twentyButton.x
        y: twentyButton.y - Globals.BUTTON_DISTANCE - schnapsenButton.height
        width: deck.width
        height: Globals.BUTTON_HEIGHT
        visible: false
        
        onClick: signalSchnapsenButtonClicked()
    }
     
    /*function resize(){
        console.log( main.width + "x" + main.height );
    }*/
    
    function setOpponentCardsGroupPos(){
        console.log( "opponentCardsGroup: "+opponentCardsGroup.width );
        
        opponentCardsGroup.x = ( main.width - opponentCardsGroup.width ) / 2;
        opponentCardsGroup.y = opponentName.animationPosYEnd() + opponentName.height + Globals.NAME_CARDSGROUP_DISTANCE;
    }
    
    function setOpponentScoreTablePos(){
        opponentScoreTable.setXAnimation( opponentCardsGroup.x + opponentCardsGroup.width + Globals.SCORE_TABLE_DISTANCE );
        opponentScoreTable.startXAnimation();
    }
    
    function setPlayerCardsGroupPos(){
        console.log( "playerCardsGroup: "+playerCardsGroup.width );
        
        playerCardsGroup.x = ( main.width - playerCardsGroup.width ) / 2;
        playerCardsGroup.y = playerName.animationPosYEnd() - Globals.NAME_CARDSGROUP_DISTANCE - deck.height;
    }
    
    function setPlayerScoreTablePos(){
        playerScoreTable.setXAnimation( playerCardsGroup.x + playerCardsGroup.width + Globals.SCORE_TABLE_DISTANCE );
        playerScoreTable.startXAnimation();
    }
    
    function setCentralCardsGroupPos(){
        //First set y then x, before when set X then set the position of cards
        centralCardsGroup.y = ( main.height - centralCardsGroup.height ) / 2;
        centralCardsGroup.x = ( main.width - centralCardsGroup.width ) / 2;
    }
    
    function clear(){
        console.log( "Clear." );
        opponentName.visible = false;
        playerName.visible = false;
        opponentScoreTable.visible = false;
        playerScoreTable.visible = false;
        
        deck.visible = false;
        closeButton.visible = false;
        trump.visible = false;
        fortyButton.visible = false;
        twentyButton.visible = false;
        schnapsenButton.visible = false;
        
        opponentCardsGroup.clear();
        playerCardsGroup.clear();
        centralCardsGroup.clear();
    }
    
    function singleShot(){
        var singleShotComponent = Qt.createComponent("Singleshot.qml");
        var timer = singleShotComponent.createObject( main );
        timer.triggered.connect( signalAnimationEnd );
        timer.interval = Globals.ANIMATION_TIME;
        
        timer.start();
    }
    
    function initialize( playerNameStr, opponentNameStr, source, scale ){
        console.log( "initialize, player:\""+playerNameStr+"\", opponent:\""+opponentNameStr+"\"" );
        
        //Set player's name
        playerName.setText( playerNameStr );
        
        //Set opponent's name
        opponentName.setText( opponentNameStr );
        
        //Set source
        cardSource = source;
        
        cardScale = scale;
        
        console.log( "deck.height: "+deck.height );
    }
    
    function startGame(){
        console.log( "Start game." );
        
        opponentName.setAnimationPosY( 0, Globals.NAME_DISTANCE );
        playerName.setAnimationPosY( main.height - playerName.height, -Globals.NAME_DISTANCE );
        
        setOpponentCardsGroupPos();
        
        opponentScoreTable.x = main.width - opponentScoreTable.width;
        opponentScoreTable.y = opponentName.animationPosYEnd() + opponentName.height + Globals.SCORE_TABLE_DISTANCE;
        
        setPlayerCardsGroupPos();
        
        playerScoreTable.x = main.width - playerScoreTable.width;
        playerScoreTable.y = playerName.animationPosYEnd() - Globals.SCORE_TABLE_DISTANCE - playerScoreTable.height;
        
        opponentName.startAnimation();
        opponentName.visible = true;
        
        playerName.startAnimation();
        playerName.visible = true;
        
        setOpponentScoreTablePos();
        opponentScoreTable.startOpacityAnimation();
        opponentScoreTable.visible = true;
        
        setPlayerScoreTablePos();
        playerScoreTable.startOpacityAnimation();
        playerScoreTable.visible = true;
        
        setCentralCardsGroupPos();
        
        //timer1.start();
        singleShot()
        
        tmpItem.visible = true;
    }
    
    function newRound(){
        console.log( "New round." );
        
        deck.x = 0;
        deck.setMoveAnimation( Globals.DECK_DISTANCE, deck.y );
        
        deck.startMoveAnimation();
        deck.startOpacityAnimation();
        deck.visible = true;
        
        //timer2.start();
        singleShot();
        
        //
        cardSize = Qt.size( deck.width, deck.height );
        console.log( "Card size: "+cardSize.width+"x"+cardSize.height );
    }
    
    function newPlayerCard( lastCard, cardText ){
        console.log( lastCard+" "+cardText );
        
        /*var component = Qt.createComponent("Card.qml");
        var card = component.createObject( main, {
            "x": deck.x, 
            "y": deck.y,
            "source": cardSource,
            "elementId": cardText,
            "scale": cardScale
        });*/
        var card = Logic.createCard( main, cardSource, cardText, cardScale, deck.x, deck.y );
        
        if( lastCard ){
            deck.visible = false;
        }
        
        playerCardsGroup.addCard( card );
        
        singleShot();
    }
    
    function newPlayerCardTrumpCard(){
        /*var component = Qt.createComponent("Card.qml");
        var card = component.createObject( main, {
            "x": trump.x, 
            "y": trump.y,
            "source": cardSource,
            "elementId": trump.elementId,
            "scale": cardScale
        });*/
        var card = Logic.createCard( main, cardSource, trump.elementId, cardScale, trump.x, trump.y );
        
        trump.elementId = Globals.CARD_BACK
        trump.visible = false;
        
        playerCardsGroup.addCard( card );
        
        singleShot();
    }
    
    function newOpponentCard( lastCard ){
        console.log( lastCard );
        
        /*var component = Qt.createComponent("Card.qml");
        var card = component.createObject( main, {
            "x": deck.x, 
            "y": deck.y,
            "source": cardSource,
            "elementId": Globals.CARD_BACK,
            "scale": cardScale
        });*/
        var card = Logic.createCard( main, cardSource, Globals.CARD_BACK, cardScale, deck.x, deck.y );
        
        if( lastCard ){
            deck.visible = false;
        }
        
        opponentCardsGroup.addCard( card );
        
        singleShot();
    }
    
    function newOpponentCardTrumpCard(){
        /*var component = Qt.createComponent("Card.qml");
        var card = component.createObject( main, {
            "x": trump.x, 
            "y": trump.y,
            "source": cardSource,
            "elementId": Globals.CARD_BACK,
            "scale": cardScale
        });*/
        var card = Logic.createCard( main, cardSource, Globals.CARD_BACK, cardScale, trump.x, trump.y );
        
        trump.elementId = Globals.CARD_BACK
        trump.visible = false;
        
        opponentCardsGroup.addCard( card );
        
        singleShot();
    }
    
    function newTrumpCard( cardText ){
        console.log( cardText );
        
        trump.x = deck.x;
        trump.elementId = cardText;
        trump.setMoveAnimation( deck.x + deck.width + Globals.DECK_TRUMP_DISTANCE, trump.y );
        trump.startMoveAnimation();
        trump.visible = true;
        
        //timer3.start();
        singleShot();
    }
    
    function playerCardSelectableChanged( id, selectable ){
        console.log( id+" "+selectable );
        playerCardsGroup.setCardSelectable( id, selectable );
    }
    
    function opponentSelectedCard( id, cardText ){
        console.log( id+" "+cardText );
        opponentCardsGroup.selectCard( id, cardText );
        
        //timer1.start();
        singleShot();
    }
    
    function opponentGetCentralCards(){
        console.log( "Opponent gets central cards." );
        
        moveAndDestroyCentralCards( main.width - cardSize.width, 0 );
    }
    
    function playerGetCentralCards(){
        console.log( "Player gets central cards" );
        
        moveAndDestroyCentralCards( main.width - cardSize.width, main.height - cardSize.height );
    }
    
    function moveAndDestroyCentralCards( posX, posY ){
        console.log( "Move position: "+posX+"x"+posY );
        
        console.log( centralCardsGroup.cardsSize() );
        while( centralCardsGroup.cardsSize() > 0 ){
            var card = centralCardsGroup.takeCard( 0 );
            card.setMoveAnimation( posX, posY );
            card.startMoveAnimation()
            card.destroy( Globals.ANIMATION_TIME );
        }
        
        singleShot();
    }
    
    function closeButtonVisibleChanged( visible ){
        closeButton.visible = visible;
    }
    
    function closeDeck(){
        trump.elementId = Globals.CARD_BACK;
    }
    
    function fortyButtonVisibleChanged( visible ){
        fortyButton.visible = visible;
    }
    
    function twentyButtonVisibleChanged( visible ){
        twentyButton.visible = visible;
    }
    
    function schnapsenButtonVisibleChanged( visible ){
        schnapsenButton.visible = visible;
    }
    
    function opponentTricksChanged( tricks ){
        opponentScoreTable.tricks = tricks;
    }
    
    function opponentScoresChanged( scores ){
        opponentScoreTable.scores = scores;
    }
    
    function playerTricksChanged( tricks ){
        playerScoreTable.tricks = tricks;
    }
    
    function playerScoresChanged( scores ){
        playerScoreTable.scores = scores;
    }
    
    function trumpCardSelectableChanged( selectable ){
        trump.selectable = selectable;
    }
    
    function playerChangeTrumpCard( cardId ){
        console.log( "Change." );
        
        var newTmpTrumpCard = playerCardsGroup.takeCard( cardId );
        newTmpTrumpCard.setMoveAnimation( trump.x, trump.y );
        
        /*var component = Qt.createComponent("Card.qml");
        var newCard = component.createObject( main, {
            "x": trump.x, 
            "y": trump.y,
            "source": cardSource,
            "elementId": trump.elementId,
            "scale": cardScale,
            "selectable": true
        });*/
        var newCard = Logic.createCard( main, cardSource, trump.elementId, cardScale, trump.x, trump.y );
        newCard.selectable = true;
        
        trump.visible = false;
        trump.elementId = newTmpTrumpCard.elementId;
        trump.selectable = false;
          
        newTmpTrumpCard.startMoveAnimation();
        newTmpTrumpCard.destroy( Globals.ANIMATION_TIME+100 );
        trump.startVisibleTimer();
        
        playerCardsGroup.addCard( newCard );
        //Becouse doesn't change the size.
        playerCardsGroup.setCardsPos();
    }
    
    function opponentChangeTrumpCard( cardId, newTrumpCardText ){
        console.log( "Change." );
        
        var newTmpTrumpCard = opponentCardsGroup.takeCard( cardId );
        newTmpTrumpCard.setMoveAnimation( trump.x, trump.y );
        
        /*var component = Qt.createComponent("Card.qml");
        var newCard = component.createObject( main, {
            "x": trump.x, 
            "y": trump.y,
            "source": cardSource,
            "elementId": Globals.CARD_BACK,
            "scale": cardScale
        });*/
        var newCard = Logic.createCard( main, cardSource, Globals.CARD_BACK, cardScale, trump.x, trump.y );
        
        trump.visible = false;
        trump.elementId = newTrumpCardText;
        
        newTmpTrumpCard.startMoveAnimation();
        newTmpTrumpCard.destroy( Globals.ANIMATION_TIME+100 );
        trump.startVisibleTimer();
        
        opponentCardsGroup.addCard( newCard );
        opponentCardsGroup.setCardsPos();
        
        singleShot();
    }
    
    function showOpponentCards( card1Id, card1Text, card2Id, card2Text ){
        console.log( "Show opponent cards." );
        
        opponentCardsGroup.showCards( card1Id, card1Text, card2Id, card2Text );
        
        singleShot();
    }
}
