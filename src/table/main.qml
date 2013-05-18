import QtQuick 1.1
import "globals.js" as Globals

Item{
    id: main
    width: 600
    height: 600
    
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
    
    /*function resize(){
        console.log( main.width + "x" + main.height );
    }*/
    
    function initialize( playerNameStr, opponentNameStr ){
        console.log( "initialize, player:\""+playerNameStr+"\", opponent:\""+opponentNameStr+"\"" );
        
        //Set player's name
        playerName.setText( playerNameStr );
        
        //Set opponent's name
        opponentName.setText( opponentNameStr );
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
    }
    
}
