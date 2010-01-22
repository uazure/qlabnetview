#ifndef EXPERIMENTCONTROL_H
#define EXPERIMENTCONTROL_H

#include <QDialog>

class gpibSocket;
class MainWindow;
class QStringListModel;
class QSettings;


namespace Ui {
    class experimentControl;
}

class experimentControl : public QDialog {
    Q_OBJECT
public:
    experimentControl(MainWindow *parent = 0, gpibSocket *gpib =0);
    ~experimentControl();

private slots:
    void serverIdleRunning(bool running);
    void serverForbidden(QString message);
    void serverStartStop(bool start);
    void intervalChanged(void);
    void serverInterval(int msec);
    void updateSettings(void);
    void userChangedIndex(int index);
    void userChangedIndex(QString str);
    void userChangedIndex(void);
    void addTemperatureStep(void);
    void removeTemperatureStep(void);
    void clearTemperatureSteps(void);
    void updateWorkplan(QStringList workplan);
    void updateWorkplanIndex(int index);
    void lockWorkplan();
    void nextStep();
    void prevStep();
    void applyWorkplan();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::experimentControl *ui;
    gpibSocket *gpib;
    bool serverRunning;
    QStringListModel *workplanModel;
    QStringList *workplan;
    QSettings *settings;

};

#endif // EXPERIMENTCONTROL_H
