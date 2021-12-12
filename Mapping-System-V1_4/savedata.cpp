#include "savedata.h"

saveData::saveData(QObject *parent) : QObject(parent)
{




}

/**
 * @brief This is a SLOT that read a file content of a csv file.
 * @param none.
 * @return none.
 */
void saveData::readfile()
{
    //    QUrl local = QFileDialog::getOpenFileUrl(nullptr, "Ler", QUrl(), "Ficheiro texto *.txt;;Ficheiro excel *.csv");
    //    if(local.isEmpty()) {
    //        return;
    //    }
        QFile file("dados.csv");
        if(!file.exists()) {
            qWarning() << "Does not exits: " << file.fileName();
            return;
        }
        if(file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString text = stream.readAll();
        }
}
/**
 * @brief This is a SLOT that save sensores values from STM32 in csv file.
 * @param lineData.
 * @return none.
 */
void saveData::writeDataSTM32(QString lineData)
{
    //    QUrl local = QFileDialog::getSaveFileUrl(nullptr, "Guardar texto", QUrl(), "Ficheiro texto *.txt;;Ficheiro excel *.csv");
    //    if(local.isEmpty()) {
    //        return;
    //    }
     QFile file("/Users/ruicarvalho/Desktop/dataCoordenates.csv");
     if(file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        lineData.remove(QRegExp("[\n\t\r]"));       // remove the carriage return to
        stream << lineData;
        qDebug()<< "Writing STM32 data in CSV file!...";
        file.close();
       emit triggerDataAurora("TX 0001\r"); //---->>>>> descomentar request data to Aurora medical.
       //emit triggerDatatest();

     }else{
         qDebug()<< "Can't write STM32 data in CSV file!...";
     }
}
/**
 * @brief This is a SLOT that save coordenates values from Aurora Medical
 * and complete the rest o the line .csv file.
 * @param lineData.
 * @return none.
 */
void saveData::writeDataAurora(QString lineData)
{
    qDebug()<< "AURORA DATA--->>" << lineData;
    QFile file("/Users/ruicarvalho/Desktop/dataCoordenates.csv");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << lineData;
        stream << "\n";
        qDebug()<< "Writing Aurora data in CSV file!...";
        file.close();
        emit confirmReceptionToKuka(false);     // put the sttoped = false

    }else{
        qDebug()<< "Can't write Aurora data in file!...";
    }
}






