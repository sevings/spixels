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

Image {
    id: button
    fillMode: Image.PreserveAspectFit
    width: parent.width
    height: parent.height
    smooth: true
    property bool active: true
    signal pressed
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: if (active) button.pressed()
    }
}
