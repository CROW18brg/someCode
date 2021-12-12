#include "communication_aurora.h"

communication_Aurora::communication_Aurora(QObject *parent) : QObject(parent)
{
    /* Instanciate Objects */
    serial_Aurora = new QSerialPort;
    /**
     * @brief Connection  QT SIGNAL (readyRead) emited to a SLOT (datareceived_Aurora).
     *
     */
    connect(serial_Aurora,&QSerialPort::readyRead, this, &communication_Aurora::datareceived_Aurora);
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
void communication_Aurora::startSerialPort_Aurora()
{
    qDebug() << "Inside 'startUSBCommunication' funtion!...";

    serial_Aurora->setPortName("tty.usbserial-00002214");
    serial_Aurora->setBaudRate(QSerialPort::Baud9600);
    serial_Aurora->setDataBits(QSerialPort::Data8);
    serial_Aurora->setParity(QSerialPort::NoParity);
    serial_Aurora->setStopBits(QSerialPort::OneStop);
    serial_Aurora->setFlowControl(QSerialPort::NoFlowControl);
    serial_Aurora->setReadBufferSize(0);                   //limit the number of Bytes read

    if(serial_Aurora->open(QIODevice::ReadWrite)){
        emit stateAuroraconnection("Connection with Aurora is open!...");
        qDebug() << "Serial Opened!...";
     }else
    {
        qDebug() << "Open failed!...";
        emit stateAuroraconnection("Connection with Aurora is failed!...");
    }
}
/**
 * @brief This is a SLOT and send a data request to Aurora Medical.
 * @param msg.
 * @return - none.
 */
void communication_Aurora::requestDataAurora(QString msg)
{
    if(serial_Aurora->isOpen()) {
        qDebug() << "Request is done";
        emit stateAuroraconnection("STM32F4 data request!..");
        serial_Aurora->write(msg.toUtf8());
    }
}
/**
 * @brief This is a SLOT Receive data from Aurora, after that
 * send a SIGNAL to saveData CLASS and UI text plain.
 * Another SIGNAL to Kuka to confirm data reception.
 * @param - none.
 * @return - none.
 */
void communication_Aurora::datareceived_Aurora()
{
    QByteArray data = serial_Aurora->readAll();
    charBuffer.append(data);

    //check Frame read into a structure until newline received. data.startsWith("030A") &&
    if (data.contains("\r"))
    {
        QString myString(charBuffer);       //convert to String
        if(myString.contains("+") || myString.contains("-")){
            qDebug()<< "Data contains '+' or '-' caratere!...";
            handle_1.q0 = myString.mid(4,6);
            handle_1.qx = myString.mid(10,6);
            handle_1.qy = myString.mid(16,6);
            handle_1.tx = myString.mid(28,7);
            handle_1.ty = myString.mid(35,7);
            handle_1.tz = myString.mid(42,7);
            handle_1.portState = myString.mid(56,6);
            handle_1.idFrame = myString.mid(62,9);

            handle_2.q0 = myString.mid(74,6);
            handle_2.qx = myString.mid(80,6);
            handle_2.qy = myString.mid(86,6);
            handle_2.tx = myString.mid(98,7);
            handle_2.ty = myString.mid(105,7);
            handle_2.tz = myString.mid(112,7);
            handle_2.portState = myString.mid(126,6);
            handle_2.idFrame = myString.mid(132,9);

            handle_3.q0 = myString.mid(144,6);
            handle_3.qx = myString.mid(150,6);
            handle_3.qy = myString.mid(156,6);
            handle_3.tx = myString.mid(168,7);
            handle_3.ty = myString.mid(175,7);
            handle_3.tz = myString.mid(182,7);
            handle_3.portState = myString.mid(196,6);
            handle_3.idFrame = myString.mid(202,9);

            //qDebug()<< "Data received: "<< charBuffer;
            qDebug()<< "**************** Data Tool Nº1 ******************";
            qDebug()<< "Tool nº1 -> q0 value is: "  << handle_1.q0;
            qDebug()<< "Tool nº1 -> qx value is: "  << handle_1.qx;
            qDebug()<< "Tool nº1 -> qy value is: "  << handle_1.qy;
            qDebug()<< "Tool nº1 -> tx value is: "  << handle_1.tx;
            qDebug()<< "Tool nº1 -> ty value is: "  << handle_1.ty;
            qDebug()<< "Tool nº1 -> tz value is: "  << handle_1.tz;
            qDebug()<< "Tool nº1 -> port state value is: "  << handle_1.portState;
            qDebug()<< "Tool nº1 -> idFrame value is: "  << handle_1.idFrame;

            qDebug()<< "**************** Data Tool Nº2 ******************";
            qDebug()<< "Tool nº2 -> q0 value is: "  << handle_2.q0;
            qDebug()<< "Tool nº2 -> qx value is: "  << handle_2.qx;
            qDebug()<< "Tool nº2 -> qy value is: "  << handle_2.qy;
            qDebug()<< "Tool nº2 -> tx value is: "  << handle_2.tx;
            qDebug()<< "Tool nº2 -> ty value is: "  << handle_2.ty;
            qDebug()<< "Tool nº2 -> tz value is: "  << handle_2.tz;
            qDebug()<< "Tool nº2 -> port state value is: "  << handle_2.portState;
            qDebug()<< "Tool nº2 -> idFrame value is: "  << handle_2.idFrame;

            qDebug()<< "**************** Data Tool Nº3 ******************";
            qDebug()<< "Tool nº3 -> q0 value is: "  << handle_3.q0;
            qDebug()<< "Tool nº3 -> qx value is: "  << handle_3.qx;
            qDebug()<< "Tool nº3 -> qy value is: "  << handle_3.qy;
            qDebug()<< "Tool nº3 -> tx value is: "  << handle_3.tx;
            qDebug()<< "Tool nº3 -> ty value is: "  << handle_3.ty;
            qDebug()<< "Tool nº3 -> tz value is: "  << handle_3.tz;
            qDebug()<< "Tool nº3 -> port state value is: "  << handle_3.portState;
            qDebug()<< "Tool nº3 -> idFrame value is: "  << handle_3.idFrame;

            QString tool1Dt = handle_1.q0 + "," + handle_1.qx + "," + handle_1.qy + "," + handle_1.tx + "," +
                    handle_1.ty + "," + handle_1.tz + "," + handle_1.portState + "," + handle_1.idFrame + ",";
            QString tool2Dt = handle_2.q0 + "," + handle_2.qx + "," + handle_2.qy + "," + handle_2.tx + "," +
                    handle_2.ty + "," + handle_2.tz + "," + handle_2.portState + "," + handle_2.idFrame + ",";
            QString tool3Dt = handle_3.q0 + "," + handle_3.qx + "," + handle_3.qy + "," + handle_3.tx + "," +
                    handle_3.ty + "," + handle_3.tz + "," + handle_3.portState + "," + handle_3.idFrame;

            qDebug()<< "data Tool 1 concatenated" << tool1Dt;
            qDebug()<< "data Tool 2 concatenated" << tool2Dt;
            qDebug()<< "data Tool 3 concatenated" << tool3Dt;

            QString allDt;
            allDt.append(tool1Dt).append(tool2Dt).append(tool3Dt);
            qDebug()<< "All data concatenated" << allDt;
            emit sendResponse(allDt);

        }else{
            qDebug()<< "Data not contains '+' or '-' caratere!...";
        }
        qDebug()<< "Data received: "<< charBuffer;
        charBuffer = "";
    }
}
/**
 * @brief This SLOT is only for simulate Aurora data send.
 */
void communication_Aurora::testRequestDataAurora()
{
    qDebug()<< "Request data to Aurora is Done!...";
    //emit stringFORtest("1200,1200,1200,1200,1200,");
    emit sendResponse("1200,1200,1200,1200,1200,");
}
