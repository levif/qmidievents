/* Copyright (C) 2022-2023 Cédric Le Dillau
 * SPDX-License-Identifier: MIT
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef QMIDIEVENTDECODER_H
#define QMIDIEVENTDECODER_H

#include <QObject>

#define MIDI_CLOCKS_PER_MIDI_BEAT  (6)

//! Details checked at http://midi.teragonaudio.com/tech/midispec/table.htm
//!
class QMidiEventDecoder: public QObject
{
    Q_OBJECT

public:
    //! \brief MIDI event control code
    enum MidiEvent {

        Invalid         = -1,

        // Events with channel in LSB nibble
        NoteOff         = 0x80,
        NoteOn          = 0x90, //!< NoteOn
        KeyPressure     = 0xa0, //!< Aftertouch
        ControlChange   = 0xb0, //!< CC messages
        ProgramChange   = 0xc0, //!< Voice, Patch, Instrument or Preset
        ChannelPressure = 0xd0,
        PitchWheel      = 0xe0,
        SysEx           = 0xf0,

        // No channel anymore
        MtcQuarter      = 0xf1,
        SongPosition    = 0xf2,
        SongSelect      = 0xf3,
        TuneRequest     = 0xf6,
        MidiClock       = 0xf8,
        MidiTick        = 0xf9, //!< Every 10ms from master to control sequence playback
        SongStart       = 0xfa, //!< start playback of some song/sequence from the beginning (ie, MIDI Beat 0)
        SongContinue    = 0xfb,
        SongStop        = 0xfc, //!< should keep track of position in case of continue
        Sensing         = 0xfe,
        Reset           = 0xff
    };

    //!< \brief Message categories
    enum MidiEventCategories {
        FlagNote            = (1 << 0), //!< Note:  NoteOn, NoteOff, KeyPRessure
        FlagControlChange   = (1 << 1), //!< CC messages
        FlagProgramChange   = (1 << 2), //!< Voice, Patch, Instrument or Preset
        FlagPitchWheel      = (1 << 3), //!< Pitch wheel
        FlagSong            = (1 << 4), //!< Song: Select, Position, Start, Stop, Continue
        FlagMidiSync        = (1 << 5), //!< MidiClock, MidiTick, Sensing
    };

    QMidiEventDecoder(QObject *parent = nullptr);
    void decode(quint32 message, quint32 timing=0);
    void setFilters(quint32 categories);
    static void debug(quint32 message);

signals:
    /* FlagNote */
    void midiNoteOn(quint8 channel, quint8 note, quint8 velocity);
    void midiNoteOff(quint8 channel, quint8 note, quint8 velocity);
    void midiKeyPressure(quint8 channel, quint8 note, quint8 pressure);
    /* FlagControlChange */
    void midiControlChange(quint8 channel, quint8 controller, quint8 value);
    /* FlagProgramChange */
    void midiProgramChange(quint8 channel, quint32 newprogram, quint32 oldprogram=0);
    /* FlagPitchWheel */
    void midiPitchWheel(quint8 channel, quint16 value);
    /* Non-maskable */
    void midiReset();

private:
    quint32 m_value;
    quint32 m_program;
    quint32 m_notifiers;
};

#endif // QMIDIEVENTDECODER_H
