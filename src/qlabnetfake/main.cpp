/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2009  Sergey Popov (aka azure)
Last modification: 30 Oct 2009

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

#include <QtCore/QCoreApplication>

#include "server.h"
#include "QFile"
#include "QString"
#include "QStringList"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


server s;
QStringList arglist=a.arguments();
if (a.argc()>1) {
    qDebug("There is command-line arguments, good...");
    QString infile=arglist.at(1);
    QFile file(infile);

    if (file.exists()) {
        qDebug()<<"Trying to open file " <<infile;



    } else {
        qCritical("File '%s' does not exist. Consider quit, bye!", qPrintable(infile));
        return 1;
    }

} else {
    qWarning()<<"No file specified. Will try to open sample.dat\n";

    qWarning("Run '%s infile'",qPrintable(arglist.at(0)));
    QFile file("sample.dat");
    if (QFile::exists("sample.dat")) {
        s.setInFile("sample.dat");
    } else {
        qCritical("File 'sample.dat' does not exist. Consider quit, bye!");
        return 1;

    }

}

    return a.exec();
}
