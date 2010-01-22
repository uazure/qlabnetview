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

#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QHostAddress>
#include <QDialog>
class MainWindow;
class gpibSocket;

namespace Ui {
    class networkDialog;
}

class networkDialog : public QDialog {
    Q_OBJECT
public:
    networkDialog(MainWindow *parent = 0, gpibSocket *gpib=0);
    ~networkDialog();

    MainWindow *w;
public slots:
        void lockCurrentServer(bool);
        void serverFound(QHostAddress);
        void searchServers(void);
        void setCurrentServer(void);
        void setServerStatus(bool start);
        void setServerInterval(int msec);

private slots:
        void refreshButtonClicked(void);
        void startStopButtonClicked(bool start);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::networkDialog *ui;
    gpibSocket *gpib;

};

#endif // NETWORKDIALOG_H
