#ifndef USERSTACK_H
#define USERSTACK_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include <QThread>
#include <QProcess>
#include "channellist.h"


class userSession : public QObject
{
    Q_OBJECT
public:
    explicit userSession(QHostAddress userAddress, QObject *parent = 0);
    ~userSession();
    QHostAddress getUserAddress();
    void addChannel(logicChannel* newChannel);
    void deleteAllChannel();
    void deleteChannel(quint16 channel);
    void deleteLastChannel();
    quint8 getUserStackState();
    QTimer* timer;
signals:
    void timerRunOut(QHostAddress user);
public slots:
    void timerRunOutSlot();
private:

    QHostAddress userAddress;
    channelList* userChannel;

};

class userStack : public QObject
{
    Q_OBJECT
public:
    explicit userStack(QObject *parent = 0);
    ~userStack();
    void addUser(QHostAddress user);
    void addChannel(QHostAddress user, logicChannel* newChannel, bool clearPrevious =false);
    void deleteAllChannel(QHostAddress user);
    void deleteChannel(QHostAddress user,quint16 channel);
    void deleteLastChannel(QHostAddress user);
    bool getPermission(QHostAddress user);
signals:

public slots:
    void deleteUser(QHostAddress user);
private:
    QList<userSession*> sessionList;
};

#endif // USERSTACK_H
