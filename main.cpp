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

#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include <QGraphicsObject>
//#include <QtOpenGL/QGLWidget>
#include "qmlapplicationviewer.h"
#include "spixels.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
    viewer.setMainQmlFile(QLatin1String("qml/spixels/main.qml"));
    //viewer.setViewport(new QGLWidget);
    //viewer.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //viewer.setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);

    Spixels sp(viewer.rootObject(), viewer.engine(), viewer.rootContext());
    viewer.rootContext()->setContextProperty("logic", &sp);

#if defined(Q_WS_MAEMO_5)
    viewer.showFullScreen();
#else
    viewer.showExpanded();
#endif
    return app->exec();
}
