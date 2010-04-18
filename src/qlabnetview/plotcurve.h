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

#ifndef PLOTCURVE_H
#define PLOTCURVE_H
#include "qwt_plot_curve.h"
class GpibData;

class PlotCurve : public QwtPlotCurve
{
public:
    PlotCurve(int xColumnId, int yColumnId, GpibData *parent);
    void setParent(GpibData* parent);
    int getXColumn(void);
    int getYColumn(void);
    void setXColumn(int columnId);
    void setYColumn(int columnId);

private:
    GpibData* parent;
    int xColumn;
    int yColumn;
};

#endif // PLOTCURVE_H
