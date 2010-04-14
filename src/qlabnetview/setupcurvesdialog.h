#ifndef SETUPCURVESDIALOG_H
#define SETUPCURVESDIALOG_H

#include <QDialog>
#include <QStringList>
#include "mainwindow.h"

namespace Ui {
    class setupCurvesDialog;
}

class setupCurvesDialog : public QDialog {
    Q_OBJECT
public:
    setupCurvesDialog(QStringList head, int mode=MainWindow::modeFile);
    ~setupCurvesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::setupCurvesDialog *ui;
};

#endif // SETUPCURVESDIALOG_H
