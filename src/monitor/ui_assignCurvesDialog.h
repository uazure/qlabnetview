/********************************************************************************
** Form generated from reading ui file 'assignCurvesDialog.ui'
**
** Created: Mon Nov 9 12:35:02 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASSIGNCURVESDIALOG_H
#define UI_ASSIGNCURVESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_assignCurvesDialog
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;
    QDialogButtonBox *buttonBox;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelX;
    QComboBox *comboBoxX;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelXValue;
    QLineEdit *XAxisLabel;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout;
    QLabel *labelY1;
    QComboBox *comboBoxY1;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelY1Value;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *Y1AxisLabel;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_8;
    QLineEdit *Y1CurveLabel;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelYY2;
    QComboBox *comboBoxY2;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelY2Value;
    QSpacerItem *horizontalSpacer_6;
    QLineEdit *Y2AxisLabel;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_9;
    QLineEdit *Y2CurveLabel;

    void setupUi(QDialog *assignCurvesDialog)
    {
        if (assignCurvesDialog->objectName().isEmpty())
            assignCurvesDialog->setObjectName(QString::fromUtf8("assignCurvesDialog"));
        assignCurvesDialog->resize(640, 480);
        assignCurvesDialog->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(assignCurvesDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        plainTextEdit = new QPlainTextEdit(assignCurvesDialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setUndoRedoEnabled(false);
        plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(assignCurvesDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);

        frame = new QFrame(assignCurvesDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_4 = new QWidget(widget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(widget_4);
        horizontalLayout_4->setMargin(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        labelX = new QLabel(widget_4);
        labelX->setObjectName(QString::fromUtf8("labelX"));

        horizontalLayout_4->addWidget(labelX);

        comboBoxX = new QComboBox(widget_4);
        comboBoxX->setObjectName(QString::fromUtf8("comboBoxX"));

        horizontalLayout_4->addWidget(comboBoxX);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        labelXValue = new QLabel(widget);
        labelXValue->setObjectName(QString::fromUtf8("labelXValue"));

        horizontalLayout_3->addWidget(labelXValue);

        XAxisLabel = new QLineEdit(widget);
        XAxisLabel->setObjectName(QString::fromUtf8("XAxisLabel"));

        horizontalLayout_3->addWidget(XAxisLabel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        horizontalLayout_3->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget_5 = new QWidget(widget_2);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        horizontalLayout = new QHBoxLayout(widget_5);
        horizontalLayout->setMargin(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelY1 = new QLabel(widget_5);
        labelY1->setObjectName(QString::fromUtf8("labelY1"));

        horizontalLayout->addWidget(labelY1);

        comboBoxY1 = new QComboBox(widget_5);
        comboBoxY1->setObjectName(QString::fromUtf8("comboBoxY1"));

        horizontalLayout->addWidget(comboBoxY1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(widget_5);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        labelY1Value = new QLabel(widget_2);
        labelY1Value->setObjectName(QString::fromUtf8("labelY1Value"));

        horizontalLayout_5->addWidget(labelY1Value);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        Y1AxisLabel = new QLineEdit(widget_2);
        Y1AxisLabel->setObjectName(QString::fromUtf8("Y1AxisLabel"));

        horizontalLayout_5->addWidget(Y1AxisLabel);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_8->addWidget(label_2);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);

        Y1CurveLabel = new QLineEdit(widget_2);
        Y1CurveLabel->setObjectName(QString::fromUtf8("Y1CurveLabel"));

        horizontalLayout_8->addWidget(Y1CurveLabel);


        verticalLayout_2->addLayout(horizontalLayout_8);


        gridLayout_2->addWidget(widget_2, 0, 2, 1, 1);

        widget_3 = new QWidget(frame);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_6 = new QWidget(widget_3);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        horizontalLayout_2 = new QHBoxLayout(widget_6);
        horizontalLayout_2->setMargin(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelYY2 = new QLabel(widget_6);
        labelYY2->setObjectName(QString::fromUtf8("labelYY2"));

        horizontalLayout_2->addWidget(labelYY2);

        comboBoxY2 = new QComboBox(widget_6);
        comboBoxY2->setObjectName(QString::fromUtf8("comboBoxY2"));

        horizontalLayout_2->addWidget(comboBoxY2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_3->addWidget(widget_6);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        labelY2Value = new QLabel(widget_3);
        labelY2Value->setObjectName(QString::fromUtf8("labelY2Value"));

        horizontalLayout_6->addWidget(labelY2Value);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        Y2AxisLabel = new QLineEdit(widget_3);
        Y2AxisLabel->setObjectName(QString::fromUtf8("Y2AxisLabel"));

        horizontalLayout_6->addWidget(Y2AxisLabel);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_9->addWidget(label_3);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_9);

        Y2CurveLabel = new QLineEdit(widget_3);
        Y2CurveLabel->setObjectName(QString::fromUtf8("Y2CurveLabel"));

        horizontalLayout_9->addWidget(Y2CurveLabel);


        verticalLayout_3->addLayout(horizontalLayout_9);


        gridLayout_2->addWidget(widget_3, 0, 3, 1, 1);


        gridLayout->addWidget(frame, 1, 0, 1, 1);


        retranslateUi(assignCurvesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), assignCurvesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), assignCurvesDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(assignCurvesDialog);
    } // setupUi

    void retranslateUi(QDialog *assignCurvesDialog)
    {
        assignCurvesDialog->setWindowTitle(QApplication::translate("assignCurvesDialog", "Select columns", 0, QApplication::UnicodeUTF8));
        labelX->setText(QApplication::translate("assignCurvesDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">X</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelXValue->setText(QApplication::translate("assignCurvesDialog", "Axis label:", 0, QApplication::UnicodeUTF8));
        XAxisLabel->setText(QApplication::translate("assignCurvesDialog", "t, sec", 0, QApplication::UnicodeUTF8));
        labelY1->setText(QApplication::translate("assignCurvesDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Y</span><span style=\" font-size:14pt; vertical-align:sub;\">1</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelY1Value->setText(QApplication::translate("assignCurvesDialog", "Axis label:", 0, QApplication::UnicodeUTF8));
        Y1AxisLabel->setText(QApplication::translate("assignCurvesDialog", "T, K", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("assignCurvesDialog", "Curve label:", 0, QApplication::UnicodeUTF8));
        Y1CurveLabel->setText(QApplication::translate("assignCurvesDialog", "sample T", 0, QApplication::UnicodeUTF8));
        labelYY2->setText(QApplication::translate("assignCurvesDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Y</span><span style=\" font-size:14pt; vertical-align:sub;\">2</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelY2Value->setText(QApplication::translate("assignCurvesDialog", "Axis label:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("assignCurvesDialog", "Curve label:", 0, QApplication::UnicodeUTF8));
        Y2CurveLabel->setText(QString());
        Q_UNUSED(assignCurvesDialog);
    } // retranslateUi

};

namespace Ui {
    class assignCurvesDialog: public Ui_assignCurvesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSIGNCURVESDIALOG_H
