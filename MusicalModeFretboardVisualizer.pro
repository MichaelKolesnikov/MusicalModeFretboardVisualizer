QT       += core gui widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FretboardScene.cpp \
    Note.cpp \
    NoteLetter.cpp \
    PianoItem.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    FretboardScene.h \
    MainWindow.h \
    Note.h \
    NoteLetter.h \
    PianoItem.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    LICENSE \
    README.md \
    modes.json

RESOURCES += \
    resource.qrc
