QT = core gui
QT += sql websockets

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        websocketserver.cpp \
        wsrouter.cpp \
        nutmeg.cpp \
        logger.cpp \
        settings.cpp \
        $$files(objects/*.cpp) \
        dbaccess/databaseconnection.cpp \
        dbaccess/exception.cpp \
        dbaccess/models.cpp \
        dbaccess/nutdb.cpp \
        dbaccess/objectmodel.cpp \
        dbaccess/sqlrecordstablemodel.cpp \
        dbaccess/tablemodel.cpp \
        dbaccess/objecttypemodel.cpp \
        dbaccess/flagclassmodel.cpp \
        dbaccess/deadlinemodel.cpp \
        dbaccess/tagmodel.cpp \
        dbaccess/viewappointmentobjectsmodel.cpp \
        dbaccess/viewclientsmodel.cpp \
        dbaccess/viewentitiesmodel.cpp \
        dbaccess/viewinventorsmodel.cpp \
        dbaccess/viewfilingsmodel.cpp \
        dbaccess/viewmattersmodel.cpp \
        dbaccess/viewobjectflagsmodel.cpp \
        dbaccess/viewobjecttagsmodel.cpp \
        dbaccess/viewparalegalsmodel.cpp \
        dbaccess/viewpatentexaminersmodel.cpp \
        dbaccess/viewpatentmattersmodel.cpp \
        dbaccess/viewpeoplemodel.cpp \
        dbaccess/viewresponsesmodel.cpp \
        dbaccess/viewtasksmodel.cpp \
        dbaccess/viewtrademarkmattersmodel.cpp \
        dbaccess/viewworkattorneysmodel.cpp \
        $$files(cache/*.cpp)

# Exclude GUI-dependent object sources from backend build
SOURCES -= objects/patentmatter.cpp
SOURCES -= dbaccess/responsesdashboardmodel.cpp

TRANSLATIONS += \
    nmbroker_en_US.ts
# Disable translation build in minimal backend builds (requires Qt Linguist tools)
# CONFIG += lrelease
# CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    nutmeg.h \
    websocketserver.h \
    wsrouter.h \
    property.h \
    settings.h \
    structures.h \
    exception.h \
    logger.h \
    $$files(objects/*.h) \
    $$files(cache/*.h) \
    dbaccess/databaseconnection.h \
    dbaccess/exception.h \
    dbaccess/models.h \
    dbaccess/nutdb.h \
    dbaccess/objectmodel.h \
    dbaccess/sqlrecordstablemodel.h \
    dbaccess/tablemodel.h \
    dbaccess/objecttypemodel.h \
    dbaccess/flagclassmodel.h \
    dbaccess/deadlinemodel.h \
    dbaccess/tagmodel.h \
    dbaccess/viewappointmentobjectsmodel.h \
    dbaccess/viewclientsmodel.h \
    dbaccess/viewentitiesmodel.h \
    dbaccess/viewinventorsmodel.h \
    dbaccess/viewfilingsmodel.h \
    dbaccess/viewmattersmodel.h \
    dbaccess/viewobjectflagsmodel.h \
    dbaccess/viewobjecttagsmodel.h \
    dbaccess/viewparalegalsmodel.h \
    dbaccess/viewpatentexaminersmodel.h \
    dbaccess/viewpatentmattersmodel.h \
    dbaccess/viewpeoplemodel.h \
    dbaccess/viewresponsesmodel.h \
    dbaccess/viewtasksmodel.h \
    dbaccess/viewtrademarkmattersmodel.h \
    dbaccess/viewworkattorneysmodel.h

QMAKE_CXXFLAGS += -fdeclspec

DISTFILES += \
    dbaccess/gentable \
    dbaccess/gentable.sh
