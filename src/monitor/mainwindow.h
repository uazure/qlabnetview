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
#include "../main/gpibsocket.h"

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
    bool burstUpdate;

    QRegExp columnDelimiter;
    QStringList rawAsciiData;
    QStringList measureData;
    QStringList transitionData;
    QString latestTimestamp;
    int measureDatacolumns;
    QString currentFileName;
    static QString getFileName(void);
    QString delimiter;
    qint32 timediff(QTime from, QTime to);
    double initialTimestamp;
    QSettings settings;

    void datagramToData(QStringList data);
    gpibSocket *gpib;
    QHostAddress gpibServer; //address of server to work with




    //Ui::MainWindow *ui;



    public slots:
void showErrorMessageDialog(QString message);
void updateRtt(int msec);
void updateRxBytes (quint32 rxBytes);
void updateTxBytes (quint32 txBytes);
void updateCurrentData(QString data);
void gpibNoNewData();
void updateInterval (int);

private:
    Ui::MainWindow *ui;
    int dataSource;
    QTimer *gpibQueryInterval;
    experimentControl *experimentControlDialog;


private slots:
//    void close();

    void showAbout();
    void showAboutQt();
    void showBrowseNetworkDialog(void);
    void gpibFetchData();




    signals:

};

#endif // MAINWINDOW_H
