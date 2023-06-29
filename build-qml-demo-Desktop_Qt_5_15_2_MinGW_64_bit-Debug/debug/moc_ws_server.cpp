/****************************************************************************
** Meta object code from reading C++ file 'ws_server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../qml-demo/ws_server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ws_server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WsServer_t {
    QByteArrayData data[19];
    char stringdata0[290];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WsServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WsServer_t qt_meta_stringdata_WsServer = {
    {
QT_MOC_LITERAL(0, 0, 8), // "WsServer"
QT_MOC_LITERAL(1, 9, 14), // "clientAccepted"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "clientId"
QT_MOC_LITERAL(4, 34, 27), // "incomingTextMessageReceived"
QT_MOC_LITERAL(5, 62, 7), // "message"
QT_MOC_LITERAL(6, 70, 29), // "incomingBinaryMessageReceived"
QT_MOC_LITERAL(7, 100, 17), // "sendIpAddressRate"
QT_MOC_LITERAL(8, 118, 23), // "QHash<QString,quint64>&"
QT_MOC_LITERAL(9, 142, 4), // "rate"
QT_MOC_LITERAL(10, 147, 12), // "serverClosed"
QT_MOC_LITERAL(11, 160, 23), // "sendOutgoingTextMessage"
QT_MOC_LITERAL(12, 184, 25), // "sendOutgoingBinaryMessage"
QT_MOC_LITERAL(13, 210, 4), // "data"
QT_MOC_LITERAL(14, 215, 15), // "onNewConnection"
QT_MOC_LITERAL(15, 231, 18), // "clientDisconnected"
QT_MOC_LITERAL(16, 250, 13), // "onTextMessage"
QT_MOC_LITERAL(17, 264, 15), // "onBinaryMessage"
QT_MOC_LITERAL(18, 280, 9) // "checkRate"

    },
    "WsServer\0clientAccepted\0\0clientId\0"
    "incomingTextMessageReceived\0message\0"
    "incomingBinaryMessageReceived\0"
    "sendIpAddressRate\0QHash<QString,quint64>&\0"
    "rate\0serverClosed\0sendOutgoingTextMessage\0"
    "sendOutgoingBinaryMessage\0data\0"
    "onNewConnection\0clientDisconnected\0"
    "onTextMessage\0onBinaryMessage\0checkRate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WsServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       6,    1,   80,    2, 0x06 /* Public */,
       7,    1,   83,    2, 0x06 /* Public */,
      10,    0,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   87,    2, 0x0a /* Public */,
      12,    1,   90,    2, 0x0a /* Public */,
      14,    0,   93,    2, 0x08 /* Private */,
      15,    0,   94,    2, 0x08 /* Private */,
      16,    1,   95,    2, 0x08 /* Private */,
      17,    1,   98,    2, 0x08 /* Private */,
      18,    0,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,

 // slots: parameters
    QMetaType::LongLong, QMetaType::QString,    5,
    QMetaType::LongLong, QMetaType::QByteArray,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void,

       0        // eod
};

void WsServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WsServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clientAccepted((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->incomingTextMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->incomingBinaryMessageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->sendIpAddressRate((*reinterpret_cast< QHash<QString,quint64>(*)>(_a[1]))); break;
        case 4: _t->serverClosed(); break;
        case 5: { qint64 _r = _t->sendOutgoingTextMessage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = std::move(_r); }  break;
        case 6: { qint64 _r = _t->sendOutgoingBinaryMessage((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->onNewConnection(); break;
        case 8: _t->clientDisconnected(); break;
        case 9: _t->onTextMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->onBinaryMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 11: _t->checkRate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WsServer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsServer::clientAccepted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WsServer::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsServer::incomingTextMessageReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WsServer::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsServer::incomingBinaryMessageReceived)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WsServer::*)(QHash<QString,quint64> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsServer::sendIpAddressRate)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (WsServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsServer::serverClosed)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WsServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WsServer.data,
    qt_meta_data_WsServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WsServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WsServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WsServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WsServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void WsServer::clientAccepted(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WsServer::incomingTextMessageReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WsServer::incomingBinaryMessageReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WsServer::sendIpAddressRate(QHash<QString,quint64> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WsServer::serverClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
