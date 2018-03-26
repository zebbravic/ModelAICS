#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QFile>
#include <QProcess>
#include <QTcpSocket>
#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "serialportreaders.h"
#include "channellist.h"
#include "userstack.h"

int main(int argc, char *argv[])
{
     QCoreApplication a(argc, argv);
     QFile log("SAML.log");
     if(log.open(QIODevice::ReadWrite|QIODevice::Append))
     {
         QTextStream writeStream(&log); // Создаем объект класса QTextStream
         writeStream << "\r\n\r\n\r\nSAML starts at "<<QDateTime::currentDateTime().toString()<<"\r\n"; // Посылаем строку в поток для записи
         log.close(); // Закрываем файл
     }
    SerialPortReaders readers;
    //qDebug() << connectedSerial.size();
    QFile file("pid");
    if(QFile::exists("pid"))
        QFile("pid").remove();
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
        QCoreApplication::exit(1);
    else
    {
        QTextStream writeStream(&file); // Создаем объект класса QTextStream
        writeStream << QCoreApplication::applicationPid(); // Посылаем строку в поток для записи
        file.close(); // Закрываем файл
    }



    //qDebug() << "endtext";
    return a.exec();
}
