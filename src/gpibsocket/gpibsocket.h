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

#ifndef GPIBSOCKET_H
#define GPIBSOCKET_H
#include <QStringList>
#include <QUdpSocket>
#include <QDateTime>
class MainWindow;


class gpibSocket:public QUdpSocket
{
    Q_OBJECT
public:
    gpibSocket(MainWindow *parent);

    ~gpibSocket();
    QHostAddress getServerAddress(void);
    void setServerAddress(QHostAddress address);
    bool getStatus(void);


private:
    QHostAddress serverAddress;
    MainWindow *w;
    int interval;       //current query interval
    QTime startPingTime;    //time mark to identify the round trip time (rtt)
    QByteArray lastQuery;
    QByteArray preLastQuery;
    quint32 rxBytes;    //count of recieved bytes
    quint32 txBytes;    //count of bytes sent
    bool running;

public slots:
    void ping (void);       //ping current server. when "pong" is come, round trip time (rtt) is caluclated and serverRtt signal emitted
    void scan (void);       //scan for available servers
    int getInterval(void);
    void serverStartStop (bool start); //tells server to start (if true) or stop (if false)
    void serverGetInterval(void); //asks for server interval
    void serverGetStatus(void); //asks server status (Idle or Running)
    void serverSetInterval(int msec); //sets server interval
    void serverGetInitialData();        //get sata from the start
    void serverGetLatest(); //get latest data
    void serverGetWorkplan(); //get the workplan
    void serverSetWorkplan(QStringList workplan);
    void serverGetWorkplanIndex(); //get the workplan index
    void serverSetWorkplanIndex(int i);
    void rx(void); //this function recieves incoming data, updating the rxBytes counter
    void tx(QByteArray data,QHostAddress *address=0); //this function transmits data, updating the txBytes counter


signals:
    void serverFound (QHostAddress);    //emitted when a server said "200 You found me"
    void serverRtt (int msec);          //emitted when "200 pong" reply arrived
    void serverRxBytes(quint32 rxbytes);
    void serverTxBytes(quint32 txBytes);
    void serverInterval(int msec);      //emitted when "200 Interval:\r\n12345" arrived. this should update querying interval
    void serverIdleRunning (bool running);
    void serverInitialData (QStringList head);
    void serverData (QStringList data,QString from);
    void serverLatestData (QString data);
    void serverNoNewData();
    void serverForbidden(QString message);
    //void serverIntervalControlForbidden(int interval);
    void serverWorkplanIndex(int workplanIndex);
    void serverWorkplan(QStringList workplan);


};

#endif // GPIBSOCKET_H
