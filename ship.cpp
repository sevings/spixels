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

#include "ship.h"

Ship::Ship(QDeclarativeItem *s, Planet *planet, int p, QObject *parent) :
    QObject(parent), maxSpeed(20), onWay(false), ship(s), itsPlanet(p), pPlanet(planet)
{
    halfWidth = ship->property("width").toInt() / 2;
    halfHeight = ship->property("height").toInt() / 2;
    float x = planet->position().x() + qrand() / (RAND_MAX / 20.0) - 10;
    float y = planet->position().y() + qrand() / (RAND_MAX / 20.0) - 10;
    itsPosition.setX(x);
    itsPosition.setY(y);
    ship->setX(x - halfWidth);
    ship->setY(y - halfHeight);
    itsTeam = planet->team();
    switch (itsTeam) {
    case 0:
        ship->setProperty("color", "dimgrey"); //neutral
        break;
    case 1:
        ship->setProperty("color", "green"); //player
        break;
    case 2:
        ship->setProperty("color", "crimson"); //enemies
        break;
    case 3:
        ship->setProperty("color", "blueviolet");
        break;
    case 4:
        ship->setProperty("color", "darkorange");
        break;
    case 5:
        ship->setProperty("color", "peru");
        break;
    case 6:
        ship->setProperty("color", "gold");
        break;
    }
    planet->addShip();
}

Ship::~Ship()
{
    if (pPlanet->team() == itsTeam && !onWay)
        pPlanet->removeShip();
    delete ship;
}

QPointF Ship::position() const
{
    return itsPosition;
}

int Ship::planet() const
{
    return itsPlanet;
}

void Ship::move(QPointF direction)
{
    itsSpeed /= 2;
    itsSpeed += direction;
    float speedAbs = qSqrt(qPow(itsSpeed.x(), 2) + qPow(itsSpeed.y(), 2));
    if (speedAbs > maxSpeed)
        itsSpeed /= speedAbs / maxSpeed;
    itsPosition += itsSpeed;
    ship->setProperty("x", itsPosition.x() - halfWidth);
    ship->setProperty("y", itsPosition.y() - halfHeight);
}

void Ship::setPlanet(int p, Planet *planet)
{
    //qDebug() << "set to " << p;
    if (p >= 0 && p != itsPlanet) {
        pPlanet->removeShip();
        itsPlanet = p;
    }
    if (planet != NULL)
        pPlanet = planet;
}

int Ship::team() const
{
    return itsTeam;
}

void Ship::setTeam(int t)
{
    itsTeam = t;
}

QPointF Ship::speed()
{
    return itsSpeed;
}
