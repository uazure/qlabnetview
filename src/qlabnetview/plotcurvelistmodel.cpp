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

#include "plotcurvelistmodel.h"

PlotCurveListModel::PlotCurveListModel(QList<PlotCurve *> *parentCurveList) :
    QAbstractListModel()
{
    this->curveList=parentCurveList;

}

int PlotCurveListModel::rowCount(const QModelIndex &parent) const {
    return curveList->count();

}

QVariant PlotCurveListModel::data(const QModelIndex &index, int role) const {
    QVariant val;
    val.setValue(curveList->at(index.row())->getName());
    qDebug()<<"PlotCurve name at index"<<index.row()<<"is"<<val.toString();
    return val;
}

bool PlotCurveListModel::insertRow(int row, const QModelIndex &parent) {

    return true;
}

void PlotCurveListModel::update(PlotCurve* curve) {
//    //beginLayoutChanged();
//    emit this->layoutAboutToBeChanged();
//    curveList->append(curve);
//    changePersistentIndex(index(0,0),index(curveList->size(),0));
//    //emit this->dataChanged(index(0,0),index(curveList->size()-1,0));
//
//    //endLayoutChanged();
//    emit this->layoutChanged();
}
