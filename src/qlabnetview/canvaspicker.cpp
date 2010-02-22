#include <qapplication.h>
#include <qevent.h>
#include <qwhatsthis.h>
#include <qpainter.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>
#include <qwt_scale_map.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include "canvaspicker.h"

CanvasPicker::CanvasPicker(QwtPlot *plot):
    QObject(plot),
    d_selectedCurve(NULL),
    d_selectedPoint(-1)
{
    QwtPlotCanvas *canvas = plot->canvas();

    canvas->installEventFilter(this);

    // We want the focus, but no focus rect. The
    // selected point will be highlighted instead.


    canvas->setFocusPolicy(Qt::StrongFocus);
#ifndef QT_NO_CURSOR
    canvas->setCursor(Qt::PointingHandCursor);
#endif
    canvas->setFocusIndicator(QwtPlotCanvas::ItemFocusIndicator);
    canvas->setFocus();

    shiftCurveCursor(true);
}

bool CanvasPicker::event(QEvent *e)
{
    if ( e->type() == QEvent::User )
    {
        showCursor(true);
        return true;
    }
    return QObject::event(e);
}

bool CanvasPicker::eventFilter(QObject *object, QEvent *e)
{
    if ( object != (QObject *)plot()->canvas() )
        return false;

    switch(e->type())
    {
        case QEvent::FocusIn:
            showCursor(true);
        case QEvent::FocusOut:
            showCursor(false);


        case QEvent::Paint:
        {   
            QApplication::postEvent(this, new QEvent(QEvent::User));
            break;
        }
        case QEvent::MouseButtonPress:
        {
            select(((QMouseEvent *)e)->pos());
            return true; 
        }
//        case QEvent::MouseMove:
//        {
//            move(((QMouseEvent *)e)->pos());
//            return true;
//        }
//        case QEvent::KeyPress:
//        {
//
//            switch(((const QKeyEvent *)e)->key())
//            {
//                case Qt::Key_Up:
//                    shiftCurveCursor(true);
//                    return true;
//
//                case Qt::Key_Down:
//                    shiftCurveCursor(false);
//                    return true;
//
//                case Qt::Key_Right:
//                case Qt::Key_Plus:
//                    if ( d_selectedCurve )
//                        shiftPointCursor(true);
//                    else
//                        shiftCurveCursor(true);
//                    return true;
//
//                case Qt::Key_Left:
//                case Qt::Key_Minus:
//                    if ( d_selectedCurve )
//                        shiftPointCursor(false);
//                    else
//                        shiftCurveCursor(true);
//                    return true;
//                default:
//                    break;
//            }
//        }
        default:
            break;
    }
    return QObject::eventFilter(object, e);
}

// Select the point at a position. If there is no point
// deselect the selected point

void CanvasPicker::select(const QPoint &pos)
{
    QwtPlotCurve *curve = NULL;
    double dist = 10e10;
    int index = -1;

    const QwtPlotItemList& itmList = plot()->itemList();
    for ( QwtPlotItemIterator it = itmList.begin();
        it != itmList.end(); ++it )
    {
        if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve )
        {
            QwtPlotCurve *c = (QwtPlotCurve*)(*it);

            double d;
            int idx = c->closestPoint(pos, &d);
            if ( d < dist )
            {
                curve = c;
                index = idx;
                dist = d;
            } 
        }
    }

    showCursor(false);
    d_selectedCurve = NULL;
    d_selectedPoint = -1;

    if ( curve && dist < 8 ) // 10 pixels tolerance
    {
        d_selectedCurve = curve;
        d_selectedPoint = index;
        showCursor(true);

    }
    this->plot()->replot();
}

// Move the selected point
void CanvasPicker::move(const QPoint &pos)
{
    if ( !d_selectedCurve )
        return;

    QwtArray<double> xData(d_selectedCurve->dataSize());
    QwtArray<double> yData(d_selectedCurve->dataSize());

    for ( int i = 0; i < d_selectedCurve->dataSize(); i++ )
    {
        if ( i == d_selectedPoint )
        {
            xData[i] = plot()->invTransform(d_selectedCurve->xAxis(), pos.x());;
            yData[i] = plot()->invTransform(d_selectedCurve->yAxis(), pos.y());;
        }
        else
        {
            xData[i] = d_selectedCurve->x(i);
            yData[i] = d_selectedCurve->y(i);
        }
    }
    d_selectedCurve->setData(xData, yData);

    plot()->replot();
    showCursor(true);
}

// Hightlight the selected point
void CanvasPicker::showCursor(bool showIt)
{
    if ( !d_selectedCurve )
        return;

    const QwtSymbol symbol = d_selectedCurve->symbol();

    QwtSymbol newSymbol = symbol;
    if ( showIt ) {
        int w=symbol.size().width();
        int h=symbol.size().height();
        newSymbol.setSize(w+2,h+2);
        newSymbol.setBrush(symbol.brush().color().darker());
        newSymbol.setPen(symbol.pen().color().darker());
    }

    const bool doReplot = plot()->autoReplot();

    plot()->setAutoReplot(false);
    d_selectedCurve->setSymbol(newSymbol);

    d_selectedCurve->draw(d_selectedPoint, d_selectedPoint);

    d_selectedCurve->setSymbol(symbol);
    plot()->setAutoReplot(doReplot);

}

// Select the next/previous curve 
void CanvasPicker::shiftCurveCursor(bool up)
{
    QwtPlotItemIterator it;

    const QwtPlotItemList &itemList = plot()->itemList();

    QwtPlotItemList curveList;
    for ( it = itemList.begin(); it != itemList.end(); ++it )
    {
        if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve )
            curveList += *it;
    }
    if ( curveList.isEmpty() )
        return;

    it = curveList.begin();

    if ( d_selectedCurve )
    {
        for ( it = curveList.begin(); it != curveList.end(); ++it )
        {
            if ( d_selectedCurve == *it )
                break;
        }
        if ( it == curveList.end() ) // not found
            it = curveList.begin();

        if ( up )
        {
            ++it;
            if ( it == curveList.end() )
                it = curveList.begin();
        }
        else
        {
            if ( it == curveList.begin() )
                it = curveList.end();
            --it;
        }
    }
        
    showCursor(false);
    d_selectedPoint = 0;
    d_selectedCurve = (QwtPlotCurve *)*it;
    showCursor(true);
}

// Select the next/previous neighbour of the selected point
void CanvasPicker::shiftPointCursor(bool up)
{
    if ( !d_selectedCurve )
        return;

    int index = d_selectedPoint + (up ? 1 : -1);
    index = (index + d_selectedCurve->dataSize()) % d_selectedCurve->dataSize();

    if ( index != d_selectedPoint )
    {
        showCursor(false);
        d_selectedPoint = index;
        showCursor(true);
    }
}
