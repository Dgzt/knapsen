import QtQuick 1.1
import "globals.js" as Globals

//import "logic.js" as Logic

Item{
    id: main
    width: Globals.MAIN_WIDTH
    height: Globals.MAIN_HEIGHT
    
    signal signalAnimationEnd()
    signal signalSelectedCard( int id, int delay )
    
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
        y: ( main.height - deck.height ) / 2
        source: "/usr/local/share/apps/knapsen/pics/william-tell.svgz"
        elementId: "back"
        
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
    
    Card{
        id: trump
        visible: false
        y: deck.y
        source: "/usr/local/share/apps/knapsen/pics/william-tell.svgz"
        elementId: "back"
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
        trump.visible = false;
        
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
    
    function initialize( playerNameStr, opponentNameStr ){
        console.log( "initialize, player:\""+playerNameStr+"\", opponent:\""+opponentNameStr+"\"" );
        
        //Set player's name
        playerName.setText( playerNameStr );
        
        //Set opponent's name
        opponentName.setText( opponentNameStr );
        
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
    }
    
    function newPlayerCard( lastCard, cardText ){
        console.log( lastCard+" "+cardText );
        
        var component = Qt.createComponent("Card.qml");
        var card = component.createObject( main, {
            "x": deck.x, 
            "y": deck.y,
            "source": "/usr/local/share/apps/knapsen/pics/william-tell.svgz",
            "elementId": cardText
        });
        
        //Logic.addPlayerCard( card );
        playerCardsGroup.addCard( card );
        
        //timer1.start();
        singleShot();
    }
    
    function newOpponentCard( lastCard ){
        console.log( lastCard );
        
        var component = Qt.createComponent("Card.qml");
        var card = component.createObject( main, {
            "x": deck.x, 
            "y": deck.y,
            "source": "/usr/local/share/apps/knapsen/pics/william-tell.svgz",
            "elementId": "back"
        });
        
        //Logic.addOpponentCard( card );
        opponentCardsGroup.addCard( card );
        
        //timer2.start();
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
    }
    
    function playerGetCentralCards(){
        console.log( "Player gets central cards" );
    }
    
}
