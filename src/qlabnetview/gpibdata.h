/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2009  Sergey Popov (aka azure)
Last modification: 15 Dec 2009

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

#ifndef GPIBDATA_H
#define GPIBDATA_H
#include <QStringList>
#include <QString>
#include <QList>
#include <QVector>
#include <QTime>

class gpibData:public QObject
{
    Q_OBJECT;
public:
    gpibData();
    ~gpibData();
    QStringList rawAsciiData;
    QStringList measureData;
    QStringList transitionData;
    QString latestTimestamp;
    QList<QList<double> > ddata;
    gpibDataLayout dataLayout;
    int measureDataXcolumn;
    int measureDataY1column;
    int measureDataY2column;
    int serviceInfoHeLevelColumn; //this int contains column in data source containing the He level values
    int serviceInfoHeaterPowerColumn; //this int contains column in data source containing the heater power level values
    QString measureDataY1Label;
    QString measureDataY2Label;
    int columnCount;


    QTime initialTime;
    double initialTimestamp;
    qint32 daydiff;

    QRegExp delimiterRegExp;

private:
    int timeToSeconds(QTime time);
    double timestampToSeconds(double time);
    void appendStringToDdata(QString string);

public slots:
    void appendAsciiData(QStringList string);



};

class gpibDataLayout:public QObject {
    Q_OBJECT;

public:
    gpibDataLayout();
    ~gpibDataLayout();
    //getters
    int getX() const;
    QList<int> getY(bool Y2axis=false) const;
    //setters
    void setX(int columnIndex);
    void appendY(int columnIndex,bool Y2axis=false);
    void appendY(QList<int> columnIndex,bool Y2axis=false);
    void clear(void);

private:
    int X; //index of the X data column in gpibData
    QList<int> Y1; //List of indexes of the Y values (left axis)
    QList<int> Y2; //Same for the right axis

};

#endif // GPIBDATA_H
