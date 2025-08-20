QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# Suppress debug output for release builds
# CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS += nutmeg.ico

SOURCES += \
    ../nmbroker/cache/cache.cpp \
    ../nmbroker/cache/deadlinecache.cpp \
    ../nmbroker/dbaccess/appointmenttypemodel.cpp \
    ../nmbroker/dbaccess/databaseconnection.cpp \
    ../nmbroker/dbaccess/deadlinemodel.cpp \
    ../nmbroker/dbaccess/exception.cpp \
    ../nmbroker/dbaccess/flagclassmodel.cpp \
    ../nmbroker/dbaccess/models.cpp \
    ../nmbroker/dbaccess/objectmodel.cpp \
    ../nmbroker/dbaccess/objecttypemodel.cpp \
    ../nmbroker/dbaccess/responsesdashboardcomplete.cpp \
    ../nmbroker/dbaccess/tagmodel.cpp \
    ../nmbroker/dbaccess/taskmodel.cpp \
    ../nmbroker/dbaccess/viewappointmentobjectsmodel.cpp \
    ../nmbroker/dbaccess/viewcopyrightfilingtypesmodel.cpp \
    ../nmbroker/dbaccess/viewfilingsincompletemodel.cpp \
    ../nmbroker/dbaccess/viewfilingsmodel.cpp \
    ../nmbroker/dbaccess/viewfilingtasktypesmodel.cpp \
    ../nmbroker/dbaccess/viewfilingtypesmodel.cpp \
    ../nmbroker/dbaccess/viewobjectflagsmodel.cpp \
    ../nmbroker/dbaccess/viewobjecttagsmodel.cpp \
    ../nmbroker/dbaccess/viewpatentfilingtypesmodel.cpp \
    ../nmbroker/dbaccess/viewpatentmattersmodel.cpp \
    ../nmbroker/dbaccess/viewresponsesmodel.cpp \
    ../nmbroker/dbaccess/viewresponsetasktypesmodel.cpp \
    ../nmbroker/dbaccess/viewtasksmodel.cpp \
    ../nmbroker/dbaccess/viewtrademarkfilingtypesmodel.cpp \
    ../nmbroker/dbaccess/viewtrademarkmattersmodel.cpp \
    ../nmbroker/dbaccess/viewupcomingappointmentsmodel.cpp \
    ../nmbroker/logger.cpp \
    ../nmbroker/nutmeg.cpp \
    ../nmbroker/objects/filingsdashboardentry.cpp \
    ../nmbroker/objects/responsesdashboardentry.cpp \
    completers/completer.cpp \
    completers/entitiescompleter.cpp \
    completers/examinercompleter.cpp \
    completers/inventorcompleter.cpp \
    completers/mattercompleter.cpp \
    completers/paralegalcompleter.cpp \
    completers/peoplecompleter.cpp \
    completers/tagcompleter.cpp \
    panels/abstracttaskpanel.cpp \
    panels/appointmentdashpanel.cpp \
    panels/appointmentpanel.cpp \
    panels/appointmentpanelfull.cpp \
    panels/appointmentsdashboard.cpp \
    panels/appointmenttaskpanel.cpp \
    panels/deadlinespanel.cpp \
    panels/entitiespanel.cpp \
    panels/entitypanel.cpp \
    panels/examinerinfopanel.cpp \
    panels/filingdashpanel.cpp \
    panels/filingsdashboard.cpp \
    panels/filingtaskpanel.cpp \
    panels/flagspanel.cpp \
    panels/frame.cpp \
    panels/insertenterprisepanel.cpp \
    panels/insertpersonpanel.cpp \
    panels/inventorspanel.cpp \
    panels/maindashboard.cpp \
    panels/matterpanel.cpp \
    panels/optionsdatabasepanel.cpp \
    panels/optionsdisplaypanel.cpp \
    panels/optionsdocumentpanel.cpp \
    panels/patentmatterpanel.cpp \
    panels/personaddselectpanel.cpp \
    panels/responsedashpanel.cpp \
    panels/responsesdashboard.cpp \
    panels/responsetaskpanel.cpp \
    panels/taskpanelfull.cpp \
    utils/email.cpp \
    utils/icalbuilder.cpp \
    widgets/abstractremovablebutton.cpp \
    widgets/checkbox.cpp \
    widgets/collapsebutton.cpp \
    widgets/comboeditadd.cpp \
    widgets/datetimeedit.cpp \
    widgets/disabledbutton.cpp \
    widgets/doublespinbox.cpp \
    widgets/dynamicstackedwidget.cpp \
    widgets/editbutton.cpp \
    widgets/emailinventorsbutton.cpp \
    widgets/examinereditadd.cpp \
    widgets/examinerinterviewinformation.cpp \
    widgets/filingtypelabel.cpp \
    widgets/flagcheckbox.cpp \
    widgets/inventorbutton.cpp \
    widgets/inventoreditadd.cpp \
    widgets/paralegalsearchbox.cpp \
    widgets/personsearchbox.cpp \
    widgets/spinbox.cpp \
    widgets/timeedit.cpp \
    widgets/workattorneysearchbox.cpp \
    widgets/xbutton.cpp \
    ../nmbroker/dbaccess/nutdb.cpp \
    ../nmbroker/dbaccess/sqlrecordstablemodel.cpp \
    ../nmbroker/dbaccess/tablemodel.cpp \
    ../nmbroker/dbaccess/tasktypemodel.cpp \
    ../nmbroker/dbaccess/viewclientsmodel.cpp \
    ../nmbroker/dbaccess/viewcontractingfirmsmodel.cpp \
    ../nmbroker/dbaccess/viewcopyrighttasktypesmodel.cpp \
    ../nmbroker/dbaccess/viewentitiesmodel.cpp \
    ../nmbroker/dbaccess/viewinventorsmodel.cpp \
    ../nmbroker/dbaccess/viewjurisdictionsmodel.cpp \
    ../nmbroker/dbaccess/viewmattersmodel.cpp \
    ../nmbroker/dbaccess/viewparalegalsmodel.cpp \
    ../nmbroker/dbaccess/viewpatentexaminersmodel.cpp \
    ../nmbroker/dbaccess/viewpatenttasktypesmodel.cpp \
    ../nmbroker/dbaccess/viewpeoplemodel.cpp \
    ../nmbroker/dbaccess/viewresponsesincompletemodel.cpp \
    ../nmbroker/dbaccess/viewtaskclassmodel.cpp \
    ../nmbroker/dbaccess/viewtasktypesmodel.cpp \
    ../nmbroker/dbaccess/viewtrademarktasktypesmodel.cpp \
    ../nmbroker/dbaccess/viewworkattorneysmodel.cpp \
    ../nmbroker/objects/appointment.cpp \
    ../nmbroker/objects/copyrightmatter.cpp \
    ../nmbroker/objects/deadline.cpp \
    ../nmbroker/objects/document.cpp \
    ../nmbroker/objects/enterprise.cpp \
    ../nmbroker/objects/entity.cpp \
    ../nmbroker/objects/filing.cpp \
    ../nmbroker/objects/flag.cpp \
    ../nmbroker/objects/flagclass.cpp \
    ../nmbroker/objects/matter.cpp \
    ../nmbroker/objects/object.cpp \
    ../nmbroker/objects/patentmatter.cpp \
    ../nmbroker/objects/person.cpp \
    ../nmbroker/objects/response.cpp \
    ../nmbroker/objects/tag.cpp \
    ../nmbroker/objects/task.cpp \
    ../nmbroker/objects/trademarkmatter.cpp \
    ../nmbroker/settings.cpp \
    main.cpp \
    mainwindow.cpp \
    widgets/abstractsearchbox.cpp \
    widgets/addnewbutton.cpp \
    widgets/arrowbutton.cpp \
    widgets/combobox.cpp \
    widgets/criticalbox.cpp \
    widgets/dateedit.cpp \
    widgets/docketnumberbutton.cpp \
    widgets/donebutton.cpp \
    widgets/eibutton.cpp \
    widgets/entitiessearchbox.cpp \
    widgets/examinersearchbox.cpp \
    widgets/filingsview.cpp \
    widgets/informationbox.cpp \
    widgets/inventorssearchbox.cpp \
    widgets/label.cpp \
    widgets/labeledwidgetleft.cpp \
    widgets/linedisplayid.cpp \
    widgets/mattersearchbox.cpp \
    widgets/messagebox.cpp \
    widgets/plusbutton.cpp \
    widgets/pushbutton.cpp \
    widgets/questionbox.cpp \
    widgets/responsetypelabel.cpp \
    widgets/tableview.cpp \
    widgets/tasktypecombo.cpp \
    widgets/textedit.cpp \
    widgets/titleedit.cpp \
    widgets/warningbox.cpp \
    widgets/widget.cpp \
    windows/addexaminerinterviewdialog.cpp \
    windows/dialog.cpp \
    widgets/lineedit.cpp \
    widgets/groupbox.cpp \
    windows/entitydialog.cpp \
    windows/insertdialog.cpp \
    windows/insertentitydialog.cpp \
    windows/insertinventordialog.cpp \
    windows/insertpatentexaminerdialog.cpp \
    windows/insertpersondialog.cpp \
    windows/inserttrademarkexaminerdialog.cpp \
    windows/matterdialog.cpp \
    windows/newfilingdialog.cpp \
    windows/newresponsedialog.cpp \
    windows/optionswindow.cpp \
    windows/patentmatterdialog.cpp \
    windows/taskdialog.cpp

