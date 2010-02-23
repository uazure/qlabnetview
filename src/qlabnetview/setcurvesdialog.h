/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2010  Sergey Popov (aka azure)
Last modification: 22 Feb 2010

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

#ifndef SETCURVESDIALOG_H
#define SETCURVESDIALOG_H

#include <QDialog>

class QStringListModel;
class QStringList;

namespace Ui {
    class setCurvesDialog;
}

class setCurvesDialog : public QDialog {
    Q_OBJECT
public:
    setCurvesDialog(QStringList head);
    ~setCurvesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::setCurvesDialog *ui;
    QStringListModel *model;
    QStringList columnList;
    QStringListModel *Y1model;
    QStringList Y1columnList;
    QStringListModel *Y2model;
    QStringList Y2columnList;

private slots:
    void appendY1Column(void);
    void appendY2Column(void);
    void appendYColumn(bool Y2);



};

#endif // SETCURVESDIALOG_H
