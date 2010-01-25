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
#define UDP_SIZE 255

server::server()
{
this->bind(25050);
connect(this,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));

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

void server::processTheDatagram(QByteArray array, QHostAddress sender,quint16 port) {
    QByteArray answer;
    array=array.trimmed();
    if (array.startsWith("gpib scan")) {
        answer="200 You found me\r\n";


    } else if (array.startsWith("get interval")) {
     answer="200 Interval 1000\r\n";


 } else if (array.startsWith("ping")) {
     answer="pong\r\n";
 } else if (array.startsWith("get all")) {
    answer.clear();
    int i=0;
    while (answer.size()<UDP_SIZE && i<data.size()) {
        answer.append(this->data.at(i).trimmed()).append("\r\n");
        i++;
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
    qDebug()<<this->data.at(0);
}

server::~server() {


}
