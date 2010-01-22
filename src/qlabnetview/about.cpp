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

#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) : QDialog(parent),  m_ui(new Ui::About)
{

    m_ui->setupUi(this);
        setWindowTitle(tr("About")+" "+QApplication::applicationName());
    connect (m_ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));
    m_ui->name_and_version_label->setText(QApplication::applicationName()+" (ver. "+QApplication::applicationVersion()+")");

}

About::~About()
{
    delete m_ui;
}

void About::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
