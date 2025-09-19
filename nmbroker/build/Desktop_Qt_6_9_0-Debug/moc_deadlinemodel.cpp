/****************************************************************************
** Meta object code from reading C++ file 'deadlinemodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dbaccess/deadlinemodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'deadlinemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN6Nutmeg13deadlineModelE_t {};
} // unnamed namespace

template <> constexpr inline auto Nutmeg::deadlineModel::qt_create_metaobjectdata<qt_meta_tag_ZN6Nutmeg13deadlineModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Nutmeg::deadlineModel"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<deadlineModel, qt_meta_tag_ZN6Nutmeg13deadlineModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Nutmeg::deadlineModel::staticMetaObject = { {
    QMetaObject::SuperData::link<Nutmeg::TableModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Nutmeg13deadlineModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Nutmeg13deadlineModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6Nutmeg13deadlineModelE_t>.metaTypes,
    nullptr
} };

void Nutmeg::deadlineModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<deadlineModel *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Nutmeg::deadlineModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Nutmeg::deadlineModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Nutmeg13deadlineModelE_t>.strings))
        return static_cast<void*>(this);
    return Nutmeg::TableModel::qt_metacast(_clname);
}

int Nutmeg::deadlineModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Nutmeg::TableModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
