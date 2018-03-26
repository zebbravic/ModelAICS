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

#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QByteArray>
#include <QObject>
#include <QtDebug>
#include <QThread>
#include <QProcess>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QTest>
#include "constants.h"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

struct msg
{
    quint8 adress;
    quint8 code;
    quint16 opsSize;
    QByteArray operands;
};


//#define CODE_REBOOTED 0
//#define CODE_NEED_DATA 1

//#define CODE_INIT_SYNC 2
//#define CODE_SYNC 3
//#define CODE_END_SYNC 4
//#define CODE_BLOCK_ERR 5

//// group Pin Modes Codes

//#define CODE_INIT_DOUT 16
//#define CODE_INIT_METADOUT 17
//#define CODE_INIT_SEGM 18
//#define CODE_INIT_LENT 19

//#define CODE_ACT_CNL 24
//#define CODE_DEACT_CNL 25
//#define CODE_DEACT_ALL 26
//#define CODE_DEACT_LAST 27


//#define LENTMODE_ONE_FLOAT 0
//#define LENTMODE_FILL 1
//#define LENTMODE_BLINK 2
//#define LENTMODE_WAVE 3


//#define DOUT_MODE_CONST 0
//#define DOUT_MODE_BLINK 1
//#define DOUT_MODE_PWM 2



class SerialPortReader : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortReader(QSerialPort *serialPort, QObject *parent = nullptr);
    ~SerialPortReader();
        void actChannel(int channel);
		void deactChannel(int channel);
		void deactLast();
		void deactAll();
        QSerialPort *m_serialPort;

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void TimerRunout();

private:
    int adress;

    QByteArray  m_readData;
    QTextStream m_standardOutput;
    msg messageInBuffer;
    bool parseBegin=false;
    void parseMessage(msg message);
    void initController(quint8 address);
    void writeMessage(msg message);
    void resetPort();
    void loopPing(QByteArray ops);
    QTimer* timer;

};



#endif
