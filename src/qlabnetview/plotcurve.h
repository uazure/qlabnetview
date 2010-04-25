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
class QString;

class PlotCurve : public QObject, public QwtPlotCurve
{

    Q_OBJECT

public:
    PlotCurve(int xColumnId, int yColumnId, GpibData *parent);
    ~PlotCurve();
    /*!
      returns the index of the column assosiated with X in GpibData object
      */
    int getXColumn(void);
    /*!
      returns the index of the column assosiated with Y in GpibData object
      */
    int getYColumn(void);

    /*!
      sets the index of the column assosiated with the X
      */
    void setXColumn(int columnId);
    /*!
      sets the index of the column assosiated with the Y
      */
    void setYColumn(int columnId);
    /*!
      returns the title (name) of the curve

      \sa setName
      */
    QString getName(void);


private:
    GpibData* gpibData;
    int xColumn;
    int yColumn;

    /*!
      This is the curve title. It is displayed in plot legend.
      */
    QString name;

    /*!
      This is the coefficient that is applied to data
      to show the correct values on the plot.

      If the multiplier is not equals to 1 then the GpibData will be
      recalculated and _copied_ to curve data. This operation takes more time
      than just referencing to GpibData values.
      */
    double multiplier;

public slots:
    /*!
      Updates the plot data to the actual data in GpibData
      */
    void update (void);
    /*!
      Sets the title of the curve. It is displayed in plot legend
      \sa getName
      */
    void setName(QString name);
};

#endif // PLOTCURVE_H
