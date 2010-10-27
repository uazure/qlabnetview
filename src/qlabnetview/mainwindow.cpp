#include "main.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "networkDialog.h"
#include "zoomer.h"
#include "plot.h"
#include "viewrawdata.h"
#include "viewparseddata.h"
#include "viewtabledata.h"
#include "setupcurvesdialog.h"
#include "errormessagedialog.h"
#include "experimentControl.h"
#include <QIcon>
#include <QToolButton>
#include <QFileDialog>
#include <QStringList>
#include <QTime>
#include <QList>
#include <QAction>
#include <QTimer>
#include <QClipboard>
#include "plotcurve.h"
#include "qwt_legend.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //setup ui described in mainwindow.ui

    this->pdata=new GpibData;
    ui->setupUi(this);
    ui->actionOpen_recent->setStatusTip(settings.value("recentFile","").toString());
    plot = new Plot(this);
    plot->legend->setVisible(true);
    ui->gridLayout->addWidget(plot,0,0);
    this->setDataSourceMode(modeNoop);
    ui->powerGroupBox->setVisible(false);
    ui->networkGroupBox->setVisible(false);
    ui->monitorGroupBox->setVisible(false);

    gpibQueryInterval = new QTimer(this);
    gpibQueryInterval->setSingleShot(false);

    gpib = new gpibSocket();
    //define slot<->signal
    //call fileOpen dialog when File->Open clicked
    connect(ui->actionOpen,SIGNAL(triggered()),SLOT(fileOpen()));
    connect(ui->actionSave,SIGNAL(triggered()),SLOT(fileSave()));
    connect(ui->actionSave_as,SIGNAL(triggered()),SLOT(fileSaveAs()));
    connect(ui->actionOpen_recent,SIGNAL(triggered()),SLOT(fileOpenRecent()));
    //call networkDialog when File->Browse network clicked
    connect(ui->actionBrowse_network,SIGNAL(triggered()),this,SLOT(showBrowseNetworkDialog()));
    //close program when File->Exit clicked
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    //show About window when Help->About clicked
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
    //show About Qt when Help->About Qt clicked
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));

    connect(ui->actionExperiment_control,SIGNAL(triggered()),SLOT(showExperimentControl()));
    connect(ui->actionASCII_data,SIGNAL(triggered()),SLOT(showViewRawData()));
    connect(ui->actionParsed_data,SIGNAL(triggered()),SLOT(showViewParsedData()));
    connect(ui->actionTable_data,SIGNAL(triggered()),SLOT(showViewTableData()));
    connect(ui->actionSetup_curves,SIGNAL(triggered()),SLOT(showSetupCurvesDialog()));


    connect(ui->replotButton,SIGNAL(clicked()),this->plot,SLOT(replot()));
    connect(ui->clearButton,SIGNAL(clicked()),this->plot,SLOT(clear()));
    connect(gpib,SIGNAL(serverInitialData(QStringList)),this,SLOT(updateInitialData(QStringList)));
    connect(gpib,SIGNAL(serverData(QStringList,QString)),this,SLOT(updateCurrentData(QStringList,QString)));
    connect(ui->startStopButton,SIGNAL(toggled(bool)),SLOT(startStopNetworkExchange(bool)));
    connect(gpib,SIGNAL(serverInterval(int)),SLOT(updateInterval(int)));
    connect(this->gpibQueryInterval,SIGNAL(timeout()),SLOT(gpibFetchData()));
    connect(gpib,SIGNAL(serverNoNewData()),SLOT(gpibNoNewData()));
    connect(gpib,SIGNAL(serverIdleRunning(bool)),ui->startStopButton,SLOT(setChecked(bool)));
    connect(ui->zoomAllButton,SIGNAL(clicked()),this,SLOT(zoomAll()));
    connect(ui->zoomButton,SIGNAL(toggled(bool)),SLOT(zoom(bool)));
    connect(ui->monitorPointCountSpinBox,SIGNAL(valueChanged(int)),SLOT(setMonitorPoints(int)));

    //this connects rx and tx counters to signals from gpib socket
    connect(gpib,SIGNAL(serverRxBytes(quint32)),SLOT(updateRxBytes(quint32)));
    connect(gpib,SIGNAL(serverTxBytes(quint32)),SLOT(updateTxBytes(quint32)));
    connect(gpib,SIGNAL(serverRtt(int)),SLOT(updateRtt(int)));




