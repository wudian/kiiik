/****************************************************************************
** Meta object code from reading C++ file 'WrUiClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../WrUiClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WrUiClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
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
struct qt_meta_stringdata_WrLoginDlg_t {
    QByteArrayData data[5];
    char stringdata0[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WrLoginDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WrLoginDlg_t qt_meta_stringdata_WrLoginDlg = {
    {
QT_MOC_LITERAL(0, 0, 10), // "WrLoginDlg"
QT_MOC_LITERAL(1, 11, 7), // "onLogin"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 8), // "onAuthor"
QT_MOC_LITERAL(4, 29, 9) // "SlotLogin"

    },
    "WrLoginDlg\0onLogin\0\0onAuthor\0SlotLogin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WrLoginDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void WrLoginDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WrLoginDlg *_t = static_cast<WrLoginDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onLogin(); break;
        case 1: _t->onAuthor(); break;
        case 2: _t->SlotLogin((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject WrLoginDlg::staticMetaObject = {
    { &BaseDlg::staticMetaObject, qt_meta_stringdata_WrLoginDlg.data,
      qt_meta_data_WrLoginDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WrLoginDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WrLoginDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WrLoginDlg.stringdata0))
        return static_cast<void*>(const_cast< WrLoginDlg*>(this));
    if (!strcmp(_clname, "Ui::LoginDlg"))
        return static_cast< Ui::LoginDlg*>(const_cast< WrLoginDlg*>(this));
    return BaseDlg::qt_metacast(_clname);
}

int WrLoginDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseDlg::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
struct qt_meta_stringdata_UpdateDlg_t {
    QByteArrayData data[4];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UpdateDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UpdateDlg_t qt_meta_stringdata_UpdateDlg = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UpdateDlg"
QT_MOC_LITERAL(1, 10, 17), // "on_ok_btn_clicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 21) // "on_cancel_btn_clicked"

    },
    "UpdateDlg\0on_ok_btn_clicked\0\0"
    "on_cancel_btn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UpdateDlg[] = {

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

void UpdateDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UpdateDlg *_t = static_cast<UpdateDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_ok_btn_clicked(); break;
        case 1: _t->on_cancel_btn_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject UpdateDlg::staticMetaObject = {
    { &BaseDlg::staticMetaObject, qt_meta_stringdata_UpdateDlg.data,
      qt_meta_data_UpdateDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UpdateDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UpdateDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateDlg.stringdata0))
        return static_cast<void*>(const_cast< UpdateDlg*>(this));
    if (!strcmp(_clname, "Ui::UpdateDlg"))
        return static_cast< Ui::UpdateDlg*>(const_cast< UpdateDlg*>(this));
    return BaseDlg::qt_metacast(_clname);
}

int UpdateDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_WrUiClient_t {
    QByteArrayData data[15];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WrUiClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WrUiClient_t qt_meta_stringdata_WrUiClient = {
    {
QT_MOC_LITERAL(0, 0, 10), // "WrUiClient"
QT_MOC_LITERAL(1, 11, 11), // "onTestWrite"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 5), // "onSub"
QT_MOC_LITERAL(4, 30, 7), // "onUnsub"
QT_MOC_LITERAL(5, 38, 7), // "onQuote"
QT_MOC_LITERAL(6, 46, 24), // "on_btnMenu_Close_clicked"
QT_MOC_LITERAL(7, 71, 22), // "on_btnMenu_Max_clicked"
QT_MOC_LITERAL(8, 94, 22), // "on_btnMenu_Min_clicked"
QT_MOC_LITERAL(9, 117, 18), // "on_btnMenu_clicked"
QT_MOC_LITERAL(10, 136, 11), // "SlotSymbols"
QT_MOC_LITERAL(11, 148, 8), // "CSymbol*"
QT_MOC_LITERAL(12, 157, 9), // "SlotQuote"
QT_MOC_LITERAL(13, 167, 17), // "const CQuotation*"
QT_MOC_LITERAL(14, 185, 10) // "SlotNetErr"

    },
    "WrUiClient\0onTestWrite\0\0onSub\0onUnsub\0"
    "onQuote\0on_btnMenu_Close_clicked\0"
    "on_btnMenu_Max_clicked\0on_btnMenu_Min_clicked\0"
    "on_btnMenu_clicked\0SlotSymbols\0CSymbol*\0"
    "SlotQuote\0const CQuotation*\0SlotNetErr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WrUiClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    1,   77,    2, 0x08 /* Private */,
       5,    0,   80,    2, 0x28 /* Private | MethodCloned */,
       6,    0,   81,    2, 0x08 /* Private */,
       7,    0,   82,    2, 0x08 /* Private */,
       8,    0,   83,    2, 0x08 /* Private */,
       9,    0,   84,    2, 0x08 /* Private */,
      10,    2,   85,    2, 0x08 /* Private */,
      12,    1,   90,    2, 0x08 /* Private */,
      14,    1,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void WrUiClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WrUiClient *_t = static_cast<WrUiClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTestWrite(); break;
        case 1: _t->onSub(); break;
        case 2: _t->onUnsub(); break;
        case 3: _t->onQuote((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onQuote(); break;
        case 5: _t->on_btnMenu_Close_clicked(); break;
        case 6: _t->on_btnMenu_Max_clicked(); break;
        case 7: _t->on_btnMenu_Min_clicked(); break;
        case 8: _t->on_btnMenu_clicked(); break;
        case 9: _t->SlotSymbols((*reinterpret_cast< CSymbol*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->SlotQuote((*reinterpret_cast< const CQuotation*(*)>(_a[1]))); break;
        case 11: _t->SlotNetErr((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject WrUiClient::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WrUiClient.data,
      qt_meta_data_WrUiClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WrUiClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WrUiClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WrUiClient.stringdata0))
        return static_cast<void*>(const_cast< WrUiClient*>(this));
    if (!strcmp(_clname, "Ui::MainWindow"))
        return static_cast< Ui::MainWindow*>(const_cast< WrUiClient*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WrUiClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
