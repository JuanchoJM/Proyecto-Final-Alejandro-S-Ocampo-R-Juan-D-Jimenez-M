QT       += core gui multimedia  multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Animales.cpp \
    Enemigo.cpp \
    enemigonivel2.cpp \
    jugador.cpp \
    jugadornivel2.cpp \
    main.cpp \
    mainwindow.cpp \
    proyectil.cpp \
    proyectilnivel2.cpp

HEADERS += \
    Animales.h \
    Enemigo.h \
    enemigonivel2.h \
    jugador.h \
    jugadornivel2.h \
    mainwindow.h \
    proyectil.h \
    proyectilnivel2.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Imagenes1.qrc

DISTFILES += \
    Usuario.txt
