/****************************************************************************
** Meta object code from reading C++ file 'VideoThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../qml-demo/util-UIElement/cppElement/VideoQuickItem/VideoThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoThread_t {
    QByteArrayData data[34];
    char stringdata0[442];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoThread_t qt_meta_stringdata_VideoThread = {
    {
QT_MOC_LITERAL(0, 0, 11), // "VideoThread"
QT_MOC_LITERAL(1, 12, 8), // "sendData"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 6), // "p_data"
QT_MOC_LITERAL(4, 29, 12), // "sendTimeAxis"
QT_MOC_LITERAL(5, 42, 12), // "QList<float>"
QT_MOC_LITERAL(6, 55, 12), // "timeAxisList"
QT_MOC_LITERAL(7, 68, 13), // "sendVideoData"
QT_MOC_LITERAL(8, 82, 28), // "std::map<int,unsigned char*>"
QT_MOC_LITERAL(9, 111, 8), // "picture_"
QT_MOC_LITERAL(10, 120, 31), // "std::map<int,RhythmUI::picture>"
QT_MOC_LITERAL(11, 152, 5), // "beam_"
QT_MOC_LITERAL(12, 158, 14), // "sendCameraData"
QT_MOC_LITERAL(13, 173, 16), // "QMap<int,QImage>"
QT_MOC_LITERAL(14, 190, 9), // "sendVideo"
QT_MOC_LITERAL(15, 200, 22), // "QVector<unsigned char>"
QT_MOC_LITERAL(16, 223, 8), // "sendBeam"
QT_MOC_LITERAL(17, 232, 13), // "cloudPicture_"
QT_MOC_LITERAL(18, 246, 6), // "float&"
QT_MOC_LITERAL(19, 253, 8), // "beamMax_"
QT_MOC_LITERAL(20, 262, 8), // "beamMin_"
QT_MOC_LITERAL(21, 271, 8), // "setVideo"
QT_MOC_LITERAL(22, 280, 6), // "width_"
QT_MOC_LITERAL(23, 287, 7), // "height_"
QT_MOC_LITERAL(24, 295, 16), // "QVector<quint8>&"
QT_MOC_LITERAL(25, 312, 7), // "setBeam"
QT_MOC_LITERAL(26, 320, 15), // "QVector<float>&"
QT_MOC_LITERAL(27, 336, 9), // "setParams"
QT_MOC_LITERAL(28, 346, 28), // "RhythmUI::AcousticImageModes"
QT_MOC_LITERAL(29, 375, 16), // "filterModelType_"
QT_MOC_LITERAL(30, 392, 13), // "beamLimitMax_"
QT_MOC_LITERAL(31, 406, 11), // "peakFactor_"
QT_MOC_LITERAL(32, 418, 13), // "dynamicRange_"
QT_MOC_LITERAL(33, 432, 9) // "distance_"

    },
    "VideoThread\0sendData\0\0p_data\0sendTimeAxis\0"
    "QList<float>\0timeAxisList\0sendVideoData\0"
    "std::map<int,unsigned char*>\0picture_\0"
    "std::map<int,RhythmUI::picture>\0beam_\0"
    "sendCameraData\0QMap<int,QImage>\0"
    "sendVideo\0QVector<unsigned char>\0"
    "sendBeam\0cloudPicture_\0float&\0beamMax_\0"
    "beamMin_\0setVideo\0width_\0height_\0"
    "QVector<quint8>&\0setBeam\0QVector<float>&\0"
    "setParams\0RhythmUI::AcousticImageModes\0"
    "filterModelType_\0beamLimitMax_\0"
    "peakFactor_\0dynamicRange_\0distance_"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       7,    2,   65,    2, 0x06 /* Public */,
      12,    2,   70,    2, 0x06 /* Public */,
      14,    1,   75,    2, 0x06 /* Public */,
      16,    3,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    3,   85,    2, 0x0a /* Public */,
      25,    3,   92,    2, 0x0a /* Public */,
      27,    7,   99,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10,    9,   11,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 10,    2,   11,
    QMetaType::Void, 0x80000000 | 15,    9,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 18, 0x80000000 | 18,   17,   19,   20,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, 0x80000000 | 24,   22,   23,    9,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, 0x80000000 | 26,   22,   23,   11,
    QMetaType::Void, 0x80000000 | 28, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::UInt,   29,   19,   20,   30,   31,   32,   33,

       0        // eod
};

void VideoThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendData((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->sendTimeAxis((*reinterpret_cast< QList<float>(*)>(_a[1]))); break;
        case 2: _t->sendVideoData((*reinterpret_cast< std::map<int,unsigned char*>(*)>(_a[1])),(*reinterpret_cast< std::map<int,RhythmUI::picture>(*)>(_a[2]))); break;
        case 3: _t->sendCameraData((*reinterpret_cast< QMap<int,QImage>(*)>(_a[1])),(*reinterpret_cast< std::map<int,RhythmUI::picture>(*)>(_a[2]))); break;
        case 4: _t->sendVideo((*reinterpret_cast< QVector<unsigned char>(*)>(_a[1]))); break;
        case 5: _t->sendBeam((*reinterpret_cast< QVector<unsigned char>(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 6: _t->setVideo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< QVector<quint8>(*)>(_a[3]))); break;
        case 7: _t->setBeam((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< QVector<float>(*)>(_a[3]))); break;
        case 8: _t->setParams((*reinterpret_cast< RhythmUI::AcousticImageModes(*)>(_a[1])),(*reinterpret_cast< const float(*)>(_a[2])),(*reinterpret_cast< const float(*)>(_a[3])),(*reinterpret_cast< const float(*)>(_a[4])),(*reinterpret_cast< const float(*)>(_a[5])),(*reinterpret_cast< const float(*)>(_a[6])),(*reinterpret_cast< const quint32(*)>(_a[7]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<float> >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<unsigned char> >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<unsigned char> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoThread::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoThread::sendData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoThread::*)(QList<float> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoThread::sendTimeAxis)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoThread::*)(std::map<int,unsigned char*> , std::map<int,RhythmUI::picture> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoThread::sendVideoData)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoThread::*)(QMap<int,QImage> , std::map<int,RhythmUI::picture> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoThread::sendCameraData)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoThread::*)(QVector<unsigned char> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoThread::sendVideo)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoThread::*)(QVector<unsigned char> , float & , float & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoThread::sendBeam)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_VideoThread.data,
    qt_meta_data_VideoThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int VideoThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void VideoThread::sendData(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoThread::sendTimeAxis(QList<float> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoThread::sendVideoData(std::map<int,unsigned char*> _t1, std::map<int,RhythmUI::picture> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VideoThread::sendCameraData(QMap<int,QImage> _t1, std::map<int,RhythmUI::picture> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VideoThread::sendVideo(QVector<unsigned char> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void VideoThread::sendBeam(QVector<unsigned char> _t1, float & _t2, float & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
