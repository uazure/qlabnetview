#include "setupcurvesdialog.h"
#include "ui_setupcurvesdialog.h"
#include "plot.h"
#include "qwt_text.h"
#include "gpibdata.h"
#include "plotcurve.h"
#include "qwt_legend.h"

setupCurvesDialog::setupCurvesDialog(
        QStringList head, GpibData* parentData, int mode) :
    QDialog(),
    ui(new Ui::setupCurvesDialog)
{
    ui->setupUi(this);
    for (int i = 0 ; i< head.size(); i++) {
        ui->plainTextEdit->appendPlainText(head.at(i));
        //scroll the widget to beginning
    }
    ui->plainTextEdit->scroll(0,-1024);
    plot = new Plot(this);
    plot->enableAxis(plot->yRight);
    ui->plotLayout->addWidget(plot);

    //plot axis titles changes
    connect(ui->plotBottomAxisTitle,SIGNAL(textChanged(QString)),this,SLOT(setBottomTitle(QString)));
    connect(ui->plotLeftAxisTitle,SIGNAL(textChanged(QString)),this,SLOT(setLeftTitle(QString)));
    connect(ui->plotRightAxisTitle,SIGNAL(textChanged(QString)),this,SLOT(setRightTitle(QString)));

    connect(ui->addCurveButton,SIGNAL(clicked()),SLOT(addCurve()));

    //make lock checkbox affect the X axis selector
    connect(ui->XLockCheckbox,SIGNAL(toggled(bool)),ui->XComboBox,SLOT(setDisabled(bool)));
    ui->XLockCheckbox->setChecked(true);
    ui->XComboBox->setDisabled(true);


    //set the x axis label to "t, sec"
    ui->plotBottomAxisTitle->setText("t, sec");
    plot->setAxisScale(plot->yLeft,0,8,2);
    plot->setAxisScale(plot->yRight,0,760);



    //create a gpibdata instance and fill it with head data
    gpibdata=new GpibData;
    gpibdata->appendAsciiData(head);

    //set the vertival header of the table to have height of the current font height+1 (for border)
    ui->tableView->verticalHeader()->setDefaultSectionSize(QFontMetrics(this->font()).height()+1);
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


}

setupCurvesDialog::~setupCurvesDialog()
{
    delete ui;
    delete plot;
    delete gpibdata;
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
    PlotCurve *curve = new PlotCurve(ui->XComboBox->currentIndex(),ui->curveColumComboBox->currentIndex(),this->gpibdata);
    plot->setAxisAutoScale(plot->xBottom);
    plot->setAxisAutoScale(plot->yLeft);

    curve->attach(this->plot);

    plot->replot();

}
