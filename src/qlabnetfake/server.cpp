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
#include <QtNetwork>
#include "server.h"
#include <QRegExp>
#define UDP_SIZE 1300

server::server()
{
    if (!this->bind(25050)) {
        qDebug() << "Failed to bind UDP 25050 port. Consider quit";
        emit wantAbort();

    }

connect(this,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));
this->delimiterRegExp.setPattern("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row}

}

void server::readPendingDatagrams(void) {
    while (this->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(this->pendingDatagramSize());
         QHostAddress sender;
         quint16 senderPort;

         this->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);

         processTheDatagram(datagram,sender,senderPort);
     }

}

QByteArray server::prepareData(int index) {
    if (index<0) {
        qDebug() << "prepareData: index is negative. aborting";
        return "";
    }
    int i=index;
    QByteArray answer;

    while (answer.size()<UDP_SIZE && i<data.size()) {
        answer.append(this->data.at(i).trimmed()).append("\r\n");
        qDebug()<<"appended "<<this->data.at(i).trimmed();
        i++;
        }

    if (answer.size()>=UDP_SIZE) {
        answer.truncate(answer.size()-data.at(i-1).trimmed().size()-2);
        }

    return answer;

}

void server::processTheDatagram(QByteArray array, QHostAddress sender,quint16 port) {
    QByteArray answer;
    array=array.trimmed();
    if (array.startsWith("gpib scan")) {
        answer="200 You found me\r\n";


    } else if (array.startsWith("get interval")) {
        answer="200 Interval:\r\n1000\r\n";

 } else if (array.startsWith("ping")) {
     answer="pong\r\n";

 } else if (array.startsWith("get all")) {
    answer="200 Initial data:\r\n"+this->prepareData();
 }

 else if (array.startsWith("get from ")) {
     QString t;
     QStringList tlist;
     QByteArray start=array.right(array.length()-9).trimmed(); //9 - is the "get from " length
     int i=data.size()-1;
     for (;i>=0;i--) {
         t=data.at(i);
         //FIXME: this is probably a cpu-consuming opertation. Should be rewriten to be faster
         tlist=t.split(this->delimiterRegExp);
         if (tlist.last().trimmed()==start) {
             i++;
             answer="200 Data from "+start+":\r\n"+this->prepareData(i);
             break;
         }
    }
 }

    if (!answer.isEmpty()) {
        this->writeDatagram(answer,sender,port);
    }


}
void server::setInFile(QString filename) {
    qDebug("Current file is '%s'",qPrintable(filename));
    this->inFile=filename;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray tmp=file.readAll();
    file.close();
    this->data=QString(tmp).split("\n");
    qDebug()<<this->data.at(0).trimmed();
}

server::~server() {


}
