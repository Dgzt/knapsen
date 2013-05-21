import QtQuick 1.1
import "globals.js" as Globals

Item{
    id: main
    width: 900
    height: 700
    
    signal signalAnimationEnd()
    
    //Opponent's name
    Name{
        id: opponentName
        visible: false
        width: 100
        height: Globals.NAME_HEIGHT
        x: ( parent.width - width ) / 2
    }
    
    //Player's name
    Name{
        id: playerName
        visible: false
        width: 100
        height: Globals.NAME_HEIGHT
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
        id: timer
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
    }
     
    /*function resize(){
        console.log( main.width + "x" + main.height );
    }*/
    
    function clear(){
        console.log( "Clear." );
        opponentName.visible = false;
        playerName.visible = false;
        opponentScoreTable.visible = false;
        playerScoreTable.visible = false;
        
        deck.visible = false;
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
        
        opponentScoreTable.y = opponentName.animationPosYEnd() + opponentName.height + Globals.SCORE_TABLE_DISTANCE;
        opponentScoreTable.setAnimationPosX( main.width - opponentScoreTable.getWidth(), main.width / 2 + Globals.SCORE_TABLE_DISTANCE );
        
        playerScoreTable.y = playerName.animationPosYEnd() - Globals.SCORE_TABLE_DISTANCE - playerScoreTable.getHeight();
        playerScoreTable.setAnimationPosX( main.width - playerScoreTable.getWidth(), main.width / 2 + Globals.SCORE_TABLE_DISTANCE );
        
        
        opponentName.startAnimation();
        opponentName.visible = true;
        
        playerName.startAnimation();
        playerName.visible = true;
        
        opponentScoreTable.startAnimation();
        opponentScoreTable.visible = true;
        
        playerScoreTable.startAnimation();
        playerScoreTable.visible = true;
        
        timer.start();
    }
    
    function newRound(){
        console.log( "New round." );
        
        deck.startAnimation();
        deck.visible = true;
    }
    
}
