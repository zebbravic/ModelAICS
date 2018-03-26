#ifndef CHANNELLIST_H
#define CHANNELLIST_H

#include <QObject>
#include <QDebug>

class logicChannel: public QObject
{
    Q_OBJECT

public:
    explicit logicChannel(quint8 blocking, quint16 number, QList<quint16> members, QObject *parent = 0);
    ~logicChannel();
	quint8 getBlocking();
	void setBlocking(quint8 blocking);
	
	void addMember(quint16 member);
    QList<quint16> getMembers();
	void clearMembers();
	
	quint16 getNumber();
	void setNumber(quint16 number);


private:
    quint8 blocking;
	quint16 number;
    QList<quint16> staticMembers;
};


class channelList: public QObject
{
    Q_OBJECT

public:
    explicit channelList(QObject *parent = 0);
    ~channelList();
    quint8 getCurrentState();
    void setCurrentState(quint8 newState);

    QList<logicChannel*> getActChannelList();
    void clearActChannelList();

    void appendActChannel(logicChannel* newChannel);
    void deleteActChannel(quint16 channel);
    void clearLastChannel();
private:
    void checkChannelsState();
    quint8 currentState=0;
    QList<logicChannel*> actChannelList;
};

#endif // CHANNELLIST_H
