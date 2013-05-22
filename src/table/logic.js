var playerCards = new Array();
var opponentCards = new Array();

var playerCardsPosY;
var opponentCardsPosY;

function setPlayerCardsPosY( posY ){
    playerCardsPosY = posY;
}

function setOpponentCardsPosY( posY ){
    opponentCardsPosY = posY;
}

function addPlayerCard( card ){
    playerCards.push( card );
    
    var cardXAnimation = ( Globals.MAIN_WIDTH - card.width ) / 2;
    
    card.setXAnimation( cardXAnimation );
    card.setYAnimation( playerCardsPosY - card.height );
    
    playerScoreTable.setXAnimation( cardXAnimation + card.width + Globals.SCORE_TABLE_DISTANCE );
    playerScoreTable.startXAnimation();
    
    card.startXAnimation();
    card.startYAnimation();
}

function addOpponentCard( card ){
    opponentCards.push( card );
    
    var cardXAnimation = ( Globals.MAIN_WIDTH - card.width ) / 2;
    
    card.setXAnimation( cardXAnimation );
    card.setYAnimation( opponentCardsPosY );
    
    opponentScoreTable.setXAnimation( cardXAnimation + card.width + Globals.SCORE_TABLE_DISTANCE );
    opponentScoreTable.startXAnimation();
    
    card.startXAnimation();
    card.startYAnimation();
}

function clear(){
    while( opponentCards.length > 0 ){
        opponentCards[0].destroy();
        opponentCards.splice( 0, 1 );
    }
    
    while( playerCards.length > 0 ){
        playerCards[0].destroy();
        playerCards.splice( 0, 1 );
    }
}
