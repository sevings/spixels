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

#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QDeclarativeItem>
#include <QtCore/qmath.h>
#include <QTimer>
#include <QDebug>

class Planet : public QObject
{
    Q_OBJECT
public:
    Planet(QDeclarativeItem *p, int np, int team = 0, QObject *parent = NULL);
    explicit Planet(QObject *parent = 0);
    ~Planet();
    QPointF position();
    int radius();
    void setPlanet(QDeclarativeItem *p, int np, int team = 0);
    int team();
    void setTeam(int t, int ships = 0);
    bool chosen();
    void unchoose();
    void addShip();
    void removeShip();
    int ships();

signals:
    void chosenChanged(int n);
    void runOutOfShips(int n);
    
public slots:
    void choiceChanged();

private slots:
    void disableHighlight();
    
private:
    QDeclarativeItem *planet;
    QPointF itsPosition;
    int itsRadius;
    int itsTeam;
    bool itsChosen;
    int count;
    int nPlanet;

    void initialize();
};

#endif // PLANET_H
