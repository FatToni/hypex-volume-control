import QtQuick 2.0

Rectangle {
    id: rectangle
    width: 400
    height: 400
    color: "#000000"

    property bool mute_onoff: false;

    Rectangle {
        id: rectangle1
        x: 56
        y: 60
        width: 100
        height: 56
        color: "#9964a0"
        radius: 4
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: text1
            x: 51
            y: 24
            text: qsTr("+")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 40
        }
        MouseArea {
            anchors.fill: parent
            onPressed: rectangle1.state = "PRESSED"
            onReleased: rectangle1.state = "RELEASED"
            onClicked: {
                //console.debug("lauter")
                if(mute_onoff) mute_onoff = hypex.volume_mute();
                text_dB.text = hypex.volume_up() + " dB";
            }
            onPressAndHold: {

            }
        }
        states: [
            State {
                name: "PRESSED"
                PropertyChanges { target: rectangle1; color: "lightblue"}
            },
            State {
                name: "RELEASED"
                PropertyChanges { target: rectangle1; color: "#9964a0"}
            }
        ]
    }

    Rectangle {
        id: rectangle2
        x: 56
        y: 190
        width: 100
        height: 56
        color: "#9964a0"
        radius: 4
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: text2
            x: 51
            y: 24
            text: qsTr("-")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 40
        }

        MouseArea {
            anchors.fill: parent
            onPressed: rectangle2.state = "PRESSED"
            onReleased: rectangle2.state = "RELEASED"
            onClicked: {
                //console.debug("leiser")
                if(mute_onoff) mute_onoff = hypex.volume_mute();
                text_dB.text = hypex.volume_down() + " dB";
            }
        }
        states: [
            State {
                name: "PRESSED"
                PropertyChanges { target: rectangle2; color: "lightblue"}
            },
            State {
                name: "RELEASED"
                PropertyChanges { target: rectangle2; color: "#9964a0"}
            }
        ]
    }

    Rectangle {
        id: rectangle3
        x: 56
        y: 250
        width: 100
        height: 56
        color: "#9964a0"
        radius: 4
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter


        Text {
            id: text3
            x: 51
            y: 24
            text: qsTr("Mute")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 30
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.debug("mute " + mute_onoff)
                mute_onoff = hypex.volume_mute();
                if(mute_onoff) text_dB.text = "muted"
                else text_dB.text = hypex.volume_up() + " dB";
            }
        }
    }

    Text {
        id: text_dB
        x: 68
        y: 140
        color: "#ffdddd"
        text: qsTr("-42 dB")
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 26
    }

    Text {
        id: text_usblist
        x: 10
        y: 8
        width: 380
        height: 52
        color: "#ffdddd"
        text: hypex.get_usblist()
        visible: false
        wrapMode: Text.WordWrap
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: rectangle3.horizontalCenter
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
        font.pixelSize: 10
    }

    Text {
        id: text_hypexlist
        x: 10
        y: 334
        width: 380
        height: 58
        color: "#ffdddd"
        text: hypex.get_hypexlist()
        wrapMode: Text.NoWrap
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
        font.pixelSize: 10
    }
}
