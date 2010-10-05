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
#include "gpibdata.h"
#include <QFile>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("qlabnetview");
    QString tmp="0.0.1 pre-alpha ";
    a.setApplicationVersion(tmp);
    a.setOrganizationName("qlabnetview");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings(a.applicationName(),"default");


    MainWindow w;
    if (a.argc()>1) {
        for (int i=1;i<a.argc();i++) {
            if (QFile(a.arguments().at(i)).exists()) {
                qDebug()<<"File" <<a.arguments().at(i)<<"exists";
                w.fileOpen(a.arguments().at(i));
                break;
            } else {
                qWarning()<<a.arguments().at(i)<<"No such file";
            }
        }
    }

    w.setWindowTitle(a.applicationName()+" "+a.applicationVersion());
    w.showMaximized();

    return a.exec();
}

