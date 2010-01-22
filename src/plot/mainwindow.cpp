#include "main.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "assignCurvesDialog.h"
#include "networkDialog.h"
#include "zoomer.h"
#include "plot.h"
#include "viewrawdata.h"
#include "viewparseddata.h"
#include "viewtabledata.h"

#include "errormessagedialog.h"
#include "experimentControl.h"
#include "gpibtablemodel.h"
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

    //pdata = new PrivateData();


    this->pdata=new gpibData();
    ui->setupUi(this);
    plot = new Plot(this);
    ui->gridLayout->addWidget(plot,0,0);
    this->setDataSourceMode(modeNoop);
    ui->powerGroupBox->setVisible(false);
    ui->networkGroupBox->setVisible(false);
    ui->monitorGroupBox->setVisible(false);

    gpibQueryInterval = new QTimer(this);
    gpibQueryInterval->setSingleShot(false);

    gpib = new gpibSocket(this);
    //define slot<->signal
    //call fileOpen dialog when File->Open clicked
    connect(ui->actionOpen,SIGNAL(triggered()),SLOT(fileOpen()));
    connect(ui->actionSave,SIGNAL(triggered()),SLOT(fileSave()));
    connect(ui->actionSave_as,SIGNAL(triggered()),SLOT(fileSaveAs()));
    //call networkDialog when File->Browse network clicked
    connect(ui->actionBrowse_network,SIGNAL(triggered()),this,SLOT(showBrowseNetworkDialog()));
    //close program when File->Exit clicked
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    //show About window when Help->About clicked
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
    //show About Qt when Help->About Qt clicked
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),SLOT(showAboutQt()));

    connect(ui->actionExperiment_control,SIGNAL(triggered()),SLOT(showExperimentControl()));
    connect(ui->actionASCII_data,SIGNAL(triggered()),SLOT(showViewRawData()));
    connect(ui->actionParsed_data,SIGNAL(triggered()),SLOT(showViewParsedData()));
    connect(ui->actionTable_data,SIGNAL(triggered()),SLOT(showViewTableData()));


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



this->delimiter=" ";
this->delimiterRegExp.setPattern("[\\ \\t]{1,9}"); //space (\\ ) or tab (\\t) {from 1 up to 9 in a row}
this->daydiff=0;
this->initialTimestamp=0;
this->burstUpdate=false;
pdata->measureDataXcolumn=-1;
pdata->measureDataY1column=-1;
pdata->measureDataY2column=-1;
pdata->serviceInfoHeaterPowerColumn=-1;
pdata->serviceInfoHeLevelColumn=-1;
}