HEADERS += \
    ../nmbroker/cache/cache.h \
    ../nmbroker/cache/deadlinecache.h \
    ../nmbroker/dbaccess/appointmenttypemodel.h \
    ../nmbroker/dbaccess/databaseconnection.h \
    ../nmbroker/dbaccess/deadlinemodel.h \
    ../nmbroker/dbaccess/exception.h \
    ../nmbroker/dbaccess/flagclassmodel.h \
    ../nmbroker/dbaccess/models.h \
    ../nmbroker/dbaccess/objectmodel.h \
    ../nmbroker/dbaccess/objecttypemodel.h \
    ../nmbroker/dbaccess/record.h \
    ../nmbroker/dbaccess/responsesdashboardcomplete.h \
    ../nmbroker/dbaccess/tagmodel.h \
    ../nmbroker/dbaccess/taskmodel.h \
    ../nmbroker/dbaccess/viewappointmentobjectsmodel.h \
    ../nmbroker/dbaccess/viewcopyrightfilingtypesmodel.h \
    ../nmbroker/dbaccess/viewfilingsincompletemodel.h \
    ../nmbroker/dbaccess/viewfilingsmodel.h \
    ../nmbroker/dbaccess/viewfilingtasktypesmodel.h \
    ../nmbroker/dbaccess/viewfilingtypesmodel.h \
    ../nmbroker/dbaccess/viewobjectflagsmodel.h \
    ../nmbroker/dbaccess/viewobjecttagsmodel.h \
    ../nmbroker/dbaccess/viewpatentfilingtypesmodel.h \
    ../nmbroker/dbaccess/viewpatentmattersmodel.h \
    ../nmbroker/dbaccess/viewresponsesmodel.h \
    ../nmbroker/dbaccess/viewresponsetasktypesmodel.h \
    ../nmbroker/dbaccess/viewtasksmodel.h \
    ../nmbroker/dbaccess/viewtrademarkfilingtypesmodel.h \
    ../nmbroker/dbaccess/viewtrademarkmattersmodel.h \
    ../nmbroker/dbaccess/viewupcomingappointmentsmodel.h \
    ../nmbroker/logger.h \
    ../nmbroker/objects/filingsdashboardentry.h \
    ../nmbroker/objects/responsesdashboardentry.h \
    completers/completer.h \
    completers/entitiescompleter.h \
    completers/examinercompleter.h \
    completers/inventorcompleter.h \
    completers/mattercompleter.h \
    completers/paralegalcompleter.h \
    completers/peoplecompleter.h \
    completers/tagcompleter.h \
    panels/abstracttaskpanel.h \
    panels/appointmentdashpanel.h \
    panels/appointmentpanel.h \
    panels/appointmentpanelfull.h \
    panels/appointmentsdashboard.h \
    panels/appointmenttaskpanel.h \
    panels/deadlinespanel.h \
    panels/entitiespanel.h \
    panels/entitypanel.h \
    panels/examinerinfopanel.h \
    panels/filingdashpanel.h \
    panels/filingsdashboard.h \
    panels/filingtaskpanel.h \
    panels/flagspanel.h \
    panels/frame.h \
    panels/insertenterprisepanel.h \
    panels/insertpersonpanel.h \
    panels/inventorspanel.h \
    panels/maindashboard.h \
    panels/matterpanel.h \
    panels/optionsdatabasepanel.h \
    panels/optionsdisplaypanel.h \
    panels/optionsdocumentpanel.h \
    panels/patentmatterpanel.h \
    panels/personaddselectpanel.h \
    panels/responsedashpanel.h \
    panels/responsesdashboard.h \
    panels/responsetaskpanel.h \
    panels/taskpanelfull.h \
    utils/email.h \
    utils/icalbuilder.h \
    widgets/abstractremovablebutton.h \
    widgets/checkbox.h \
    widgets/collapsebutton.h \
    widgets/comboeditadd.h \
    widgets/datetimeedit.h \
    widgets/disabledbutton.h \
    widgets/doublespinbox.h \
    widgets/dynamicstackedwidget.h \
    widgets/editbutton.h \
    widgets/emailinventorsbutton.h \
    widgets/examinereditadd.h \
    widgets/examinerinterviewinformation.h \
    widgets/filingtypelabel.h \
    widgets/flagcheckbox.h \
    widgets/inventorbutton.h \
    widgets/inventoreditadd.h \
    widgets/paralegalsearchbox.h \
    widgets/personsearchbox.h \
    widgets/spinbox.h \
    widgets/timeedit.h \
    widgets/workattorneysearchbox.h \
    widgets/xbutton.h \
    ../nmbroker/dbaccess/viewcontractingfirmsmodel.h \
    ../nmbroker/dbaccess/viewentitiesmodel.h \
    ../nmbroker/dbaccess/viewinventorsmodel.h \
    ../nmbroker/dbaccess/viewjurisdictionsmodel.h \
    ../nmbroker/dbaccess/viewpatentexaminersmodel.h \
    ../nmbroker/dbaccess/viewtaskclassmodel.h \
    ../nmbroker/dbaccess/viewtasktypesmodel.h \
    ../nmbroker/exception.h \
    ../nmbroker/dbaccess/nutdb.h \
    ../nmbroker/dbaccess/sqlrecordstablemodel.h \
    ../nmbroker/dbaccess/tablemodel.h \
    ../nmbroker/dbaccess/tasktypemodel.h \
    ../nmbroker/dbaccess/viewclientsmodel.h \
    ../nmbroker/dbaccess/viewcopyrighttasktypesmodel.h \
    ../nmbroker/dbaccess/viewmattersmodel.h \
    ../nmbroker/dbaccess/viewparalegalsmodel.h \
    ../nmbroker/dbaccess/viewpatenttasktypesmodel.h \
    ../nmbroker/dbaccess/viewpeoplemodel.h \
    ../nmbroker/dbaccess/viewresponsesincompletemodel.h \
    ../nmbroker/dbaccess/viewtrademarktasktypesmodel.h \
    ../nmbroker/dbaccess/viewworkattorneysmodel.h \
    ../nmbroker/nutmeg.h \
    ../nmbroker/objects/appointment.h \
    ../nmbroker/objects/copyrightmatter.h \
    ../nmbroker/objects/deadline.h \
    ../nmbroker/objects/document.h \
    ../nmbroker/objects/enterprise.h \
    ../nmbroker/objects/entity.h \
    ../nmbroker/objects/filing.h \
    ../nmbroker/objects/flag.h \
    ../nmbroker/objects/flagclass.h \
    ../nmbroker/objects/matter.h \
    ../nmbroker/objects/object.h \
    ../nmbroker/objects/patentmatter.h \
    ../nmbroker/objects/person.h \
    ../nmbroker/objects/response.h \
    ../nmbroker/objects/tag.h \
    ../nmbroker/objects/task.h \
    ../nmbroker/objects/trademarkmatter.h \
    ../nmbroker/property.h \
    ../nmbroker/settings.h \
    ../nmbroker/structures.h \
    mainwindow.h \
    widgets/abstractsearchbox.h \
    widgets/addnewbutton.h \
    widgets/arrowbutton.h \
    widgets/combobox.h \
    widgets/criticalbox.h \
    widgets/dateedit.h \
    widgets/docketnumberbutton.h \
    widgets/donebutton.h \
    widgets/eibutton.h \
    widgets/entitiessearchbox.h \
    widgets/examinersearchbox.h \
    widgets/filingsview.h \
    widgets/informationbox.h \
    widgets/inventorssearchbox.h \
    widgets/label.h \
    widgets/labeledwidgetleft.h \
    widgets/linedisplayid.h \
    widgets/mattersearchbox.h \
    widgets/messagebox.h \
    widgets/plusbutton.h \
    widgets/pushbutton.h \
    widgets/questionbox.h \
    widgets/responsetypelabel.h \
    widgets/tableview.h \
    widgets/tasktypecombo.h \
    widgets/textedit.h \
    widgets/titleedit.h \
    widgets/warningbox.h \
    widgets/widget.h \
    windows/addexaminerinterviewdialog.h \
    windows/dialog.h \
    widgets/lineedit.h \
    widgets/groupbox.h \
    windows/entitydialog.h \
    windows/insertdialog.h \
    windows/insertentitydialog.h \
    windows/insertinventordialog.h \
    windows/insertpatentexaminerdialog.h \
    windows/insertpersondialog.h \
    windows/inserttrademarkexaminerdialog.h \
    windows/matterdialog.h \
    windows/newfilingdialog.h \
    windows/newresponsedialog.h \
    windows/optionswindow.h \
    windows/patentmatterdialog.h \
    windows/taskdialog.h

FORMS += \
    mainwindow.ui \
    windows/newfilingdialog.ui \
    windows/newresponsedialog.ui

INCLUDEPATH += ../nmbroker

#INCLUDEPATH += /usr/local/src/boost

TRANSLATIONS += \
    nmgui_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!mac {
    QMAKE_CXXFLAGS += -fdeclspec #-Wno-unused-function
}

DISTFILES += \
    ../nmbroker/dbaccess/gentable.sh

RESOURCES += \
    nutmeg_resources.qrc

unix: CONFIG += link_pkgconfig
