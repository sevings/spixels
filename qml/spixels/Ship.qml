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
    width: 5
    height: 5
    radius: 2
    color: "#808080"
    property int time: 300
    /*Text {
        text: parent.x + ', ' + parent.y
        color: 'red'
    }*/

    Behavior on x {
        NumberAnimation {
            duration: time
        }
    }
    Behavior on y {
        NumberAnimation {
            duration: time
        }
    }
}
