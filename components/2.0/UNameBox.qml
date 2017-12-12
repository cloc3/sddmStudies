/***************************************************************************
*Copyright (c) 2013 Nikita Mikhaylov <nslqqq@gmail.com>
*
*Permission is hereby granted, free of charge, to any person
*obtaining a copy of this software and associated documentation
*files (the "Software"), to deal in the Software without restriction,
*including without limitation the rights to use, copy, modify, merge,
*publish, distribute, sublicense, and/or sell copies of the Software,
*and to permit persons to whom the Software is furnished to do so,
*subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included
*in all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
*OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
*THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
*OR OTHER DEALINGS IN THE SOFTWARE.
*
***************************************************************************/

import QtQuick 2.0

FocusScope {
        id: container
        width: 150; height: 30

        property int auC: usermodel.initAutoCompletion;
        property color color: "white"
        property color borderColor: "#ababab"
        property color focusColor: "#266294"
        property color hoverColor: "#5692c4"
        property alias radius: main.radius
        property alias font: innerText.font
        property alias textColor: innerText.color
        property string inputText: ""
        property string completion: " - completion"
        property alias text: innerText.text
        property string head
        property string tail
        property string username: head+tail

    Rectangle {
        id: main

        width: 150; height: 30
        anchors.verticalCenter: parent.verticalCenter

        color: parent.activeFocus? "#f1f1f1" : "#ffffff"
        border.color: container.borderColor
        border.width:1

        Behavior on border.color { ColorAnimation { duration: 100 } }
        states: [
            State {
                name: "hover"; when: mouseArea.containsMouse
                PropertyChanges {
                    target: main
                    border.width: 1
                    border.color: container.hoverColor
                    color: parent.activeFocus? "#f1f1f1" : "#ffffff"
                }
            },
            State {
                name: "focus"; when: container.activeFocus && !mouseArea.containsMouse
                PropertyChanges {
                    target: main
                    border.width: 1
                    border.color: container.focusColor
                }
            }
        ]
    }

    Keys.onPressed: {
        if (
            event.key === Qt.Key_Return ||
            event.key === Qt.Key_Enter  ||
            event.key === Qt.Key_Escape ||
            event.key === Qt.Key_Tab
            )
        {
            this.focus= false
            event.editingFinished()
        }
          else if (event.key === Qt.Key_Backspace)
        {
            this.inputText=this.inputText.slice(0,-1)
        }
          else if (event.text==="");
          else
        {
            this.inputText += event.text
        }
        usermodel.head=event.text
        usermodel.tail=""
        head = usermodel.head
        tail = usermodel.tail
        event.accepted
    }

    MouseArea {
        id: mouseArea
        cursorShape: parent.activeFocus? Qt.IBeamCursor : Qt.PointingHandCursor

        anchors.fill: container

        hoverEnabled: true

        onEntered: if(main.state=="") main.state = "hover"
        onExited: if(main.state=="hover") main.state = ""
        onClicked: parent.focus = true
    }
    Text {
        id: innerText
        width: parent.width - 16
        anchors.centerIn: parent
        textFormat: Text.RichText
        text: "<span style='font:bold'>" + head + "</span><span style='font: italic; color: grey'>" + tail + "</span>"
        focus: true
    }
}
