QT = core
QT += sql websockets

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        websocketserver.cpp \
        nutmeg.cpp \
        logger.cpp \
        settings.cpp \
        $$files(objects/*.cpp) \
        $$files(dbaccess/*.cpp) \
        $$files(cache/*.cpp)

TRANSLATIONS += \
    nmbroker_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    nutmeg.h \
    websocketserver.h \
    property.h \
    settings.h \
    structures.h \
    exception.h \
    logger.h \
    $$files(objects/*.h) \
    $$files(dbaccess/*.h) \
    $$files(cache/*.h)

QMAKE_CXXFLAGS += -fdeclspec

DISTFILES += \
    dbaccess/gentable \
    dbaccess/gentable.sh
