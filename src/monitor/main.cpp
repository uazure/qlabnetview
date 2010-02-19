/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2009  Sergey Popov (aka azure)
Last modification: 26 Oct 2009

        This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
        This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
        You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui/QApplication>
#include "mainwindow.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("qlabnetmonitor");
    a.setApplicationVersion("0.0.1 pre-alpha");
    a.setOrganizationName("qlabnetview");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings();
    MainWindow w;
    w.setWindowTitle(a.applicationName()+" "+a.applicationVersion());
    w.show();
    return a.exec();
}
