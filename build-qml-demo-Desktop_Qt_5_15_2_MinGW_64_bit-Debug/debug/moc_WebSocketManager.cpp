/****************************************************************************
** Meta object code from reading C++ file 'WebSocketManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../qml-demo/WebSocketManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WebSocketManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WebSocketManager_t {
    QByteArrayData data[20];
    char stringdata0[317];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebSocketManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebSocketManager_t qt_meta_stringdata_WebSocketManager = {
    {
QT_MOC_LITERAL(0, 0, 16), // "WebSocketManager"
QT_MOC_LITERAL(1, 17, 14), // "clientAccepted"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "clientId"
QT_MOC_LITERAL(4, 42, 27), // "incomingTextMessageReceived"
QT_MOC_LITERAL(5, 70, 7), // "message"
QT_MOC_LITERAL(6, 78, 23), // "sendOutgoingTextMessage"
QT_MOC_LITERAL(7, 102, 16), // "deviceIDReceived"
QT_MOC_LITERAL(8, 119, 8), // "deviceID"
QT_MOC_LITERAL(9, 128, 15), // "leqDataReceived"
QT_MOC_LITERAL(10, 144, 4), // "data"
QT_MOC_LITERAL(11, 149, 15), // "fftDataReceived"
QT_MOC_LITERAL(12, 165, 16), // "prpdDataReceived"
QT_MOC_LITERAL(13, 182, 16), // "waveDataReceived"
QT_MOC_LITERAL(14, 199, 7), // "channel"
QT_MOC_LITERAL(15, 207, 14), // "QVector<float>"
QT_MOC_LITERAL(16, 222, 16), // "onClientAccepted"
QT_MOC_LITERAL(17, 239, 29), // "onIncomingTextMessageReceived"
QT_MOC_LITERAL(18, 269, 31), // "onIncomingBinaryMessageReceived"
QT_MOC_LITERAL(19, 301, 15) // "sendTextMessage"

    },
    "WebSocketManager\0clientAccepted\0\0"
    "clientId\0incomingTextMessageReceived\0"
    "message\0sendOutgoingTextMessage\0"
    "deviceIDReceived\0deviceID\0leqDataReceived\0"
    "data\0fftDataReceived\0prpdDataReceived\0"
    "waveDataReceived\0channel\0QVector<float>\0"
    "onClientAccepted\0onIncomingTextMessageReceived\0"
    "onIncomingBinaryMessageReceived\0"
    "sendTextMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebSocketManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       6,    1,   80,    2, 0x06 /* Public */,
       7,    1,   83,    2, 0x06 /* Public */,
       9,    1,   86,    2, 0x06 /* Public */,
      11,    1,   89,    2, 0x06 /* Public */,
      12,    1,   92,    2, 0x06 /* Public */,
      13,    2,   95,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,  100,    2, 0x0a /* Public */,
      17,    1,  103,    2, 0x0a /* Public */,
      18,    1,  106,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      19,    1,  109,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,   14,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void WebSocketManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WebSocketManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clientAccepted((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->incomingTextMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->sendOutgoingTextMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->deviceIDReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->leqDataReceived((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->fftDataReceived((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->prpdDataReceived((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 7: _t->waveDataReceived((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QVector<float>(*)>(_a[2]))); break;
        case 8: _t->onClientAccepted((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->onIncomingTextMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->onIncomingBinaryMessageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 11: _t->sendTextMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WebSocketManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::clientAccepted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WebSocketManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::incomingTextMessageReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WebSocketManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::sendOutgoingTextMessage)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WebSocketManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::deviceIDReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (WebSocketManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::leqDataReceived)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (WebSocketManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::fftDataReceived)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (WebSocketManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::prpdDataReceived)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (WebSocketManager::*)(const int , const QVector<float> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WebSocketManager::waveDataReceived)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WebSocketManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WebSocketManager.data,
    qt_meta_data_WebSocketManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WebSocketManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebSocketManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WebSocketManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WebSocketManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void WebSocketManager::clientAccepted(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WebSocketManager::incomingTextMessageReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WebSocketManager::sendOutgoingTextMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WebSocketManager::deviceIDReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WebSocketManager::leqDataReceived(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void WebSocketManager::fftDataReceived(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void WebSocketManager::prpdDataReceived(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void WebSocketManager::waveDataReceived(const int _t1, const QVector<float> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
