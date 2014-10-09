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

#include "spixels.h"

Spixels::Spixels(QObject *qmlObject, QDeclarativeEngine *qmlEngine, QObject *parent) :
    QObject(parent), root(qobject_cast<QDeclarativeItem*>(qmlObject)), engine(qmlEngine), isRunning(false), nOfPlayerShips(0)
{
    cShip = new QDeclarativeComponent(engine, QUrl::fromLocalFile("qml/spixels/Ship.qml"));
    connect(&timer, SIGNAL(timeout()), this, SLOT(moveShips()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(buildShips()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(ai()));
}

void Spixels::newGame()
{
    //qDebug() << "start new game";
    timer.stop();
    QDeclarativeComponent *planet = new QDeclarativeComponent(engine, QUrl::fromLocalFile("qml/spixels/Planet.qml"));
    QObject *area = root->findChild<QObject*>("gameArea");
    int width = area->property("width").toInt();
    int height = area->property("height").toInt();
    int minDiameter = qSqrt(width * height) / 15;
    bool f;
    int x, y, size;

    while (!fleet.isEmpty())
        delete fleet.takeFirst();
    while (!planets.isEmpty())
        delete planets.takeFirst();


    do {
        QObject* p = planet->create();
        QDeclarativeItem *qmlPlanet = qobject_cast<QDeclarativeItem*>(p);
        qmlPlanet->setParentItem(root);

        size = qrand() % (minDiameter / 2) + minDiameter;
        int essay = 0;
        do {
            x = qrand() % (width - size);
            y = qrand() % (height - size);
            f = true;
            for (int k = 0; k < planets.size(); k++) {
                if (qSqrt(qPow(planets.at(k)->position().x() - x, 2) + qPow(planets.at(k)->position().y() - y, 2)) < size + planets.at(k)->radius()*2 + minDiameter) {
                    f = false;
                    break;
                }
            }
            essay++;
        } while (!f && essay < 10);
        if (f) {
            qmlPlanet->setX(x);
            qmlPlanet->setY(y);
            qmlPlanet->setProperty("size", size);
            Planet *p = new Planet(qmlPlanet, planets.size(), 0, this);
            connect(p, SIGNAL(chosenChanged(int)), this, SLOT(sendShips(int)));
            connect(p, SIGNAL(runOutOfShips(int)), this, SLOT(conquerPlanet(int)));
            planets.append(p);
            //planets[i].setPlanet(qmlPlanet, i);
        }
        else
            delete qmlPlanet;
    } while (f);
    delete planet;

    nOfEnemies = planets.size() / 5 + 3;
    for (int i = 0; i < nOfEnemies; i++)
        planets.at(i)->setTeam(i);

    fleet.reserve(1000);
    nOfPlayerShips = 0;
    for (int k = 0; k < planets.size(); k++) {
        int n = qrand() % (planets.at(k)->radius() / 5) + 1;
        for (int i = 0; i < n; i++) {
            createShip(k);
        }
    }
    emit playersPlanetsChanged();
    emit playersShipsChanged();
    emit totalPlanetsChanged();
    emit totalShipsChanged();

    timer.start(300);
    isRunning = true;
    chosenPlanet = -1;
}

void Spixels::pauseGame()
{
    if (isRunning) {
        timer.stop();
        isRunning = false;
    }
    else if (!planets.isEmpty() && playersPlanets() > 0 && playersPlanets() < planets.size()) {
        timer.start(300);
        isRunning = true;
    }
}

void Spixels::moveShips()
{
    Ship *killed;
    bool playersShip;
    for (int k = 0; k < fleet.size(); k++) {
        QPointF direction = awayFromShips(fleet.at(k), killed);
        if (killed == NULL) {
            direction += avoidPlanets(fleet.at(k)) + towardPlanet(fleet.at(k));// + uniteShips(fleet.at(k));// + levelSpeed(fleet.at(i));
            fleet.at(k)->move(direction);
        }
        else {
            if (fleet.at(k)->team() == 1 || killed->team() == 1)
                playersShip = true;
            else
                playersShip = false;
            delete fleet.takeAt(k);
            fleet.removeOne(killed);
            delete killed;
            emit totalShipsChanged();
            if (playersShip) {
                nOfPlayerShips--;
                emit playersShipsChanged();
            }
        }
    }
}

void Spixels::buildShips()
{
    for (int i = 0; i < planets.size(); i++) {
        int radius = planets.at(i)->radius();
        int ships = planets.at(i)->ships();
        int k = ships < 5 ? radius / 3 : ships / 4;
        if ((planets.at(i)->team() == 0 && qrand() % (k * 4) == 0)
                || (planets.at(i)->team() != 0 && qrand() % (k) == 0))
            createShip(i);
    }
}

void Spixels::ai()
{
    for (int i = 0; i < planets.size(); i++)
        if (planets.at(i)->team() > 1 && planets.at(i)->ships() > 10 && qrand() % 15 == 0) {
            unsigned int target = 0, minK = 65535, k, distance;
            for (int j = 0; j < planets.size(); j++)
                if (i != j && planets.at(j)->team() != planets.at(i)->team()) {
                    distance = qSqrt(qPow(planets.at(i)->position().x() - planets.at(j)->position().x(), 2) + qPow(planets.at(i)->position().y() - planets.at(j)->position().y(), 2));
                    k = distance * planets.at(j)->ships();
                    if (k < minK) {
                        minK = k;
                        target = j;
                    }
                }
            sendToPlanet(i, target);
        }
}

void Spixels::sendShips(int n)
{
    if (n == -1) {
        qDebug() << "send to -1";
        return;
    }
    if (chosenPlanet == -1) {
        if (planets.at(n)->team() == 1)
            chosenPlanet = n;
        else
            planets.at(n)->unchoose();
    }
    else if (n == chosenPlanet) {
        planets.at(n)->unchoose();
        chosenPlanet = -1;
    }
    else {
        sendToPlanet(chosenPlanet, n);
        planets.at(chosenPlanet)->unchoose();
        planets.at(n)->unchoose();
        chosenPlanet = -1;
    }
}

void Spixels::sendToPlanet(int sender, int target) {
    for (int k = 0; k < fleet.size(); k++) {
        if (!fleet.at(k)->onWay && fleet.at(k)->planet() == sender && fleet.at(k)->team() == planets.at(sender)->team() && qrand() % 2) {
            fleet.at(k)->setPlanet(target, planets[target]);
            fleet.at(k)->onWay = true;
        }
    }
}

void Spixels::conquerPlanet(int n)
{
    //qDebug() << "conquer";
    if (planets.at(n)->ships() <= 0) {
        int count[nOfEnemies];
        for (int k = 0; k < nOfEnemies; k++)
            count[k] = 0;
        for (int k = 0; k < fleet.size(); k++)
            if (fleet.at(k)->planet() == n && !fleet.at(k)->onWay)
                count[fleet.at(k)->team()]++;
        int team = -1;//count[0] > 0 ? (count[1] > 0 ? -1 : count[2] > 0 ? -1 : 0) : count[1] > 0 ? (count[2] > 0 ? -1 : 1) : count[2] > 0 ? 2 : -1;
        int max = 0;
        for (int k = 0; k < nOfEnemies; k++)
            if (count[k] > max) {
                max = count[k];
                team = k;
            }
        if (team != -1) {
            int oldTeam = planets.at(n)->team();
            planets.at(n)->setTeam(team, count[team]);
            if (oldTeam == 1 || team == 1)
                emit playersPlanetsChanged();
        }
    }
}

int Spixels::totalPlanets()
{
    return planets.size();
}

int Spixels::playersPlanets()
{
    int n = 0;
    for (int i = 0; i < planets.size(); i++)
        if (planets.at(i)->team() == 1)
            n++;
    if (n == 0 || n == planets.size())
        timer.stop();
    return n;
}

int Spixels::totalShips()
{
    return fleet.size();
}

int Spixels::playersShips()
{
    return nOfPlayerShips;
}

void Spixels::createShip(int planet)
{
    QObject *s = cShip->create();
    QDeclarativeItem *qmlShip = qobject_cast<QDeclarativeItem*>(s);
    qmlShip->setParentItem(root);
    Ship *ship = new Ship(qmlShip, planets[planet], planet, this);
    fleet.append(ship);

    emit totalShipsChanged();
    if (ship->team() == 1) {
        nOfPlayerShips++;
        emit playersShipsChanged();
    }
}

QPointF Spixels::uniteShips(Ship *const &ship)
{
    QPointF massCenter;
    if (!ship->onWay)
        return massCenter;
    int ships = 0;
    for (int i = 0; i < fleet.size(); i++)
        if (fleet.at(i) != ship && fleet.at(i)->team() == ship->team() && fleet.at(i)->planet() == ship->planet()) {
            massCenter += fleet.at(i)->position();
            ships++;
        }
    if (!ships)
        return massCenter;
    return (massCenter / ships - ship->position()) / 300;
}

QPointF Spixels::awayFromShips(Ship *const &ship, Ship* &deleted)
{
    QPointF direction, dir;
    QPointF position = ship->position(), otherPosition;
    int team = ship->team();
    Ship *s;
    deleted = NULL;
    for (int i = 0; i < fleet.size(); i++) {
        s = fleet.at(i);
        if (s != ship) {
            otherPosition = s->position();
            float distance = qSqrt(qPow(otherPosition.x() - position.x(), 2) + qPow(otherPosition.y() - position.y(), 2));
            if (distance < 10) {
                if (team == s->team()) {
                    dir = otherPosition - position;
                    dir /= distance / 3;
                    direction -= dir;
                }
                else {
                    //delete fleet.takeAt(i);
                    //fleet.removeOne(ship);
                    //delete ship;
                    deleted = s;
                    //qDebug() << "deleted";
                    return direction;
                }
            }
        }
    }
    return direction;
}

QPointF Spixels::levelSpeed(Ship *const&ship)
{
    QPointF massCenter;
    if (!ship->onWay)
        return massCenter;
    int ships = 0;
    for (int i = 0; i < fleet.size(); i++)
        if (fleet.at(i) != ship && fleet.at(i)->team() == ship->team() && fleet.at(i)->planet() == ship->planet()) {
            massCenter += fleet.at(i)->speed();
            ships++;
        }
    if (!ships)
        return massCenter;
    massCenter /= ships;
    return (massCenter - ship->speed()) / 20;
}

QPointF Spixels::towardPlanet(Ship *const &ship)
{
    QPointF position = ship->position();
    QPointF planetPosition = planets.at(ship->planet())->position();
    QPointF direction = planetPosition - position;
    float distance = qSqrt(qPow(direction.x(), 2) + qPow(direction.y(), 2));
    int radius = planets.at(ship->planet())->radius();
    float k;
    if (distance < radius + 10) {
        if (ship->onWay) {
            k = 0;
            if (ship->team() == planets.at(ship->planet())->team())
                planets.at(ship->planet())->addShip();
            ship->onWay = false;
        }
        else {
            k = 0;
        }
    }
    else if (distance < radius * 2) {
        if (ship->onWay) {
            k = 1;
        }
        else {
            k = 1;
        }
    }
    else {
        if (ship->onWay) {
            k = 3;
        }
        else {
            k = 1;
        }
    }
    if (k == 0) {
        int speed = planets.at(ship->planet())->team() == ship->team() ? 0 : -3;
        if (planetPosition.x() > position.x())
            direction.setY(speed);
        else
            direction.setY(-speed);
        if (planetPosition.y() > position.y())
            direction.setX(-speed);
        else
            direction.setX(speed);
    }
    else
        direction /= planets.at(ship->planet())->radius() / k;
    /*if (ship->onWay) {
        k = 3;
        if (distance <= radius * 2.5) {
            if (ship->team() == planets.at(ship->planet())->team())
                planets.at(ship->planet())->addShip();
            ship->onWay = false;
        }
    }
    else
        k = 1;
    if (distance <= radius + 10) {
        int speed = planets.at(ship->planet())->team() == ship->team() ? 0 : -3;
        if (planetPosition.x() > position.x())
            direction.setY(speed);
        else
            direction.setY(-speed);
        if (planetPosition.y() > position.y())
            direction.setX(-speed);
        else
            direction.setX(speed);
    }
    else {
        direction /= planets.at(ship->planet())->radius() / k;
    }*/
    return direction;
}

QPointF Spixels::avoidPlanets(Ship *const &ship) {
    QPointF direction;
    QPointF position = ship->position();
    QPointF planetPosition;
    int radius;
    float distance, k;
    for (int i = 0; i < planets.size(); i++) {
        radius = planets.at(i)->radius();
        planetPosition = planets.at(i)->position();
        distance = qSqrt(qPow(planetPosition.x() - position.x(), 2) + qPow(planetPosition.y() - position.y(), 2));
        if (distance < radius + 10) {
            if (ship->onWay) {
                if (ship->planet() == i) {
                    k = 1; //never
                    radius *= 2.5;
                }
                else {
                    k = 3;
                    radius *= 2.5;
                }
            }
            else {
                if (ship->planet() == i) {
                    k = 1;
                    radius += 10;
                }
                else {
                    k = 1; //never
                    radius *= 2.5;
                }
            }
        }
        else if (distance < radius * 2) {
            if (ship->onWay) {
                if (ship->planet() == i) {
                    k = 0;
                    radius *= 2.5;
                }
                else {
                    k = 3;
                    radius *= 2.5;
                }
            }
            else {
                if (ship->planet() == i) {
                    k = 0;
                    radius += 10;
                }
                else {
                    k = 1; //never
                    radius *= 2.5;
                }
            }
        }
        else {
            if (ship->onWay) {
                if (ship->planet() == i) {
                    k = 0;
                    radius *= 2.5;
                }
                else {
                    k = 0;
                    radius *= 2.5;
                }
            }
            else {
                if (ship->planet() == i) {
                    k = 0; //never
                    radius += 10;
                }
                else {
                    k = 0;
                    radius *= 2.5;
                }
            }
        }
        if (k > 0) {
            QPointF dir = planetPosition - position;
            dir *= (distance - radius) / radius * k;
            direction +=  dir;
        }
        /*if (i == ship->planet() && !ship->onWay) {
            radius += 10;
            k = 0.9;
        }
        else {
            radius *= 2.5;
            k = 3;
        }
        if (distance <= radius) {
            QPointF dir = planetPosition - position;
            dir *= (distance - radius) / radius * k;
            direction +=  dir;
        }*/
    }
    return direction;
}
