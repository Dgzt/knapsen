import QtQuick 1.1
import "globals.js" as Globals

Item{
    id: main
    width: 600
    height: 600
    //anchors.fill: parent
    
    //Opponent's name
    Name{
        id: opponentName
        color: "white"
        width: 100
        height: Globals.NAME_HEIGHT
        x: ( parent.width - width ) / 2
        y: Globals.NAME_DISTANCE
    }
    
    //Player's name
    Name{
        id: playerName
        width: 100
        height: Globals.NAME_HEIGHT
        x: ( parent.width - width ) / 2
        y: parent.height - Globals.NAME_DISTANCE - height
    }
    
    //Opponent's scoretable
    ScoreTable{
        id: opponentScoreTable
        x: ( parent.width / 2 ) + Globals.SCORE_TABLE_DISTANCE
        y: opponentName.y + opponentName.height + Globals.SCORE_TABLE_DISTANCE
    }
    
    //Player's scoretable
    ScoreTable{
        id: playerScoreTable
        x: ( parent.width / 2 ) + Globals.SCORE_TABLE_DISTANCE
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
        
        //Clear variables
        opponentScoreTable.setTricks( 0 );
        opponentScoreTable.setScores( 0 );
        playerScoreTable.setTricks( 0 );
        playerScoreTable.setScores( 0 );
    }
    
    
}
