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
    property int size: 50
    property bool chosen: false
    property int ships: 0
    width: size
    height: size
    radius: size/2
    color: '#808080'
    Text {
        anchors.centerIn: parent
        text: ships
    }

    MouseArea {
        anchors.centerIn: parent
        width: parent.width * 2
        height: parent.height * 2
        onClicked: {
            chosen = !chosen
        }
    }
    Rectangle {
        id: circle
        width: parent.width + 5
        height: width
        z: 1000
        color: 'transparent'
        radius: width / 2
        border.width: 5
        border.color: 'white'
        visible: chosen
        anchors.centerIn: parent
    }
}
