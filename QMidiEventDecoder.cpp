/* Copyright (C) 2022-2023 Cédric Le Dillau
 * SPDX-License-Identifier: MIT
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "QMidiEventDecoder.h"
#include <QDebug>

QMidiEventDecoder::QMidiEventDecoder(QObject *parent)
    : QObject(parent)
    , m_value(0)
    , m_program(0)
    , m_notifiers(FlagNote | FlagControlChange | FlagPitchWheel)
{
}

void QMidiEventDecoder::decode(quint32 message, quint32 timing)
{
    quint8 command = message & 0xff;
    switch(message & 0xf0)
    {
        /* FlagNote */
    case MidiEvent::NoteOff:
    {
        quint32 channel = message & 0xf;
        quint32 note = (message >> 8) & 0xff;
        quint32 velocity = (message >> 16) & 0xff;
        qDebug() << "NoteOff(channel=" << channel << ", note=" << note << ", velocity=" << velocity << ")";
        if (m_notifiers & FlagNote)
            emit midiNoteOff(channel, note, velocity);
        break;
    }
    case MidiEvent::NoteOn:
    {
        quint8 channel = message & 0xf;
        quint8 note = (message >> 8) & 0xff;
        quint8 velocity = (message >> 16) & 0xff;
        qDebug() << "NoteOn(channel=" << channel << ", note=" << note << ", velocity=" << velocity << ")";
        if (m_notifiers & FlagNote)
            emit midiNoteOn(channel, note, velocity);
        break;
    }
    case MidiEvent::KeyPressure:
    {
        quint32 channel = message & 0xf;
        quint32 note = (message >> 8) & 0xff;
        quint32 pressure = (message >> 16) & 0xff;
        qDebug() << "NoteOn(channel=" << channel << ", note=" << note << ", pressure=" << pressure << ")";
        if (m_notifiers & FlagNote)
            emit midiKeyPressure(channel, note, pressure);
        break;
    }

        /* FlagControlChange */
    case MidiEvent::ControlChange:
    {
        quint8 channel = message & 0xf;
        quint8 controller = (message >> 8) & 0xff;
        quint8 value = (message >> 16) & 0xff;
        qDebug() << "ControlChange(channel=" << channel
                 << ", controller=" << controller
                 << ", value=" << value << ")";
        if (m_notifiers & FlagControlChange)
            emit midiControlChange(channel, controller, value);
        break;
    }

        /* FlagProgramChange */
    case MidiEvent::ProgramChange:
    {
        quint8 channel = message & 0xf;
        quint32 program = (message >> 8) & 0x7f;
        qDebug() << "ProgramChange(" << channel << ", program=" << program << ")";
        if (m_notifiers & FlagProgramChange)
            emit midiProgramChange(channel, program, m_program);
        m_program = program;
        break;
    }

    case MidiEvent::ChannelPressure:
    {
        quint32 channel = message & 0xf;
        qDebug() << "ChannelPressure(" << channel << ")";
        break;
    }

        /* FlagPitchWheel */
    case MidiEvent::PitchWheel:
    {
        quint8 channel = message & 0xf;
        quint16 value = ((message & 0x7f0000) >> 9) | ((message & 0x7f00) >> 8);

        qDebug() << "PitchWheel(" << channel << ", value=" << value << ")";
        if (m_notifiers & FlagPitchWheel)
            emit midiPitchWheel(channel, value);
        break;
    }

    default:
        switch(command) {
        case MidiEvent::SongPosition:
        {
            quint32 beats = (message >> 8) & 0xfff;
            quint32 position = beats * MIDI_CLOCKS_PER_MIDI_BEAT;
            qDebug() << "SetSongPosition(" << position << ")";
            break;
        }
        case MidiEvent::SongSelect:
        {
            quint32 song = (message >> 8) & 0xff;
            qDebug() << "SelectSong(" << song << ")";
            break;
        }
        case MidiEvent::TuneRequest: // Mostly used for sound modules with analog oscillator circuits.
            qDebug() << "TuneRequest()";
            break;
        case MidiEvent::MidiClock: // Every 24 MIDI clocks
            qDebug() << "MidiClock()";
            break;
        case MidiEvent::MidiTick: // Every 10ms
            qDebug() << "MidiTick()";
            break;

        case MidiEvent::SongStart:
            qDebug() << "MidiStart()";
            break;

        case MidiEvent::SongContinue:
            qDebug() << "MidiContinue()";
            break;

        case MidiEvent::SongStop:
            qDebug() << "MidiStop()";
            break;

        case MidiEvent::Sensing: // no activity for 300ms
            qDebug() << "Sensing()";
            break;

        case MidiEvent::Reset:
            qDebug() << "Reset()";
            emit midiReset(); // non-maskabe
            break;

        default:
            qDebug() << "Event " << Qt::hex << message << " not decoded";
        }
    }
}


