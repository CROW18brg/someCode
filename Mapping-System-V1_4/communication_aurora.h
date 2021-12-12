#ifndef COMMUNICATION_AURORA_H
#define COMMUNICATION_AURORA_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

class communication_Aurora : public QObject
{
    Q_OBJECT
public:
    explicit communication_Aurora(QObject *parent = nullptr);
     QSerialPort *serial_Aurora;

     struct dataHandle_1{
             QString q0;
             QString qx;
             QString qy;
             QString tx;
             QString ty;
             QString tz;
             QString error;
             QString portState;
             QString idFrame;
     };
     struct dataHandle_2{
             QString q0;
             QString qx;
             QString qy;
             QString tx;
             QString ty;
             QString tz;
             QString error;
             QString portState;
             QString idFrame;
     };
     struct dataHandle_3{
             QString q0;
             QString qx;
             QString qy;
             QString tx;
             QString ty;
             QString tz;
             QString error;
             QString portState;
             QString idFrame;
     };

      dataHandle_1 handle_1;
      dataHandle_2 handle_2;
      dataHandle_3 handle_3;

private:
      QByteArray charBuffer;

signals:
     void stateAuroraconnection(QString);
     void stringFORtest(QString);           // Only for test, later will be change.
     void sendResponse(QString);

public slots:
     void startSerialPort_Aurora();
     void requestDataAurora(QString msg);
     void datareceived_Aurora();

     void testRequestDataAurora();          // Only for test, later will be delete.


};

#endif // COMMUNICATION_AURORA_H
