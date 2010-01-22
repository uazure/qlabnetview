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

#include "assignCurvesDialog.h"
#include "ui_assignCurvesDialog.h"
#include <QFile>

assignCurvesDialog::assignCurvesDialog(QStringList head, int mode) :
//    QDialog(parent),
        ui(new Ui::assignCurvesDialog)
{
    ui->setupUi(this);
    connect(ui->Y2CurveLabel,SIGNAL(textEdited(QString)),ui->Y2AxisLabel,SLOT(setText(QString)));
    QByteArray buf;
    QString tmp;
    for (int i = 0 ; i< head.size(); i++) {
        ui->plainTextEdit->appendPlainText(head.at(i));
        //scroll the widget to beginning
    }
    ui->plainTextEdit->scroll(0,-1024);
    QStringList columns;
    for (int i=0; i< head.size(); i++){
        tmp=head.at(i);
        if (! (tmp.startsWith("#") || tmp.startsWith("/") || tmp.startsWith("*"))) {
            QRegExp delimiter("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row}
            columns=tmp.split(delimiter,QString::KeepEmptyParts);
            this->columns_count=columns.count();
            break;
        }

    }
    tmp="(none)";
    ui->comboBoxX->addItem(tmp);
    ui->comboBoxY1->addItem(tmp);
    ui->comboBoxY2->addItem(tmp);
    ui->heLevelComboBox->addItem(tmp);
    ui->heaterPowerComboBox->addItem(tmp);

    for (int i=0;i<columns.count();i++) {
        tmp=QString::number(i+1)+" ("+columns.at(i)+")";
        ui->comboBoxX->addItem(tmp);
        ui->comboBoxY1->addItem(tmp);
        ui->comboBoxY2->addItem(tmp);
        ui->heLevelComboBox->addItem(tmp);
        ui->heaterPowerComboBox->addItem(tmp);
    }
    if (mode==MainWindow::modeFile) {
    ui->comboBoxX->setCurrentIndex(1);
} else {
    ui->comboBoxX->setCurrentIndex(columns.count());
}
    ui->comboBoxY1->setCurrentIndex(2);

}

void assignCurvesDialog::accept() {

    emit this->setColumn(ui->comboBoxX->currentIndex()-1,QwtPlot::xBottom,ui->XAxisLabel->text());
    emit this->setColumn(ui->comboBoxY1->currentIndex()-1,QwtPlot::yLeft,ui->Y1AxisLabel->text(),ui->Y1CurveLabel->text());
    emit this->setColumn(ui->comboBoxY2->currentIndex()-1,QwtPlot::yRight,ui->Y2AxisLabel->text(),ui->Y2CurveLabel->text());
    emit this->setColumn(ui->heaterPowerComboBox->currentIndex()-1,MainWindow::serviceHeater);
    emit this->setColumn(ui->heLevelComboBox->currentIndex()-1,MainWindow::serviceHeLevel);

    this->setResult(QDialog::Accepted);
    this->hide();
}

int assignCurvesDialog::columns() {
return this->columns_count;
}



assignCurvesDialog::~assignCurvesDialog()
{
    delete ui;
}

void assignCurvesDialog::changeEvent(QEvent *e)
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

void assignCurvesDialog::on_comboBoxY2_currentIndexChanged(int index)
{
    if (index<1) {
        ui->Y2AxisLabel->setEnabled(false);
        ui->Y2CurveLabel->setEnabled(false);
    } else {
        ui->Y2AxisLabel->setEnabled(true);
        ui->Y2CurveLabel->setEnabled(true);
    }
}

void assignCurvesDialog::Y2AxisLabelChanged (QString text) {
    ui->Y2CurveLabel->setText(text);
}
