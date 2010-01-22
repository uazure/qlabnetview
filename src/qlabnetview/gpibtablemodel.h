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

#ifndef GPIBTABLEMODEL_H
#define GPIBTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

class gpibTableModel : public QAbstractTableModel
{
Q_OBJECT
public:
    explicit gpibTableModel(QObject *parent = 0);

    void setArray(QList<QList <double> > *ddata);

    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const ;
    int columnCount(const QModelIndex &parent=QModelIndex()) const ;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QList<QList <double> > *ddata;


signals:

public slots:



};

#endif // GPIBTABLEMODEL_H
