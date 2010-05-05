/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2009  Sergey Popov (aka azure)
Last modification: 26 Oct 2009

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

#include "qwt_plot.h"
#include "qwt_plot_panner.h"
#ifndef PLOT_H
#define PLOT_H

class Zoomer;
class QwtPlotGrid;

class QwtPlotMagnifier;
class QwtLegend;
class QwtPlotPicker;
class QwtPlotMarker;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget *parent = NULL);
    ~Plot();
    virtual bool eventFilter(QObject *, QEvent *);
    Zoomer *zoomer;
    Zoomer *zoomerRight;
    QwtPlotPanner *panner;
    QwtPlotPicker *picker;
    QwtPlotMagnifier *magnifier;
    QwtLegend *legend;
    QwtPlotGrid *grid, *gridRight;
    QList<QwtPlotMarker *> markerList;

public slots:
    void clear();
};




#endif // PLOT_H
