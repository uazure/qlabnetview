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

/*!
  \brief gpibData - is the object containing a 2-dimension dynamic array of double.

  This array is stored as QList<QList<double> >.
  gpibData array = new gpibData();
  array[0] - is the pointer to the first _column_ of data (time in h:mm:ss format)
  array->size() - is the number of columns.
  array[0]->size() - is the number of rows.
  Since that, every single column data can be easily accessed. This is very useful
  for realtime update of QwtPlot widget w/o deleting/adding new curves.
  This is also needed for the gpibTableModel
  */


#include "gpibdata.h"
#include <QTime>
#include <QDebug>

gpibData::gpibData()
{

    measureDataXcolumn=-1;
    measureDataY1column=-1;
    measureDataY2column=-1;
    serviceInfoHeaterPowerColumn=-1;
    serviceInfoHeLevelColumn=-1;
    columnCount=0;

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
    double tmpdouble;
    bool tmpbool=false;

    //here the every incoming data must be processed.
    //We have just some QString in input. We should check if the column count
    //in this string is equal to the column count of the very first data portion.
    //incoming data MUST have single tab separators ("\t")
    QStringList strlist=string.split("\t",QString::KeepEmptyParts);

    //
    if ((this->columnCount<=0 || ddata.size()==0 ) && strlist.size()>1 ) {
      //this is a _first_ data portion. ddata must be prepared and filled as well
        this->columnCount=strlist.size();
        for (int i=0;i<strlist.size();i++) {
            tmpdouble=strlist.at(i).toDouble(&tmpbool);
            if (tmpbool) {
                QList<double> tmpdoublelist;
                tmpdoublelist.append(tmpdouble);
                this->ddata.append(tmpdoublelist);
            } else {
                qCritical()<<"Critical: row 0, column"<<i<<"is NaN";
                ddata.clear();
                this->columnCount=0;
                return;
            }
        }
    } else if (strlist.size()==this->columnCount){
        for (int i=0;i<this->columnCount;i++) {
            this->ddata[i].append(strlist.at(i).toDouble(&tmpbool));
            if (!tmpbool) {
                qWarning()<<"Warning: row"<<ddata.at(0).size()-1<<", column"<<i
                        << "is NaN";
            }
        }

    }

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



gpibDataLayout::gpibDataLayout() {
    clear();

}

gpibDataLayout::~gpibDataLayout() {

}

void gpibDataLayout::clear() {
    X=-1;
    Y1.clear();
    Y2.clear();
}

int gpibDataLayout::getX() const {
    return X;
}

QList<int> gpibDataLayout::getY(bool Y2axis) const {
    if (!Y2axis) return Y1;
    else return Y2;
}

void gpibDataLayout::setX(int columnIndex) {
    X=columnIndex;
}

void gpibDataLayout::appendY(int columnIndex, bool Y2axis) {
    if (!Y2axis) {
        Y1.append(columnIndex);
    } else {
        Y2.append(columnIndex);
    }
}

void gpibDataLayout::appendY(QList<int>columnIndex, bool Y2axis) {
    if (!Y2axis) {
        Y1.append(columnIndex);
    } else {
        Y2.append(columnIndex);
    }
}



