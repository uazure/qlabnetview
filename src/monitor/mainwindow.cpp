#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "networkDialog.h"
#include "errormessagedialog.h"

#include <QIcon>
#include <QToolButton>
#include <QFileDialog>
#include <QStringList>
#include <QTime>
#include <QList>
#include <QAction>
#include <QTimer>
#include "../gpibsocket/gpibsocket.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //setup ui described in mainwindow.ui
    ui->setupUi(this);
    this->mode=this->modePoll;
    gpibQueryInterval = new QTimer(this);
    gpibQueryInterval->setSingleShot(false);
    columnDelimiter=QRegExp::QRegExp("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row}

    gpib = new gpibSocket();

    //call networkDialog when File->Browse network clicked
    connect(ui->actionBrowse_network,SIGNAL(triggered()),this,SLOT(showBrowseNetworkDialog()));
    //close program when File->Exit clicked
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    //show About window when Help->About clicked
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
    //show About Qt when Help->About Qt clicked
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),SLOT(showAboutQt()));
    connect(gpib,SIGNAL(serverLatestData(QString)),this,SLOT(updateCurrentData(QString)));
    connect(gpib,SIGNAL(serverInterval(int)),SLOT(updateInterval(int)));
    connect(gpib,SIGNAL(serverRtt(int)),SLOT(updateRtt(int)));
    connect(gpib,SIGNAL(serverRxBytes(quint32)),SLOT(updateRxBytes(quint32)));
    connect(gpib,SIGNAL(serverTxBytes(quint32)),SLOT(updateTxBytes(quint32)));
    connect(this->gpibQueryInterval,SIGNAL(timeout()),SLOT(gpibFetchData()));
    connect(gpib,SIGNAL(serverNoNewData()),SLOT(gpibNoNewData()));
    connect(ui->radioPoll,SIGNAL(toggled(bool)),SLOT(switchMode(bool)));



this->delimiter=" ";
this->showBrowseNetworkDialog();
}


MainWindow::~MainWindow()
{
    if (this->mode==this->modeMonitor)
        this->gpib->serverSetMonitorMode(false);
    delete gpib;
    delete ui;

}


void MainWindow::showAbout()
{
  About *about = new About();
  about->exec();
  delete about;
}

void MainWindow::showAboutQt() {
    QApplication::aboutQt();
}



void MainWindow::showBrowseNetworkDialog() {
    networkDialog *a = new networkDialog(this,this->gpib);
    if (a->exec()==QDialog::Accepted) {
        if (!QHostAddress(gpib->getServerAddress()).isNull()) {
            this->gpibFetchData();
            this->gpibQueryInterval->start();

        }

    }
    delete a;
}

void MainWindow::datagramToData(QStringList data) {

}


void MainWindow::showErrorMessageDialog(QString message) {
    errorMessageDialog *a=new errorMessageDialog(this);
    a->setMessage(message);
    a->exec();
    delete a;

}

void MainWindow::updateRtt(int msec) {
    ui->rttLabel->setText("rtt: "+QString::number(msec));
}

void MainWindow::updateRxBytes(quint32 rxBytes) {
    ui->rxLabel->setText("Rx: "+QString::number(rxBytes));
}

void MainWindow::updateTxBytes(quint32 txBytes) {
    ui->txLabel->setText("Tx: "+QString::number(txBytes));
}

