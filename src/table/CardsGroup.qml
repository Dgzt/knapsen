import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

/*Item{*/
Rectangle{
    color: "white"
    //
    id: cardsGroup
    
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
        cards.add( card );
    }
    
    
    function clear(){
        while( cards.size > 0 ){
            cards.takeAt( 0 ).destroy();
        }
    }
}
