import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

Item{
    id: cardsGroup
    
    property int mouseEnteredCardId: Globals.INVALID_CARD_ID;
    
    Array{
        id: cards
        
        onSizeChanged: setCardsGroupWidth()
    }
    
    signal widthChanged()
    
    function setCardsGroupWidth(){
        if( cards.size == 0 ){
            cardsGroup.width = 0;
        }else if( cards.size == 1 ){
            cardsGroup.width = cards.at(0).width;
        }else{
            cardsGroup.width = cards.at(0).width + ( cards.size - 1 ) * ( cards.at(0).width / 2 );
        }
        
        widthChanged();
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
        
        //setCardsGroupWidth();
        setCardsPos();
    }
    
    function setCardSelectable( id, selectable ){
        cards.at(id).selectable = selectable;
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
    }
    
    function mouseEntered(){
        mouseEnteredCardId = containsMouseCardId();
        console.log( "Entered to "+mouseEnteredCardId+". card." );
    }
    
    function mouseExited(){
        console.log( "Exited from "+mouseEnteredCardId+". card." );
        mouseEnteredCardId = Globals.INVALID_CARD_ID;
    }
    
    function clear(){
        while( cards.size > 0 ){
            cards.takeAt( 0 ).destroy();
        }
    }
    
}
