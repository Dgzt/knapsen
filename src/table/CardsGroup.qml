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
    
    Timer{
        id: cardConectsTimer
        interval: Globals.ANIMATION_TIME + 10
        
        onTriggered: { cardsGroup.setConnects(); }
    }
    
    Timer{
        id: hideCardsTimer

        property int cardId1: Globals.INVALID_CARD_ID;
        property int cardId2: Globals.INVALID_CARD_ID;
        
        interval: Globals.SHOW_OPPONENT_CARDS_INTERVAL
        onTriggered: hideCards()
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
    
    /*function addCard( card ){
        card.mouseEntered.connect( mouseEnteredToCard );
        card.mouseExited.connect( mouseExitedFromCard );
        card.mouseClicked.connect( mouseClickedToCard );
        cards.add( card );
        
        setCardsZValue();
    }*/
    
    function addCard( card, nowSetConnects ){
        cards.add( card );
        
        if( nowSetConnects ){
            setConnects();
        }else{
            cardConectsTimer.start();
        }
        
        setCardsZValue();
    }
    
    //
    function setConnects(){
        cards.at( cards.size-1 ).mouseEntered.connect( mouseEnteredToCard );
        cards.at( cards.size-1 ).mouseExited.connect( mouseExitedFromCard );
        cards.at( cards.size-1 ).mouseClicked.connect( mouseClickedToCard );
    }
    //
    
    /*function setSelectable( id, selectable ){
        cards.at( id ).selectable = selectable;
    }*/
    function setSelectable( id, selectable, mousePosX, mousePosY ){
        cards.at( id ).selectable = selectable;
        
        if( selectable ){
            var card = cards.at( id );
            
            if( id == cards.size-1 ){
                console.log( id+": "+Math.round(mousePosX)+"x"+Math.round(mousePosY)+" "+Math.round(card.x)+"-"+Math.round(card.x+card.width) );
            }else{
                console.log( id+": "+Math.round(mousePosX)+"x"+Math.round(mousePosY)+" "+Math.round(card.x)+"-"+Math.round(card.x+card.width)+" "+Math.round(cards.at(id+1).x)+"-"+Math.round(cards.at(id+1).x+cards.at(id+1).width) );
            }
            
            if( card.x <= mousePosX && mousePosX < card.x + card.width &&
                card.y <= mousePosY && mousePosY < card.y + card.height )
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
        
        //
        if( cardConectsTimer.running ){
            cardConectsTimer.stop();
        }
        //
        
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
    
    function setCardsZValue(){
        for( var i=0; i < cards.size; ++i ){
            cards.at( i ).z = i;
        }
    }
    
    function selectCard( id, elementId ){
        //var card = cards.takeAt( id );
        //card.newElementId = elementId;
        cards.at( id ).newElementId = elementId;
        
        selectedCard( cards.takeAt( id ) );
    }
    
    function clear(){
        while( cards.size > 0 ){
            cards.takeAt( 0 ).destroy();
        }
    }
    
    function sendCardsAndDestroy( posX, posY ){
        while( cards.size > 0 ){
            cards.at( 0 ).startAnimation( posX, posY );
            cards.takeAt( 0 ).destroy( Globals.ANIMATION_TIME );
        }
    }
    
    function takeAt( id ){
        cards.at( id ).selectable = false;
        cards.at( id ).mouseEntered.disconnect();
        cards.at( id ).mouseExited.disconnect();
        cards.at( id ).mouseClicked.disconnect();
        return cards.takeAt( id );
    }
    
    function showCards( card1Id, card1ElementId, card2Id, card2ElementId ){
        hideCardsTimer.cardId1 = card1Id;
        hideCardsTimer.cardId2 = card2Id;
        
        cards.at( hideCardsTimer.cardId1 ).elementId = card1ElementId;
        cards.at( hideCardsTimer.cardId2 ).elementId = card2ElementId;
        
        var highlightValue = cards.at( hideCardsTimer.cardId1 ).height * ( Globals.HIGHLIGHT_PERCENT / 100 );
        
        cards.at( hideCardsTimer.cardId1 ).y += highlightValue;
        cards.at( hideCardsTimer.cardId2 ).y += highlightValue;
        
        hideCardsTimer.start();
    }
    
    function hideCards(){
        console.log( "Hide cards." );
        
        cards.at( hideCardsTimer.cardId1 ).elementId = Globals.CARD_BACK;
        cards.at( hideCardsTimer.cardId2 ).elementId = Globals.CARD_BACK;
        
        var highlightValue = cards.at( hideCardsTimer.cardId1 ).height * ( Globals.HIGHLIGHT_PERCENT / 100 );
        
        cards.at( hideCardsTimer.cardId1 ).y -= highlightValue;
        cards.at( hideCardsTimer.cardId2 ).y -= highlightValue;
        
        hideCardsTimer.cardId1 = Globals.INVALID_CARD_ID;
        hideCardsTimer.cardId2 = Globals.INVALID_CARD_ID;
    }
}
