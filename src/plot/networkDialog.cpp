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

#include "networkDialog.h"
#include "ui_networkDialog.h"
#include "gpibsocket.h"
#include "mainwindow.h"

networkDialog::networkDialog(MainWindow *parent, gpibSocket *gpibSocket) :
    QDialog(parent),
    ui(new Ui::networkDialog)
{
    ui->setupUi(this);
    w=parent;
    this->gpib=gpibSocket;

    connect(ui->lockButton,SIGNAL(toggled(bool)),this,SLOT(lockCurrentServer(bool)));
    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(searchServers()));
    connect(gpib,SIGNAL(serverFound(QHostAddress)),this,SLOT(serverFound(QHostAddress)));
    connect(this,SIGNAL(accepted()),this,SLOT(setCurrentServer()));
    connect(gpib,SIGNAL(serverIdleRunning(bool)),this,SLOT(setServerStatus(bool)));
    connect(gpib,SIGNAL(serverInterval(int)),this,SLOT(setServerInterval(int)));
    connect(ui->refreshButton,SIGNAL(clicked()),SLOT(refreshButtonClicked()));
    connect(ui->startStopButton,SIGNAL(clicked(bool)),SLOT(startStopButtonClicked(bool)));



    if (!gpib->getServerAddress().isNull()) {
        ui->serversComboBox->insertItem(0,gpib->getServerAddress().toString());
        ui->lockButton->setChecked(true);
        ui->settingsGroupBox->setEnabled(true);
        this->refreshButtonClicked();
    } else {
        gpib->scan();
    }



}

networkDialog::~networkDialog()
{
    delete ui;
}

void networkDialog::lockCurrentServer(bool lock) {
        ui->searchButton->setEnabled(! lock);
        ui->serversComboBox->setEnabled(! lock);
}

void networkDialog::changeEvent(QEvent *e)
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

void networkDialog::refreshButtonClicked(void) {
    QString a=ui->serversComboBox->currentText();
    QHostAddress current(a);
    QHostAddress previous(gpib->getServerAddress());
    gpib->setServerAddress(current);
    gpib->serverGetStatus();
    gpib->serverGetInterval();
    gpib->setServerAddress(previous);
}

void networkDialog::startStopButtonClicked(bool start) {
    gpib->serverStartStop(start);
    gpib->serverGetStatus();
}

void networkDialog::searchServers() {
    ui->serversComboBox->clear();
    ui->settingsGroupBox->setEnabled(false);
    gpib->scan();
}

void networkDialog::serverFound(QHostAddress server) {
    ui->serversComboBox->addItem(server.toString());
    ui->settingsGroupBox->setEnabled(true);
    this->refreshButtonClicked();
}

void networkDialog::setServerInterval(int msec)  {
    ui->intervalLabel->setText(tr("Interval ")+QString::number(double (msec)/1000)+tr(" sec"));
}

void networkDialog::setServerStatus(bool running) {
    //ui->startStopButton->setDown(running);
    ui->startStopButton->setChecked(running);
}

void networkDialog::setCurrentServer() {
    if (ui->serversComboBox->currentIndex()>=0) {
        QHostAddress address(ui->serversComboBox->currentText());
        gpib->setServerAddress(address);
    }
}
