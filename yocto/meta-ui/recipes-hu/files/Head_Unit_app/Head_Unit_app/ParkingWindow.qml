import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle {
    id: parkingWindow
    color: "#CC000000"
    anchors.fill: parent

    Rectangle {
        id: parkingMain
        width: 640
        height: 400
        color: "#000000"
        anchors.centerIn: parent

        Text {
            id: parkingHeader
            text: "Parking Status"
            font.bold: true
            font.pixelSize: 40
            color: "#FFFFFF"

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 90
            anchors.leftMargin: 60
        }

        Text {
            id: distanceIndex
            text: "Distance:             cm"
            font.bold: false
            font.pixelSize: 30
            color: "#FFFFFF"

            anchors.top: parkingHeader.bottom
            anchors.left: parent.left
            anchors.topMargin: 160
            anchors.leftMargin: 60
        }

        Text {
            id: distValue
            font.pixelSize: 35
            font.bold: true
            color: "#FFFFFF"

            anchors.right: distanceIndex.right
            anchors.bottom: distanceIndex.bottom
            anchors.rightMargin: 50

            text: carInfoController.distanceCM.toFixed(1)
        }

        Connections {
            target: carInfoController
            onDirectionChanged: {
                distValue.text = carInfoController.distanceCM.toFixed(1)
            }
        }

        DistanceImageSet {
            id: distanceImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.verticalCenterOffset: 20
            anchors.rightMargin: 50
        }

    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parkingWindow.visible = false
        }
        z: -1
    }
}
