QT = core
QT += sql websockets

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        dbaccess/nmdatabase.cpp \
        dbaccess/responsesdashboardtablemodel.cpp \
        dbaccess/tablemodel.cpp \
        main.cpp \
        websocketserver.cpp \
        objects/appointment.cpp \
        objects/copyrightmatter.cpp \
        objects/document.cpp \
        objects/enterprise.cpp \
        objects/entity.cpp \
        objects/filing.cpp \
        objects/flag.cpp \
        objects/flagclass.cpp \
        objects/matter.cpp \
        objects/object.cpp \
        objects/patentmatter.cpp \
        objects/person.cpp \
        objects/response.cpp \
        objects/tag.cpp \
        objects/task.cpp \
        objects/trademarkmatter.cpp \
        settings.cpp

TRANSLATIONS += \
    nmbroker_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    dbaccess/nmdatabase.h \
    dbaccess/responsesdashboardtablemodel.h \
    dbaccess/tablemodel.h \
    nutmeg.h \
    websocketserver.h \
    objects/appointment.h \
    objects/copyrightmatter.h \
    objects/document.h \
    objects/enterprise.h \
    objects/entity.h \
    objects/filing.h \
    objects/flag.h \
    objects/flagclass.h \
    objects/matter.h \
    objects/object.h \
    objects/patentmatter.h \
    objects/person.h \
    objects/response.h \
    objects/tag.h \
    objects/task.h \
    objects/trademarkmatter.h \
    property.h \
    settings.h \
    structures.h

QMAKE_CXXFLAGS += -fdeclspec

DISTFILES += \
    dbaccess/gentable \
    dbaccess/gentable.sh
