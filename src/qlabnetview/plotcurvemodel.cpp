/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2010  Sergey Popov (aka azure)
Last modification: 26 Apr 2010

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


#include <QDebug>

#include "plotcurvemodel.h"

PlotCurveModel::PlotCurveModel(QList<PlotCurve *> *parentCurveList) :
    QAbstractListModel()
{
    this->curveList=parentCurveList;
}

int PlotCurveModel::rowCount(const QModelIndex &parent) const {
    return curveList->count();
}

QVariant PlotCurveModel::data(const QModelIndex &index, int role) const {

    if (role == Qt::DisplayRole) {
        QVariant val(curveList->at(index.row())->getName());
        //qDebug()<<"PlotCurve name at index"<<index.row()<<"is"<<val.toString();
        return val;
    } else
        return QVariant();
}


void PlotCurveModel::appendCurve(PlotCurve* curve) {
    emit this->layoutAboutToBeChanged();
    curveList->append(curve);
    emit this->layoutChanged();
}

Qt::ItemFlags PlotCurveModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
             return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool PlotCurveModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;
    if (role==Qt::EditRole) {
        PlotCurve *curve=curveList->value(index.row());
        curve->setName(value.toString());
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

void PlotCurveModel::setGpibData(GpibData *data) {
    for (int i=0;i<curveList->size();i++) {
        curveList->value(i)->setGpibData(data);
    }
}
