/* Copyright (C) 2022-2023 Cédric Le Dillau
 * SPDX-License-Identifier: MIT
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <stdio.h>
#include <QMidiFile.h>
#include <QDebug>

#include "QMidiListener.h"

QMidiListener::QMidiListener(QObject *parent)
    : QThread{parent}
{

}

QMidiListener::QMidiListener(QMidiIn* in, QObject *parent)
    : QThread{parent}
{
    midi_in = in;
    QObject::connect(&m_eventRaw, SIGNAL(midiProgramChange(quint8,quint32,quint32)),
                     this, SLOT(onMidiProgramChange(quint8,quint32,quint32)));
    QObject::connect(&m_eventRaw, SIGNAL(midiNoteOn(quint8,quint8,quint8)),
                     this, SLOT(onMidiNoteOn(quint8,quint8,quint8)));
    QObject::connect(&m_eventRaw, SIGNAL(midiPitchWheel(quint8,quint16)),
                     this, SLOT(onMidiPitchWheel(quint8,quint16)));
    QObject::connect(&m_eventRaw, SIGNAL(midiControlChange(quint8,quint8,quint8)),
                     this, SLOT(onMidiControlChange(quint8,quint8,quint8)));
}
void QMidiListener::onMidiEvent(quint32 message, quint32 timing)
{
    QMidiEvent event;
    event.setMessage(message);

    if ((message & 0xff) != 0xfe)
        m_eventRaw.decode(message, timing);
}

void QMidiListener::onMidiSysExEvent(QByteArray data)
{
    qDebug() << "sys message" << Qt::hex << data.toHex();
}

void QMidiListener::onMidiProgramChange(quint8 channel, quint32 newprogram, quint32 oldprogram)
{
    qDebug() << "onProgramChange(channel=" << channel
             << ", program=" << newprogram
             << ", oldprogram=" << oldprogram << ")";
}

void QMidiListener::onMidiNoteOn(quint8 channel, quint8 note, quint8 velocity)
{
    qDebug() << "onNoteOn(channel=" << channel
             << ", note=" << note
             << ", velocity=" << velocity << ")";
}

void QMidiListener::onMidiPitchWheel(quint8 channel, quint16 value)
{
    qDebug() << "onPitchWheel(channel=" << channel
             << ", value=" << value << ")";
}

void QMidiListener::onMidiControlChange(quint8 channel, quint8 controller, quint8 value)
{
    qDebug() << "onControlChange(channel=" << channel
             << ", controller=" << controller
             << ", value=" << value << ")";
}

void QMidiListener::run()
{
    QElapsedTimer t;
    t.start();
    midi_in->start();
    if (!midi_in->isConnected()) {
        qWarning() << "Not connected to device";
        return;
    }
    for(;;) {
        sleep(50);
    }

    if (midi_in->isConnected()) {
        midi_in->stop();
        midi_in->disconnect();
    }
}