void QMidiEventDecoder::debug(quint32 message)
{
    quint8 command = message & 0xff;
    switch(message & 0xf0)
    {
    case MidiEvent::NoteOff:
    {
        quint32 channel = message & 0xf;
        quint32 note = (message >> 8) & 0xff;
        quint32 velocity = (message >> 16) & 0xff;
        qDebug() << "NoteOff(channel=" << channel << ", note=" << note << ", velocity=" << velocity << ")";
        break;
    }
    case MidiEvent::NoteOn:
    {
        quint32 channel = message & 0xf;
        quint32 note = (message >> 8) & 0xff;
        quint32 velocity = (message >> 16) & 0xff;
        qDebug() << "NoteOn(channel=" << channel << ", note=" << note << ", velocity=" << velocity << ")";
        break;
    }

    case MidiEvent::ProgramChange:
    {
        quint32 channel = message & 0xf;
        quint32 program = (message >> 8) & 0x7f;
        qDebug() << "ProgramChange(channel=" << channel << ", program=" << program << ")";
        break;
    }
    case MidiEvent::KeyPressure:
    {
        quint32 channel = message & 0xf;
        quint32 note = (message >> 8) & 0xff;
        quint32 pressure = (message >> 16) & 0xff;
        qDebug() << "KeyPressure(channel=" << channel << ", note=" << note << ", pressure=" << pressure << ")";
        break;
    }
    case MidiEvent::ChannelPressure:
    {
        quint32 channel = message & 0xf;
        qDebug() << "ChannelPressure(channel=" << channel << ")";
        break;
    }
    case MidiEvent::PitchWheel:
    {
        quint32 channel = message & 0xf;
        quint32 value = ((message & 0x7f0000) >> 9) | ((message & 0x7f00) >> 8);

        qDebug() << "PitchWheel(channel=" << channel << ", value=" << value << ")";
        break;
    }
    case MidiEvent::ControlChange:
    {
        quint32 channel = message & 0xf;
        quint32 controller = (message >> 8) & 0xff;
        quint32 value = (message >> 16) & 0xff;
        qDebug() << "ControlChange(channel=" << channel
                 << ", controller=" << controller
                 << ", value=" << value << ")";
        break;
    }
    default:
        switch(command) {
        case MidiEvent::SongPosition:
        {
            quint32 beats = (message >> 8) & 0xfff;
            quint32 position = beats * MIDI_CLOCKS_PER_MIDI_BEAT;
            qDebug() << "SetSongPosition(" << position << ")";
        }
            break;
        case MidiEvent::SongSelect:
        {
            quint32 song = (message >> 8) & 0xff;
            qDebug() << "SelectSong(" << song << ")";
        }
            break;
        case MidiEvent::TuneRequest: // Mostly used for sound modules with analog oscillator circuits.
            qDebug() << "TuneRequest()";
            break;
        case MidiEvent::MidiClock: // Every 24 MIDI clocks
            qDebug() << "MidiClock()";
            break;
        case MidiEvent::MidiTick: // Every 10ms
            qDebug() << "MidiTick()";
            break;

        case MidiEvent::SongStart:
            qDebug() << "MidiStart()";
            break;

        case MidiEvent::SongContinue:
            qDebug() << "MidiContinue()";
            break;

        case MidiEvent::SongStop:
            qDebug() << "MidiStop()";
            break;

        case MidiEvent::Sensing: // no activity for 300ms
            qDebug() << "Sensing()";
            break;
        case MidiEvent::Reset: // no activity for 300ms
            qDebug() << "Reset()";
            break;

        default:
            qDebug() << "Event " << Qt::hex << message << " not decoded";
        }
    }
}
