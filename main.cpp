/****************************************************************************
*
Copyright (C) 20016-2017 ferdl <ferdl@online.de>
All rights reserved.

This file is part of hypex-volume (hvc) (hypex-volume-control).

hvc is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

hvc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with usb-svc; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
or see <http://www.gnu.org/licenses/>.

Hypex is a seperate company and I have nothing to with it! Please see
https://www.hypex.nl

*
****************************************************************************/

#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include "qtquick2applicationviewer.h"
#include "hypex.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;

    hypex hy;
    viewer.rootContext()->setContextProperty("hypex", &hy);
    viewer.setMainQmlFile(QStringLiteral("qml/Hypex_Volume/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
