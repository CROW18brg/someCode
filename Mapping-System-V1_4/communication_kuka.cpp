#include "communication_kuka.h"


communication_Kuka::communication_Kuka(QObject * parent) :
    QObject(parent)
{
    stopped = true;
}

/**
 * @brief This is a slot that make the socket with the robot.
 * @param - none.
 * @return - none.
 * SIGNAL <connect(ui->connectKuka, &QPushButton::clicked, robotKuka, &communication_Kuka::connectionTCP_Kuka);>
 */
void communication_Kuka::connectionTCP_Kuka()
{
    QString IPadressKuka = "192.168.10.254";
    emit stateKUKAconnection("Connection with IP: " + IPadressKuka);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(socketTCP_OK()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(socketTCP_NOK()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost(IPadressKuka, 7000);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
        QString error = socket->errorString();
        emit stateKUKAconnection(error);
    }
}

void communication_Kuka::disconnectSocket()
{
    qDebug() << "Disconnected SOCKET";
    emit stateKUKAconnection("Socket TCP is disconnected!...");
    if(socket->isOpen()) {
        socket->disconnectFromHost();
    }
}
/**
 * @brief This is a SLOT for the main LOOP Program.
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
void communication_Kuka::doLoop(int x_MIN, int y_MIN, int z_MIN, int ywa_MIN, int pitch_MIN,
                                int x_MAX, int y_MAX, int z_MAX, int ywa_MAX, int pitch_MAX,
                                int x_INC, int y_INC, int z_INC, int ywa_INC, int pitch_INC)
{
    qDebug() << "Position x_MIN" << x_MIN;
    qDebug() << "Position y_MIN" << y_MIN;
    qDebug() << "Position z_MIN" << z_MIN;
    qDebug() << "Position ywa_MIN" << ywa_MIN;
    qDebug() << "Position pitch_MIN" << pitch_MIN;

    qDebug() << "Position x_MAX" << x_MAX;
    qDebug() << "Position y_MAX" << y_MAX;
    qDebug() << "Position z_MAX" << z_MAX;
    qDebug() << "Position ywa_MAX" << ywa_MAX;
    qDebug() << "Position pitch_MAX" << pitch_MAX;


    qDebug() << "Position x_INC" << x_INC;
    qDebug() << "Position y_INC" << y_INC;
    qDebug() << "Position z_INC" << z_INC;
    qDebug() << "Position ywa_INC" << ywa_INC;
    qDebug() << "Position pitch_INC" << pitch_INC;

    qDebug() << "LOOP";

//for (int x=x_MIN; x<=x_MAX; x+=x_INC){
//  for (int y=y_MIN; y<=y_MAX; y+=y_INC){
    int aux = 360;
    for(int i = 0; i <= aux ; i += 10){

        float a = -i/6;
        qreal y = 35 * qSin((qreal(qDegreesToRadians(double(i)))));
        qreal x = 35 * qCos((qreal(qDegreesToRadians(double(i)))));
          for (int z=z_MIN; z<=z_MAX; z+=z_INC){


//    for (int a=ywa_MIN; a<=ywa_MAX; a+= ywa_INC){
//        for (int b=pitch_MIN; b<=pitch_MAX; b+= pitch_INC){
//             for (int c=c_min; c<=c_max; c+= inc_c){
                socket->write(formatMsg("AI_STATUS","FALSE",43981));
                QByteArray value = readKUKAvariable("AI_STATUS");

                qDebug() << "Message Read AI_STATUS" << value;

      /*********** IF AI_POS = true Loop keeping in this while,
                * if AI_POS = false don't keep in this while  **********/

                while (value == "TRUE") {
                    QThread::msleep(200);
                    qDebug() << "Not ready";
                    value = readKUKAvariable("AI_STATUS");
                }
     /************** AI_POS = false *************************/
                qDebug() << "After While____STart Sending" << value;
                QThread::msleep(1000); //Para garantir que o kuka não treme ao parar numa posição
                //qDebug() << positionString(x,y,z,0,0,0);    ---------->>>>commented
                qDebug() <<"Positions that will be sended------->>" +  positionString(x,y,z,int(a),0,0);
     /************** Writing the values of position *************************/
                //               ( X;Y;Z;ROLL;YAW;PITCH )
                socket->write(formatMsg("AI_POS",positionString(int(x),int(y),z,0,0,320).toUtf8(),43981));//$OV_PRO
                if(!socket->waitForBytesWritten(Timeout)) {
                    qDebug() << "Timeout message sent!";
                    return;
                }
                QThread::msleep(1000);

                //sleep(0.1);
     /************** Go to position *************************/
                socket->write(formatMsg("AI_STATUS","TRUE", 43981));
                if(!socket->waitForBytesWritten(Timeout)){
                    qDebug() << "Timeout message sent!";
                    //return -1;
                }
                qDebug()<<"------------->>>>Send order for go to position!...";

                QByteArray value1 = readKUKAvariable("AI_STATUS");

                qDebug() << "read";
                while ( value1 == "TRUE") {
                    QThread::msleep(200);
                    qDebug() << "Not ready to emit trigger!...";
                    value1 = readKUKAvariable("AI_STATUS");
                }

                //posicaoKuka = QString::number(z) +"\t;" +  QString::number(a) + "\t;" +  QString::number(b) + "\t;" ;
                stopped = true;
                emit triggerDataSTM32("S");
                qDebug()<<"--------------->>>>>Trigger is emited in this moment!...";

                while(stopped) {
                    QThread::msleep(1);
                    qDebug() << "-------------- >>>>  I'm waiting to save data in CSV file!...";
                }
                qDebug()<< "DATA COMPLETLY RECEIVED STM32 AND AURORA SYSTEM!...";
     /*           if(i == 340) {
                    i = 0;
                    a = 0;
                }                   */
//           }
//          }
//         }
          }
        }
