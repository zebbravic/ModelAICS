#include "channellist.h"


//logic channel methods defining
logicChannel::logicChannel(quint8 blocking, quint16 number, QList<quint16> members, QObject *parent ) : QObject(parent)
{
    this->blocking = blocking;
    this->number = number;
    this->staticMembers  = members;
}
logicChannel::~logicChannel()
{

}

quint8 logicChannel::getBlocking()
{
    return this->blocking;
}

void logicChannel::setBlocking(quint8 blocking)
{
    this->blocking = blocking;
}


quint16 logicChannel::getNumber()
{
    return this->number;
}

void logicChannel::setNumber(quint16 number)
{
    this->number = number;
}

void logicChannel::addMember(quint16 member)
{
    this->staticMembers.append(member);
}
QList<quint16> logicChannel::getMembers()
{
    return this->staticMembers;
}

void  logicChannel::clearMembers()
{
    this->staticMembers.clear();
}


//channel list methods defining
channelList::channelList(QObject *parent) : QObject(parent)
{

}
channelList::~channelList()
{
    this->clearActChannelList();
}
quint8 channelList::getCurrentState()
{
    return this->currentState;
}

void channelList::setCurrentState(quint8 newState)
{
    this->currentState=newState;
}

QList<logicChannel*> channelList::getActChannelList()
{
    return this->actChannelList;
}
void channelList::clearActChannelList()
{
    while(this->actChannelList.size()>0)
    {
        this->clearLastChannel();
    }
}

void channelList::appendActChannel(logicChannel* newChannel)
{

    int i=0;
    while(i<this->actChannelList.size()&&this->actChannelList.at(i)!=newChannel)
    {
        i++;
    }
    if(i==this->actChannelList.size())
    {
        this->actChannelList.append(newChannel);

        this->checkChannelsState();
    }

}

void channelList::deleteActChannel(quint16 channel)
{
    int i=0;
    while(i<this->actChannelList.size() && this->actChannelList.at(i)->getNumber()!=channel)
    {
        i++;
    }
    if(i!=this->actChannelList.size())
    {
        delete this->actChannelList.at(i);
        this->actChannelList.removeAt(i);
        this->checkChannelsState();
    }
}

void channelList::clearLastChannel()
{
    if(this->actChannelList.size()!=0)
    {
        delete this->actChannelList.last();
        this->actChannelList.removeLast();
        this->checkChannelsState();
    }
}

void channelList::checkChannelsState()
{
    int i=0;
    this->currentState=1;
    while(i<this->actChannelList.size())
    {
        if(this->actChannelList.at(i)->getBlocking()==0)
        {
            this->currentState=0;
        }
        i++;
    }
}
