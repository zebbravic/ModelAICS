#include "userstack.h"
#include <QCoreApplication>

userSession::userSession(QHostAddress userAddress, QObject *parent) : QObject(parent)
{
    this->userAddress=userAddress;
    this->timer=new QTimer(this);
    this->timer->start(180*1000);
    this->userChannel= new channelList(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerRunOutSlot()));
}
userSession::~userSession()
{
    delete this->timer;
    delete this->userChannel;
}


QHostAddress userSession::getUserAddress()
{
    return this->userAddress;
}

void userSession::addChannel(logicChannel *newChannel)
{
    this->userChannel->appendActChannel(newChannel);
    this->timer->start();
}

void userSession::deleteAllChannel()
{
    this->userChannel->clearActChannelList();
}

void userSession::deleteChannel(quint16 channel)
{
    this->userChannel->deleteActChannel(channel);
}

void userSession::deleteLastChannel()
{
    this->userChannel->clearLastChannel();
}
quint8 userSession::getUserStackState()
{
    return this->userChannel->getCurrentState();
}

void userSession::timerRunOutSlot()
{
    emit this->timerRunOut(this->userAddress);
}

userStack::userStack(QObject *parent) : QObject(parent)
{

}
userStack::~userStack()
{
    while(this->sessionList.size()>0)
    {
        delete this->sessionList.last();
        this->sessionList.removeLast();
    }
}
void userStack::deleteUser(QHostAddress user)
{
    int i = 0;
    while(i<this->sessionList.size()&&this->sessionList[i]->getUserAddress()!=user)
    {
        i++;
    }
    if(i!=this->sessionList.size())
    {
        delete this->sessionList.at(i);
        this->sessionList.removeAt(i);
    }
    if(this->sessionList.size()==0)
    {
        QProcess getContrParams;
        //qDebug()<<"entered initController()";
        QString exec = "php";
        QStringList params;
        params << "actChannel.php" << "i.indexPage";
        getContrParams.start(exec, params);
            getContrParams.waitForFinished();
    }
}

void userStack::addUser(QHostAddress user)
{
    int i =0;
    while(i<this->sessionList.size()&&this->sessionList.at(i)->getUserAddress()!=user)
    {
        i++;
    }
    if(i==this->sessionList.size())
    {
        userSession* newUser = new userSession(user);
        connect(newUser,SIGNAL(timerRunOut(QHostAddress)),this,SLOT(deleteUser(QHostAddress)));
        this->sessionList.append(newUser);
    }
}
void userStack::addChannel(QHostAddress user, logicChannel *newChannel, bool clearPrevious)
{
    int i=0;
    while(i<this->sessionList.size()&&this->sessionList.at(i)->getUserAddress()!=user)
    {
        i++;
    }
    if(i!=this->sessionList.size())
    {
        if(clearPrevious)
        {
            this->sessionList.at(i)->deleteAllChannel();
        }
        this->sessionList.at(i)->addChannel(newChannel);
    }
}

bool userStack::getPermission(QHostAddress user)
{
    bool permission=true;
    int i=0;
    while(i<this->sessionList.size())
    {
        if(this->sessionList.at(i)->getUserStackState()==1 && this->sessionList.at(i)->getUserAddress()!=user)
        {
            permission=false;
        }
       // qDebug()<<this->sessionList.at(i)->getUserStackState();
//        qDebug()<<this->sessionList.at(i)->getUserAddress();
//        qDebug()<<permission;
        i++;
    }
    return permission;
}
void userStack::deleteChannel(QHostAddress user, quint16 channel)
{
    int i=0;
    while(i<this->sessionList.size()&&this->sessionList.at(i)->getUserAddress()!=user)
    {
        i++;
    }
    if(i!=this->sessionList.size())
    {
        this->sessionList.at(i)->deleteChannel(channel);
    }
}

void userStack::deleteLastChannel(QHostAddress user)
{
    int i=0;
    while(i<this->sessionList.size()&&this->sessionList.at(i)->getUserAddress()!=user)
    {
        i++;
    }
    if(i!=this->sessionList.size())
    {
        this->sessionList.at(i)->deleteLastChannel();
    }
}

void userStack::deleteAllChannel(QHostAddress user)
{
    int i=0;
    while(i<this->sessionList.size()&&this->sessionList.at(i)->getUserAddress()!=user)
    {
        i++;
    }
    if(i!=this->sessionList.size())
    {
        this->sessionList.at(i)->deleteAllChannel();
    }
}
