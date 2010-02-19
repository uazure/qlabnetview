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

#include <QtNetwork>
#include <QSettings>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include "gpibsocket.h"
//#include "mainwindow.h"

#define GPIB_UDP_PORT 25050



gpibSocket::gpibSocket(void)
{
    //init the QUdpSocket
    rxBytes=0;
    txBytes=0;
    running=false;

    //bind to port 25051
    quint16 port=GPIB_UDP_PORT+1;
    while (!this->bind(port) and port<65535) {
        qWarning() << "Failed to bind udp port"<< port;
        port++;}



    //call rx() when readyRead signal emitted (when udp packet arrives)
    connect(this,SIGNAL(readyRead()),this,SLOT(rx()));

}
gpibSocket::~gpibSocket() {

}

int gpibSocket::getInterval(void) {
    return this->interval;
}

void gpibSocket::setServerAddress(QHostAddress address) {
    if (!address.isNull())
    {this->serverAddress=address;}
}

QHostAddress gpibSocket::getServerAddress() {
    return this->serverAddress;
}


void gpibSocket::ping() {
this->startPingTime=QTime::currentTime();
this->tx("ping");
}

void gpibSocket::scan (void) {
    QSettings settings(this);
    QHostAddress broadcast(settings.value("broadcastAddress","255.255.255.255").toString());
    this->tx("gpib scan",&broadcast);
}

void gpibSocket::serverGetInterval() {
    this->tx("get interval");
}

void gpibSocket::serverSetInterval(int msec) {
    this->tx("set interval="+QByteArray::number(msec));
}

void gpibSocket::serverStartStop(bool start) {
    if (start) this->tx("start");
    else this->tx("stop");
}


void gpibSocket::serverGetStatus() {
    this->tx("status");
}

void gpibSocket::serverGetInitialData() {
    this->tx("get all");
}

void gpibSocket::serverGetLatest() {
    this->tx("get latest");
}

void gpibSocket::serverGetWorkplan() {
    this->tx("get workplan");
}

void gpibSocket::serverGetWorkplanIndex() {
    this->tx("get workplan index");
}

void gpibSocket::serverSetWorkplan(QStringList workplan) {
    QString string="set workplan=";
    QString temp;
    bool ok;
    for (int i=0;i<workplan.size();i++) {
        temp=workplan.at(i);
        temp=temp.trimmed();
        temp.toDouble(&ok);
        if (ok) {
            string.append(temp+" ");
        }
    }

    this->tx(string.trimmed().toAscii());
}

void gpibSocket::serverSetMonitorMode(bool mode) {
    if (mode) {
    this->tx("monitor on");
} else {
    this->tx("monitor off");
}

}

void gpibSocket::serverSetWorkplanIndex(int index) {
    this->tx("set workplan index="+QByteArray::number(index));
}



void gpibSocket::rx() {
    QByteArray datagram;
    while (this->hasPendingDatagrams()) {
        this->rxBytes+=this->pendingDatagramSize();
        emit serverRxBytes(this->rxBytes);
        datagram.clear();
        datagram.resize(this->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        this->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        datagram=datagram.trimmed();
        if (datagram.startsWith("200 You found me")) {
            emit serverFound(sender);
            continue;
        } else

        if (datagram.startsWith("200 pong")) {
            int msec=this->startPingTime.msecsTo(QTime::currentTime());
	    emit this->serverRtt (msec);
//            w->updateRtt(msec);
            continue;

        } else

        if (datagram.startsWith("200 Running")) {
            this->running=true;
            emit this->serverIdleRunning(true);
            continue;
        } else

        if (datagram.startsWith("200 Idle")) {
            this->running=false;
            emit this->serverIdleRunning(false);
            continue;
        }

        if (datagram.startsWith("200 Interval:")) {
            this->interval=datagram.split('\n').at(1).trimmed().toInt();
            emit this->serverInterval(interval);
            continue;
        }

        if (datagram.startsWith("200 Initial data:")) {
            QString stringDatargram(datagram);
            QStringList idata=stringDatargram.split("\r\n");
            idata.removeLast();
            idata.removeFirst();
            emit this->serverInitialData(idata);
            continue;
        } else

        //FIXME: criteria should be more restrictive
        if (datagram.startsWith("200 Data from ")) {
            QString stringDatargram(datagram);
            QStringList idata=stringDatargram.split("\r\n");
            QString from=idata.first();
            from=from.mid(14);
            from.chop(1);
            idata.removeFirst();
            while (idata.last().isEmpty() && idata.size()>0) {
                idata.removeLast();
            }
            if (idata.size()>0) {
                emit this->serverData(idata,from);
            }

            continue;
        } else

        if (datagram.startsWith("200 Latest data")) {
            QString stringDatagram(datagram);
            QStringList idata=stringDatagram.split("\r\n");
            idata.removeFirst();
            QString data=idata.first();
            emit this->serverLatestData(data);

        } else

        if (datagram.startsWith("204 No new data available")) {
            emit this->serverNoNewData();
            continue;
        } else

        if (datagram.startsWith("403 Remote control is forbidden")) {
            emit this->serverIdleRunning(this->running);
            emit this->serverForbidden(tr("Remote control is forbidden"));
            continue;
        } else

        if (datagram.startsWith("403 Remote control of measuring interval is forbidden")) {
            emit this->serverInterval(this->interval);
            emit this->serverForbidden(tr("Remote control of measuring interval is forbidden"));

            continue;
        } else

            if (datagram.startsWith("200 Workplan index:")) {
            QString stringDatagram(datagram);
            QStringList idata=stringDatagram.split("\r\n");
            idata.removeFirst();
            int index=idata.first().toInt();
            emit this->serverWorkplanIndex(index);
            continue;
        } else

            if (datagram.startsWith("200 Workplan:")) {
            QString stringDatagram(datagram);
            QStringList idata=stringDatagram.split("\r\n");
            idata.removeFirst(); //remove the "200 Workplan:" line from the response
            for (int i=idata.size()-1;i>=0;i--) {
                if (idata.at(i).isEmpty() || idata.at(i).isNull()) {
                    idata.removeAt(i);
                }
            }
            emit this->serverWorkplan(idata);
            continue;
        }

        int i=datagram.indexOf("\n");
        int j=datagram.size();
        if (i==j-2 && datagram.contains(":")) { //if it is just one string
        emit this->serverLatestData(datagram);
        }


    }
}

void gpibSocket::tx(QByteArray datagram,QHostAddress *address) {
    this->txBytes+=datagram.size();
    emit serverTxBytes(this->txBytes);
    if (address==0) {
        this->writeDatagram(datagram,this->serverAddress,GPIB_UDP_PORT);
    } else {
        QHostAddress a(*address);
        this->writeDatagram(datagram,a,GPIB_UDP_PORT);
    }
    this->preLastQuery=this->lastQuery;
    this->lastQuery=datagram;
}

bool gpibSocket::getStatus(void) {
    return this->running;
}


