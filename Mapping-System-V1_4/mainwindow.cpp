
#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow
 * @note Class constructor, start when the application inicialize
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    this->setFixedSize(800,665);        // Fixe the size of Application
//    ui->startMeasure->setEnabled(false);
//    ui->stopMeasure->setEnabled(false);

   time = new QTime;
   workerThread = new QThread(this);

  /*     Instantiate a new object from others classes on mainwindow     */
   STM32F4 = new communication_STM32;
   robotKuka = new communication_Kuka;
   m_Aurora = new communication_Aurora;
   m_saveData = new saveData;
   workerThread = new QThread(this);
   connect(workerThread, &QThread::finished, robotKuka, &QObject::deleteLater);
   robotKuka->moveToThread(workerThread);

   /*   Connections related to the STM32F407 (signal acquisition board)  */
   connect(ui->connectSTM32, &QPushButton::clicked, STM32F4, &communication_STM32::startSerialPort_STM32);
   connect(STM32F4,&communication_STM32::stateSTM32connection,this, &MainWindow::msgStateSTM32port);

   /*    Connections related to Aurora System (data acquisition)     */
   connect(ui->connectAurora, &QPushButton::clicked, m_Aurora, & communication_Aurora::startSerialPort_Aurora);
   connect(m_Aurora, &communication_Aurora::stateAuroraconnection,this, &MainWindow::msgStateAuroraconnection);
   connect(this, &MainWindow::sendCommand,m_Aurora, &communication_Aurora::requestDataAurora);

   /*    Connections related to the robot KUKA  */
   connect(ui->connectKuka, &QPushButton::clicked, robotKuka, &communication_Kuka::connectionTCP_Kuka);
   connect(ui->disconnectkuka, &QPushButton::clicked, robotKuka, &communication_Kuka::disconnectSocket);
   connect(robotKuka,&communication_Kuka::stateKUKAconnection,this, &MainWindow::msgStateKUKAconnection);
   connect(ui->startMeasure,&QPushButton::clicked, this, &MainWindow::runRobotpositions);
   connect(this,&MainWindow::sendPositions,robotKuka,&communication_Kuka::doLoop);
   connect(ui->upARMrobot, &QPushButton::clicked, robotKuka, &communication_Kuka::upRobotposition);


    /*   Connection between STM32, KuKa, Aurora and saveData   */
    /* Lambda funtion to pass stopped = false because is in while loop */
   connect(m_saveData, &saveData::confirmReceptionToKuka, this, [=](){robotKuka->stopped = false;});



   /******* descomentar o 1º connect e commentar o de baixo para trabalhar com o AURORA ******/

    connect(m_saveData, &saveData::triggerDataAurora, m_Aurora, &communication_Aurora::requestDataAurora);
   //connect(m_saveData, &saveData::triggerDatatest, m_Aurora, &communication_Aurora::testRequestDataAurora);

   /*********-------------------------------------------------------------------------*********/

   connect(m_Aurora, &communication_Aurora::sendResponse,m_saveData, &saveData::writeDataAurora);

   connect(STM32F4, &communication_STM32::stateSTM32connection,this, &MainWindow::msgStateSTM32port);
   connect(STM32F4, &communication_STM32::sendDataSTM32Tofile, m_saveData, &saveData::writeDataSTM32);
   connect(robotKuka, &communication_Kuka::triggerDataSTM32,STM32F4, &communication_STM32::requestDataSTM32);


   workerThread->start();

   qDebug() << "BEGIN PROGRAM";
}

MainWindow::~MainWindow()
{
    workerThread->quit();
    delete ui;
    STM32F4->serial_STM32->close();

    //robotKuka->disconnectSocket();
}
/**
 * @brief This is a SLOT that emit a SIGNAL with positons selected on UI.
 */
void MainWindow::runRobotpositions()
{
    emit sendPositions(ui->x_min->value(),ui->y_min->value(),ui->z_min->value(),ui->yaw_min->value(),ui->pitch_min->value(),
                       ui->x_max->value(),ui->y_max->value(),ui->z_max->value(),ui->yaw_max->value(),ui->pitch_max->value(),
                       ui->x_inc->value(),ui->y_inc->value(),ui->z_inc->value(),ui->yaw_step->value(),ui->pitch_inc->value());
}
/**
 * @brief This is a button event to enable TCP connection with Kuka robot.
 * @note When this button is clicked, estabilish a socket with a Kuka robot.
 * SIGNAL << connect(ui->connectKuka, &QPushButton::clicked, robotKuka,
 *           &communication_Kuka::connectionTCP_Kuka); >>
 */
void MainWindow::on_connectKuka_clicked()
{

}

void MainWindow::on_disconnectkuka_clicked()
{

}
/**
 * @brief This is a button event to enable USB connection with STM32F407.
 * @note When this button is clicked, configure serial Port and connect.
 * SIGNAL << connect(ui->connectSTM32, &QPushButton::clicked, STM32F4,
 *          &communication_STM32::startSerialPort_STM32);>>
 */
