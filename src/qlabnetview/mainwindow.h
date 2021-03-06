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
#include "../gpibsocket/gpibsocket.h"
#include "gpibdata.h"

class QTimer;
class experimentControl;
class PlotCurve;

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
    /*! Type of data source.
      inNone - no data
      inFile - data read from file
      inUDP - data read over network (UDP packets, legacy version)
      inTCP - data read over network (TCP stream, new version)
      */
    enum inType {inNone=0, inFile=1, inUDP=2, inTCP=3, inOther=99};

    /*!
      Name of input. Either filename or network server address
      */
    QString inName;

    /*! It is set to true when we need to fetch all the data quickly over udp
      */
    bool burstUpdate;

    /*! This is main dynamic array of doubles
      with interface of QTableModel
      */
    GpibData *pdata;

    /*! This is the main plot widget
      */
    Plot *plot;
    QString getFileName(void) const;
    QString delimiter;
    QRegExp delimiterRegExp;
    qint32 timediff(QTime from, QTime to);
    QTime initialTime;
    double initialTimestamp;
    qint32 daydiff;
    QSettings settings;
    void fileToData();
    //void datagramToData(QStringList data);
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
    void updatePlotCurves();
    void gpibNoNewData();
    void setDataSourceMode (int);
    void updateInterval (int);
    void updateHeaterPower();
    void fileOpen(QString filename);

private:
    Ui::MainWindow *ui;
    int dataSource;
    QTimer *gpibQueryInterval;
    experimentControl *experimentControlDialog;
    QList<PlotCurve*> curveList;
    QVector<double> tmpDoubleVector;
    double crossAverage(double a,double b,double start,double end);
    int monitorPointsCount;


private slots:   
    //! this function is used to prepare curves for new data (new file or
    //! network source)
    bool setupCurves(QStringList head, int mode=modeFile);
    //! this function is used to adjust curves using current data
    void setupCurves(GpibData *data);
    void showAbout();
    void showExperimentControl(void);
    void showViewRawData(void);
    void showViewParsedData(void);
    void showViewTableData(void);
    void showSetupCurvesDialog(void);
    void fileOpen();
    void fileOpenRecent(void);
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
    void setMonitorPoints (int);
    void pointSelected(double x, double y);
    void updateLastValues(QString value);
    void showLastValuesDialog(void);

signals:

};

#endif // MAINWINDOW_H
