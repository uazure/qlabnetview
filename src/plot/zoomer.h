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

#ifndef ZOOMER_H
#define ZOOMER_H

#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_canvas.h>

class Zoomer : public QwtPlotZoomer
{
    Q_OBJECT
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas);
    ~Zoomer();

};

#endif // ZOOMER_H
