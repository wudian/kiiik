/****************************************************************************
** Meta object code from reading C++ file 'SubsTblModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SubsTblModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SubsTblModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SubsTblModel_t {
    QByteArrayData data[6];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SubsTblModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SubsTblModel_t qt_meta_stringdata_SubsTblModel = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SubsTblModel"
QT_MOC_LITERAL(1, 13, 11), // "SlotTickRsp"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 17), // "const CQuotation*"
QT_MOC_LITERAL(4, 44, 9), // "SlotRmSym"
QT_MOC_LITERAL(5, 54, 7) // "CSymbol"

    },
    "SubsTblModel\0SlotTickRsp\0\0const CQuotation*\0"
    "SlotRmSym\0CSymbol"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SubsTblModel[] = {

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
       1,    1,   24,    2, 0x08 /* Private */,
       4,    1,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void SubsTblModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SubsTblModel *_t = static_cast<SubsTblModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SlotTickRsp((*reinterpret_cast< const CQuotation*(*)>(_a[1]))); break;
        case 1: _t->SlotRmSym((*reinterpret_cast< const CSymbol(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SubsTblModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_SubsTblModel.data,
      qt_meta_data_SubsTblModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SubsTblModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SubsTblModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SubsTblModel.stringdata0))
        return static_cast<void*>(const_cast< SubsTblModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int SubsTblModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
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
