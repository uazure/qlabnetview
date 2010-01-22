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




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //setup ui described in mainwindow.ui
    ui->setupUi(this);
    gpibQueryInterval = new QTimer(this);
    gpibQueryInterval->setSingleShot(false);
    columnDelimiter=QRegExp::QRegExp("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row}

    gpib = new gpibSocket(this);

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
    connect(this->gpibQueryInterval,SIGNAL(timeout()),SLOT(gpibFetchData()));
    connect(gpib,SIGNAL(serverNoNewData()),SLOT(gpibNoNewData()));


this->delimiter=" ";
this->initialTimestamp=0;
this->burstUpdate=false;
this->showBrowseNetworkDialog();
}


MainWindow::~MainWindow()
{
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
QString st;
double d;
tmp.removeFirst();
tmp.removeLast();
if (tmp.count()>0) {

st=tmp.at(0);
d=st.toDouble();
ui->meterLabel1->setNum(d);
}

if (tmp.count()>1) {
    st=tmp.at(1);
d=st.toDouble();
ui->meterLabel2->setNum(d);
}

if (tmp.count()>2) {
    st=tmp.at(2);
d=st.toDouble();
ui->meterLabel3->setNum(d);
}

if (tmp.count()>3) {
    st=tmp.at(3);
d=st.toDouble();
ui->meterLabel4->setNum(d);
}

}


void MainWindow::gpibFetchData() {
gpib->serverGetLatest();

}

void MainWindow::updateInterval (int msec) {
    this->gpibQueryInterval->setInterval(msec);
}


void MainWindow::gpibNoNewData(void) {
    this->burstUpdate=false;
    gpib->serverGetInterval();
}
