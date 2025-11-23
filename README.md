# QMidiEventDecoder

A simple tool connect MIDI events to Qt objects/widgets/QML from a device.

This work is based on [QMidi](https://github.com/waddlesplash/QMidi)/[my QMidi](https://github.com/levif/QMidi) cross-platform project.

`QMidiEventDecoder` will be proposed to QMidi library.

## Support
The project can use Qt6 or Qt5 ([my QMidi](https://github.com/levif/QMidi)). Note that QMidi library comes with some files to compile alongside with this project.

## Compilation

Add folder of QMidiConfig.cmake to CMake call: (`QMidi_DIR:PATH`)
```sh
cmake -S . -B ../build-qmidieventdecoder -DQMidi_DIR:PATH=/path/to/QMidi
```
