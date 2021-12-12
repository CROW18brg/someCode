#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <QObject>
#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QDebug>
//#include "communication_stm32.h"


class saveData : public QObject
{
    Q_OBJECT
public:
    explicit saveData(QObject *parent = nullptr);
    QString STM32Data;
    QString AuroraData;

public slots:
        void readfile();
        void writeDataSTM32(QString lineData);
        void writeDataAurora(QString lineData);


signals:
        void confirmReceptionToKuka(bool confirmation);
        void triggerDataAurora(QByteArray); //descomentar quando trabalhar com o AURORA
        void triggerDatatest();

private:


};

#endif // SAVEDATA_H
