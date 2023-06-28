/****************************************************************************
** Meta object code from reading C++ file 'datathread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../qml-demo/datathread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datathread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataThread_t {
    QByteArrayData data[53];
    char stringdata0[598];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataThread_t qt_meta_stringdata_DataThread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DataThread"
QT_MOC_LITERAL(1, 11, 20), // "sendEigenvalueByType"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 4), // "json"
QT_MOC_LITERAL(4, 38, 9), // "channelId"
QT_MOC_LITERAL(5, 48, 4), // "type"
QT_MOC_LITERAL(6, 53, 20), // "sendSpectraByChannel"
QT_MOC_LITERAL(7, 74, 14), // "QList<QString>"
QT_MOC_LITERAL(8, 89, 4), // "data"
QT_MOC_LITERAL(9, 94, 21), // "sendTimedataByChannel"
QT_MOC_LITERAL(10, 116, 12), // "QList<float>"
QT_MOC_LITERAL(11, 129, 2), // "dt"
QT_MOC_LITERAL(12, 132, 3), // "len"
QT_MOC_LITERAL(13, 136, 3), // "max"
QT_MOC_LITERAL(14, 140, 13), // "sendFinalData"
QT_MOC_LITERAL(15, 154, 30), // "QMap<quint64,QVector<quint8> >"
QT_MOC_LITERAL(16, 185, 7), // "picture"
QT_MOC_LITERAL(17, 193, 24), // "QMap<quint64,QByteArray>"
QT_MOC_LITERAL(18, 218, 6), // "camera"
QT_MOC_LITERAL(19, 225, 29), // "QMap<quint64,QVector<float> >"
QT_MOC_LITERAL(20, 255, 4), // "beam"
QT_MOC_LITERAL(21, 260, 7), // "spectra"
QT_MOC_LITERAL(22, 268, 13), // "spectra_local"
QT_MOC_LITERAL(23, 282, 3), // "raw"
QT_MOC_LITERAL(24, 286, 4), // "prpd"
QT_MOC_LITERAL(25, 291, 3), // "leq"
QT_MOC_LITERAL(26, 295, 2), // "ev"
QT_MOC_LITERAL(27, 298, 8), // "time_len"
QT_MOC_LITERAL(28, 307, 23), // "getEigenvalueDataThread"
QT_MOC_LITERAL(29, 331, 25), // "QMap<quint64,QJsonObject>"
QT_MOC_LITERAL(30, 357, 3), // "ev_"
QT_MOC_LITERAL(31, 361, 20), // "getSpectraDataThread"
QT_MOC_LITERAL(32, 382, 4), // "spa_"
QT_MOC_LITERAL(33, 387, 7), // "spa_len"
QT_MOC_LITERAL(34, 395, 17), // "getTimedataThread"
QT_MOC_LITERAL(35, 413, 5), // "data_"
QT_MOC_LITERAL(36, 419, 13), // "sortFinalData"
QT_MOC_LITERAL(37, 433, 8), // "getFinal"
QT_MOC_LITERAL(38, 442, 8), // "addAudio"
QT_MOC_LITERAL(39, 451, 4), // "time"
QT_MOC_LITERAL(40, 456, 14), // "QVector<float>"
QT_MOC_LITERAL(41, 471, 6), // "addRaw"
QT_MOC_LITERAL(42, 478, 10), // "addPicture"
QT_MOC_LITERAL(43, 489, 15), // "QVector<quint8>"
QT_MOC_LITERAL(44, 505, 7), // "addBeam"
QT_MOC_LITERAL(45, 513, 10), // "addSpectra"
QT_MOC_LITERAL(46, 524, 15), // "addSpectraLocal"
QT_MOC_LITERAL(47, 540, 14), // "addSpectraJson"
QT_MOC_LITERAL(48, 555, 7), // "addPRPD"
QT_MOC_LITERAL(49, 563, 5), // "addEv"
QT_MOC_LITERAL(50, 569, 6), // "addLeq"
QT_MOC_LITERAL(51, 576, 11), // "addTimedata"
QT_MOC_LITERAL(52, 588, 9) // "addCamera"

    },
    "DataThread\0sendEigenvalueByType\0\0json\0"
    "channelId\0type\0sendSpectraByChannel\0"
    "QList<QString>\0data\0sendTimedataByChannel\0"
    "QList<float>\0dt\0len\0max\0sendFinalData\0"
    "QMap<quint64,QVector<quint8> >\0picture\0"
    "QMap<quint64,QByteArray>\0camera\0"
    "QMap<quint64,QVector<float> >\0beam\0"
    "spectra\0spectra_local\0raw\0prpd\0leq\0"
    "ev\0time_len\0getEigenvalueDataThread\0"
    "QMap<quint64,QJsonObject>\0ev_\0"
    "getSpectraDataThread\0spa_\0spa_len\0"
    "getTimedataThread\0data_\0sortFinalData\0"
    "getFinal\0addAudio\0time\0QVector<float>\0"
    "addRaw\0addPicture\0QVector<quint8>\0"
    "addBeam\0addSpectra\0addSpectraLocal\0"
    "addSpectraJson\0addPRPD\0addEv\0addLeq\0"
    "addTimedata\0addCamera"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,  119,    2, 0x06 /* Public */,
       6,    2,  126,    2, 0x06 /* Public */,
       9,    5,  131,    2, 0x06 /* Public */,
      14,   10,  142,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      28,    3,  163,    2, 0x0a /* Public */,
      31,    3,  170,    2, 0x0a /* Public */,
      34,    2,  177,    2, 0x0a /* Public */,
      36,    0,  182,    2, 0x0a /* Public */,
      37,    0,  183,    2, 0x0a /* Public */,
      38,    2,  184,    2, 0x0a /* Public */,
      41,    2,  189,    2, 0x0a /* Public */,
      42,    2,  194,    2, 0x0a /* Public */,
      44,    2,  199,    2, 0x0a /* Public */,
      45,    3,  204,    2, 0x0a /* Public */,
      46,    3,  211,    2, 0x0a /* Public */,
      47,    3,  218,    2, 0x0a /* Public */,
      48,    2,  225,    2, 0x0a /* Public */,
      49,    3,  230,    2, 0x0a /* Public */,
      50,    3,  237,    2, 0x0a /* Public */,
      51,    3,  244,    2, 0x0a /* Public */,
      52,    2,  251,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QString,    8,    4,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Float, QMetaType::QString, QMetaType::Int, QMetaType::Float,    8,   11,    4,   12,   13,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 17, 0x80000000 | 19, 0x80000000 | 19, 0x80000000 | 19, 0x80000000 | 19, QMetaType::QJsonObject, QMetaType::QJsonObject, QMetaType::QJsonObject, QMetaType::UInt,   16,   18,   20,   21,   22,   23,   24,   25,   26,   27,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 29, QMetaType::QString, QMetaType::QString,   30,    4,    5,
    QMetaType::Void, 0x80000000 | 29, QMetaType::UInt, QMetaType::QString,   32,   33,    4,
    QMetaType::Void, 0x80000000 | 29, QMetaType::QString,   35,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 40,   39,    8,
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 40,   39,    8,
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 43,   39,    8,
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 40,   39,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, 0x80000000 | 40,    4,   39,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, 0x80000000 | 40,    4,   39,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::QJsonObject,    4,   39,    8,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QJsonObject,   39,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::QJsonObject,    4,   39,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::QJsonObject,    4,   39,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::QJsonObject,    4,   39,    8,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QByteArray,   39,    8,

       0        // eod
};

void DataThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendEigenvalueByType((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->sendSpectraByChannel((*reinterpret_cast< QList<QString>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->sendTimedataByChannel((*reinterpret_cast< QList<float>(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5]))); break;
        case 3: _t->sendFinalData((*reinterpret_cast< QMap<quint64,QVector<quint8> >(*)>(_a[1])),(*reinterpret_cast< QMap<quint64,QByteArray>(*)>(_a[2])),(*reinterpret_cast< QMap<quint64,QVector<float> >(*)>(_a[3])),(*reinterpret_cast< QMap<quint64,QVector<float> >(*)>(_a[4])),(*reinterpret_cast< QMap<quint64,QVector<float> >(*)>(_a[5])),(*reinterpret_cast< QMap<quint64,QVector<float> >(*)>(_a[6])),(*reinterpret_cast< QJsonObject(*)>(_a[7])),(*reinterpret_cast< QJsonObject(*)>(_a[8])),(*reinterpret_cast< QJsonObject(*)>(_a[9])),(*reinterpret_cast< quint32(*)>(_a[10]))); break;
        case 4: _t->getEigenvalueDataThread((*reinterpret_cast< QMap<quint64,QJsonObject>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->getSpectraDataThread((*reinterpret_cast< QMap<quint64,QJsonObject>(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 6: _t->getTimedataThread((*reinterpret_cast< QMap<quint64,QJsonObject>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->sortFinalData(); break;
        case 8: _t->getFinal(); break;
        case 9: _t->addAudio((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QVector<float>(*)>(_a[2]))); break;
        case 10: _t->addRaw((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QVector<float>(*)>(_a[2]))); break;
        case 11: _t->addPicture((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QVector<quint8>(*)>(_a[2]))); break;
        case 12: _t->addBeam((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QVector<float>(*)>(_a[2]))); break;
        case 13: _t->addSpectra((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< QVector<float>(*)>(_a[3]))); break;
        case 14: _t->addSpectraLocal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< QVector<float>(*)>(_a[3]))); break;
        case 15: _t->addSpectraJson((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< QJsonObject(*)>(_a[3]))); break;
        case 16: _t->addPRPD((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        case 17: _t->addEv((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< QJsonObject(*)>(_a[3]))); break;
        case 18: _t->addLeq((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< QJsonObject(*)>(_a[3]))); break;
        case 19: _t->addTimedata((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< QJsonObject(*)>(_a[3]))); break;
        case 20: _t->addCamera((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QString> >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<float> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<quint8> >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataThread::*)(const QString & , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataThread::sendEigenvalueByType)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataThread::*)(QList<QString> , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataThread::sendSpectraByChannel)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataThread::*)(QList<float> , float , QString , int , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataThread::sendTimedataByChannel)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataThread::*)(QMap<quint64,QVector<quint8>> , QMap<quint64,QByteArray> , QMap<quint64,QVector<float>> , QMap<quint64,QVector<float>> , QMap<quint64,QVector<float>> , QMap<quint64,QVector<float>> , QJsonObject , QJsonObject , QJsonObject , quint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataThread::sendFinalData)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_DataThread.data,
    qt_meta_data_DataThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int DataThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void DataThread::sendEigenvalueByType(const QString & _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataThread::sendSpectraByChannel(QList<QString> _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataThread::sendTimedataByChannel(QList<float> _t1, float _t2, QString _t3, int _t4, float _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataThread::sendFinalData(QMap<quint64,QVector<quint8>> _t1, QMap<quint64,QByteArray> _t2, QMap<quint64,QVector<float>> _t3, QMap<quint64,QVector<float>> _t4, QMap<quint64,QVector<float>> _t5, QMap<quint64,QVector<float>> _t6, QJsonObject _t7, QJsonObject _t8, QJsonObject _t9, quint32 _t10)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t8))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t9))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t10))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
