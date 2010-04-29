#include "setupcurvesdialog.h"
#include "ui_setupcurvesdialog.h"
#include "plot.h"
#include "qwt_text.h"
#include "gpibdata.h"
#include "plotcurve.h"
#include "qwt_legend.h"
#include "qwt_symbol.h"
#include "plotcurvemodel.h"


setupCurvesDialog::setupCurvesDialog(
        QStringList head, GpibData* parentData, int mode) :
    QDialog(),
    ui(new Ui::setupCurvesDialog)
{
    ui->setupUi(this);
    ui->plainTextEdit->clear();
    for (int i = 0 ; i< head.size(); i++) {
        ui->plainTextEdit->appendPlainText(head.at(i));
        //scroll the widget to beginning
    }
    ui->plainTextEdit->scroll(0,-1024);

    //! disable curve properties. It should be enabled only when curve
    //! is selected
    ui->curvePropertiesBox->setEnabled(false);

    //! Create a new plot and attach it to window
    plot = new Plot(this);
    //set the x axis label to "t, sec"
    ui->plotBottomAxisTitle->setText("t, sec");
    plot->setAxisScale(plot->yLeft,0,8,2);
    plot->setAxisScale(plot->yRight,0,760);

    plot->setAxisAutoScale(plot->xBottom);
    plot->setAxisAutoScale(plot->yLeft);
    ui->plotLayout->addWidget(plot);

    //plot axis titles changes
    connect(ui->plotBottomAxisTitle,SIGNAL(textChanged(QString)),this,SLOT(setBottomTitle(QString)));
    connect(ui->plotLeftAxisTitle,SIGNAL(textChanged(QString)),this,SLOT(setLeftTitle(QString)));
    connect(ui->plotRightAxisTitle,SIGNAL(textChanged(QString)),this,SLOT(setRightTitle(QString)));

    connect(ui->addCurveButton,SIGNAL(clicked()),SLOT(addCurve()));

    //! create PenStyleBox selector
    ui->Color_and_WidthLayout->addWidget(&colorBox);
    ui->SymbolBoxLayout->addWidget(&symbolBox);

    connect(&symbolBox,SIGNAL(currentIndexChanged(int)),SLOT(currentSymbolChanged(int)));
    connect(&colorBox,SIGNAL(currentIndexChanged(int)),SLOT(currentColorChanged(int)));
    connect(ui->widthSpinBox,SIGNAL(valueChanged(double)),SLOT(currentWidthChanged(double)));


    //make lock checkbox affect the X axis selector
    connect(ui->XLockCheckbox,SIGNAL(toggled(bool)),ui->XComboBox,SLOT(setDisabled(bool)));
    ui->XLockCheckbox->setChecked(true);
    ui->XComboBox->setDisabled(true);




    //create a gpibdata instance and fill it with head data
    gpibdata=new GpibData;
    gpibdata->appendAsciiData(head);

    //set the vertival header of the table to have height of the current font height+1 (for border)
    ui->tableView->verticalHeader()->setDefaultSectionSize(QFontMetrics(this->font()).height()+2);
    ui->tableView->verticalHeader()->setMinimumSectionSize(QFontMetrics(this->font()).height());
    ui->tableView->horizontalHeader()->setDefaultSectionSize(QFontMetrics(this->font()).width('0')*10);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(QFontMetrics(this->font()).width('0')*4);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    //assign gpibdata as model for ui->tableview
    ui->tableView->setModel(gpibdata);

    //fill the possible values in the selectors
    for (int i=0;i<gpibdata->columnCount();i++) {
        ui->XComboBox->addItem(QString::number(i+1)+": "+gpibdata->data(gpibdata->index(0,i)).toString());
        ui->curveColumComboBox->addItem(QString::number(i+1)+": "+gpibdata->data(gpibdata->index(0,i)).toString());
    }

    //set the default values in selectors with respect to the mode:
    if (mode==MainWindow::modeNetwork) {
        ui->XComboBox->setCurrentIndex(ui->XComboBox->count()-1);
    }
    ui->curveColumComboBox->setCurrentIndex(1);

    curveListModel=new PlotCurveModel(&curveList);
    ui->listView->setModel(curveListModel);
    connect(ui->listView,SIGNAL(activated(QModelIndex)),
            SLOT(currentCurveIndexChanged(QModelIndex)));
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),
            SLOT(currentCurveIndexChanged(QModelIndex)));

}

