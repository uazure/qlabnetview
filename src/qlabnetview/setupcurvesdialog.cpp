#include "setupcurvesdialog.h"
#include "ui_setupcurvesdialog.h"

setupCurvesDialog::setupCurvesDialog(
        QStringList head, int mode) :
    QDialog(),
    ui(new Ui::setupCurvesDialog)
{
    ui->setupUi(this);
    ui->checkBox->setEnabled(false);
    for (int i = 0 ; i< head.size(); i++) {
        ui->plainTextEdit->appendPlainText(head.at(i));
        //scroll the widget to beginning
    }
    ui->plainTextEdit->scroll(0,-1024);
}

setupCurvesDialog::~setupCurvesDialog()
{
    delete ui;
}

void setupCurvesDialog::changeEvent(QEvent *e)
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
