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
       connect(server, SIGNAL(newConnection()), this, SLOT(newuser()));
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
    qDebug() << readers.size();
    QCoreApplication::processEvents();

}


void SerialPortReaders::newuser()
{
    QTcpSocket* clientSocket=server->nextPendingConnection();
    recieverSock=clientSocket;
    connect(recieverSock,SIGNAL(readyRead()),this, SLOT(slotReadClient()));
}

void SerialPortReaders::slotReadClient()
{
    QString buff = recieverSock->readAll();
    qDebug()<<buff;
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
    }
    recieverSock->close();
}

