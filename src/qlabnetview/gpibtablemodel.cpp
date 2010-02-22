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

#include "gpibtablemodel.h"

gpibTableModel::gpibTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    //ddata = new QList<QList <double> >;
}

QVariant gpibTableModel::data(const QModelIndex &index, int role) const {

    if (ddata->isEmpty() || role!=Qt::DisplayRole) {
        return QVariant();
    }
    QVariant val(QVariant::Invalid);
    if (index.column()<ddata->size() || index.row()<ddata->at(0).size()) {
        val=this->ddata->at(index.column()).at(index.row());
    }
    return val;
}

void gpibTableModel::setArray(QList<QList <double> > *datastorage) {
    this->ddata=datastorage;

}

int gpibTableModel::columnCount(const QModelIndex &parent) const {
    int val=0;
    if (parent.isValid()) return 0;
    val=ddata->size();
    return val;
}

int gpibTableModel::rowCount(const QModelIndex &parent) const {
    int val=0;
    if (parent.isValid()) return 0;
    val=ddata->at(0).size();
    return val;
}

Qt::ItemFlags gpibTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
             return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
