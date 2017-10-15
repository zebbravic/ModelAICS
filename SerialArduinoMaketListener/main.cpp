#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QProcess>
#include <QTcpSocket>
#include <QTcpServer>
#include "serialportreaders.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
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

    qDebug() << "endtext";
    return a.exec();
}
