import QtQuick 1.1
import "globals.js" as Globals

//import "logic.js" as Logic

Item{
    id: main
    width: Globals.MAIN_WIDTH
    height: Globals.MAIN_HEIGHT
    
    signal signalAnimationEnd()
    
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
    
    //Timer emit signalAnimationEnd signal
    Timer{
        id: timer1
        running: false
        onTriggered: signalAnimationEnd()
        interval: Globals.ANIMATION_TIME
    }
    
    //Timer emit signalAnimationEnd signal
    Timer{
        id: timer2
        running: false
        onTriggered: signalAnimationEnd()
        interval: Globals.ANIMATION_TIME
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
        
        onWidthChanged: setOpponentCardsGroupPos(), setOpponentScoreTablePos()
    }
    
    CardsGroup{
        id: playerCardsGroup
        
        onWidthChanged: setPlayerCardsGroupPos(), setPlayerScoreTablePos()
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
    
    function clear(){
        console.log( "Clear." );
        opponentName.visible = false;
        playerName.visible = false;
        opponentScoreTable.visible = false;
        playerScoreTable.visible = false;
        
        deck.visible = false;
        trump.visible = false;
        
        //Logic.clear();
        opponentCardsGroup.clear();
        playerCardsGroup.clear();
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
        
        timer1.start();
    }
    
    function newRound(){
        console.log( "New round." );
        
        deck.x = 0;
        deck.setMoveAnimation( Globals.DECK_DISTANCE, deck.y );
        
        deck.startMoveAnimation();
        deck.startOpacityAnimation();
        deck.visible = true;
        
        timer2.start();
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
        
        timer1.start();
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
        
        timer2.start();
    }
    
    function newTrumpCard( cardText ){
        console.log( cardText );
        
        trump.x = deck.x;
        trump.elementId = cardText;
        trump.setMoveAnimation( deck.x + deck.width + Globals.DECK_TRUMP_DISTANCE, trump.y );
        trump.startMoveAnimation();
        trump.visible = true;
    }
    
}
