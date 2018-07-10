/****************************************************************************
** Meta object code from reading C++ file 'versionupdate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../versionupdate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'versionupdate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseDlg_t {
    QByteArrayData data[1];
    char stringdata0[8];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseDlg_t qt_meta_stringdata_BaseDlg = {
    {
QT_MOC_LITERAL(0, 0, 7) // "BaseDlg"

    },
    "BaseDlg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void BaseDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject BaseDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BaseDlg.data,
      qt_meta_data_BaseDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BaseDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BaseDlg.stringdata0))
        return static_cast<void*>(const_cast< BaseDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int BaseDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_VersionUpdate_t {
    QByteArrayData data[4];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VersionUpdate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VersionUpdate_t qt_meta_stringdata_VersionUpdate = {
    {
QT_MOC_LITERAL(0, 0, 13), // "VersionUpdate"
QT_MOC_LITERAL(1, 14, 25), // "on_pushButton_yes_clicked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 24) // "on_pushButton_no_clicked"

    },
    "VersionUpdate\0on_pushButton_yes_clicked\0"
    "\0on_pushButton_no_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VersionUpdate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VersionUpdate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VersionUpdate *_t = static_cast<VersionUpdate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_yes_clicked(); break;
        case 1: _t->on_pushButton_no_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject VersionUpdate::staticMetaObject = {
    { &BaseDlg::staticMetaObject, qt_meta_stringdata_VersionUpdate.data,
      qt_meta_data_VersionUpdate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VersionUpdate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VersionUpdate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VersionUpdate.stringdata0))
        return static_cast<void*>(const_cast< VersionUpdate*>(this));
    if (!strcmp(_clname, "Ui::VersionUpdateDialog"))
        return static_cast< Ui::VersionUpdateDialog*>(const_cast< VersionUpdate*>(this));
    return BaseDlg::qt_metacast(_clname);
}

int VersionUpdate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseDlg::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
