import QtQuick 1.1

Item{
    id: scoreTable
    
    property int rectWidth: 100
    property int rectHeight: 50
    
    Name{
        id: pointsText
        width: rectWidth
        height: rectHeight / 2
    }
    
    Name{
        id: scpresText
        width: rectWidth
        height: rectHeight / 2
        y: rectHeight / 2 
    }
    
}
