/****************************************************************************
**
** Copyright (C) 2013 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "serialportreader.h"

#include <QCoreApplication>

QT_USE_NAMESPACE

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
    , m_standardOutput(stdout)
{
    this->timer = new QTimer(this);
    this->timer->start(10*1000);
    resetPort();
    connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortReader::handleError);
    connect(timer,SIGNAL(timeout()),this,SLOT(TimerRunout()));
}

SerialPortReader::~SerialPortReader()
{
}

void SerialPortReader::handleReadyRead()
{

    //qDebug()<<"read";
    m_readData.append(m_serialPort->readAll());
//    qDebug()<<m_readData.size();
    if (m_readData.isEmpty()) {
        //m_standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(m_serialPort->portName()) << endl;
    } else {
        //m_standardOutput << QObject::tr("Data successfully received from port %1").arg(m_serialPort->portName()) << endl;
        //m_standardOutput << m_readData << endl;
        //qDebug()<<m_readData.size();

        while((m_readData.size()>=4 && !parseBegin)||(parseBegin && m_readData.size()>= messageInBuffer.opsSize))
        {
            //qDebug()<<"buffer size"<<m_readData.size()<<" parseBegin "<<parseBegin<<" opsSize "<<messageInBuffer.opsSize;
        if(m_readData.size()>=4 && !parseBegin)
        {

            messageInBuffer.adress = m_readData[0];
            messageInBuffer.code = m_readData[1];
            messageInBuffer.opsSize = m_readData[2]*256+m_readData[3];

//            switch (messageInBuffer.code) {
//            case CODE_REBOOTED:
//                messageInBuffer.opsSize=1;
//                break;
//            case CODE_NEED_DATA:
//                messageInBuffer.opsSize=1;
//                break;
//            case CODE_END_SYNC:
//                messageInBuffer.opsSize=2;
//                break;
//            default:
//                messageInBuffer.opsSize=1;
//                break;
//            }
            parseBegin=true;
            m_readData.remove(0,4);
//            QFile log("SAML.log");
//            if(log.open(QIODevice::ReadWrite|QIODevice::Append))
//            {
//                QTextStream writeStream(&log); // Создаем объект класса QTextStream
//                writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Started recieved from serial "<<messageInBuffer.adress<<" message with code "<<messageInBuffer.code<<" with size "<<messageInBuffer.opsSize<<"\r\n"; // Посылаем строку в поток для записи
//                log.close(); // Закрываем файл
//            }
//            qDebug()<<"succesfully recieved to "<<messageInBuffer.adress<<" message with code "<<messageInBuffer.code<<" with operands "<<messageInBuffer.operands;
        }
        if(parseBegin && m_readData.size()>= messageInBuffer.opsSize)
        {
            messageInBuffer.operands.resize(messageInBuffer.opsSize);
            for(int i=0;i<messageInBuffer.opsSize;i++)
            {
                messageInBuffer.operands[i]=m_readData[i];
            }
            m_readData.remove(0,messageInBuffer.opsSize);
//            qDebug()<<"size after parse"<< m_readData.size();
            parseBegin=false;
            parseMessage(messageInBuffer);
            QFile log("SAML.log");
            if(log.open(QIODevice::ReadWrite|QIODevice::Append))
            {
                QTextStream writeStream(&log); // Создаем объект класса QTextStream
                writeStream << QDateTime::currentDateTime().toString()<<": [INFO] Finished recieving from serial "<<messageInBuffer.adress<<" message with code "<<messageInBuffer.code<<" with size "<<messageInBuffer.opsSize<<" operands:";
                for(int i = 0;i<messageInBuffer.operands.size();i++)
                {
                    writeStream << static_cast<quint8>(messageInBuffer.operands.at(i))<<" "; // Посылаем строку в поток для записи
                }
                writeStream<<"\r\n";
                log.close(); // Закрываем файл
            }
            qDebug()<<"succesfully recieved to "<<messageInBuffer.adress<<" message with code "<<messageInBuffer.code<<" with operands "<<messageInBuffer.operands;

        }
        //qDebug()<<"at end buffer size"<<m_readData.size()<<" parseBegin "<<parseBegin<<" opsSize "<<messageInBuffer.opsSize;
        }
    }
//    qDebug()<<"out of handle";

}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}

void SerialPortReader::parseMessage(msg message)
{
    if(message.adress==0)
    {
    switch (message.code) {
    case CODE_NEED_DATA:
        initController(message.operands[0]);
        break;
    case CODE_LOOP_PING:
        loopPing(message.operands);
        break;
    default:
        break;
    }
    }
            else
    {
            resetPort();
}
}

void SerialPortReader::loopPing(QByteArray ops)
{
    this->timer->start();
//    unsigned long millis = ((ops[0]<<24) + (ops[1]<<16) +(ops[2]<<8) +ops[3]);
    QFile log("SAML.log");
    if(log.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QTextStream writeStream(&log); // Создаем объект класса QTextStream
        unsigned long millis = (ops[1]<<24)+(ops[2]<<16)+(ops[3]<<8)+ops[4];
        int mem = (ops[5]<<8)+ops[6];
        writeStream << QDateTime::currentDateTime().toString() << ": [INFO] Recieved ping from "<<this->adress<<". Current uptime in millis "<<millis<<" and free ram "<<mem<<" \r\n"; // Посылаем строку в поток для записи
        log.close(); // Закрываем файл
    }

}


void SerialPortReader::TimerRunout()
{
    this->resetPort();
}
void SerialPortReader::initController(quint8 address)
{
    adress = address;

    QProcess getContrParams;
    //qDebug()<<"entered initController()";
    QString exec = "php";
    QStringList params;

    params << "getActuatorsCnt.php" << QString::number(address);
    getContrParams.start(exec, params);
    getContrParams.waitForFinished(); // sets current thread to sleep and waits for pingProcess end
    QString output(getContrParams.readAllStandardOutput());
    QByteArray buffByte;
    buffByte.append(output);
    QJsonDocument document = QJsonDocument::fromJson(buffByte);
    QJsonObject root = document.object();
//    qDebug()<<output;
    msg messageToWrite;
    messageToWrite.adress=address;
    messageToWrite.code=CODE_INIT_SYNC;
    messageToWrite.opsSize=2;
    messageToWrite.operands.resize(2);
    messageToWrite.operands[0]=0;
    messageToWrite.operands[1]=0;
    messageToWrite.operands+=(((root.value("dOutCnt").toString().toUInt())>>8)&0xFF);
    messageToWrite.operands+=(((root.value("dOutCnt").toString().toUInt()))&0xFF);
    messageToWrite.operands+=(((root.value("metaDOutCnt").toString().toUInt())>>8)&0xFF);
    messageToWrite.operands+=(((root.value("metaDOutCnt").toString().toUInt()))&0xFF);
    messageToWrite.operands+=(((root.value("dOutRandomizerCnt").toString().toUInt())>>8)&0xFF);
    messageToWrite.operands+=(((root.value("dOutRandomizerCnt").toString().toUInt()))&0xFF);
    messageToWrite.operands+=(((root.value("segmentCnt").toString().toUInt())>>8)&0xFF);
    messageToWrite.operands+=(((root.value("segmentCnt").toString().toUInt()))&0xFF);
    messageToWrite.operands+=(((root.value("lentCnt").toString().toUInt())>>8)&0xFF);
    messageToWrite.operands+=(((root.value("lentCnt").toString().toUInt()))&0xFF);
    messageToWrite.opsSize=messageToWrite.operands.count();
    writeMessage(messageToWrite);

    QStringList params1;
    params1 << "getActuators.php" << QString::number(address);
    getContrParams.start(exec, params1);
    getContrParams.waitForFinished(); // sets current thread to sleep and waits for pingProcess end
    QString output1(getContrParams.readAllStandardOutput());
    buffByte="";
    buffByte.append(output1);
    document = QJsonDocument::fromJson(buffByte);
    root = document.object();
//    QStringList outputList = output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    //qDebug()<<outputList;
    messageToWrite.adress=address;
    messageToWrite.code=CODE_INIT_SYNC;
    messageToWrite.opsSize=2;
    messageToWrite.operands.resize(2);
    messageToWrite.operands[0]=0;
    messageToWrite.operands[1]=0;
    writeMessage(messageToWrite);

    QJsonArray actuators = root.value("actuators").toArray();
    for(int i = 0; i<actuators.count();i++)
    {
        QJsonObject actuator = actuators.at(i).toObject();
//        QString str=outputList[i];
//        QStringList params=str.split(" ", QString::SkipEmptyParts);
        switch (actuator.value("type").toString().toInt()) {
        case 0:
            messageToWrite.code=CODE_INIT_DOUT;
            messageToWrite.operands="";
//            messageToWrite.operands+=params[2].toUInt();//pin
//            messageToWrite.operands+=((params[3].toUInt()>>8)&0xFF);//num HighByte
//            messageToWrite.operands+=((params[3].toUInt())&0xFF);//num LowByte
//            messageToWrite.operands+=params[4].toUInt();//mode

            messageToWrite.operands+=actuator.value("pin").toString().toUInt();
            messageToWrite.operands+=((actuator.value("num").toString().toUInt()>>8)&0xFF);
            messageToWrite.operands+=((actuator.value("num").toString().toUInt())&0xFF);
            messageToWrite.opsSize=messageToWrite.operands.size();
            break;
        case 1:
            messageToWrite.code=CODE_INIT_METADOUT;
            messageToWrite.operands="";
//            messageToWrite.operands+=((params[2].toUInt()>>8)&0xFF);//channel HighByte
//            messageToWrite.operands+=((params[2].toUInt())&0xFF);//channel LowByte
//            messageToWrite.operands+=params[3].toUInt();//mode
//            messageToWrite.operands+=params[4].toUInt();//doutCnt
//            for(int j=5;j<params.size();j++)
//            {
//                messageToWrite.operands+=((params[j].toUInt()>>8)&0xFF);//doutnum HighByte
//                messageToWrite.operands+=((params[j].toUInt())&0xFF);//doutnum LowByte
//            }
            messageToWrite.operands+=((actuator.value("channel").toString().toUInt()>>8)&0xFF);
            messageToWrite.operands+=((actuator.value("channel").toString().toUInt())&0xFF);
            messageToWrite.operands+=actuator.value("mode").toString().toUInt();
            messageToWrite.operands+=((actuator.value("num").toString().toUInt()>>8)&0xFF);
            messageToWrite.operands+=((actuator.value("num").toString().toUInt())&0xFF);
            if(actuator.keys().contains("segArr"))
            {
                messageToWrite.operands+=(unsigned int)actuator.value("segArr").toArray().count();
                for(int j=0;j<actuator.value("segArr").toArray().count();j++)
                {
                    messageToWrite.operands+=((actuator.value("segArr").toArray().at(j).toString().toUInt()>>8)&0xFF);
                    messageToWrite.operands+=((actuator.value("segArr").toArray().at(j).toString().toUInt())&0xFF);
                }

            }
            else
            {
                messageToWrite.operands.append((char)0x00);
             //   messageToWrite.operands.append((char)0x00);
            }
            if(actuator.keys().contains("segRange"))
            {
                messageToWrite.operands+=(unsigned int)actuator.value("segRange").toArray().count();
                for(int j=0;j<actuator.value("segRange").toArray().count();j++)
                {
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("start").toString().toUInt()>>8)&0xFF);
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("start").toString().toUInt())&0xFF);
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("end").toString().toUInt()>>8)&0xFF);
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("end").toString().toUInt())&0xFF);
                }

            }
            else
            {
                messageToWrite.operands.append((char)0x00);
               // messageToWrite.operands.append((char)0x00);
            }
            messageToWrite.opsSize=messageToWrite.operands.count();
//            qDebug()<<actuator.value("num").toString();
            break;
        case 2:
            messageToWrite.code=CODE_INIT_SEGM;
            messageToWrite.operands="";
//            messageToWrite.operands+=params[2].toUInt();//pin
//            messageToWrite.operands+=params[3].toUInt();//size
//            messageToWrite.operands+=((params[4].toUInt()>>8)&0xFF);//num HighByte
//            messageToWrite.operands+=((params[4].toUInt())&0xFF);//num LowByte

            messageToWrite.operands+=actuator.value("pin").toString().toUInt();
            messageToWrite.operands+=actuator.value("size").toString().toUInt();
            messageToWrite.operands+=((actuator.value("num").toString().toUInt()>>8)&0xFF);
            messageToWrite.operands+=((actuator.value("num").toString().toUInt())&0xFF);

            break;
        case 3:
            messageToWrite.code=CODE_INIT_LENT;
            messageToWrite.operands="";
//            messageToWrite.operands+=((params[2].toUInt()>>8)&0xFF);//channel HighByte
//            messageToWrite.operands+=((params[2].toUInt())&0xFF);//channel LowByte
//            messageToWrite.operands+=params[3].toUInt();//mode
//            messageToWrite.operands+=params[4].toUInt();//red
//            messageToWrite.operands+=params[5].toUInt();//green
//            messageToWrite.operands+=params[6].toUInt();//blue
//            messageToWrite.operands+=params[7].toUInt();//segCnt
//            for(int j=8;j<params.size();j++){
//                messageToWrite.operands+=((params[j].toUInt()>>8)&0xFF);//segnum HighByte
//                messageToWrite.operands+=((params[j].toUInt())&0xFF);//segnum LowByte
//            }
            messageToWrite.operands+=((actuator.value("channel").toString().toUInt()>>8)&0xFF);
            messageToWrite.operands+=((actuator.value("channel").toString().toUInt())&0xFF);
            messageToWrite.operands+=actuator.value("mode").toString().toUInt();
            messageToWrite.operands+=((actuator.value("num").toString().toUInt()>>8)&0xFF);
            messageToWrite.operands+=((actuator.value("num").toString().toUInt())&0xFF);
            messageToWrite.operands+=actuator.value("red").toString().toUInt();
            messageToWrite.operands+=actuator.value("green").toString().toUInt();
            messageToWrite.operands+=actuator.value("blue").toString().toUInt();
            if(actuator.keys().contains("segArr"))
            {
                messageToWrite.operands+=(unsigned int)actuator.value("segArr").toArray().count();
                for(int j=0;j<actuator.value("segArr").toArray().count();j++)
                {
                    messageToWrite.operands+=((actuator.value("segArr").toArray().at(j).toString().toUInt()>>8)&0xFF);
                    messageToWrite.operands+=((actuator.value("segArr").toArray().at(j).toString().toUInt())&0xFF);
                }

            }
            else
            {
                messageToWrite.operands.append((char)0x00);
//                messageToWrite.operands.append((char)0x00);
            }
            if(actuator.keys().contains("segRange"))
            {
                messageToWrite.operands+=(unsigned int)actuator.value("segRange").toArray().count();
                for(int j=0;j<actuator.value("segRange").toArray().count();j++)
                {
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("start").toString().toUInt()>>8)&0xFF);
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("start").toString().toUInt())&0xFF);
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("end").toString().toUInt()>>8)&0xFF);
                    messageToWrite.operands+=((actuator.value("segRange").toArray().at(j).toObject().value("end").toString().toUInt())&0xFF);
                }

            }
            else
            {
                messageToWrite.operands.append((char)0x00);
                //messageToWrite.operands.append((char)0x00);
            }
            messageToWrite.opsSize=messageToWrite.operands.count();
            break;
        case 4:
            messageToWrite.code=CODE_INIT_DOUTRANDOMIZER;
            messageToWrite.operands="";
            messageToWrite.operands+=((actuator.value("metaDOutNum").toString().toUInt()>>8)&0xFF);
            messageToWrite.operands+=((actuator.value("metaDOutNum").toString().toUInt())&0xFF);
            messageToWrite.operands+=actuator.value("randomType").toString().toUInt();
            messageToWrite.operands+=actuator.value("randomMaxGlobal").toString().toUInt();
            messageToWrite.operands+=actuator.value("randomMaxPerDevice").toString().toUInt();
            messageToWrite.operands+=actuator.value("randomTimeout").toString().toUInt();
            messageToWrite.opsSize=messageToWrite.operands.count();
//            qDebug()<<actuator.value("metaDOutNum").toString()<<" "<<actuator.value("randomType").toString()<<" ";
            break;
        default:
            break;
        }
        writeMessage(messageToWrite);
//        qDebug()<<"adres " <<messageToWrite.adress<<" code "<<messageToWrite.code<<" opsSize "<<messageToWrite.opsSize;
    }
    messageToWrite.adress=adress;
    messageToWrite.code=CODE_END_SYNC;
    messageToWrite.opsSize=1;
    messageToWrite.operands="";
    messageToWrite.operands+=actuators.count();
    QFile log("SAML.log");
    if(log.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QTextStream writeStream(&log); // Создаем объект класса QTextStream
        writeStream << QDateTime::currentDateTime().toString() << ": [INFO] initiated Serial"<<this->m_serialPort->portName()<<" controller "<<this->adress <<" with actCnt"<<actuators.count()<<"\r\n"; // Посылаем строку в поток для записи
        log.close(); // Закрываем файл
    }
    writeMessage(messageToWrite);
//    qDebug()<<"adres " <<messageToWrite.adress<<" code "<<messageToWrite.code<<" opsSize "<<messageToWrite.opsSize;
}
void SerialPortReader::actChannel(int channel)
{
	msg message;
	message.adress = 255;
	message.code = CODE_ACT_CNL;
    message.opsSize = 2;
    message.operands += (quint8)(channel>>8)&0xFF;
    message.operands+=(quint8)channel&0xFF;
	writeMessage(message);

}
void SerialPortReader::deactChannel(int channel)
{
	msg message;
	message.adress = 255;
	message.code = CODE_DEACT_CNL;
    message.opsSize = 2;
    message.operands += (channel>>8)&0xFF;
    message.operands+=channel&0xFF;
	writeMessage(message);

}
void SerialPortReader::deactAll()
{
	msg message;
	message.adress = 255;
	message.code = CODE_DEACT_ALL;
	message.opsSize = 0;
	writeMessage(message);

}
void SerialPortReader::deactLast()
{
	msg message;
	message.adress = 255;
	message.code = CODE_DEACT_LAST;
	message.opsSize = 0;
	writeMessage(message);

}

void SerialPortReader::writeMessage(msg message)
{
    QByteArray messageAsArr;
    messageAsArr+=message.adress;
    messageAsArr+=message.code;
    messageAsArr+=(message.opsSize>>8)&0xFF;
    messageAsArr+=message.opsSize&0xFF;
    messageAsArr+=message.operands;
    qDebug()<<messageAsArr;
    m_serialPort->write(messageAsArr);
//    QTest::qWait(100);
//    qDebug()<<messageAsArr;
}

void SerialPortReader::resetPort()
{
    m_serialPort->close();
    parseBegin=false;
    messageInBuffer.adress=0;
    messageInBuffer.code=0;
    messageInBuffer.opsSize=0;
    this->parseBegin=false;
    messageInBuffer.operands="";
    m_readData="";
    QFile log("SAML.log");
    if(log.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QTextStream writeStream(&log); // Создаем объект класса QTextStream
        writeStream << QDateTime::currentDateTime().toString()<<": [WARNING] Restarted serial "<<m_serialPort->portName()<<"\r\n"; // Посылаем строку в поток для записи
        log.close(); // Закрываем файл
    }
    //qDebug()<<"restarted";
    this->timer->start();
    m_serialPort->open(QIODevice::ReadWrite);
}
