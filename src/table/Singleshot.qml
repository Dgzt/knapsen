import QtQuick 1.1

Timer{
    id: timer
    running: false
    
    onTriggered: { destroy( 1000 ); }
}