//    }
//  }

}


/**
 * @brief This is a SLOT to send arm robot to up position
 */
void communication_Kuka::upRobotposition()
{
    socket->write(formatMsg("AI_STATUS","FALSE",43981));     // put AI_STATUS in FALSE
    QByteArray value = readKUKAvariable("AI_STATUS");

    while (value == "TRUE") {
        QThread::msleep(200);
        qDebug() << "Not ready";
        value = readKUKAvariable("AI_STATUS");
    }
    qDebug() << "After While____STart Sending" << value;
    QThread::msleep(1000); //Para garantir que o kuka não treme ao parar numa posição

    qDebug() << "Arm up position sended: " + positionString(100,50,-600,0,0,0);
    //qDebug() << positionString(x,y,z,a,b,c);
    /* socket->write(formatMsg("AI_POS",positionString(x,y,-z,a,b,c).toUtf8(),43981));//$OV_PRO   */
    socket->write(formatMsg("AI_POS",positionString(100,50,-400,0,0,0).toUtf8(),43981));//$OV_PRO

    QThread::msleep(1000);      // wait one second

    if(!socket->waitForBytesWritten(Timeout)) {
        qDebug() << "Timeout message sent!";
        return; }
    /* Put AI_STATUS value TRUE to go for position */
    socket->write(formatMsg("AI_STATUS","TRUE", 43981));

    if(!socket->waitForBytesWritten(Timeout)){
        qDebug() << "Timeout message sent!";
        //return -1;
    }
    qDebug() << "Sent";

    stopped = true;   //----------->>>> commented
    QThread::msleep(1000);  //Para não efetuar leituras no primeiro movimento do robot

    socket->write(formatMsg("AI_POS",positionString(0,0,0,0,0,0).toUtf8(),43981));//$OV_PRO
    QThread::msleep(1000);
    socket->write(formatMsg("AI_STATUS","TRUE", 43981));
}
/**
 * @brief This is  SLOT receive confirmation data from strain sensores <class communication_STM32>
 * @param completly.
 * @return none.
 */
void communication_Kuka::confirmCompletlydata(bool state)
{
    emit stateKUKAconnection("Stopped is turn to FALSE!..." );
    stopped = state;
}

/**
 * @brief This is a SLOT that validate KUKA scoket.
 * @param none.
 * @return none.
 */
void communication_Kuka::socketTCP_OK()
{
    qDebug() << "connected...";
    emit stateKUKAconnection("Socket TCP is ready!...");

}
/**
 * @brief Receive the bytes from kuka communication.
 * @param bytes.
 * @return none.
 */
void communication_Kuka::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

/**
 * @brief Method to read the status of register AI_STATUS.
 * @param msg.
 * @param idMsg.
 * @return QByteArray.
 */