void MainWindow::updateCurrentData(QString data) {
QStringList tmp=data.split(columnDelimiter);
QString st,suffix; //st - the temp string, suffix - m, k, M etc.
double d;
QStringList columns;
//set the measureDatacolumns to the column count of the first data string
if (this->measureDatacolumns<=0) {
    this->measureDatacolumns=tmp.size();
    for (int i=1;i<=this->measureDatacolumns;i++) {
        columns.append(QString::number(i));
    }
    ui->comboBox1->insertItems(0,columns);
    ui->comboBox2->insertItems(0,columns);
    ui->comboBox3->insertItems(0,columns);
    ui->comboBox4->insertItems(0,columns);
    for (int i=1;i<=this->measureDatacolumns;i++) {
        if (i==2) ui->comboBox1->setCurrentIndex(i-1);
        if (i==3) ui->comboBox2->setCurrentIndex(i-1);
        if (i==4) ui->comboBox3->setCurrentIndex(i-1);
        if (i==5) ui->comboBox4->setCurrentIndex(i-1);
    }
}



if (tmp.count()>0) {
    if (tmp.count()<this->measureDatacolumns) {
        qCritical()<<"Column count in the incoming data differs from the initial values. Got"
                <<tmp.count()<<"expected"<<this->measureDatacolumns;
    }

    bool ok=0;
    /*indian code starts here */
st=tmp.at(ui->comboBox1->currentIndex());
d=st.toDouble(&ok);
if (!ok) {
    qWarning()<<"Failed converting string to double in column"<<ui->comboBox1->currentIndex();
    qWarning()<<"Original string is"<<tmp.at(ui->comboBox1->currentIndex());
    d=0;
}
ui->meterLabel1->setText(this->double2HumanReadableString(d));

//---------------//

st=tmp.at(ui->comboBox2->currentIndex());
d=st.toDouble(&ok);
if (!ok) {
    qWarning()<<"Failed converting string to double in column"<<ui->comboBox2->currentIndex();
    qWarning()<<"Original string is"<<tmp.at(ui->comboBox2->currentIndex());
    d=0;
}

ui->meterLabel2->setText(this->double2HumanReadableString(d));

//---------------//

st=tmp.at(ui->comboBox3->currentIndex());
d=st.toDouble(&ok);
if (!ok) {
    qWarning()<<"Failed converting string to double in column"<<ui->comboBox3->currentIndex();
    qWarning()<<"Original string is"<<tmp.at(ui->comboBox3->currentIndex());
    d=0;
}

ui->meterLabel3->setText(this->double2HumanReadableString(d));

//---------------//

st=tmp.at(ui->comboBox4->currentIndex());
d=st.toDouble(&ok);
if (!ok) {
    qWarning()<<"Failed converting string to double in column"<<ui->comboBox4->currentIndex();
    qWarning()<<"Original string is"<<tmp.at(ui->comboBox4->currentIndex());
    d=0;
}

ui->meterLabel4->setText(this->double2HumanReadableString(d));

/*indian code ends here*/
}


}

QString MainWindow::double2HumanReadableString(double d) {
    QString string;
    QString suffix;
    int precision=3;
    if (qAbs(d)<0.001) {
        d=d*1000000;
        suffix=QChar(0x00B5);
    } else if (qAbs(d)<1) {
        d=d*1000;
        suffix="m";
    }

    if (qAbs(d)>100) precision=1;
    else if (qAbs(d)>10) precision=2;
    else if (qAbs(d)>1) precision=3;
    string=QString::number(d,'f',precision);
    string+=suffix;

    return string;
}


void MainWindow::gpibFetchData() {
gpib->serverGetLatest();

}
void MainWindow::updateInterval (int msec) {
    this->gpibQueryInterval->setInterval(msec);
}


void MainWindow::gpibNoNewData(void) {
    gpib->serverGetInterval();
}

void MainWindow::switchMode(bool b) {
    if (ui->radioMonitor->isChecked()) {
        if (this->mode==this->modePoll) {
            this->gpibQueryInterval->stop();
            this->gpib->serverSetMonitorMode(true);
            this->mode=modeMonitor;
        }
    }
    if (ui->radioPoll->isChecked()) {
        if (this->mode==this->modeMonitor) {
            this->gpib->serverSetMonitorMode(false);
            this->gpib->getInterval();
            this->gpibQueryInterval->start();
            this->mode=modePoll;
        }
    }

}
