/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2010  Sergey Popov (aka azure)
Last modification: 6 Oct 2010

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

#ifndef LASTVALUESDIALOG_H
#define LASTVALUESDIALOG_H

#include <QDialog>

namespace Ui {
    class lastValuesDialog;
}

class lastValuesDialog : public QDialog {
    Q_OBJECT
public:
    lastValuesDialog(QWidget *parent = 0);
    ~lastValuesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::lastValuesDialog *ui;
};

#endif // LASTVALUESDIALOG_H