MainWindow::~MainWindow()
{
    delete gpib;
    delete plot;
    delete ui;
    delete pdata;

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

void MainWindow::showFileChooseColumnsDialog() {
    QFile file(this->currentFileName);
    if (!file.open(QFile::ReadOnly)) {
        this->showErrorMessageDialog("Error opening file \""+this->currentFileName+"\"");
        return;}
        QByteArray buf;
        QString tmp;
        QStringList head;
        for (int i=0;i<=10;i++) {
            buf=file.readLine();
            if (buf.size()<=0) {
                break;
            } else {
                head.append(buf.trimmed());
            }
        }
        file.close();

        this->assignCurves(head);
    }

bool MainWindow::assignCurves(QStringList head) {

    assignCurvesDialog *a = new assignCurvesDialog(head,this->dataSource);
    connect(a,SIGNAL(setColumn(int,int,double,double)),this,SLOT(setColumn(int,int,double,double)));
    connect(a,SIGNAL(setColumn(int,int,QString,QString)),this,SLOT(setColumn(int,int,QString,QString)));

    if (a->exec()==QDialog::Accepted) {
        pdata->measureDataColumns=a->columns();
        return true;
    }
return false;
delete a;
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


void MainWindow::asciiToData(QStringList *data, bool overwrite) {
    if (overwrite) {
        this->daydiff=0;
        this->initialTimestamp=0;
        pdata->rawAsciiData.clear();
        pdata->measureData.clear();
        pdata->transitionData.clear();
    }
    //deprecated with pdata->appendAsciiData
    //pdata->rawAsciiData.append(*data);
    pdata->appendAsciiData(*data);

//    QTime start_t,current_t;
//    double start_timestamp=0,current_timestamp=0.0;
//    QStringList column; //this string list will contain list of columns in current row.
//    QString tmpstring, nextline;
//    if (!this->initialTime.isNull()) start_t=this->initialTime;
//    if (this->initialTimestamp>1) start_timestamp=this->initialTimestamp;
//
//
//    for (int row=0;row<data->size();row++) {
//        tmpstring=data->at(row);
//        tmpstring=tmpstring.trimmed();
//        if (tmpstring.startsWith("#",Qt::CaseSensitive)
//            || tmpstring.startsWith("/",Qt::CaseSensitive)
//            || tmpstring.trimmed().isEmpty()
//             ) {
//            continue;
//        } else if (tmpstring.startsWith("*",Qt::CaseSensitive) && pdata->measureData.size()>0) {
//            if (pdata->transitionData.last()!=pdata->measureData.last()) {
//                pdata->transitionData.append(pdata->measureData.last());
//            }
//            continue;
//        }
//
//
//        column=tmpstring.split(this->delimiterRegExp);
//        int columnCount=column.count();
//        if (row==data->size()-1) {
//            //this will update the latestTimestamp to the very last value in incoming data
//            pdata->latestTimestamp=column.last();
//        }
//
//        for (int columnIndex=0;columnIndex<columnCount;++columnIndex) {
//            tmpstring=column[columnIndex];
//
//            //this will check the first row. if it is time in format h:mm:ss,
//            //it will update the starting time and calculate difference in seconds.
//            if (columnIndex==0 && tmpstring.contains(":")) {
//                if (start_t.isNull()) {
//                    start_t=QTime::fromString(tmpstring,"h:mm:ss");
//                    this->initialTime=start_t;
//                    if (!start_t.isValid()) {
//                        start_t=QTime::QTime();
//                    } else {
//                        tmpstring="0";
//                    }
//
//                } else {
//                    current_t=QTime::fromString(tmpstring,"h:mm:ss");
//                    tmpstring.setNum(timediff(initialTime,current_t));
//                }
//
//                nextline+=tmpstring;
//                continue;
//
//
//            } else if (columnIndex==columnCount-1 && pdata->measureDataXcolumn==columnIndex) {
//                if (start_timestamp<1) { //we should not compare timestamp to zero since it is a double
//                    start_timestamp=tmpstring.toDouble();
//                    this->initialTimestamp=start_timestamp;
//                    tmpstring="0";
//                } else {
//                    current_timestamp=tmpstring.toDouble()-start_timestamp;
//                    tmpstring.setNum(current_timestamp);
//                }
//            }
//            if (columnIndex!=0)
//            nextline+="\t"+tmpstring;
//            else nextline+=tmpstring;
//
//            if (columnIndex==columnCount-1) {
//                pdata->measureData.append(nextline);
//                ui->plainTextEdit->appendPlainText(nextline);
//                nextline.clear();
//            }
//
//        }
//
//        if (row==0 && (overwrite || pdata->transitionData.isEmpty())) {
//            pdata->transitionData.append(pdata->measureData.first());
//        }
//
//    }



    this->setNumPoints(pdata->measureData.count());
    this->setNumTransitionPoints(pdata->transitionData.count());
}


void MainWindow::datagramToData(QStringList data) {
    this->asciiToData(&data,false);
}

void MainWindow::fileToData() {

    QFile inFile(this->currentFileName);
    inFile.open(QIODevice::ReadOnly);
    QByteArray fileData,tmp; //temp data arrays
    QString tmpstring="",str="",nextline="";
    QStringList tmpstringlist;
    QTime start_t,current_t;
    fileData=inFile.readAll();
    QString fileDataString(fileData);
    tmpstringlist=fileDataString.split("\n");
    while (tmpstringlist.last().trimmed().isEmpty()) {
        tmpstringlist.removeLast();
    }
    this->asciiToData(&tmpstringlist,true);
    inFile.close();
}


void MainWindow::fileOpen(QString filename) {

    this->currentFileName=filename;
    this->showFileChooseColumnsDialog();
    this->fileToData();
    if (pdata->measureDataXcolumn>=0 && pdata->measureDataXcolumn<pdata->measureDataColumns) {
        if (pdata->measureDataY1column>=0 && pdata->measureDataY1column<pdata->measureDataColumns  && pdata->measureDataY1column!=pdata->measureDataXcolumn) {
            drawData(pdata->measureDataXcolumn,pdata->measureDataY1column,false,pdata->measureDataY1Label,measurePlot);
            drawData(pdata->measureDataXcolumn,pdata->measureDataY1column,false,QString::QString(),transitionPlot);
        }
        if (pdata->measureDataY2column>=0 && pdata->measureDataY2column<pdata->measureDataColumns  && pdata->measureDataY2column!=pdata->measureDataXcolumn) {
            drawData(pdata->measureDataXcolumn,pdata->measureDataY2column,true,pdata->measureDataY2Label);
            drawData(pdata->measureDataXcolumn,pdata->measureDataY2column,true,pdata->measureDataY2Label+" (transition)",true);
        }
        plot->replot();
    }
}

void MainWindow::fileOpen() {
    this->setDataSourceMode(this->modeFile);
    this->resetPlot();
    this->currentFileName=QFileDialog::getOpenFileName(this,
        tr("Open data file"), this->settings.value("recentDir",QDir::rootPath()).toString(),tr("Data files (*.dat *.txt)"));
    if (this->currentFileName.isEmpty()) {
        return;
    }

    QString dirname=this->currentFileName.left(this->currentFileName.lastIndexOf("/")+1);
    this->settings.setValue("recentFile", this->currentFileName);
    this->settings.setValue("recentDir",dirname);
    this->fileOpen(this->currentFileName);

}

void MainWindow::fileSave() {
    if (this->currentFileName.isNull() or this->currentFileName.isEmpty()) {
        this->fileSaveAs();
    }
}

void MainWindow::fileSaveAs() {
    this->currentFileName=QFileDialog::getSaveFileName(this,
    tr("Open data file"), this->settings.value("recentDir",QDir::rootPath()).toString(),tr("Data files (*.dat *.txt)"));
}

void MainWindow::showViewRawData() {
    viewRawData *a = new viewRawData(this);
    a->setText(pdata->rawAsciiData);
    a->exec();
    delete a;
}

void MainWindow::showViewParsedData() {
    viewParsedData *a = new viewParsedData(this);
    a->model->setStringList(pdata->measureData);
    a->exec();
    delete a;
}

void MainWindow::showViewTableData() {
    viewTableData *a = new viewTableData(this);
    //a->model->setStringList(pdata->measureData);
    a->exec();
    delete a;
}

void MainWindow::drawData(int x_index, int y_index, bool rightAxis,QString label, int plotType) {
    QStringList *data;
    if (plotType==measurePlot) {
        data=&pdata->measureData;
    } else {
        data=&pdata->transitionData;
    }
    qint64 points_count;
    double *x_val;
    double *y_val;
    QString tmpstring="",str="";
    QStringList tmpstringlist;

    if (ui->monitorOnOffCheckBox->isChecked() && plotType==measurePlot) {
        //points_count=ui->monitorPointCountSpinBox->value();
        points_count=qMin(ui->monitorPointCountSpinBox->value(),data->size());
        x_val = new double[points_count];
        y_val = new double[points_count];
        qint64 startposition=data->size()-points_count;
        for (qint64 i=0;i<points_count;i++) {
            tmpstring=data->at(startposition+i);
            tmpstringlist=tmpstring.split("\t");
            str=tmpstringlist[x_index];
            x_val[i]=str.toDouble();
            str=tmpstringlist[y_index];
            y_val[i]=str.toDouble();
        }


    } else {

        points_count=data->count();
        x_val = new double[points_count];
        y_val = new double[points_count];
        for (int i=0;i<points_count;i++) {
            //FIXME: need some more debug information here if any problem occurs
            tmpstring=data->at(i);
            tmpstringlist=tmpstring.split("\t");
            if (x_index<tmpstringlist.size()) {
                x_val[i]=tmpstringlist.at(x_index).toDouble();}
            else {x_val[i]=0;}
            if (y_index<tmpstringlist.size()) {
                y_val[i]=tmpstringlist.at(y_index).toDouble();
            }
            else {y_val[i]=0;}


        }
    }

    if (plotType==this->measurePlot) {
        plot->insertCurve(x_val,y_val,points_count,rightAxis,label);
    } else if (plotType==transitionPlot && ui->monitorOnOffCheckBox->isChecked()==false) {
        plot->insertTransitionPoints(x_val,y_val,points_count,rightAxis,label);
    }

    delete x_val;
    delete y_val;

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

void MainWindow::updateInitialData(QStringList head) {
    if (pdata->measureDataXcolumn<0) {
        this->resetPlot();
        this->assignCurves(head);
    }
//    this->gpibQueryInterval->stop(); //stop the measuring timer
    this->burstUpdate=true;
    ui->startStopButton->setChecked(false); //uncheck the button;


    this->updateCurrentData(head,"0");
    ui->startStopButton->setChecked(true); //check the button;
    }

void MainWindow::updateCurrentData(QStringList data,QString from) {
    if ((from=="0" && pdata->rawAsciiData.size()==0) || from==pdata->latestTimestamp) {
        this->datagramToData(data);
        plot->clear();

        if (pdata->measureDataXcolumn>=0 && pdata->measureDataXcolumn<pdata->measureDataColumns) {
            //if (!this->burstUpdate) {
                if (pdata->measureDataY1column>=0 && pdata->measureDataY1column<pdata->measureDataColumns  && pdata->measureDataY1column!=pdata->measureDataXcolumn) {
                    drawData(pdata->measureDataXcolumn,pdata->measureDataY1column,false,pdata->measureDataY1Label,measurePlot);
                    drawData(pdata->measureDataXcolumn,pdata->measureDataY1column,false,QString::QString(),transitionPlot);
                }
                if (pdata->measureDataY2column>=0 && pdata->measureDataY2column<pdata->measureDataColumns  && pdata->measureDataY2column!=pdata->measureDataXcolumn) {
                    drawData(pdata->measureDataXcolumn,pdata->measureDataY2column,true,pdata->measureDataY2Label,measurePlot);
                    drawData(pdata->measureDataXcolumn,pdata->measureDataY2column,true,QString::QString(),transitionPlot);
                }
                plot->replot();
                this->updateHeaterPower();


            //}
            if (this->burstUpdate && this->gpibQueryInterval->isActive()) {
                this->gpibQueryInterval->stop();
                this->gpibFetchData();
                this->gpibQueryInterval->start();
            }

        }
    }
    //this feature seems to be buggy
    //if (this->burstUpdate) this->gpibQueryInterval->setInterval(200);
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
    pdata->rawAsciiData.clear();
    pdata->measureData.clear();;
    pdata->transitionData.clear();

    this->initialTimestamp=0;
    this->initialTime=QTime::QTime();
    plot->clear();
    plot->enableAxis(QwtPlot::yRight,false);
    pdata->measureDataXcolumn=-1;
    pdata->measureDataY1column=-1;
    pdata->measureDataY2column=-1;
    pdata->measureDataY1Label="";
    pdata->measureDataY2Label="";
    pdata->measureDataColumns=0;


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
    if (pdata->latestTimestamp.size()>0) {
        QString tmp="get from "+pdata->latestTimestamp;
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
    QString line(pdata->measureData.last());
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

void MainWindow::setColumn(int column,int axis,QString axisLabel,QString dataLabel) {
    if (axis==QwtPlot::xBottom) {
        pdata->measureDataXcolumn=column;
    }
     else
    if (axis==QwtPlot::yLeft) {
        pdata->measureDataY1column=column;
        pdata->measureDataY1Label=dataLabel;
    } else

    if (axis==QwtPlot::yRight) {
        pdata->measureDataY2column=column;
        pdata->measureDataY2Label=dataLabel;
    }

    QwtText qwtAxisLabel(axisLabel);
    if (axis==QwtPlot::yLeft) {
        qwtAxisLabel.setColor(Qt::blue);
    } else if (axis==QwtPlot::yRight) {
        qwtAxisLabel.setColor(Qt::red);
    }
    QFont font;
    font.setPointSize(font.pointSize()+2);
    font.setBold(true);
    qwtAxisLabel.setFont(font);
    plot->setAxisTitle(axis, qwtAxisLabel);
}

void MainWindow::setColumn(int column,int service,double low_limit,double high_limit) {
    //just go back if the column is not set (i.e. negative)
    if (column<0) return;

    switch (service) {
        case serviceHeater:
            pdata->serviceInfoHeaterPowerColumn=column;
            ui->powerGroupBox->setVisible(true);
            break;
        case serviceHeLevel:
            pdata->serviceInfoHeLevelColumn=column;
            break;
    }
}
