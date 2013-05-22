var playerCards = new Array();
var opponentCards = new Array();

var playerCardsPosY;

function setPlayerCardsPosY( posY ){
    playerCardsPosY = posY;
}

function addPlayerCard( card ){
    playerCards.push( card );
    
    card.setXAnimation( ( Globals.MAIN_WIDTH - card.width ) / 2 );
    card.setYAnimation( playerCardsPosY - card.height );
    card.startXAnimation();
    card.startYAnimation();
}

function clear(){
    while( playerCards.length > 0 ){
        playerCards[0].destroy();
        playerCards.splice( 0, 1 );
    }
}