setupCurvesDialog::~setupCurvesDialog()
{
    delete ui;
    delete plot;
    delete gpibdata;
    delete curveListModel;
}

void setupCurvesDialog::changeEvent(QEvent *e)
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

void setupCurvesDialog::setBottomTitle(QString title) {
    QwtText t(title);
    QFont font;
    font.setBold(true);
    t.setFont(font);
    plot->setAxisTitle(plot->xBottom,t);
}

void setupCurvesDialog::setLeftTitle(QString title) {
    QwtText t(title);
    QFont font;
    font.setBold(true);
    t.setFont(font);
    t.setColor(QColor(Qt::blue));
    plot->setAxisTitle(plot->yLeft,t);

}

void setupCurvesDialog::setRightTitle(QString title) {
    QwtText t(title);
    QFont font;
    font.setBold(true);
    t.setFont(font);
    t.setColor(QColor(Qt::red));
    plot->setAxisTitle(plot->yRight,t);
}

void setupCurvesDialog::addCurve() {

    PlotCurve *curve = new PlotCurve(
            ui->XComboBox->currentIndex(),
            ui->curveColumComboBox->currentIndex(),
            this->gpibdata);
    QPen pen;

    QBrush brush(Qt::SolidPattern);

    QwtSymbol symbol(
            QwtSymbol::Triangle,        //default is the triangle
            QBrush(Qt::black),          //default color
            QPen(Qt::NoPen),            //empty pen
            QSize(7,7));                //hard-coded size


    switch(ui->curveYComboBox->currentIndex()) {

    case 1: //if yRight is selected
        plot->enableAxis(plot->yRight);
        plot->setAxisAutoScale(plot->yRight);
        pen.setColor(Qt::red);
        brush.setColor(Qt::red);
        curve->setYAxis(Plot::yRight);
        break;
    default: //if yLeft is selected
        pen.setColor(Qt::blue);
        brush.setColor(Qt::blue);
        break;
    }
    //pen.setWidthF(0.5);
    symbol.setBrush(brush);
    curve->setSymbol(symbol);
    curve->setName(ui->curveLabel->text());
    curve->setPen(pen);
    curve->attach(this->plot);
    curveListModel->appendCurve(curve);
    plot->replot();
}

void setupCurvesDialog::currentCurveIndexChanged(QModelIndex index) {
    ui->curvePropertiesBox->setEnabled(index.isValid());
    const PlotCurve *curve = curveList.at(index.row());
    ui->currentCurveTitle->setText(curve->getName());
    colorBox.setCurrentIndex(colorBox.colorIndex(curve->pen().color()));
    ui->widthSpinBox->blockSignals(true);
    ui->widthSpinBox->setValue(curve->pen().widthF());
    ui->widthSpinBox->blockSignals(false);
    symbolBox.setCurrentIndex(symbolBox.symbolIndex(curve->symbol().style()));

    if (curve->yAxis()==QwtPlot::yLeft) {
        ui->curveLeftRadio->setChecked(true);
    } else {
        ui->curveRightRadio->setChecked(true);
    }
}

void setupCurvesDialog::currentSymbolChanged(int index) {
    PlotCurve *curve =  curveList[ui->listView->currentIndex().row()];
    QwtSymbol symbol(
            symbolBox.selectedSymbol(), //current symbol
            QBrush(colorBox.color()),   //current color
            QPen(Qt::NoPen),                     //empty pen
            QSize(7,7));                //hard-coded size
    curve->setSymbol(symbol);
    plot->replot();
}

void setupCurvesDialog::currentColorChanged(int index) {
    PlotCurve *curve =  curveList[ui->listView->currentIndex().row()];
    QPen pen(curve->pen());
    pen.setColor(colorBox.color());
    curve->setPen(pen);
    QwtSymbol symbol=curve->symbol();
    symbol.setBrush(colorBox.color());
    curve->setSymbol(symbol);
    plot->replot();
}

void setupCurvesDialog::currentWidthChanged(double width) {
    if (width<0.49) return;
    if (ui->listView->currentIndex().row()>=curveList.size()) return;

    PlotCurve *curve =  curveList[ui->listView->currentIndex().row()];

    QPen pen(curve->pen());
    pen.setWidthF(width);
    curve->setPen(pen);
    plot->replot();
}
