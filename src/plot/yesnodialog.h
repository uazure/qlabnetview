/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2009  Sergey Popov (aka azure)
Last modification: 24 Nov 2009

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

#ifndef YESNODIALOG_H
#define YESNODIALOG_H

#include <QDialog>

namespace Ui {
    class yesNoDialog;
}

class yesNoDialog : public QDialog {
    Q_OBJECT
public:
    yesNoDialog(QWidget *parent = 0);
    ~yesNoDialog();
    void setMessage(QString);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::yesNoDialog *ui;
};

#endif // YESNODIALOG_H
