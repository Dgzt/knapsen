import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

Item{
    id: cardsGroup
    
    property int mouseEnteredCardId: Globals.INVALID_CARD_ID;
    
    //signal widthChanged()
    //signal heightChanged()
    signal sizeChanged()
    signal selectedCardId( int id )
    signal selectedCard( variant card )
    
    onXChanged: setCardsPos()
    
    Array{
        id: cards
        
        //onSizeChanged: setCardsGroupSize()
    }
    
    function setCardsGroupSize(){
        //Width
        if( cards.size == 0 ){
            cardsGroup.width = 0;
        }else if( cards.size == 1 ){
            cardsGroup.width = cards.at(0).width;
        }else{
            cardsGroup.width = cards.at(0).width + ( cards.size - 1 ) * ( cards.at(0).width / 2 );
        }
        
        //widthChanged();
        
        //Height
        if( cards.size == 0 ){
            cardsGroup.height = 0;
            //heightChanged();
        }else if( cards.at( 0 ).height != cardsGroup.height ){
            cardsGroup.height = cards.at( 0 ).height;
            //heightChanged();
        }
        
        sizeChanged();
    }
    
    function setCardsPos(){
        console.log( "Set cards new pos." );
        
        var distance = .0;
        if( cards.size >= 1 ){
            distance = cards.at(0).width / 2;
        }
        
        for( var i = 0; i < cards.size; ++i ){
            cards.at(i).setMoveAnimation( cardsGroup.x + ( i * distance ), cardsGroup.y );
            cards.at(i).startMoveAnimation();
        }
    }
    
    function addCard( card ){
        card.mouseClicked.connect( click );
        card.mouseEntered.connect( mouseEntered );
        card.mouseExited.connect( mouseExited );
        
        cards.add( card );
        //
        //cards.at( cards.size - 1 ).cardId = cards.size - 1;
        //
        
        setCardsZValue();
        //
        setCardsGroupSize();
    }
    
    function setCardSelectable( id, selectable ){
        cards.at(id).selectable = selectable;
        
        /*if( id == containsMouseCardId() ){
            mouseEnteredCardId = id;
            highlightCard();
        }*/
    }
    
    function containsMouseCardId(){
        for( var i=0; i< cards.size; ++i ){
            if( cards.at(i).isContainsMouse() ){
                return i;
            }
        }
    }
    
    function click(){
        console.log( "Click to "+mouseEnteredCardId+". card." );
        
        var cardId = mouseEnteredCardId;
        mouseEnteredCardId = Globals.INVALID_CARD_ID;
        
        selectedCardId( cardId );
        selectedCard( cards.takeAt( cardId ) );
        //
        setCardsGroupSize()
    }
    
    function mouseEntered(){
        mouseEnteredCardId = containsMouseCardId();
        console.log( "Entered to "+mouseEnteredCardId+". card." );
        
        highlightCard();
    }
    
    function mouseExited(){
        console.log( "Exited from "+mouseEnteredCardId+". card." );
        
        removeHighlight();
        
        mouseEnteredCardId = Globals.INVALID_CARD_ID;
    }
    
    function highlightCard(){
        var highlightValue = cards.at( mouseEnteredCardId ).height * ( Globals.HIGHLIGHT_PERCENT / 100 );
        cards.at( mouseEnteredCardId ).y -= highlightValue;
    }
    
    function removeHighlight(){
        var highlightValue = cards.at( mouseEnteredCardId ).height * ( Globals.HIGHLIGHT_PERCENT / 100 );
        cards.at( mouseEnteredCardId ).y += highlightValue;
    }
    
    function selectCard( id, cardText ){
        cards.at( id ).elementId = cardText;
        selectedCard( cards.takeAt( id ) );
        //setCardsPos();
        setCardsGroupSize()
    }
    
    function setCardsZValue(){
        for( var i=0; i < cards.size; ++i ){
            cards.at(i).z = i;
        }
    }
    
    function clear(){
        while( cards.size > 0 ){
            cards.takeAt( 0 ).destroy();
        }
        
        setCardsGroupSize();
    }
    
    //
    function cardsSize(){
        return cards.size;
    }
    
    function takeCard( id ){
        //
        var retCard = cards.takeAt( id );
        
        retCard.mouseClicked.disconnect();
        retCard.mouseEntered.disconnect();
        retCard.mouseExited.disconnect();
        
        return retCard;
        //
    }
    //
}