this->delimiter=" ";
this->delimiterRegExp.setPattern("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row}
this->daydiff=0;
this->initialTimestamp=0;
this->burstUpdate=false;
pdata->serviceInfoHeaterPowerColumn=-1;
pdata->serviceInfoHeLevelColumn=-1;
}

MainWindow::~MainWindow()
{
    delete gpib;
    delete plot;
    delete ui;
    delete pdata;
    delete gpibQueryInterval;

}


void MainWindow::showAbout()
{
  About *about = new About();
  about->exec();
  delete about;
}



void MainWindow::showBrowseNetworkDialog() {
    this->resetPlot();
    networkDialog *a = new networkDialog(this,this->gpib);
    if (a->exec()==QDialog::Accepted) {
        if (!QHostAddress(gpib->getServerAddress()).isNull()) {
            this->setDataSourceMode(modeNetwork);
            gpib->serverGetInitialData();
            ui->networkGroupBox->setVisible(true);
            ui->monitorGroupBox->setVisible(true);
        } else {
            this->setDataSourceMode(modeNoop);
        }

    } else {
        this->setDataSourceMode(this->modeNoop);
    }

delete a;
}

qint32 MainWindow::timediff(QTime from, QTime to) {
    qint32 diff=from.secsTo(to)+this->daydiff*86400;
    if (diff<0) {
        this->daydiff++;
        diff=from.secsTo(to)+this->daydiff*86400;
    }
    return diff;
}

void MainWindow::fileToData() {

    pdata->reset();
    QFile inFile(this->inName);
    inFile.open(QIODevice::ReadOnly);
    QByteArray fileData,tmp; //temp data arrays
    QString tmpstring="",str="",nextline="";
    QStringList tmpstringlist;
    QTime start_t,current_t;
    fileData=inFile.readAll();
    inFile.close();
    QString fileDataString(fileData);
    tmpstringlist=fileDataString.split("\n");
    while (tmpstringlist.last().trimmed().isEmpty()) {
        tmpstringlist.removeLast();
    }
    pdata->appendAsciiData(tmpstringlist);
}


bool MainWindow::setupCurves(QStringList head, int mode) {
    bool result=false;
    setupCurvesDialog dialog(head,mode);
    dialog.exec();
    PlotCurve *curve;
    if (dialog.result()==QDialog::Accepted) {
        result=true;
        plot->enableAxis(QwtPlot::yRight,dialog.plotAxisEnabled(QwtPlot::yRight));
        plot->setAxisTitle(QwtPlot::yLeft,dialog.plotAxisTitle(QwtPlot::yLeft));
        plot->setAxisTitle(QwtPlot::yRight,dialog.plotAxisTitle(QwtPlot::yRight));
        plot->setAxisTitle(QwtPlot::xBottom,dialog.plotAxisTitle(QwtPlot::xBottom));

        this->curveList=dialog.getCurveList();
        for (int i=0;i<curveList.size();i++) {
            curve=curveList.value(i);
            curve->attach(plot);
            curve->setGpibData(pdata);
        }
        this->updatePlotCurves();
    } else {
        result=false;
    }

    return result;
}
void MainWindow::setupCurves(GpibData *data) {
    setupCurvesDialog dialog(data,this->curveList);
    dialog.exec();
    if (dialog.result()==QDialog::Accepted) {
        plot->enableAxis(QwtPlot::yRight,dialog.plotAxisEnabled(QwtPlot::yRight));
        plot->setAxisTitle(QwtPlot::yLeft,dialog.plotAxisTitle(QwtPlot::yLeft));
        plot->setAxisTitle(QwtPlot::yRight,dialog.plotAxisTitle(QwtPlot::yRight));
        plot->setAxisTitle(QwtPlot::xBottom,dialog.plotAxisTitle(QwtPlot::xBottom));
    }
    //regardless of the result returnd we should reassign PlotCurves back to
    //mainwindows plot
    this->curveList=dialog.getCurveList();
    PlotCurve *curve;
    for (int i=0;i<curveList.size();i++) {
        curve=curveList.value(i);
        curve->attach(plot);
        curve->setGpibData(pdata);
    }
}

