import CustomComponents 1.0
import QtQuick 1.1
import "globals.js" as Globals

SvgImage{
    id: card
    
    NumberAnimation on x { 
        id: xAnimation
        running: false
        from: 0
        to: 50
        duration: Globals.ANIMATION_TIME
    }
        
    NumberAnimation on opacity {
        id: opacityAnimation
        running: false
        from: 0
        to: 1
        duration: Globals.ANIMATION_TIME
    }
    
    function startAnimation(){
        xAnimation.start();
        opacityAnimation.start();
    }
}
