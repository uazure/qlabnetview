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
  \brief this is the class containing a 2-dimension dynamic array of double and providing model for tableview

  The 2d dynamic array is stored as QList<QList<double> >. array.at(0) - is the pointer to the first _column_ of data (time in h:mm:ss format)

  This class provides a model interface, so data can be easily represented as table

  Every column can be easily accessed, this is very useful for realtime update of QwtPlot widget w/o deleting/adding new curves.

  */



#include <QDebug>
#include "gpibdata.h"

gpibData::gpibData()
{

    serviceInfoHeaterPowerColumn=-1;
    serviceInfoHeLevelColumn=-1;
    //columnCount=0;

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

QVariant gpibData::data(const QModelIndex &index, int role) const {

    if (ddata.isEmpty() || role!=Qt::DisplayRole) {
        return QVariant();
    }
    QVariant val(QVariant::Invalid);
    if (index.column()<ddata.size() || index.row()<ddata.at(0).size()) {
        val=this->ddata.at(index.column()).at(index.row());
    }
    return val;
}

int gpibData::columnCount(const QModelIndex &parent) const {
    int val=0;
    if (parent.isValid()) return 0;
    val=ddata.size();
    return val;
}

int gpibData::rowCount(const QModelIndex &parent) const {
    int val=0;
    if (parent.isValid()) return 0;
    if (ddata.isEmpty()) return 0;
    val=ddata.at(0).size();
    return val;
}

Qt::ItemFlags gpibData::flags(const QModelIndex &index) const {
    if (!index.isValid())
             return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


double gpibData::timestampToSeconds(double time) {
    /*!
      \brief Converts current timestamp to Seconds from the start of experiment

      */
    if (this->initialTimestamp<1)
        this->initialTimestamp=time;

    return time-initialTimestamp;
}

void gpibData::appendStringToDdata(QString string) {

    /*!
    \brief Every single string with data is processed here.

    We have just some QString in input. We should check if the column count
    in this string is equal to the column count of the very first data portion.
    incoming data MUST have single tab separators ("\t")
      */

    double tmpdouble;
    bool tmpbool=false;

    QStringList strlist=string.split("\t",QString::KeepEmptyParts);

    if (ddata.isEmpty() && strlist.size()>1 ) {
        //this is a _first_ data portion. ddata must be prepared
        //and filled as well
        for (int i=0;i<strlist.size();i++) {
            tmpdouble=strlist.at(i).toDouble(&tmpbool);
            if (tmpbool) {
                //QList<QList<T> > should be filled with QList<T>
                QList<double> tmpdoublelist;
                tmpdoublelist.append(tmpdouble);
                this->ddata.append(tmpdoublelist);
            } else {
                qCritical()<<"Critical: row 0, column"<<i<<"is NaN";
                ddata.clear();
                return;
            }
        }
    } else //it is not the first row
        if (strlist.size()==ddata.size()) {
        for (int i=0;i<ddata.size();i++) {
            ddata[i].append(strlist.at(i).toDouble(&tmpbool));
            if (!tmpbool) {
                qWarning()<<"Warning: row"<<ddata.at(0).size()-1<<", column"<<i
                        << "is NaN";
            }
        }

    } else {
        qCritical()<<"Invalid column count in the incoming data. Expected"
                << ddata.size() <<", got"<<strlist.size();

    }

}

void gpibData::appendAsciiData(QStringList stringList) {
    /*!
      \brief When this slot is called, the list of string with data is processed

      Processing includes next steps
      \list
      \li append of the stringlist to rawAsciiData
      \li exclude empty lines and lines starting with '#', '/' and '*' then append data to measureData
      \li parse the lines starting with * and append em to transitionData
      \li last step is to convert every column in every row to double and add to ddata array
      */

    QStringList *data=&stringList;
    int tmpint;
    this->rawAsciiData.append(stringList);
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


            } else if (columnIndex==columnCount-1) {
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


QStringList gpibData::getRawData() const {
    return this->rawAsciiData;
}

QString gpibData::getLatestTimestamp() const {
    return this->latestTimestamp;
}


void gpibData::reset(void) {
    ddata.clear();
    this->daydiff=0;
}

