#ifndef SERIALPORTREADERS_H
#define SERIALPORTREADERS_H

#include <QObject>
#include "serialportreader.h"

class SerialPortReaders : public QObject
{
    Q_OBJECT
public:
    SerialPortReaders(QObject *parent = nullptr);
private slots:
    void newuser();
    void slotReadClient();
private:
    void getTcpCommand(QString command);
    QTcpServer *server;
    QTcpSocket *recieverSock;
    QList <SerialPortReader*> readers;

};

#endif // SERIALPORTREADERS_H