void MainWindow::on_connectSTM32_clicked()
{

}


void MainWindow::on_connectAurora_clicked()
{
    if(m_Aurora->serial_Aurora->isOpen()){
        m_Aurora->serial_Aurora->close();
        qDebug()<< "Serial Port is closed!...";
        //sendText("Serial Port is closed!...");
    }else{
        qDebug()<< "Can't close Serial Port";
        //sendText("Can't close Serial Port");
    }
}

/**
 * @brief This is a SLOT that called by stateSTM32connection SIGNAL in
 * communication_stm32 class.
 * @note The SLOT send events related STM32F4 to ui->PlainText.
 * @param state
 */
void MainWindow::msgStateSTM32port(const QString state)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTime_text = dateTime.toString();
    //qDebug()<< "Date is: " + dateTime_text;
    ui->plainTextEdit->appendPlainText(state + "\t-----> " + dateTime_text);
}
/**
 * @brief This is a SLOT that called by stateKUKAconnection SIGNAL in
 * communication_KUKA class.
 * @note The SLOT send events related KUKA to ui->PlainText.
 * @param state
 */
void MainWindow::msgStateKUKAconnection(const QString state)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTime_text = dateTime.toString();
    //qDebug()<< "Date is: " + dateTime_text;
    ui->plainTextEdit->appendPlainText(state + "\t-----> " + dateTime_text);
}

void MainWindow::msgStateAuroraconnection(const QString state)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTime_text = dateTime.toString();
    //qDebug()<< "Date is: " + dateTime_text;
    ui->plainTextEdit->appendPlainText(state + "\t-----> " + dateTime_text);
}

void MainWindow::on_startMeasure_clicked()
{

}

void MainWindow::on_stopMeasure_clicked()
{

}

void MainWindow::on_upARMrobot_clicked()
{

}

void MainWindow::on_requestdataSTM32_clicked()
{
     STM32F4->requestDataSTM32("S");
}

void MainWindow::on_closeApp_clicked()
{

    QApplication::quit();
}

void MainWindow::on_requestDataAurora_clicked()
{
   emit sendCommand("TX 0001\r");
}

void MainWindow::on_disconnectSTM32_clicked()
{
   STM32F4->serial_STM32->close();
   QDateTime dateTime = QDateTime::currentDateTime();
   QString dateTime_text = dateTime.toString();
   ui->plainTextEdit->appendPlainText("Connection with STM32F407 is closed!...\t-----> " + dateTime_text);
}
/**
 * @brief This slot have a THread QtConcurrent that
 * make a boot for Aurora device
 */
void MainWindow::on_boot_Aurora_clicked()
{
    // Calling a lambda function
   // Code in this block will run in another thread
       ui->boot_Aurora->setEnabled(false);
       QFuture<void> future = QtConcurrent::run([=]() {
          for(int i = 0; i<1; i++){
             time->start();
             qDebug()<< "value of i = "<< i;
             emit sendCommand("INIT \r");          // Replies ->    "OKAYA896\r"
             QThread::msleep(1800);
             emit sendCommand("BEEP 1\r");           // Replies ->  1D4C1
             QThread::msleep(1000);
             emit sendCommand("PHSR 00\r");       // Replies ->   "030A0010B0010C001C254\r"
             QThread::msleep(2000);
             emit sendCommand("PINIT 0A\r");       // Replies ->    "OKAYA896\r"
             QThread::msleep(4000);
             emit sendCommand("PINIT 0B\r");       // Replies ->    "OKAYA896\r"
             QThread::msleep(4000);
             emit sendCommand("PINIT 0C\r");       // Replies ->    "OKAYA896\r"
             //QThread::sleep(4000);
             //emit sendCommand("PHSR 03\r");
             QThread::msleep(4000);
             emit sendCommand("PENA 0AD\r");
             QThread::msleep(2500);
             emit sendCommand("PENA 0BD\r");
             QThread::msleep(2500);
             emit sendCommand("PENA 0CD\r");
             QThread::msleep(2500);
             emit sendCommand("TSTART 80\r");        // Replies -> OKAY<CRC16><CR>
             QThread::msleep(1800);
          }
           qDebug()<<" Automatic inicialization is finish!...";
           ui->boot_Aurora->setEnabled(true);
           qDebug() << "Startup Time inicilalization AURORA SYSTEM: " << time->elapsed() << "msec";
   });
}

void MainWindow::on_STOP_tracking_clicked()
{
    emit sendCommand("TSTOP \r");           // Replies -> OKAY<CRC16><CR>
}

void MainWindow::on_START_tracking_clicked()
{
    emit sendCommand("TSTART 80\r");        // Replies -> OKAY<CRC16><CR>
}
