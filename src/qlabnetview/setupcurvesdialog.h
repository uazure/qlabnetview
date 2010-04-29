#ifndef SETUPCURVESDIALOG_H
#define SETUPCURVESDIALOG_H

#include <QDialog>
#include <QStringList>
#include "mainwindow.h"
#include <QString>


class Plot;
class GpibData;
class PlotCurve;
class PlotCurveModel;

namespace Ui {
    class setupCurvesDialog;
}

class setupCurvesDialog : public QDialog {
    Q_OBJECT
public:
    setupCurvesDialog(QStringList head, GpibData *pdata, int mode=MainWindow::modeFile);

    ~setupCurvesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::setupCurvesDialog *ui;
    Plot *plot;
    GpibData *gpibdata;
    QList<PlotCurve*> curveList;
    PlotCurveModel *curveListModel;




private slots:
    void addCurve();
    void setBottomTitle(QString title);
    void setLeftTitle(QString title);
    void setRightTitle(QString title);

public slots:
    void currentCurveChanged(QModelIndex index);

};

#endif // SETUPCURVESDIALOG_H
