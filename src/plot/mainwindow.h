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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QComboBox>
#include <QtGui/QToolButton>
#include <QList>
#include <QSettings>
#include <QVector>
#include <QUdpSocket>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <QRegExp>
#include "plot.h"
#include "gpibsocket.h"
#include "gpibdata.h"

class QTimer;
class experimentControl;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum dataSourceMode {modeNoop=0, modeNetwork=1, modeFile=2};
    enum plotType {measurePlot=0,transitionPlot=1};
    enum serviceInfo {serviceHeater,serviceHeLevel};
    bool burstUpdate;


    gpibData *pdata;

    QString currentFileName;
    Plot *plot;
    static QString getFileName(void);
    QString delimiter;
    QRegExp delimiterRegExp;
    qint32 timediff(QTime from, QTime to);
    QTime initialTime;
    double initialTimestamp;
    qint32 daydiff;
    QSettings settings;
    void drawData(int x_index, int y_index, bool rightAxis=false,QString label="",int plotType=measurePlot);
    void fileToData();
    void datagramToData(QStringList data);
    void asciiToData(QStringList *data,bool overwrite=false);
    gpibSocket *gpib;
    QHostAddress gpibServer; //address of server to work with




    //Ui::MainWindow *ui;



public slots:
    void showErrorMessageDialog(QString message);
    void updateRtt(int msec);
    void updateRxBytes (quint32 rxBytes);
    void updateTxBytes (quint32 txBytes);
    void updateInitialData(QStringList head);
    void updateCurrentData(QStringList data,QString from);
    void gpibNoNewData();
    void setDataSourceMode (int);
    void updateInterval (int);
    void updateHeaterPower();

private:
    Ui::MainWindow *ui;
    int dataSource;
    bool assignCurves(QStringList head);
    QTimer *gpibQueryInterval;
    experimentControl *experimentControlDialog;


private slots:
    //    void close();

    void setColumn(int column,int axis,QString axisLabel,QString dataLabel);
    void setColumn(int column,int serviceInfo,double low_limit,double high_limit);
    void showFileChooseColumnsDialog();
    void showAbout();
    void showAboutQt();
    void showExperimentControl(void);
    void showViewRawData(void);
    void showViewParsedData(void);
    void showViewTableData(void);
    void fileOpen(QString filename);
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void showBrowseNetworkDialog(void);
    void startStopNetworkExchange(bool start);
    void resetPlot();
    void gpibFetchData();
    void setNumPoints(int num);
    void setNumTransitionPoints(int num);
    void zoomAll();
    void zoom(bool on);


signals:

};

#endif // MAINWINDOW_H
