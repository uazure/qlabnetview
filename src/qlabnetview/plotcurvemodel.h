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

#ifndef PlotCurveModel_H
#define PlotCurveModel_H

#include <QObject>
#include <QList>
#include <QModelIndex>
#include <QAbstractListModel>
#include "plotcurve.h"


/*!
Model for the list of PlotCurve objects.
It can be used to fill ListView
  \sa PlotCurve
  \param QList<PlotCurve *> *
  this is the list of the PlotCurve pointers.
  */
class PlotCurveModel : public QAbstractListModel
{
Q_OBJECT
public:
    explicit PlotCurveModel(QList<PlotCurve *> *);
    int rowCount(const QModelIndex & parent = QModelIndex() ) const;
    QVariant data (const QModelIndex & index, int role=Qt::DisplayRole) const ;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData ( const QModelIndex & index,
                                       const QVariant & value, int role = Qt::EditRole );



private:
    QList<PlotCurve *> *curveList;

signals:
    //void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

public slots:
    void appendCurve (PlotCurve*);

};

#endif // PlotCurveModel_H
