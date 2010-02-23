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

#include "setcurvesdialog.h"
#include <QStringListModel>
#include "ui_setcurvesdialog.h"

setCurvesDialog::setCurvesDialog(QStringList head) :
    QDialog(),
    ui(new Ui::setCurvesDialog)
{
    ui->setupUi(this);
    connect(ui->Y1AddColumnButton,SIGNAL(clicked()),SLOT(appendY1Column()));
    connect(ui->Y2AddColumnButton,SIGNAL(clicked()),SLOT(appendY2Column()));

    //columnList=new QStringList;
    for (int i=0;i<head.size();i++) {
        if (head.at(i).startsWith("#") || head.at(i).startsWith("*"))
            continue;
        else {
            QRegExp regexp("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row})
            columnList=head.at(i).split(regexp,QString::SkipEmptyParts);
            break;
        }
    }
    model = new QStringListModel(columnList);
    Y1model = new QStringListModel(Y1columnList);
    Y2model = new QStringListModel (Y2columnList);
    ui->AllColumnsListView->setModel(model);
    ui->Y1ListView->setModel(Y1model);
    ui->Y2ListView->setModel(Y2model);
    ui->XComboBox->addItems(columnList);

}

setCurvesDialog::~setCurvesDialog()
{
    delete ui;
    delete model;
    delete Y1model;
    delete Y2model;

}

void setCurvesDialog::appendY1Column() {
    this->appendYColumn(false);
}

void setCurvesDialog::appendY2Column() {
    this->appendYColumn(true);
}

void setCurvesDialog::appendYColumn(bool Y2) {
    if (ui->AllColumnsListView->currentIndex().isValid()) {
        if (!Y2) {
            Y1columnList.append(this->columnList.at(ui->AllColumnsListView->currentIndex().row()));
            Y1model->revert();
            } else {
            Y2columnList.append(this->columnList.at(ui->AllColumnsListView->currentIndex().row()));
        }

    }
}

void setCurvesDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
