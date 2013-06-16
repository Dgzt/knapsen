import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

Item{
/*Rectangle{
    color: "white"
    //*/
    id: cardsGroup
    
    property int highlightCardId: Globals.INVALID_CARD_ID
    
    signal selectedCardId( int id )
    signal selectedCard( variant card )
    
    onXChanged: setCardsPos()
    
    Array{
        id: cards
        
        onSizeChanged: setCardsGroupSize()
    }
    
    function setCardsGroupSize(){
        //Height
        if( cards.size == 0 ){
            cardsGroup.height = 0;
            //heightChanged();
        }else if( cards.at( 0 ).height != cardsGroup.height ){
            cardsGroup.height = cards.at( 0 ).height;
            //heightChanged();
        }
        
        //Width
        if( cards.size == 0 ){
            cardsGroup.width = 0;
        }else if( cards.size == 1 ){
            cardsGroup.width = cards.at(0).width;
        }else{
            cardsGroup.width = cards.at(0).width + ( cards.size - 1 ) * ( cards.at(0).width / 2 );
        }
        
        console.log( "CardsGroup size: "+cardsGroup.width+"x"+cardsGroup.height );
    }
    
    function setCardsPos(){
        console.log( "Set cards new pos." );
        
        var distance = .0;
        if( cards.size >= 1 ){
            distance = cards.at(0).width / 2;
        }
        
        for( var i = 0; i < cards.size; ++i ){
            cards.at(i).startAnimation( cardsGroup.x + ( i * distance ), cardsGroup.y );
        }
    }
    
    function addCard( card ){
        card.mouseEntered.connect( mouseEnteredToCard );
        card.mouseExited.connect( mouseExitedFromCard );
        card.mouseClicked.connect( mouseClickedToCard );
        cards.add( card );
    }
    
    /*function setSelectable( id, selectable ){
        cards.at( id ).selectable = selectable;
    }*/
    function setSelectable( id, selectable, mousePosX, mousePosY ){
        cards.at( id ).selectable = selectable;
        
        if( selectable ){
            var card = cards.at( id );
            
            if( card.x <= mousePosX && mousePosX <= card.x + card.width &&
                card.y <= mousePosY && mousePosY <= card.y + card.height )
            {
                if( id == cards.size-1 || mousePosX < cards.at( id+1 ).x ){
                    console.log( id+" contains mouse." );
                    highlightCardId = id;
                    highlightCard();
                }
            }
        }
            
    }
    
    //
    function mouseEnteredToCard(){
        for( var i = 0; i < cards.size; ++i ){
            if( cards.at( i ).containsMouse && i != highlightCardId ){
                highlightCardId = i;
                highlightCard();
            }
        }
    }
    
    function mouseExitedFromCard(){
        removeHighlight();
        highlightCardId = Globals.INVALID_CARD_ID;
    }
    
    function mouseClickedToCard(){
        console.log( "Selected card: "+highlightCardId );
        
        selectedCardId( highlightCardId );
        selectedCard( cards.takeAt( highlightCardId ) );
        
        highlightCardId = Globals.INVALID_CARD_ID;
    }
    
    function highlightCard(){
        var highlightValue = cards.at( highlightCardId ).height * ( Globals.HIGHLIGHT_PERCENT / 100 );
        cards.at( highlightCardId ).y -= highlightValue;
    }
    
    function removeHighlight(){
        var highlightValue = cards.at( highlightCardId ).height * ( Globals.HIGHLIGHT_PERCENT / 100 );
        cards.at( highlightCardId ).y += highlightValue;
    }
    //
    
    function clear(){
        while( cards.size > 0 ){
            cards.takeAt( 0 ).destroy();
        }
    }
}
