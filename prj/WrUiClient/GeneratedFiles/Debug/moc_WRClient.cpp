/****************************************************************************
** Meta object code from reading C++ file 'WRClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../WRClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WRClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WRClient_t {
    QByteArrayData data[10];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WRClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WRClient_t qt_meta_stringdata_WRClient = {
    {
QT_MOC_LITERAL(0, 0, 8), // "WRClient"
QT_MOC_LITERAL(1, 9, 12), // "SignalNetErr"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "SignalLogin"
QT_MOC_LITERAL(4, 35, 13), // "SignalSymbols"
QT_MOC_LITERAL(5, 49, 8), // "CSymbol*"
QT_MOC_LITERAL(6, 58, 11), // "SignalQuote"
QT_MOC_LITERAL(7, 70, 17), // "const CQuotation*"
QT_MOC_LITERAL(8, 88, 13), // "SignalTickRsp"
QT_MOC_LITERAL(9, 102, 10) // "GetSymbols"

    },
    "WRClient\0SignalNetErr\0\0SignalLogin\0"
    "SignalSymbols\0CSymbol*\0SignalQuote\0"
    "const CQuotation*\0SignalTickRsp\0"
    "GetSymbols"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WRClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,
       4,    2,   50,    2, 0x06 /* Public */,
       6,    1,   55,    2, 0x06 /* Public */,
       8,    1,   58,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 7,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void WRClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WRClient *_t = static_cast<WRClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SignalNetErr((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->SignalLogin((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->SignalSymbols((*reinterpret_cast< CSymbol*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->SignalQuote((*reinterpret_cast< const CQuotation*(*)>(_a[1]))); break;
        case 4: _t->SignalTickRsp((*reinterpret_cast< const CQuotation*(*)>(_a[1]))); break;
        case 5: _t->GetSymbols(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WRClient::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WRClient::SignalNetErr)) {
                *result = 0;
            }
        }
        {
            typedef void (WRClient::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WRClient::SignalLogin)) {
                *result = 1;
            }
        }
        {
            typedef void (WRClient::*_t)(CSymbol * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WRClient::SignalSymbols)) {
                *result = 2;
            }
        }
        {
            typedef void (WRClient::*_t)(const CQuotation * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WRClient::SignalQuote)) {
                *result = 3;
            }
        }
        {
            typedef void (WRClient::*_t)(const CQuotation * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WRClient::SignalTickRsp)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject WRClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WRClient.data,
      qt_meta_data_WRClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WRClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WRClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WRClient.stringdata0))
        return static_cast<void*>(const_cast< WRClient*>(this));
    if (!strcmp(_clname, "SocketReaderSpi"))
        return static_cast< SocketReaderSpi*>(const_cast< WRClient*>(this));
    if (!strcmp(_clname, "ReConnSpi"))
        return static_cast< ReConnSpi*>(const_cast< WRClient*>(this));
    if (!strcmp(_clname, "TimerSpi"))
        return static_cast< TimerSpi*>(const_cast< WRClient*>(this));
    return QObject::qt_metacast(_clname);
}

int WRClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void WRClient::SignalNetErr(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WRClient::SignalLogin(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WRClient::SignalSymbols(CSymbol * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WRClient::SignalQuote(const CQuotation * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WRClient::SignalTickRsp(const CQuotation * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