QByteArray communication_Kuka::formatMsg(QByteArray msg, unsigned short idMsg){

    const char READVARIABLE=0;

    QByteArray header, block;
    int lunghezza,varNameLen;
    unsigned char hByte, lByte;
    unsigned char hByteMsg,lByteMsg;

    varNameLen=msg.size();
    hByte=(varNameLen & 0xff00) >> 8;
    lByte=(varNameLen & 0x00ff);

    block.append(READVARIABLE).append(hByte).append(lByte).append(msg);
    lunghezza=block.size();

    hByte=(lunghezza & 0xff00) >> 8;
    lByte=(lunghezza & 0x00ff);

    // Message ID ( MAX: 0xFFFF )
    hByteMsg=(idMsg & 0xff00) >> 8;
    lByteMsg=(idMsg & 0x00ff);

    header.append(hByteMsg).append(lByteMsg).append(hByte).append(lByte);
    block.prepend(header);

    qDebug()<<"Message send:"<<block.toHex();

    return block;
}
/**
 * @brief This Method change the value of AI_STATUS register (FALSE or TRUE).
 * @param msg.
 * @param value.
 * @param idMsg.
 * @return QByteArray.
 */
QByteArray communication_Kuka::formatMsg(QByteArray msg, QByteArray value, unsigned short idMsg){

    const char WRITEVARIABLE=1;

    QByteArray header, block;
    short lunghezza,varNameLen,varValueLen;
    unsigned char hByte, lByte;
    unsigned char hByteMsg,lByteMsg;

    varNameLen=msg.size();
    hByte=(varNameLen & 0xff00) >> 8;
    lByte=(varNameLen & 0x00ff);

    block.append(WRITEVARIABLE).append(hByte).append(lByte).append(msg);

    varValueLen=value.size();
    hByte=(varValueLen & 0xff00) >> 8;
    lByte=(varValueLen & 0x00ff);

    block.append(hByte).append(lByte).append(value);

    lunghezza=block.size();

    hByte=(lunghezza & 0xff00) >> 8;
    lByte=(lunghezza & 0x00ff);

    hByteMsg=(idMsg & 0xff00) >> 8;
    lByteMsg=(idMsg & 0x00ff);

    header.append(hByteMsg).append(lByteMsg).append(hByte).append(lByte);
    block.prepend(header);

    qDebug()<<"Message send:"<<block.toHex();

    return block;
}

unsigned short communication_Kuka::clearMsg(QByteArray msg, QByteArray &value){

    short lenMsg,func,lenValue;
    unsigned short idReadMsg;

    if(msg.size() > 0){
        //Message ID
        idReadMsg=((unsigned char)msg[0])<<8 | ((unsigned char)msg[1]);
        qDebug() << "Message ID: " << idReadMsg;

        //Message Length
        lenMsg=((unsigned char)msg[2])<<8 | ((unsigned char)msg[3]);
        qDebug() << "Message Length:" << lenMsg;

        //Function(read:0/Write:1)
        func=((int)msg[4]);
        qDebug() << "Function(read:0/Write:1) " << func;

        //Value Length
        lenValue=((unsigned char)msg[5])<<8 | ((unsigned char)msg[6]);
        qDebug() << "Value Length:" << lenValue;

        qDebug() << "Message return:" << msg.toHex();

        // the byte7 begin the value
        value = msg.mid(7,lenValue);
        return idReadMsg;

    }
    else{
        value = QByteArray("");
        return 0;
    }
}
/**
 * @brief Read kuka variable.
 * @param variable.
 * @return none.
 */
QByteArray communication_Kuka::readKUKAvariable(QByteArray variable)
{
    socket->write(formatMsg(variable,43981));       // Request the value of AI_POS
    if(!socket->waitForBytesWritten(Timeout))
    {
        qDebug() << "Timeout message sent!";
        return "error" ;
    }

    if (!socket->waitForReadyRead(Timeout)) {
        qDebug() << "Timeout message return!";
        return "error" ;
    }

    qDebug() << "Read Done";
    QByteArray returnMsg = socket->readAll();
    qDebug() << "State AI_STATUS"<< returnMsg;
    QByteArray value;
    unsigned short idMsg = clearMsg(returnMsg,value);
    return value;               // return the value of AI_POS
}

QString communication_Kuka::positionString(int x, int y, int z, int a, int b, int c)
{
    QString s;
    s = "{X " +  QString::number(x) + ", Y "  + QString::number(y) + ", Z "  + QString::number(z) + ", A "  + QString::number(a) + ", B "  + QString::number(b) + ", C "  + QString::number(c) + "}";
    return s;
}
