import QtQuick 2.15

Item {

    property var cabQtInstance //cabCppClass

    Image {
        source: "images/cab.png"
        height: 40
        width: 40
        Connections {
            target: cabQtInstance
            onCurrentLocationChanged: {
                x = cabQtInstance.getCurrentRow() * 40
                y = cabQtInstance.getCurrentCol() * 40
            }
        }

    }

}
