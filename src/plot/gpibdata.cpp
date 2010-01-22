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

#include "gpibdata.h"
#include <QTime>

gpibData::gpibData()
{

    measureDataXcolumn=-1;
    measureDataY1column=-1;
    measureDataY2column=-1;
    serviceInfoHeaterPowerColumn=-1;
    serviceInfoHeLevelColumn=-1;
    measureDataColumns=0;

    delimiterRegExp.setPattern("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row}
    this->daydiff=0;
    this->initialTimestamp=0;

}

gpibData::~gpibData() {

}

int gpibData::timeToSeconds(QTime time) {
    if (this->initialTime.isNull()) {
        this->initialTime=time;
        return 0;
    } else {
        qint32 diff=this->initialTime.secsTo(time)+this->daydiff*86400;
        if (diff<0) {
            this->daydiff++;
            diff=initialTime.secsTo(time)+this->daydiff*86400;
        }
        return diff;
    }
}

double gpibData::timestampToSeconds(double time) {
    if (this->initialTimestamp<1)
        this->initialTimestamp=time;

    return time-initialTimestamp;
}

void gpibData::appendStringToDdata(QString string) {
    QList<double> dlist;
    QStringList strlist=string.split("\t");
    for (int i=0;i<strlist.size();i++) {
        dlist.append(strlist.at(i).toDouble());
    }
    this->ddata.append(dlist);
}

void gpibData::appendAsciiData(QStringList string) {
    QStringList *data=&string;
    int tmpint;
    this->rawAsciiData.append(string);
    QTime start_t,current_t;
    QStringList column; //this string list will contain list of columns in current row.
    QString tmpstring, nextline;

    for (int row=0;row<data->size();row++) {
        tmpstring=data->at(row);
        tmpstring=tmpstring.trimmed();

        if (tmpstring.startsWith("#",Qt::CaseSensitive)
            || tmpstring.startsWith("/",Qt::CaseSensitive)
            || tmpstring.trimmed().isEmpty()
             ) {
            continue;
        } else if (tmpstring.startsWith("*",Qt::CaseSensitive) && this->measureData.size()>0) {
            if (this->transitionData.last()!=this->measureData.last()) {
                this->transitionData.append(this->measureData.last());
            }
            continue;
        }


        column=tmpstring.split(this->delimiterRegExp);
        int columnCount=column.count();
        if (row==data->size()-1) {
            //this will update the latestTimestamp to the very last value in incoming data
            this->latestTimestamp=column.last();
        }

        for (int columnIndex=0;columnIndex<columnCount;++columnIndex) {
            tmpstring=column.at(columnIndex);

            //this will check the first row. if it is time in format h:mm:ss,
            //it will update the starting time and calculate difference in seconds.
            if (columnIndex==0 && tmpstring.contains(":")) {
                tmpint=timeToSeconds(QTime::fromString(tmpstring,"h:mm:ss"));
                nextline+=tmpstring.setNum(tmpint);
                continue;


            } else if (columnIndex==columnCount-1 && this->measureDataXcolumn==columnIndex) {
                tmpstring.setNum(this->timestampToSeconds(tmpstring.toDouble()));

            }
            if (columnIndex!=0)
            nextline+="\t"+tmpstring;
            else nextline+=tmpstring;

            if (columnIndex==columnCount-1) {
                this->measureData.append(nextline);
                this->appendStringToDdata(nextline);
                nextline.clear();
            }

        }

        if (row==0 && (this->transitionData.isEmpty())) {
            this->transitionData.append(this->measureData.first());
        }

    }

}