void MainWindow::fileOpen(QString filename) {

    QFile file(filename);
    if (file.exists()) {
        if (!file.open(QFile::ReadOnly)) {
            this->showErrorMessageDialog("Error opening file \""+this->inName+"\"");
            qWarning()<<"Error opening file"<<filename;
            return;}

        QByteArray buf;
        QString tmp;
        QStringList head;
        //read first 10 lines
        for (int i=0;i<=10;i++) {
            buf=file.readLine();
            if (buf.size()<=0) {
                break;
            } else {
                head.append(buf.trimmed());
            }
        }
        file.close();
        if (this->setupCurves(head,modeFile)) {
            this->inName=filename;
            this->setDataSourceMode(modeFile);
            this->fileToData();
            this->updatePlotCurves();
            plot->replot();
        }

    } else {
        fileOpen();
    }
}

void MainWindow::fileOpen() {
    QFileDialog *dialog=new QFileDialog();
    dialog->selectFile(this->settings.value("recentFile","").toString());
    QString filename=dialog->getOpenFileName(this,
                                             tr("Open data file"), this->settings.value("recentDir",QDir::rootPath()).toString(),tr("Data files (*.dat *.txt)"));
    if (dialog->result()==QDialog::Rejected) {
        delete dialog;
        return;
    } else {

        delete dialog;
        if (filename.isEmpty()) {
            return;
        }
        QString dirname=filename.left(filename.lastIndexOf("/")+1);
        this->settings.setValue("recentFile", filename);
        this->settings.setValue("recentDir",dirname);
        this->fileOpen(filename);
    }
}

void MainWindow::fileOpenRecent() {
    this->setDataSourceMode(this->modeFile);
    //this->resetPlot();
    fileOpen(settings.value("recentFile","").toString());
}

void MainWindow::fileSave() {
    if (this->inName.isNull() or this->inName.isEmpty()) {
        this->fileSaveAs();
    }
}

void MainWindow::fileSaveAs() {
    this->inName=QFileDialog::getSaveFileName(this,
    tr("Open data file"), this->settings.value("recentDir",QDir::rootPath()).toString(),tr("Data files (*.dat *.txt)"));
}

void MainWindow::showViewRawData() {
    viewRawData *a = new viewRawData(this);
    a->setText(pdata->getRawData());
    a->exec();
    delete a;
}

void MainWindow::showViewParsedData() {
    viewParsedData *a = new viewParsedData(pdata->getMeasureData(),this);
    //a->model->setStringList(pdata->measureData);
    a->exec();
    delete a;
}

void MainWindow::showViewTableData() {
    viewTableData *a = new viewTableData(this);
    //a->model->setStringList(pdata->measureData);
    a->exec();
    delete a;
}


void MainWindow::showErrorMessageDialog(QString message) {
    errorMessageDialog *a=new errorMessageDialog(this);
    a->setMessage(message);
    a->exec();
    delete a;
}

