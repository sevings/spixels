/*
    Space Pixels
    Copyright ⓒ 2014 Vasily Khodakov <binque@ya.ru>.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 1.0

Rectangle {
    id: root
    width: 480
    height: 800
    Image {
        anchors.fill: parent
        source: '../../background.png'
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        id: gameArea
        objectName: "gameArea"
        width: root.width
        height: root.height - menu.height
        color: 'transparent'
    }
    Rectangle {
        id: menu
        y: parent.height - height
        z: 1000
        height: parent.height / 16
        width: root.width
        color: 'transparent'
        Image {
            id: planetIcon
            fillMode: Image.PreserveAspectFit
            width: height
            height: parent.height
            source: 'icons/planet.png'
            smooth: true
            x: (parent.width - parent.height * 5 - planetsStatus.width - shipsStatus.width) / 4

        }
        Text {
            id: planetsStatus
            text: logic.playersPlanets + '/' + logic.totalPlanets
            font.pixelSize: parent.height / 2
            color: 'white'
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            height: parent.height
            x: planetIcon.x + planetIcon.width
        }
        Image {
            id: shipIcon
            fillMode: Image.PreserveAspectFit
            width: height
            height: parent.height
            source: 'icons/rocket.png'
            smooth: true
            x: planetsStatus.x + planetsStatus.width + planetIcon.x * 2
        }
        Text {
            id: shipsStatus
            text: logic.playersShips + '/' + logic.totalShips
            font.pixelSize: parent.height / 2
            color: 'white'
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            height: parent.height
            x: shipIcon.x + shipIcon.width
        }
        Button {
            id: playButton
            source: 'icons/play.png'
            width: height
            anchors.right: pauseButton.left
            onPressed: logic.newGame();
        }
        Button {
            id: pauseButton
            source: 'icons/pause.png'
            width: height
            anchors.right: exitButton.left
            onPressed: logic.pauseGame();
        }
        Button {
            id: exitButton
            source: 'icons/exit.png'
            width: height
            anchors.right: menu.right
            onPressed: Qt.quit();
        }
    }
}
