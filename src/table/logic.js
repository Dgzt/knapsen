
function createCard( parent, source, elementId, scale, x, y ){
    var component = Qt.createComponent("Card.qml");
    var card = component.createObject( parent, {
        "x": x, 
        "y": y,
        "source": source,
        "elementId": elementId,
        "scale": scale
    });
    
    return card;
}

function createSingleShot(){
    
}
