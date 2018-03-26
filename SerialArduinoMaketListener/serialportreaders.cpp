#include "serialportreaders.h"

SerialPortReaders::SerialPortReaders(QObject *parent): QObject(parent)
{

    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            QSerialPort* sPort = new QSerialPort(port);
            //sPort.setPort(port);
            bool opened = sPort->open(QIODevice::ReadWrite);
            //qDebug()<<connectedSerial[i]->portName()<<connectedSerial[i]->baudRate();
            if(!opened)
            {
//                qDebug()<<"error" <<connectedSerial[i]->error();
//                connectedSerial.removeAt(i);
//                i--;
            }
            else
            {
                SerialPortReader* portReader = new SerialPortReader(sPort);
                readers+=portReader;
            }
            //connectedSerial+= sPort;
        }
    server = new QTcpServer(this);
       connect(server, SIGNAL(newConnection()), this, SLOT(newTCPUser()));
       if (!server->listen(QHostAddress::Any, 33333)) {
//           qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
//           ui->textinfo->append(tcpServer->errorString());
           QCoreApplication::exit(1);}
//       } else {
//           server_status=1;
//           qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
//           ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));
//           qDebug() << QString::fromUtf8("Сервер запущен!");
//   }
//    qDebug() << readers.size();
       QFile log("SAML.log");
       if(log.open(QIODevice::ReadWrite|QIODevice::Append))
       {
           QTextStream writeStream(&log); // Создаем объект класса QTextStream
           writeStream << QDateTime::currentDateTime().toString()<<": [INFO] SerialPortReaders iniciated. Found "<<readers.size()<<" serial readers.\r\n"; // Посылаем строку в поток для записи
           log.close(); // Закрываем файл
       }
    QCoreApplication::processEvents();

}