void MainWindow::showSetupCurvesDialog() {
    this->setupCurves(this->pdata);
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

void MainWindow::updateInitialData(QStringList head) {
    setupCurvesDialog dialog(head,this->dataSource);
    dialog.exec();
    if (dialog.result()==QDialog::Accepted) {
        plot->enableAxis(QwtPlot::yRight,dialog.plotAxisEnabled(QwtPlot::yRight));
        plot->setAxisTitle(QwtPlot::yLeft,dialog.plotAxisTitle(QwtPlot::yLeft));
        plot->setAxisTitle(QwtPlot::yRight,dialog.plotAxisTitle(QwtPlot::yRight));
        this->curveList=dialog.getCurveList();
        for (int i=0;i<curveList.size();i++) {
            curveList.value(i)->attach(plot);
        }
    }

    
    this->gpibQueryInterval->stop(); //stop the measuring timer
    this->burstUpdate=true;
    ui->startStopButton->setChecked(false); //uncheck the button;

    this->updateCurrentData(head,"0");
    ui->startStopButton->setChecked(true); //check the button;
    }

void MainWindow::updateCurrentData(QStringList data,QString from) {
    if ((from=="0" && pdata->rowCount()==0) || from==pdata->getLatestTimestamp()) {
        pdata->appendAsciiData(data);
        this->updatePlotCurves();
        ui->numPointsLabel->setText(QString::number(pdata->rowCount()));
        if (ui->monitorOnOffCheckBox->isChecked()) {
            int rows=pdata->rowCount();
            /*
            int start=rows-1-monitorPointsCount;
            int end=rows-1;
            */
            /*this is well-performance technique for drawing new points of the curve
              without replotting all the plot.
            for (int i=0;i<curveList.size();i++) {
                curveList.at(i)->draw(start,end);
            }
            */
            double max=curveList.first()->maxXValue()+30;
            double min=max-double(ui->monitorPointCountSpinBox->value());
            plot->setAxisScale(QwtPlot::xBottom,min,max);
            plot->replot();
        }
        else plot->replot();
    }
}

void MainWindow::setDataSourceMode (int mode) {
    this->dataSource=mode;
    switch (mode) {
    case modeFile:
        ui->networkGroupBox->setEnabled(false);
        ui->startStopButton->setChecked(false);
        ui->actionExperiment_control->setEnabled(false);
        break;
    case modeNetwork:
        if (QHostAddress(gpib->getServerAddress()).isNull()) {
            this->showBrowseNetworkDialog();
            if (QHostAddress(gpib->getServerAddress()).isNull()) {
                this->setDataSourceMode(modeNoop);
                return;
            }
        }
        ui->networkGroupBox->setEnabled(true);
        ui->startStopButton->setEnabled(true);
        ui->actionExperiment_control->setEnabled(true);
        this->initialTimestamp=0;
        break;
           default:
        ui->networkGroupBox->setEnabled(false);
    }
}

void MainWindow::resetPlot(void) {
    pdata->reset();

    this->initialTimestamp=0;
    this->initialTime=QTime();
    plot->clear();
    plot->enableAxis(QwtPlot::yRight,false);
    //pdata->columnCount=0;


}


void MainWindow::startStopNetworkExchange(bool start) {

    if (start) {
        this->gpibQueryInterval->start();
        this->gpibFetchData();
    } else {
        //this->burstUpdate=false;
        this->gpibQueryInterval->stop();
    }
}

void MainWindow::gpibFetchData() {
    QByteArray query;
    if (pdata->getLatestTimestamp().size()>0) {
        QString tmp="get from "+pdata->getLatestTimestamp();
        query.append(tmp);
    }
    gpib->tx(query);
}

void MainWindow::updateInterval (int msec) {
    this->gpibQueryInterval->setInterval(msec);
}


void MainWindow::gpibNoNewData(void) {
    static int noNewDataEvents=0;
    if (noNewDataEvents>10) {
        gpib->serverGetInterval();
        gpib->serverGetStatus();
        noNewDataEvents=0;
    }
    this->burstUpdate=false;

}


void MainWindow::showExperimentControl(void) {
if (gpib->getServerAddress().isNull()) return;

    experimentControl *experimentControlDialog = new experimentControl(this,gpib);

    experimentControlDialog->show();
    experimentControlDialog->raise();
    experimentControlDialog->activateWindow();

    }

void MainWindow::zoomAll() {

    plot->setAxisAutoScale(QwtPlot::xBottom);
    plot->setAxisAutoScale(QwtPlot::yLeft);
    plot->setAxisAutoScale(QwtPlot::yRight);
    plot->replot();

}

void MainWindow::zoom(bool on) {

    if (on) {
    plot->zoomer = new Zoomer (QwtPlot::xBottom,QwtPlot::yLeft,plot->canvas());
    plot->zoomerRight = new Zoomer (QwtPlot::xBottom,QwtPlot::yRight,plot->canvas());
    plot->zoomerRight->setMousePattern(QwtEventPattern::MouseSelect1,Qt::LeftButton,Qt::ControlModifier);

} else {
    delete plot->zoomer;
    delete plot->zoomerRight;
}
    plot->picker->setEnabled(!on);
}

void MainWindow::setNumPoints(int num) {
    ui->numPointsLabel->setText(tr("Points")+": "+QString::number(num));
}

void MainWindow::setNumTransitionPoints(int num) {
    ui->numTransitionPointsLabel->setText(tr("Transitions")+": "+QString::number(num));
}

void MainWindow::updateHeaterPower() {
    if (pdata->serviceInfoHeaterPowerColumn<0) {
        if (ui->powerGroupBox->isVisible()) ui->powerGroupBox->setVisible(false);
        return;
    }
    ui->powerGroupBox->setVisible(true);
    QString line;//(pdata->measureData.last());
    QStringList column=line.split(delimiterRegExp);
    double power=column.at(pdata->serviceInfoHeaterPowerColumn).toDouble();
    int percent=0;
    QString limit;
    if (power<0.05) {
        percent=int (power/0.05*100);
        limit="50 mW";
    } else if (power<0.5) {
        percent=int (power/0.5*100);
        limit="500 mW";
    } else if (power<5) {
        percent=int (power/5*100);
        limit="5 W";
    } else if (power<50) {
        percent=int (power/50*100);
        limit="50 W";
    }
    ui->powerLimitLabel->setText(limit);
    ui->powerBar->setValue(percent);
    if (power<0.95) {
        ui->powerValueLabel->setText(QString::number(power*1000,'g',4)+" mW");
    } else {
        ui->powerValueLabel->setText(QString::number(power,'g',4)+" W");
    }
    ui->powerPercentLabel->setText(QString::number(percent)+"%");
}

void MainWindow::updatePlotCurves() {
    PlotCurve *curve;
    QString lastValue;
    for (int i=0;i<curveList.size();i++) {
        curve=curveList.value(i);
        if (!curve->getLinked()) {
            curve->setRawData(
                    pdata->getColumnData(curve->getXColumn()),
                    pdata->getColumnData(curve->getYColumn()),
                    pdata->rowCount()
                    );
        }
        else {
            if (tmpDoubleVector.size()<pdata->rowCount()) {

                double a,b;
                for (int j=tmpDoubleVector.size();j<pdata->rowCount();j++) {
                    a=pdata->data(pdata->index(j,curve->getYColumn()),Qt::DisplayRole).toDouble();
                    b=pdata->data(pdata->index(j,curve->getYColumn()+1),Qt::DisplayRole).toDouble()*100;
                    tmpDoubleVector.append(crossAverage(a,b,9,10));
                }
                curve->setData(pdata->getColumnData(curve->getXColumn()),
                               tmpDoubleVector.data(),
                               tmpDoubleVector.size());
            }
        }
        //updating last values on main window
        lastValue+=(QString::number(curve->y(curve->dataSize()-1))+"\n");
    }
    this->updateLastValues(lastValue.trimmed());

}

double MainWindow::crossAverage(double a, double b, double start, double end) {
    double value,interval,weight;
    interval=end-start;

    if (a<=start)
    {
        return a;
    } else if (b>=end && a>=end)
    {
        return b;
    } else if ((a>=end && b<end) || (a<end && b>=end))
    {
        value=(a+b)/2;
        return value;
    } else {
        weight=(a-start)/interval;
        value=b*weight+a*(1-weight);
        qDebug()<<"Crossing a ="<<a<<"and b ="<<b<<"weight is"<<weight
                <<", result is"<<value;
    }

    return value;
}

void MainWindow::setMonitorPoints(int count) {
    this->monitorPointsCount=count;
}

void MainWindow::pointSelected(double x, double y) {
    ui->selectedValueLabel->setText(QString::number(y));
    ui->selectedValueLabel->setToolTip(QString::number(x));
    QClipboard *clipboard=QApplication::clipboard();
    clipboard->setText(QString::number(y));
}

void MainWindow::updateLastValues(QString value) {
    ui->lastValueLabel->setText(value);
}

void MainWindow::showLastValuesDialog() {

}
