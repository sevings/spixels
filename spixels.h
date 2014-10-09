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

#ifndef SPIXELS_H
#define SPIXELS_H

#include <QObject>
#include <QDeclarativeContext>
#include <QDeclarativeComponent>
#include <QDeclarativeItem>
#include <QtCore/qmath.h>
#include <QTimer>
#include <QList>
//#include <QThread>
#include <QDebug>
#include "ship.h"
#include "planet.h"

class Spixels : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int totalPlanets READ totalPlanets NOTIFY totalPlanetsChanged)
    Q_PROPERTY(int playersPlanets READ playersPlanets NOTIFY playersPlanetsChanged)
    Q_PROPERTY(int totalShips READ totalShips NOTIFY totalShipsChanged)
    Q_PROPERTY(int playersShips READ playersShips NOTIFY playersShipsChanged)
public:
    explicit Spixels(QObject *qmlObject, QDeclarativeEngine *qmlEngine, QObject *parent);
    
signals:
    void totalPlanetsChanged();
    void playersPlanetsChanged();
    void totalShipsChanged();
    void playersShipsChanged();
    
public slots:
    void newGame();
    void pauseGame();
    void moveShips();
    void buildShips();
    void ai();
    void sendShips(int n);
    void conquerPlanet(int n);

    int totalPlanets();
    int playersPlanets();
    int totalShips();
    int playersShips();

private slots:
    void sendToPlanet(int sender, int target);
    
private:
    QDeclarativeItem *root;
    QDeclarativeEngine *engine;
    QDeclarativeComponent* cShip;
    QList<Planet*> planets;
    QList<Ship*> fleet;
    QTimer timer;
    bool isRunning;
    int nOfPlayerShips;
    int chosenPlanet;
    int nOfEnemies; //including player and neutral

    void createShip(int planet);
    QPointF uniteShips(Ship *const &ship);
    QPointF awayFromShips(Ship *const &ship, Ship *&deleted);
    QPointF levelSpeed(Ship *const &ship);
    QPointF towardPlanet(Ship *const &ship);
    QPointF avoidPlanets(Ship *const &ship);
};

#endif // SPIXELS_H