void SerialPortReaders::newTCPUser()
{
    QTcpSocket* clientSocket=server->nextPendingConnection();
    recieverSock=clientSocket;
    connect(recieverSock,SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    connect(recieverSock,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    QFile log("SAML.log");
    if(log.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QTextStream writeStream(&log); // Создаем объект класса QTextStream
        writeStream << QDateTime::currentDateTime().toString() << ": [INFO] Connected new TCP user\r\n"; // Посылаем строку в поток для записи
        log.close(); // Закрываем файл
    }
}

void SerialPortReaders::slotReadClient()
{
    QString buff = recieverSock->readAll();
    QByteArray buffByte;
    buffByte.append(buff);
    QJsonDocument document = QJsonDocument::fromJson(buffByte);
    QJsonObject root = document.object();
    QHostAddress user;
    user.setAddress(root.value("ipaddress").toString());
    stack.addUser(user);
    QJsonValue actions =root.value("actions");
//    qDebug()<<buffByte;
    if(actions.isArray())
    {
        QJsonArray actionArray = actions.toArray();
        for(int i = 0;i<actionArray.count();i++)
        {
            QJsonObject action = actionArray.at(i).toObject();
//            qDebug()<<action.value("action").toString()<<"  "<<action.value("action").toString().toInt();
            switch (action.value("action").toString().toInt()) {
            case 1:
            {
                QJsonArray staticMembers = action.value("members").toArray();
                qDebug()<<"members"<<action.value("members").toString();
                if(stack.getPermission(user))
                {
                QList<quint16> members;
                for(int k=0;k<staticMembers.count();k++)
                {
                    for(int j=0;j<readers.size();j++)
                    {
                        readers[j]->actChannel(staticMembers.at(k).toString().toInt());
                        qDebug()<<"member" <<staticMembers.at(k).toString();
                    }
                    members.append(staticMembers.at(k).toString().toInt());
                }
                logicChannel* channel = new logicChannel(action.value("blocking").toString().toInt(),action.value("channel").toString().toInt(),members);
                stack.addChannel(user,channel);
                }
                QFile log("SAML.log");
                if(log.open(QIODevice::ReadWrite|QIODevice::Append))
                {
                    QTextStream writeStream(&log); // Создаем объект класса QTextStream
                    writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Recieved message with code 1(actChannel) from ip address "<<user.toString()<<" for channel "<<action.value("channel").toString()<<"\r\n"; // Посылаем строку в поток для записи
                    log.close(); // Закрываем файл
                }

                break;
            }
            case 2:
            {
                QJsonArray staticMembers = action.value("staticMembers").toArray();

                for(int k=0;k<staticMembers.count();k++)
                {
                    for (int j = 0; j<readers.size(); j++)
                    {
                        readers[j]->deactChannel(staticMembers.at(k).toString().toInt());
                    }
                }
                stack.deleteChannel(user,action.value("channel").toString().toInt());
                QFile log("SAML.log");
                if(log.open(QIODevice::ReadWrite|QIODevice::Append))
                {
                    QTextStream writeStream(&log); // Создаем объект класса QTextStream
                    writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Recieved message with code 2(deactChannel) from ip address "<<user.toString()<<" for channel "<<action.value("channel").toString()<<"\r\n"; // Посылаем строку в поток для записи
                    log.close(); // Закрываем файл
                }

                break;
            }
            case 4:
            {
                if(stack.getPermission(user))
                {
                for (int j = 0; j<readers.size(); j++)
                {
                    readers[j]->deactAll();
                }
                    stack.deleteAllChannel(user);
                 }
                QFile log("SAML.log");
                    if(log.open(QIODevice::ReadWrite|QIODevice::Append))
                    {
                        QTextStream writeStream(&log); // Создаем объект класса QTextStream
                        writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Recieved message with code 4(deactAllChannel) from ip address "<<user.toString()<<"\r\n"; // Посылаем строку в поток для записи
                        log.close(); // Закрываем файл
                    }

                break;
            }
            case 0:
            {
                QJsonObject obj;
                obj["allow"]=stack.getPermission(user)? "true" : "false";
                QJsonDocument doc(obj);
                recieverSock->write(doc.toJson());
                recieverSock->close();
                QFile log("SAML.log");
                if(log.open(QIODevice::ReadWrite|QIODevice::Append))
                {
                    QTextStream writeStream(&log); // Создаем объект класса QTextStream
                    writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Recieved message with code 0(getPermission) from ip address "<<user.toString()<<"\r\n"; // Посылаем строку в поток для записи
                    writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Returned to ip address "<<user.toString()<<doc.toJson()<<"\r\n"; // Посылаем строку в поток для записи
                    log.close(); // Закрываем файл
                }
                break;
            }
            default:
                break;
            }
        }
    }
    /*qDebug()<<buff;
    qDebug() << readers.size() << readers[0]->m_serialPort->portName() << readers[0]->m_serialPort->baudRate()<<readers[0]->m_serialPort->bytesAvailable();
    QStringList sepBuff = buff.split(';');
    for(int i=0;i<sepBuff.size();i++)
    {
        QStringList tBuff = sepBuff[i].split(' ');
		if (tBuff[0].toInt() == 1)
		{
			for (int j = 0; j<readers.size(); j++)
			{
				readers[j]->actChannel(tBuff[1].toInt());
			}
		}
		if (tBuff[0].toInt() == 2)
		{
			for (int j = 0; j<readers.size(); j++)
			{
				readers[j]->deactChannel(tBuff[1].toInt());
			}
		}
		if (tBuff[0].toInt() == 3)
		{
			for (int j = 0; j<readers.size(); j++)
			{
				readers[j]->deactLast();
			}
		}
		if (tBuff[0].toInt() == 4)
		{
			for (int j = 0; j<readers.size(); j++)
			{
				readers[j]->deactAll();
			}
		}
    }*/
    recieverSock->close();
}


void SerialPortReaders::slotDisconnected()
{
    QFile log("SAML.log");
    if(log.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QTextStream writeStream(&log); // Создаем объект класса QTextStream
        writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Socket closed\r\n"; // Посылаем строку в поток для записи
        log.close(); // Закрываем файл
    }
}
