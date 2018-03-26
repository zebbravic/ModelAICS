#ifndef SERIALPORTREADERS_H
#define SERIALPORTREADERS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include "serialportreader.h"
#include "userstack.h"
#include "channellist.h"
class SerialPortReaders : public QObject
{
    Q_OBJECT
public:
    SerialPortReaders(QObject *parent = nullptr);
private slots:
    void newTCPUser();
    void slotReadClient();
    void slotDisconnected();
private:
    void getTcpCommand(QString command);
    QTcpServer *server;
    QTcpSocket *recieverSock;
    QList <SerialPortReader*> readers;
    userStack stack;
};

#endif // SERIALPORTREADERS_H
