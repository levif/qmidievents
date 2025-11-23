/* Copyright (C) 2022-2023 Cédric Le Dillau
 * SPDX-License-Identifier: MIT
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef MIDILISTENER_H
#define MIDILISTENER_H

#include <QThread>
#include <QMidiIn.h>
#include "QMidiEventDecoder.h"

class QMidiListener : public QThread
{
    Q_OBJECT
public:
    explicit QMidiListener(QObject *parent = nullptr);
    QMidiListener(QMidiIn* in, QObject *parent = nullptr);
    QMidiEventDecoder m_eventRaw;

private:
    QMidiIn* midi_in;

private slots:
    void onMidiEvent(quint32 message, quint32 timing);
    void onMidiSysExEvent(QByteArray data);

    void onMidiProgramChange(quint8 channel, quint32 newprogram, quint32 oldprogram=0);
    void onMidiNoteOn(quint8 channel, quint8 note, quint8 velocity);
    void onMidiPitchWheel(quint8 channel, quint16 value);
    void onMidiControlChange(quint8 channel, quint8 controller, quint8 value);

protected:
    void run();
};

#endif // MIDILISTENER_H
