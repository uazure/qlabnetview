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
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <QColor>


Plot::~Plot() {

}

Plot::Plot(QWidget *parent):
        QwtPlot(parent)
{
    setAutoReplot(false);
    QwtPlotDict *dict=new QwtPlotDict();
    dict->setAutoDelete(true);

    this->setAxisAutoScale(QwtPlot::xBottom);
    this->setAxisAutoScale(QwtPlot::yLeft);


    //legend
    legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    insertLegend(legend, QwtPlot::BottomLegend);

    panner = new QwtPlotPanner(this->canvas());
    panner->setMouseButton(Qt::MidButton);
    panner->setEnabled(true);

    magnifier = new QwtPlotMagnifier(this->canvas());
    magnifier->setMouseButton(Qt::NoButton);
    magnifier->setWheelFactor(1.1);
    magnifier->setEnabled(true);

    picker = new QwtPlotPicker(
            QwtPlot::xBottom,
            QwtPlot::yLeft,
            QwtPicker::PointSelection | QwtPicker::DragSelection,
            QwtPlotPicker::CrossRubberBand,
            QwtPicker::ActiveOnly,
            this->canvas());
    picker->setRubberBandPen(QColor(Qt::darkGray));
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerPen(QColor(Qt::black));



//     grid
    grid=new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::darkBlue,0,Qt::DashLine));
    grid->attach(this);

    gridRight=new QwtPlotGrid();
    gridRight->setMajPen(QPen(Qt::darkRed,0,Qt::DashLine));
    gridRight->setAxis(QwtPlot::xBottom,QwtPlot::yRight);
    gridRight->enableX(false);





}



bool Plot::eventFilter(QObject *object, QEvent *e)
{
return true;
}



long Plot::insertCurve(double *x_data,double *y_data,long points_count,bool rightAxis,QString label) {

 QwtPlotCurve *curve = new QwtPlotCurve(label);


    curve->setTitle(label);
    if (!rightAxis) {
        QColor c(Qt::blue);
        curve->setPen(c);
        curve->setSymbol(QwtSymbol(QwtSymbol::Ellipse, c, c, QSize(5, 5)));

    } else {
    this->enableAxis(QwtPlot::yRight);
    this->setAxisAutoScale(QwtPlot::yRight);
    gridRight->attach(this);

        QColor c(Qt::red);
        curve->setPen(c);
        curve->setYAxis(QwtPlot::yRight);
        curve->setSymbol(QwtSymbol(QwtSymbol::Triangle, c, c, QSize(5, 5)));
    }


    //curve->setRenderHint(QwtPlotItem::RenderAntialiased);

    curve->setData(x_data, y_data, points_count);
    curve->attach(this);
    //delete curve;

    int tmpint;

    tmpint=QwtPlotDict::itemList().count();

    QList<QwtPlotItem*> itemList;
    itemList=QwtPlotDict::itemList();

return tmpint;
}

void Plot::insertTransitionPoints(double *x_data,double *y_data,long points_count,bool rightAxis,QString label) {


QwtPlotCurve *curve = new QwtPlotCurve();

    //curve->setTitle(label);
        QColor c(Qt::white);
        QColor d(Qt::black);
        curve->setStyle(QwtPlotCurve::NoCurve);
        curve->setSymbol(QwtSymbol(QwtSymbol::Diamond, c, d, QSize(9, 11)));

        if (rightAxis) {
        curve->setYAxis(QwtPlot::yRight);
    }

    //curve->setRenderHint(QwtPlotItem::RenderAntialiased);

    curve->setData(x_data, y_data, points_count);
    curve->attach(this);

}

void Plot::clear() {
    QwtPlotDict::detachItems(QwtPlotItem::Rtti_PlotCurve,true);
    }
