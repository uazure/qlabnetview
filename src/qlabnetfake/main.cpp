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
#include "iostream"
#include "QFile"
#include "QString"
#include "QStringList"

void showHelp(void);
void showVersion(void);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("qlabnetfake");
    a.setApplicationVersion("0.0.1-pre-alpha");


server s;
//a.connect(&s,SIGNAL(wantAbort()),&a,SLOT(quit()));
QStringList arglist=a.arguments();

for (int i=0;i<arglist.size();i++) {
    if (arglist.at(i)=="--version") {
        showVersion();
        return 0;
    }
    if (arglist.at(i)=="--help") {
        showHelp();
        return 0;

    }

}


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


void showHelp(void) {
    std::cout<<"Usage:\n\t"<<QCoreApplication::applicationName().toAscii().data()<<" [datafile.dat]\n";
    std::cout<<"Where datafile.dat is the ASCII file with constant data row columns count.\n";
    std::cout<<"If no datafile.dat is specified, the sample.dat is used by default.\n";
}

void showVersion(void) {
    std::cout<<QCoreApplication::applicationName().toAscii().data();
    std::cout<<" ver. "<<QCoreApplication::applicationVersion().toAscii().data();
    std::cout<<"\n";
}
