/**************************************************************************
qlabnetview - is a Qt-based GUI application for experimental data representation and processing
Copyright (C) 2009  Sergey Popov (aka azure)
Last modification: 3 Dec 2009

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

#include "viewrawdata.h"
#include "ui_viewrawdata.h"

viewRawData::viewRawData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewRawData)
{
    ui->setupUi(this);
}

viewRawData::~viewRawData()
{
    delete ui;
}

void viewRawData::changeEvent(QEvent *e)
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

void viewRawData::setText(QStringList text){
    QString t;
    for (quint32 i=0;i<text.size();i++) {
        t+=text.at(i)+"\n";
    }
    ui->plainTextEdit->setPlainText(t);
}
