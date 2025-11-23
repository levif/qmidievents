/* Copyright (C) 2022-2023 Cédric Le Dillau
 * SPDX-License-Identifier: MIT
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <stdio.h>

#include <QThread>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QCommandLineParser>
//#include <QRegularExpression>

#include <QMidiOut.h>
#include <QMidiIn.h>
#include <QMidiFile.h>

#include "QMidiListener.h"

static void usage(char* program_name)
{
    fprintf(stderr, "Usage: %s -p<port> <MidiFile>\n\n", program_name);
    fputs("Ports:\nID\tName\n----------------\n", stderr);
    QMap<QString, QString> vals = QMidiIn::devices();
    for(auto [port, devicename] : vals.asKeyValueRange()) {
        fputs(port.toUtf8().constData(), stderr);
        fputs("\t", stderr);
        fputs(devicename.toUtf8().constData(), stderr);
        fputs("\n", stderr);
    }
    exit(1);
}


int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("midievents");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "MIDI event listener"));
    parser.addHelpOption();
    parser.addVersionOption();

    // An option with a value
    QCommandLineOption midiDeviceOption(QStringList() << "d" << "devicename" << "device",
                                        QCoreApplication::translate("main", "Device number or name"),
                                        QCoreApplication::translate("main", "device")
                                        );
    parser.addOption(midiDeviceOption);

    QCommandLineOption midiPortOption(QStringList() << "p" << "port",
                                      QCoreApplication::translate("main", "MIDI port"),
                                      QCoreApplication::translate("main", "port")
                                      );
    parser.addOption(midiPortOption);

    QCommandLineOption midiListDevices(QStringList() << "l" << "list",
                                      QCoreApplication::translate("list", "list devices")
                                      );
    parser.addOption(midiListDevices);

    // Process the actual command line arguments given by the user
    parser.process(a);

    QString midiDeviceName = parser.value(midiDeviceOption);
    QString midiInName = parser.value(midiPortOption) ;
    bool showDeviceList = parser.isSet(midiListDevices);

    if (showDeviceList) {
        QMap<QString, QString> vals = QMidiIn::devices();
        for(auto [port, devicename] : vals.asKeyValueRange()) {
            qDebug() << port << devicename;
        }
        vals = QMidiOut::devices();
        for(auto [port, devicename] : vals.asKeyValueRange()) {
            qDebug() << port << devicename;
        }
    }

    if (parser.isSet(midiDeviceOption)) {
        QMap<QString, QString> vals = QMidiIn::devices();
        qDebug() << vals;
        for(auto [port, devicename] : vals.asKeyValueRange()) {
            if (devicename.startsWith(midiDeviceName)) {
                midiInName = port;
                qDebug() << "Found device" << midiDeviceName;
            }
        }
        if (midiInName.isEmpty()) {
            qWarning() << "Unable to find a device starting with" << midiDeviceName;
        }
    }

    bool ok = false;
    midiInName.toInt(&ok);
    if (!ok) {
        qWarning() << "no valid device given";
        parser.showHelp(1);
    }

    QMidiIn* midi_in = new QMidiIn();
    if (midi_in->connect(midiInName)) {
        qWarning() << "Unable to connect to device:" << midiInName;
        //exit(1);
    }
    qDebug() << "Use device:" << midiInName;

    QMidiListener* p = new QMidiListener(midi_in);
    QObject::connect(midi_in, SIGNAL(midiEvent(quint32,quint32)),
                     p, SLOT(onMidiEvent(quint32,quint32)));
    QObject::connect(midi_in, SIGNAL(midiSysExEvent(QByteArray)),
                     p, SLOT(onMidiSysExEvent(QByteArray)));

    QObject::connect(p, SIGNAL(finished()), &a, SLOT(quit()));
    p->start();

    return a.exec();
}

#include "main.moc"
