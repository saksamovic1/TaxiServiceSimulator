import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 2.15
//import io.qt.examples.backend 1.0

Window {
    id: mainWindow
    height: 420
    width: 420
    visible: true

    GridLayout { //ovdje ispod ubaciti ispise da li ima ili nema cabova, da li je okej lokacija neka ili nesto tako

        Canvas {
            id: canvas
            height: parent.height
            width: parent.width

            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: 400
            Layout.preferredHeight: 400
            Layout.margins: 10

            readonly property string grassUrl: "images/grass.png"
            readonly property string roadUrl: "images/road.png"
            property list<string> cabUrlList: ["images/cab0.png", "images/cab1.png", "images/cab2.png", "images/cab3.png"]
            readonly property string passengerEntryUrl: "images/passenger_entry.png"
            readonly property string passengerExitUrl: "images/passenger_exit.png"

            readonly property int squareSize: 40

            property bool drawEntry: false
            property bool drawExit: false


            Connections {
                target: cab1
                onCurrentLocationChanged: {
                    canvas.requestPaint()
                }
            }

            Connections {
                target: cab2
                onCurrentLocationChanged: {
                    canvas.requestPaint()
                }
            }

            Connections {
                target: cab3
                onCurrentLocationChanged: {
                    canvas.requestPaint()
                }
            }


            Component.onCompleted: {
                loadImage(grassUrl)
                loadImage(roadUrl)
                for (var i = 0; i < 4; i++) {
                    loadImage(cabUrlList[i])
                }
                loadImage(passengerEntryUrl)
                loadImage(passengerExitUrl)
            }

            onImageLoaded: requestPaint()

            onPaint:{
                var ctx = canvas.getContext("2d");

                for (var i = 0; i < 10; i++) {
                    for (var j = 0; j < 10; j++) {
                        env.isObstacle(i, j)? ctx.drawImage(grassUrl, i*squareSize, j*squareSize,
                                                                           squareSize, squareSize) :
                                                             ctx.drawImage(roadUrl, i*squareSize, j*squareSize,
                                                                            squareSize, squareSize)
                    }
                }
                ctx.drawImage(cabUrlList[cab1.getRotation()], cab1.getCurrentRow()*squareSize,
                              cab1.getCurrentCol()*squareSize, squareSize, squareSize)
                ctx.drawImage(cabUrlList[cab2.getRotation()], cab2.getCurrentRow()*squareSize,
                              cab2.getCurrentCol()*squareSize, squareSize, squareSize)
                ctx.drawImage(cabUrlList[cab3.getRotation()], cab3.getCurrentRow()*squareSize,
                              cab3.getCurrentCol()*squareSize, squareSize, squareSize)

                if((cab1.getCurrentRow() === env.getCurrentEntryRow() || cab2.getCurrentRow() === env.getCurrentEntryRow()
                       || cab3.getCurrentRow() === env.getCurrentEntryRow())
                        && (cab1.getCurrentCol() === env.getCurrentEntryCol() || cab2.getCurrentCol() === env.getCurrentEntryCol()
                            || cab3.getCurrentCol() === env.getCurrentEntryCol()))
                    drawEntry = false;

                if((cab1.getCurrentRow() === env.getCurrentExitRow() || cab2.getCurrentRow() === env.getCurrentExitRow()
                       || cab3.getCurrentRow() === env.getCurrentExitRow())
                        && (cab1.getCurrentCol() === env.getCurrentExitCol() || cab2.getCurrentCol() === env.getCurrentExitCol()
                            || cab3.getCurrentCol() === env.getCurrentExitCol()))
                    drawExit = false;

                if(drawEntry) {
                    ctx.drawImage(passengerEntryUrl, env.getCurrentEntryRow()*squareSize,
                                  env.getCurrentEntryCol()*squareSize, squareSize, squareSize)
                }

                if(drawExit) {
                    ctx.drawImage(passengerExitUrl, env.getCurrentExitRow()*squareSize,
                                  env.getCurrentExitCol()*squareSize, squareSize, squareSize)
                }

                ctx.save()
            }

            MouseArea {

                property bool entry: true

                id: mouse
                anchors.fill: parent
                onClicked: {
                    if(entry && !env.isObstacle(mouseX / canvas.squareSize, mouseY / canvas.squareSize)) {
                        console.log("entry registered")
                        env.setEntryLocation(mouseX / canvas.squareSize, mouseY / canvas.squareSize)
                        canvas.requestPaint()
                        entry = !entry
                        canvas.drawEntry = true
                    }
                    else if(!env.isObstacle(mouseX / canvas.squareSize, mouseY / canvas.squareSize)){
                        console.log("exit registered")
                        env.setExitLocation(mouseX / canvas.squareSize, mouseY / canvas.squareSize)
                        canvas.requestPaint()
                        entry = !entry
                        canvas.drawExit = true
                    }
                }

                Connections {
                    target: cab1
                    function onBusyChanged() {
                        mouse.enabled = !mouse.enabled
                    }
                }

                Connections {
                    target: cab2
                    function onBusyChanged() {
                        mouse.enabled = !mouse.enabled
                    }
                }

                Connections {
                    target: cab3
                    function onBusyChanged() {
                        mouse.enabled = !mouse.enabled
                    }
                }
            }

        }
    }
}
