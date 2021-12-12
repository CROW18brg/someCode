#include "communication_stm32.h"


communication_STM32::communication_STM32(QObject *parent) : QObject(parent)
{
    /* Instanciate Objects */
    serial_STM32 = new QSerialPort;

    /**
     * @brief Connection  QT SIGNAL (readyRead) emited to a SLOT (datareceived_STM32).
     *
     */
    connect(serial_STM32,&QSerialPort::readyRead, this, &communication_STM32::datareceived_STM32);



    qDebug() << " Run constructor it's OK!...";
    //this->startSerialPort_STM32();
}

/**
 * @brief Inicialize communication with the SerialPort
 *
 * @details
 * Specific for MAC USBport "tty.usbserial-1410" -> ON WINDOWS is "com3"
 * Set the port speed with 9600 bps
 * Set the null parity bit
 * Set the StopBits to one
 * Set the FlowControl for null
 *
 * @note
 * Commands for see SERIALPORTS on terminal MAC
 * ls /dev/tty.*
 * screen /dev/tty.usbserial-1410 9600  (USB left side)
 * screen  /dev/tty.usbserial-00000000  (USB right side)
 */
void communication_STM32::startSerialPort_STM32()
{
    qDebug() << "Inside 'startUSBCommunication' funtion!...";

    serial_STM32->setPortName("tty.usbserial-00000000");
    serial_STM32->setBaudRate(QSerialPort::Baud115200);
    serial_STM32->setDataBits(QSerialPort::Data8);
    serial_STM32->setParity(QSerialPort::NoParity);
    serial_STM32->setStopBits(QSerialPort::OneStop);
    serial_STM32->setFlowControl(QSerialPort::NoFlowControl);
    serial_STM32->setReadBufferSize(0);                   //limit the number of Bytes read

    if(serial_STM32->open(QIODevice::ReadWrite)){
        emit stateSTM32connection("Connection with STM32F4 is open!...");
        qDebug() << "Serial Opened!...";
     }else
    {
        qDebug() << "Open failed!...";
        emit stateSTM32connection("Connection with STM32F4 failed!...");
    }
}
/**
 * @brief Close Serial port connection with STM32F4.
 * @param - none.
 * @return -none.
 */
void communication_STM32::closeSTM32connection()
{
    if(serial_STM32->open(QIODevice::ReadWrite))
    {
        serial_STM32->close();
        qDebug() << "STM32 communication is closed!...";
    }else
    {
         qDebug() << "Fail to close STM32F4 connection!...";
    }
}
/**
 * @brief This is a SLOT and send a data request to STM32F407.
 * @note The signal <readyRead> call a SLOT <datareceived_STM32> to Get data.
 * @param msg.
 * @return - none.
 */
void communication_STM32::requestDataSTM32(QString msg)
{
        if(serial_STM32->isOpen()) {
            qDebug() << "Request is done from Kuka to STM32!..";
            emit stateSTM32connection("STM32F4 data request!..");
            serial_STM32->write(msg.toUtf8());
        }
}

/**
 * @brief This is a SLOT Receive data from STM32F407, after that
 * send a SIGNAL to saveData CLASS and UI text plain.
 * @param - none.
 * @return - none.
 */
void communication_STM32::datareceived_STM32()
{
    while(serial_STM32->canReadLine())
    {
        QString data = QString(serial_STM32->readLine());
        qDebug() << "Dados: " << data;
        data.remove(QRegExp("[\n\t\r]"));       // remove the carriage return to
        emit stateSTM32connection("Data from STM32: " + data);        // SIGNAL sended to ui plainText
        emit sendDataSTM32Tofile(data);                     // SIGNAL sended to save data
    }
}

