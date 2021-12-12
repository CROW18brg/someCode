#ifndef COMMUNICATION_STM32_H
#define COMMUNICATION_STM32_H

#include <QCoreApplication>
#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QTime>
#include <QThread>
//#include "communication_kuka.h"
//#include "savedata.h"

class communication_STM32 : public QObject
{
    Q_OBJECT
public:
    explicit communication_STM32(QObject *parent = nullptr);
     QSerialPort *serial_STM32;


signals:
     void stateSTM32connection(QString);
     void sendDataSTM32Tofile(QString);


public slots:
     void startSerialPort_STM32();
     void requestDataSTM32(QString msg);
     void closeSTM32connection();
     void datareceived_STM32();


private:

};

#endif // COMMUNICATION_STM32_H
