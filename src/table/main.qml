import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

Item{
    id: main
    width: 600
    height: 600
    
    //
    signal signalAnimationEnd()
    //
    
    //Opponent's name
    Name{
        id: opponentName
        width: 100
        height: Globals.NAME_HEIGHT
        x: ( parent.width - width ) / 2
    }
    
    //Player's name
    Name{
        id: playerName
        width: 100
        height: Globals.NAME_HEIGHT
        x: ( parent.width - width ) / 2
        y: parent.height - height
    }
    
    //Opponent's scoretable
    ScoreTable{
        id: opponentScoreTable
        x: main.width - opponentScoreTable.getWidth()
        y: opponentName.y + opponentName.height + Globals.SCORE_TABLE_DISTANCE
    }
    
    //Player's scoretable
    ScoreTable{
        id: playerScoreTable
        x: main.width - playerScoreTable.getWidth()
        y: playerName.y - Globals.SCORE_TABLE_DISTANCE - 50
    }
    
    //Timer emit signalAnimationEnd signal
    Timer{
        id: timer
        running: false
        onTriggered: signalAnimationEnd()
        interval: Globals.ANIMATION_TIME
    }
    
    /*function resize(){
        console.log( main.width + "x" + main.height );
    }*/
    
    SvgImage{
        id: deck
        //x: main.width / 2
        y: ( main.height - deck.height ) / 2
        source: "/usr/local/share/apps/knapsen/pics/william-tell.svgz"
        elementId: "back"
    }
    
    function clear(){
        console.log( "Clear." );
        /*opponentName.visible = false;
        playerName.visible = false;
        opponentScoreTable.visible = false;
        playerScoreTable.visible = false;*/
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
        opponentName.setAnimationEnd( opponentName.y + Globals.NAME_DISTANCE );
        playerName.setAnimationEnd( playerName.y - Globals.NAME_DISTANCE );
        opponentScoreTable.setAnimationEnd( ( main.width / 2 ) + Globals.SCORE_TABLE_DISTANCE );
        playerScoreTable.setAnimationEnd( ( main.width / 2 ) + Globals.SCORE_TABLE_DISTANCE );
        
        opponentName.startAnimation();
        playerName.startAnimation();
        opponentScoreTable.startAnimation();
        playerScoreTable.startAnimation();
        
        timer.start();
    }
    
    function newRound(){
        console.log( "New round." );
    }
    
}
