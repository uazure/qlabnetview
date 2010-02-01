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

#ifndef SERVER_H
#define SERVER_H
#include <QStringList>
#include <QUdpSocket>

class server : public QUdpSocket
{
    Q_OBJECT
public:
    server();
    ~server();
    void processTheDatagram(QByteArray,QHostAddress sender,quint16 senderPort);
    void setInFile(QString filename);
    QByteArray prepareData(int index=0);
    QStringList data;


public slots:
    void readPendingDatagrams(void);
private:
    QString inFile;
    QRegExp delimiterRegExp;
signals:
    void wantAbort();
};

#endif // SERVER_H
