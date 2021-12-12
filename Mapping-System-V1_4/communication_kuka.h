#ifndef COMMUNICATION_KUKA_H
#define COMMUNICATION_KUKA_H

#include <QObject>
#include <QTcpSocket>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QCoreApplication>
#include <QTime>
#include <QDebug>
#include <QAbstractSocket>
#include <QThread>
#include <QtMath>

class communication_Kuka : public QObject
{
    Q_OBJECT
public:
    explicit communication_Kuka (QObject * parent = 0);
    QByteArray formatMsg(QByteArray msg, unsigned short idMsg);
    QByteArray formatMsg(QByteArray msg, QByteArray value, unsigned short idMsg);
    unsigned short clearMsg(QByteArray msg, QByteArray &value);

    bool stopped;
    QString posicaoKuka;

    QByteArray readKUKAvariable(QByteArray variable);

private:
    //QString positionString(int x, int y, int z, int a, int b, int c);
    const int Timeout = 5 * 1000;
    QString positionString(int x, int y, int z, int a, int b, int c);

signals:
    void stateKUKAconnection(QString);
    void triggerDataSTM32(QString);

    //void pedirDadosArduino(QString msg);

public slots:

    void socketTCP_OK();
    void bytesWritten(qint64 bytes);
    void connectionTCP_Kuka();
    void disconnectSocket();
    void doLoop(int x_MIN, int y_MIN, int z_MIN, int ywa_MIN, int pitch_MIN,
                int x_MAX, int y_MAX, int z_MAX,int ywa_MAX, int pitch_MAX,
                int x_INC, int y_INC, int z_INC,int ywa_INC, int pitch_INC);
    void upRobotposition();
    void confirmCompletlydata(bool stopped);



private:
    QTcpSocket * socket;
};

#endif // COMMUNICATION_KUKA_H
