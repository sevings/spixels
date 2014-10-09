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

#include "planet.h"

Planet::Planet(QDeclarativeItem *p, int np, int team, QObject *parent) :
    QObject(parent), planet(NULL)
{
    setPlanet(p, np, team);
}

Planet::Planet(QObject *parent): QObject(parent), planet(NULL), itsPosition(0, 0), itsRadius(0), itsTeam(0),
    itsChosen(false), count(0), nPlanet(-1)
{
}

Planet::~Planet()
{
    delete planet;
}

QPointF Planet::position()
{
    return itsPosition;
}

int Planet::radius()
{
    return itsRadius;
}

void Planet::setPlanet(QDeclarativeItem *p, int np, int team)
{
    if (p != NULL) {
        if (planet != NULL)
            delete planet;
        planet = p;
        initialize();
    }
    nPlanet = np;

    setTeam(team);
}

int Planet::team()
{
    return itsTeam;
}

void Planet::setTeam(int t, int ships)
{
    itsTeam = t;
    count = ships;
    planet->setProperty("ships", count);
    if (planet != NULL) {
        switch (t) {
        case 0:
            planet->setProperty("color", "dimgrey"); //neutral
            break;
        case 1:
            planet->setProperty("color", "green"); //player
            break;
        case 2:
            planet->setProperty("color", "crimson"); //enemies
            break;
        case 3:
            planet->setProperty("color", "blueviolet");
            break;
        case 4:
            planet->setProperty("color", "darkorange");
            break;
        case 5:
            planet->setProperty("color", "peru");
            break;
        case 6:
            planet->setProperty("color", "gold");
            break;
        }
    }
}

bool Planet::chosen()
{
    return itsChosen;
}

void Planet::unchoose()
{
    itsChosen = false;
    QTimer::singleShot(200, this, SLOT(disableHighlight()));
}

void Planet::addShip()
{
    count++;
    planet->setProperty("ships", count);
}

void Planet::removeShip()
{
    count--;
    planet->setProperty("ships", count);
    if (count <= 0)
        emit runOutOfShips(nPlanet);
}

int Planet::ships()
{
    return count;
}

void Planet::choiceChanged()
{
    if (planet != NULL) {
        itsChosen = planet->property("chosen").toBool();
        emit chosenChanged(nPlanet);
    }
}

void Planet::disableHighlight()
{
    if (planet != NULL)
        planet->setProperty("chosen", false);
}

void Planet::initialize()
{
    if (planet != NULL) {
        itsRadius = planet->property("size").toInt() / 2;
        int x = planet->x() + itsRadius;
        int y = planet->y() + itsRadius;
        itsPosition.setX(x);
        itsPosition.setY(y);
        unchoose();
        count = 0;
        planet->setProperty("ships", count);
        connect(planet, SIGNAL(chosenChanged()), this, SLOT(choiceChanged()));
    }
}

