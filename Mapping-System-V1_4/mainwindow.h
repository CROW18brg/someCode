
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QtCore>
#include <QApplication>
#include <QtDebug>
#include <QtSerialPort>
#include <QTime>
#include <QDateTime>
#include <QThread>
#include <QtConcurrent>
#include "communication_kuka.h"
#include "communication_stm32.h"
#include "communication_aurora.h"
#include "savedata.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief Funtion that senda SIGNAL (sendPositions) with positons values to the robot.
     *
     */
    void runRobotpositions();

private slots:

    void on_connectKuka_clicked();

    void on_disconnectkuka_clicked();

    void on_connectSTM32_clicked();

    void on_connectAurora_clicked();


    void msgStateSTM32port(const QString state);

    void msgStateKUKAconnection(const QString state);

    void msgStateAuroraconnection(const QString state);

    //void on_clearValues_clicked();

    void on_startMeasure_clicked();

    void on_stopMeasure_clicked();

    void on_upARMrobot_clicked();

    void on_requestdataSTM32_clicked();

    void on_closeApp_clicked();



    void on_requestDataAurora_clicked();

    void on_disconnectSTM32_clicked();

    void on_boot_Aurora_clicked();

    void on_STOP_tracking_clicked();

    void on_START_tracking_clicked();

signals:
    /**
         * @brief SIGNAL that send positions from UI to communications_kuka class.
         * @param x_MIN
         * @param y_MIN
         * @param z_MIN
         * @param ywa_MIN
         * @param pitch_MIN
         * @param x_MAX
         * @param y_MAX
         * @param z_MAX
         * @param ywa_MAX
         * @param pitch_MAX
         * @param x_INC
         * @param y_INC
         * @param z_INC
         * @param ywa_INC
         * @param pitch_INC
         */
        void sendPositions(int x_MIN, int y_MIN, int z_MIN, int ywa_MIN, int pitch_MIN,
                           int x_MAX, int y_MAX, int z_MAX,int ywa_MAX, int pitch_MAX,
                           int x_INC, int y_INC, int z_INC,int ywa_INC, int pitch_INC);

        void sendCommand(QByteArray);


private:
    Ui::MainWindow *ui;                         // Pointer for ui application
    communication_Kuka *robotKuka = nullptr;    // Create a null pointer for object
    communication_STM32 *STM32F4 = nullptr;     // Create a null pointer for object
    QThread *workerThread = nullptr;            // Create a null pointer for object
    saveData *m_saveData  = nullptr;            // Create a null pointer for object
    communication_Aurora *m_Aurora = nullptr;   // Create a null pointer for object
    QTime *time = nullptr;
};
#endif // MAINWINDOW_H
