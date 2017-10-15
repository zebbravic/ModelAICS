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
    connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortReader::handleError);
}

SerialPortReader::~SerialPortReader()
{
}

void SerialPortReader::handleReadyRead()
{

    qDebug()<<"read";
    m_readData.append(m_serialPort->readAll());

    if (m_readData.isEmpty()) {
        //m_standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(m_serialPort->portName()) << endl;
    } else {
        //m_standardOutput << QObject::tr("Data successfully received from port %1").arg(m_serialPort->portName()) << endl;
        //m_standardOutput << m_readData << endl;
        while((m_readData.size()>2 && !parseBegin)||(parseBegin && m_readData.size()>= messageInBuffer.opsSize))
        {
        if(m_readData.size()>2 && !parseBegin)
        {

            messageInBuffer.adress = m_readData[0];
            messageInBuffer.code = m_readData[1];
            switch (messageInBuffer.code) {
            case CODE_REBOOTED:
                messageInBuffer.opsSize=1;
                break;
            case CODE_NEED_DATA:
                messageInBuffer.opsSize=1;
                break;
            case CODE_END_SYNC:
                messageInBuffer.opsSize=2;
                break;
            default:
                messageInBuffer.opsSize=1;
                break;
            }
            parseBegin=true;
            m_readData.remove(0,2);
            qDebug()<<"succesfully recieved to "<<messageInBuffer.adress<<" message with code "<<messageInBuffer.code<<" with operands "<<messageInBuffer.operands;
        }
        if(parseBegin && m_readData.size()>= messageInBuffer.opsSize)
        {
            messageInBuffer.operands.resize(messageInBuffer.opsSize);
            for(int i=0;i<messageInBuffer.opsSize;i++)
            {
                messageInBuffer.operands[i]=m_readData[i];
            }
            m_readData.remove(0,messageInBuffer.opsSize);
            parseBegin=false;
            parseMessage(messageInBuffer);
            qDebug()<<"succesfully recieved to "<<messageInBuffer.adress<<" message with code "<<messageInBuffer.code<<" with operands "<<messageInBuffer.operands;

        }
        }
    }

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
    default:
        break;
    }
    }
            else
    {
            resetPort();
}
}

void SerialPortReader::initController(quint8 address)
{
    adress = address;
    QProcess getContrParams;
    qDebug()<<"entered initController()";
    QString exec = "php";
    QStringList params;
    params << "getActuators.php" << QString::number(address);
    getContrParams.start(exec, params);
    getContrParams.waitForFinished(); // sets current thread to sleep and waits for pingProcess end
    QString output(getContrParams.readAllStandardOutput());
    QStringList outputList = output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    qDebug()<<outputList;
    msg messageToWrite;
    messageToWrite.adress=address;
    messageToWrite.code=CODE_INIT_SYNC;
    messageToWrite.opsSize=2;
    messageToWrite.operands.resize(2);
    messageToWrite.operands[0]=0;
    messageToWrite.operands[1]=0;
    writeMessage(messageToWrite);
    for(int i = 0; i<outputList.size();i++)
    {
        QString str=outputList[i];
        QStringList params=str.split(" ", QString::SkipEmptyParts);
        switch (params[0].toInt()) {
        case 0:
            messageToWrite.code=CODE_INIT_DOUT;
            messageToWrite.operands="";
            messageToWrite.operands+=params[2].toInt();//pin
            messageToWrite.operands+=params[3].toInt();//channel
//            messageToWrite.operands+=params[4].toInt();//mode
            break;
        case 1:
            messageToWrite.code=CODE_INIT_METADOUT;
            messageToWrite.operands="";
            messageToWrite.operands+=params[2].toInt();//channel
            messageToWrite.operands+=params[3].toInt();//mode
            messageToWrite.operands+=params[4].toInt();//red
            for(int j=5;j<params.size();j++)
                messageToWrite.operands+=params[j].toInt();
            break;
        case 2:
            messageToWrite.code=CODE_INIT_SEGM;
            messageToWrite.operands="";
            messageToWrite.operands+=params[2].toInt();//pin
            messageToWrite.operands+=params[3].toInt();//size
            messageToWrite.operands+=params[4].toInt();//num
            break;
        case 3:
            messageToWrite.code=CODE_INIT_LENT;
            messageToWrite.operands="";
            messageToWrite.operands+=params[2].toInt();//channel
            messageToWrite.operands+=params[3].toInt();//mode
            messageToWrite.operands+=params[4].toInt();//red
            messageToWrite.operands+=params[5].toInt();//green
            messageToWrite.operands+=params[6].toInt();//blue
            messageToWrite.operands+=params[7].toInt();//segCnt
            for(int j=8;j<params.size();j++)
                messageToWrite.operands+=params[j].toInt();
            break;
        default:
            break;
        }
        writeMessage(messageToWrite);
    }
    messageToWrite.adress=adress;
    messageToWrite.code=CODE_END_SYNC;
    messageToWrite.opsSize=1;
    messageToWrite.operands="";
    messageToWrite.operands+=outputList.size();
    writeMessage(messageToWrite);
}
void SerialPortReader::actChannel(int channel)
{
	msg message;
	message.adress = 255;
	message.code = CODE_ACT_CNL;
	message.opsSize = 1;
	message.operands += channel;
	writeMessage(message);

}
void SerialPortReader::deactChannel(int channel)
{
	msg message;
	message.adress = 255;
	message.code = CODE_DEACT_CNL;
	message.opsSize = 1;
	message.operands += channel;
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
    messageAsArr+=message.operands;
    m_serialPort->write(messageAsArr);
    qDebug()<<messageAsArr;
}

void SerialPortReader::resetPort()
{
    m_serialPort->close();
    parseBegin=false;
    messageInBuffer.adress=0;
    messageInBuffer.code=0;
    messageInBuffer.operands="";
    m_readData="";
    qDebug()<<"restarted";
    m_serialPort->open(QIODevice::ReadWrite);
}
