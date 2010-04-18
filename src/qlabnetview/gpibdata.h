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
#include <QAbstractTableModel>
#include <QStringList>
#include <QString>
#include <QTime>
#include <QList>
#include <QVector>


class GpibData:public QAbstractTableModel
{
    Q_OBJECT;
public:
    GpibData();
    ~GpibData();
    //this section is for TableModel
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const ;
    int columnCount(const QModelIndex &parent=QModelIndex()) const ;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //this section is public getters
    QStringList getRawData() const;
    const QStringList getMeasureData(void) const;
    QString getLatestTimestamp() const;

    const double* getColumnData(int columnId) const;


    int serviceInfoHeLevelColumn; //this int contains column in data source containing the He level values
    int serviceInfoHeaterPowerColumn; //this int contains column in data source containing the heater power level values



    QRegExp delimiterRegExp;

protected:
    QVector<QVector<double> > ddata;

private:
    QStringList rawAsciiData;
    QStringList measureData;
    QStringList transitionData;
    QString latestTimestamp;
    QTime initialTime;
    double initialTimestamp;
    qint32 daydiff;

    int timeToSeconds(QTime time);
    double timestampToSeconds(double time);
    void appendStringToDdata(QString string);

public slots:
    void appendAsciiData(QStringList string);
    void reset(void);

};


#endif // GPIBDATA_H
