#include "experimentControl.h"
#include "ui_experimentControl.h"
#include "mainwindow.h"
#include "../gpibsocket/gpibsocket.h"
#include "qstringlistmodel.h"
#include "qsettings.h"
#include <QVariant>
#include "yesnodialog.h"
#include "errormessagedialog.h"

experimentControl::experimentControl(MainWindow *parent, gpibSocket *socket) :
    QDialog(parent),
    ui(new Ui::experimentControl)
{

    gpib=socket;
    ui->setupUi(this);
    workplan = new QStringList();
    workplanModel = new QStringListModel();
    //workplanModelIndex=workplanModel->createIndex();
    settings=new QSettings();
    ui->workplanView->setModel(this->workplanModel);

    this->setWindowTitle(tr("Experiment control"));
    connect(gpib,SIGNAL(serverIdleRunning(bool)),this,SLOT(serverIdleRunning(bool)));
    connect(gpib,SIGNAL(serverForbidden(QString)),this,SLOT(serverForbidden(QString)));

    connect(ui->startStopButton,SIGNAL(clicked(bool)),this,SLOT(serverStartStop(bool)));

    connect(gpib,SIGNAL(serverInterval(int)),this,SLOT(serverInterval(int)));
    connect(ui->updateButton,SIGNAL(clicked()),this,SLOT(updateSettings()));
    connect(ui->intervalApplyButton,SIGNAL(clicked()),SLOT(intervalChanged()));
    connect(ui->intervalComboBox,SIGNAL(activated(int)),this,SLOT(userChangedIndex(int)));
    connect(ui->intervalComboBox,SIGNAL(editTextChanged(QString)),this,SLOT(userChangedIndex(QString)));

    connect(ui->addTemperatureStepButton,SIGNAL(clicked()),SLOT(addTemperatureStep()));
    connect(ui->removeTemperatureStepButton,SIGNAL(clicked()),SLOT(removeTemperatureStep()));
    connect(ui->clearTemperatureStepsButton,SIGNAL(clicked()),SLOT(clearTemperatureSteps()));
    connect(gpib,SIGNAL(serverWorkplan(QStringList)),this,SLOT(updateWorkplan(QStringList)));
    connect(gpib,SIGNAL(serverWorkplanIndex(int)),this,SLOT(updateWorkplanIndex(int)));
    connect(ui->lockCheckbox,SIGNAL(clicked()),SLOT(lockWorkplan()));
    connect(ui->nextStepButton,SIGNAL(clicked()),SLOT(nextStep()));
    connect(ui->prevStepButton,SIGNAL(clicked()),SLOT(prevStep()));
    connect(ui->applyWorkplan,SIGNAL(clicked()),SLOT(applyWorkplan()));

    ui->lockCheckbox->click();
    ui->updateButton->click();
}

experimentControl::~experimentControl()
{
    delete ui;
}

void experimentControl::changeEvent(QEvent *e)
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


void experimentControl::serverIdleRunning(bool running) {
ui->startStopButton->setChecked(running);
if (!ui->startStopButton->isEnabled()) {
    ui->startStopButton->setEnabled(true);
}
    if (running) {

        ui->serverStatusPixmap->setPixmap(QPixmap(":/icons/running"));
        ui->startStopButton->setText(tr("Running"));
        ui->startStopButton->setChecked(running);

    } else {
        ui->serverStatusPixmap->setPixmap(QPixmap(":/icons/idle"));
        ui->startStopButton->setText(tr("Idle"));

    }


}

void experimentControl::serverStartStop(bool start) {
    gpib->serverStartStop(start);
}

void experimentControl::intervalChanged() {
    QString interval=ui->intervalComboBox->currentText();
    int msec = int (interval.toFloat()*1000);
    gpib->serverSetInterval(msec);


}

