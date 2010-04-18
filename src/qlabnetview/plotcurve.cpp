/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2010  Sergey Popov (aka azure)
Last modification: 18 Apr 2010

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

#include "plotcurve.h"
#include "gpibdata.h"
#include <QDebug>

PlotCurve::PlotCurve(int xColumnId, int yColumnId, GpibData *parent)
{
    if (parent)
        this->setParent(parent);
    else {
        qCritical()<<"parent of GpibData does not exist";
        return;
}
    if (xColumnId>=0 && xColumnId<parent->columnCount())
        this->xColumn=xColumnId;
    else qCritical()<<"Error while constructing PlotCurve: xColumnId="<<xColumnId;

    if (yColumnId>=0 && yColumnId<parent->columnCount())
        this->yColumn=yColumnId;
    else qCritical()<<"Error while constructing PlotCurve: yColumnId="<<yColumnId;

    this->setRawData(parent->getColumnData(xColumn),parent->getColumnData(yColumn),parent->rowCount());

}

void PlotCurve::setParent(GpibData *parent) {
    this->parent=parent;
}

void PlotCurve::setXColumn(int columnId) {
    if (columnId<0) {
        qCritical()<<"Trying to set negative value as columnId";
        return;
    }
    if (parent && columnId>=parent->columnCount())
        this->xColumn=columnId;
    else
        qCritical()<<"Either parent GpibData is not set or columnId is greater than maximum value";
}

void PlotCurve::setYColumn(int columnId) {
    if (columnId<0) {
        qCritical()<<"Trying to set negative value as columnId";
        return;
    }

    if (parent && columnId>=parent->columnCount())
        this->yColumn=columnId;
    else
        qCritical()<<"Either parent GpibData is not set or columnId is greater than maximum value";
}

int PlotCurve::getXColumn(void) {
    return this->xColumn;
}

int PlotCurve::getYColumn(void) {
    return this->yColumn;
}
