/****************************************************************************
** Meta object code from reading C++ file 'uithread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../qml-demo/uithread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uithread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UIThread_t {
    QByteArrayData data[16];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UIThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UIThread_t qt_meta_stringdata_UIThread = {
    {
QT_MOC_LITERAL(0, 0, 8), // "UIThread"
QT_MOC_LITERAL(1, 9, 13), // "sendFinalData"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 4), // "data"
QT_MOC_LITERAL(4, 29, 7), // "setType"
QT_MOC_LITERAL(5, 37, 5), // "type_"
QT_MOC_LITERAL(6, 43, 7), // "setData"
QT_MOC_LITERAL(7, 51, 5), // "data_"
QT_MOC_LITERAL(8, 57, 14), // "changeTimeLine"
QT_MOC_LITERAL(9, 72, 8), // "timeline"
QT_MOC_LITERAL(10, 81, 9), // "clearData"
QT_MOC_LITERAL(11, 91, 9), // "setEVdata"
QT_MOC_LITERAL(12, 101, 3), // "ev_"
QT_MOC_LITERAL(13, 105, 8), // "baseTime"
QT_MOC_LITERAL(14, 114, 2), // "dt"
QT_MOC_LITERAL(15, 117, 10) // "frameCount"

    },
    "UIThread\0sendFinalData\0\0data\0setType\0"
    "type_\0setData\0data_\0changeTimeLine\0"
    "timeline\0clearData\0setEVdata\0ev_\0"
    "baseTime\0dt\0frameCount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UIThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       4,    1,   47,    2, 0x02 /* Public */,
       6,    1,   50,    2, 0x02 /* Public */,
       8,    1,   53,    2, 0x02 /* Public */,
      10,    0,   56,    2, 0x02 /* Public */,
      11,    4,   57,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::QString, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Double, QMetaType::Int,   12,   13,   14,   15,

       0        // eod
};

void UIThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UIThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendFinalData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setType((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: { QString _r = _t->changeTimeLine((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->clearData(); break;
        case 5: _t->setEVdata((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UIThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UIThread::sendFinalData)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UIThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_UIThread.data,
    qt_meta_data_UIThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UIThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UIThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UIThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int UIThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void UIThread::sendFinalData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
