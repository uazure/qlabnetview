#ifndef SETUPCURVESDIALOG_H
#define SETUPCURVESDIALOG_H

#include <QDialog>
#include <QStringList>
#include "mainwindow.h"
#include <QString>
#include "PenStyleBox.h"
#include "ColorBox.h"
#include "SymbolBox.h"


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
    setupCurvesDialog(GpibData *parentData, QList<PlotCurve *> clist);
    setupCurvesDialog(QStringList head, int mode=MainWindow::modeFile);
    QList<PlotCurve *> getCurveList();
    void setCurveList(QList<PlotCurve *>);
    QwtText plotAxisTitle(int axis) const;
    bool plotAxisEnabled(int axis) const;
    ~setupCurvesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::setupCurvesDialog *ui;
    Plot *plot;
    GpibData *gpibdata;
    QList<PlotCurve*> curveList;
    PlotCurveModel *curveListModel;
    SymbolBox symbolBox;
    ColorBox colorBox;
    void init(void);


private slots:
    void accept();
    void reject();
    void addCurve();
    void setBottomTitle(QString title);
    void setLeftTitle(QString title);
    void setRightTitle(QString title);
    void currentColumnIndexChanged(int index);

public slots:
    void currentCurveIndexChanged(QModelIndex index);
    void currentSymbolChanged (int index);
    void currentColorChanged (int index);
    void currentWidthChanged (double width);
    void currentYAxisChanged (void);
    void currentLinkChanged (bool link);

};

#endif // SETUPCURVESDIALOG_H
