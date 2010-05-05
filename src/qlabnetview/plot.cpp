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

#include "plot.h"
#include <zoomer.h>
#include "qwt_plot_dict.h"
#include "canvaspicker.h"
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <qwt_plot_marker.h>
#include <QColor>



Plot::~Plot() {
delete panner;
delete magnifier;
}

Plot::Plot(QWidget *parent):
        QwtPlot(parent)
{
    (void) new CanvasPicker(this);
    setAutoReplot(false);
    QwtPlotDict *dict=new QwtPlotDict();
    dict->setAutoDelete(true);
    QSizePolicy policy;
    policy.setHorizontalPolicy(policy.MinimumExpanding);
    policy.setVerticalPolicy(policy.MinimumExpanding);
    this->setSizePolicy(policy);
//    QwtPlotMarker *marker = new QwtPlotMarker();
//    marker->setValue(1,1);
//    marker->setLineStyle(QwtPlotMarker::VLine);
//    QPen pen;
//    pen.setStyle(Qt::SolidLine);
//    pen.setWidthF(2);
//    pen.setColor(Qt::red);
//    marker->setLinePen(pen);
//    marker->attach(this);

    this->setAxisAutoScale(QwtPlot::xBottom);
    this->setAxisAutoScale(QwtPlot::yLeft);

    //legend
//    legend = new QwtLegend;
//    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
//    insertLegend(legend, QwtPlot::BottomLegend);

    panner = new QwtPlotPanner(this->canvas());
    panner->setMouseButton(Qt::MidButton);
    panner->setEnabled(true);

    magnifier = new QwtPlotMagnifier(this->canvas());
    magnifier->setMouseButton(Qt::NoButton);
    magnifier->setWheelFactor(1.2);
    magnifier->setEnabled(true);

    picker = new QwtPlotPicker(
            QwtPlot::xBottom,
            QwtPlot::yLeft,
            //QwtPicker::PointSelection |
            QwtPicker::DragSelection,
            QwtPlotPicker::CrossRubberBand,
            QwtPicker::ActiveOnly,
            this->canvas());
    picker->setRubberBandPen(QColor(Qt::darkGray));
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerPen(QColor(Qt::black));



//     grid
    grid=new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::darkBlue,0,Qt::SolidLine));
    grid->attach(this);

    gridRight=new QwtPlotGrid();
    gridRight->setMajPen(QPen(Qt::darkRed,0,Qt::SolidLine));
    gridRight->setAxis(QwtPlot::xBottom,QwtPlot::yRight);
    gridRight->enableX(false);

}



bool Plot::eventFilter(QObject *object, QEvent *e)
{
return true;
}

void Plot::clear() {
    QwtPlotDict::detachItems(QwtPlotItem::Rtti_PlotCurve,true);
    QwtPlotDict::detachItems(QwtPlotItem::Rtti_PlotMarker,true);
    }
