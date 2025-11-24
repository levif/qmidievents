cmake_minimum_required(VERSION 3.16)
project(qmidievents VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(QMIDI_DIR ../QMidi)
find_package(QMidi REQUIRED HINTS ${QMIDI_DIR})

add_executable(qmidievents MACOSX_BUNDLE
    ${QMidi_INTERFACE}
    main.cpp
    QMidiListener.h QMidiListener.cpp
    QMidiEventDecoder.cpp QMidiEventDecoder.h
)
target_include_directories(qmidievents PRIVATE
    ${QMidi_INCLUDE_DIR}
)
target_link_libraries(qmidievents PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    ${QMidi_LIBRARIES}
)

install(TARGETS qmidievents
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
qt_generate_deploy_app_script(
    TARGET qmidievents
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
endif()
