import QtQuick 1.0

Item{
  
    id: window

    //Background
    Image{
        id: backgroundImage
        anchors.fill: parent
        source: "../pics/background.png"
        fillMode: Image.Tile
    }
  
    function initialize(/* playerName, opponentName */){
        console.log( "initialize" );
    }
  
  
}
