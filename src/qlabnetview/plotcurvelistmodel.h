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

#ifndef PLOTCURVELISTMODEL_H
#define PLOTCURVELISTMODEL_H

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
class PlotCurveListModel : public QAbstractListModel
{
Q_OBJECT
public:
    explicit PlotCurveListModel(QList<PlotCurve *> *);
    int rowCount(const QModelIndex & parent = QModelIndex() ) const;
    QVariant data (const QModelIndex & index, int role=Qt::DisplayRole) const ;



private:
    QList<PlotCurve *> *curveList;

signals:

public slots:

};

#endif // PLOTCURVELISTMODEL_H