void experimentControl::serverInterval(int msec) {
    ui->intervalComboBox->setEnabled(true);
    ui->intervalApplyButton->setEnabled(true);
    QPalette *p = new QPalette() ;
    p->setColor(QPalette::Text,QColor(Qt::black));
    ui->intervalComboBox->setPalette(*p);


    for (int i=0;i<ui->intervalComboBox->count();i++) {
        if (int (ui->intervalComboBox->itemText(i).toFloat()*1000) == msec) {
            ui->intervalComboBox->setCurrentIndex(i);
            break;
        } else if (int (ui->intervalComboBox->itemText(i).toFloat()*1000) > msec) {
            double t=double (msec)/1000;
            ui->intervalComboBox->insertItem(i,QString::number(t));
            break;
        }

    }
}

void experimentControl::updateSettings(void) {
gpib->serverGetInterval();
gpib->serverGetStatus();
gpib->serverGetWorkplan();
gpib->serverGetWorkplanIndex();
}

void experimentControl::userChangedIndex(int index) {
    this->userChangedIndex();
}
void experimentControl::userChangedIndex(QString str) {
    this->userChangedIndex();
}

void experimentControl::userChangedIndex() {
    ui->intervalApplyButton->setFocus();
    QPalette *p = new QPalette() ;
    p->setColor(QPalette::Text,QColor(Qt::gray));
    ui->intervalComboBox->setPalette(*p);
}


void experimentControl::addTemperatureStep(void) {
    //insert one row to the end of the model
    workplanModel->insertRows(ui->workplanView->currentIndex().row()+1,1);
    //set current index in the model to the latest row
    ui->workplanView->setCurrentIndex(this->workplanModel->index(ui->workplanView->currentIndex().row()+1,0));
    //show editing field on the latest row
    ui->workplanView->edit(workplanModel->index(ui->workplanView->currentIndex().row(),0));
}

void experimentControl::removeTemperatureStep() {
    this->workplanModel->removeRow(ui->workplanView->currentIndex().row());

}

void experimentControl::clearTemperatureSteps() {
    this->workplanModel->removeRows(0,workplanModel->rowCount());
    ui->workplanView->setCurrentIndex(this->workplanModel->index(0,0));
}

void experimentControl::updateWorkplan(QStringList workplan) {
    ui->temperatureControl->setEnabled(true);
    ui->nextStepButton->setEnabled(true);
    this->workplanModel->setStringList(workplan);
}

void experimentControl::updateWorkplanIndex(int index) {
    ui->workplanView->setCurrentIndex(this->workplanModel->index(index,0));
}

void experimentControl::lockWorkplan() {
    ui->applyWorkplan->setEnabled(!ui->lockCheckbox->isChecked());
    ui->workplanView->setEnabled(!ui->lockCheckbox->isChecked());
    ui->addTemperatureStepButton->setEnabled(!ui->lockCheckbox->isChecked());
    ui->removeTemperatureStepButton->setEnabled(!ui->lockCheckbox->isChecked());
    ui->clearTemperatureStepsButton->setEnabled(!ui->lockCheckbox->isChecked());
}

void experimentControl::nextStep() {
    yesNoDialog *dialog= new yesNoDialog(this);
    dialog->setMessage(tr("Are you sure that you want to go next step?"));
    if (dialog->exec()==QDialog::Accepted) {
        gpib->serverSetWorkplanIndex(ui->workplanView->currentIndex().row()+1);
        this->applyWorkplan();
    }
    delete dialog;
}

void experimentControl::prevStep() {
    yesNoDialog *dialog= new yesNoDialog(this);
    dialog->setMessage(tr("Are you sure that you want to go previous step?"));
    if (dialog->exec()==QDialog::Accepted) {
        gpib->serverSetWorkplanIndex(ui->workplanView->currentIndex().row()-1);
        this->applyWorkplan();
    }
    delete dialog;
}

void experimentControl::applyWorkplan() {
    ui->lockCheckbox->setChecked(true);
    this->lockWorkplan();

    gpib->serverSetWorkplan(this->workplanModel->stringList());

    gpib->serverGetWorkplanIndex();
    this->workplanModel->setStringList(QStringList());

}

void experimentControl::serverForbidden(QString message) {
    errorMessageDialog *a=new errorMessageDialog();
    a->setMessage(message);
    a->exec();
    delete a;
}
