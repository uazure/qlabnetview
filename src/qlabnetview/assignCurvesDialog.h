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

#ifndef ASSIGNCURVESDIALOG_H
#define ASSIGNCURVESDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
    class assignCurvesDialog;
}

class assignCurvesDialog : public QDialog {
    Q_OBJECT
public:
    assignCurvesDialog(QStringList head, int mode=MainWindow::modeFile);
    int columns();



    ~assignCurvesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::assignCurvesDialog *ui;
    int columns_count;


private slots:
    void accept();
    void on_comboBoxY2_currentIndexChanged(int index);
    void Y2AxisLabelChanged (QString text);


signals:
    void setColumn(int column,int axis,QString axisLabel,QString curveLabel="");
    void setColumn(int column,int serviceInfo,double low_limit=0,double high_limit=0);


};

#endif // ASSIGNCURVESDIALOG_H
