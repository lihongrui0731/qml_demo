/****************************************************************************
** Meta object code from reading C++ file 'RhythmVideo.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../qml-demo/util-UIElement/cppElement/VideoQuickItem/RhythmVideo.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RhythmVideo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RhythmVideo_t {
    QByteArrayData data[102];
    char stringdata0[1282];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RhythmVideo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RhythmVideo_t qt_meta_stringdata_RhythmVideo = {
    {
QT_MOC_LITERAL(0, 0, 11), // "RhythmVideo"
QT_MOC_LITERAL(1, 12, 11), // "fullChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 19), // "nightVersionChanged"
QT_MOC_LITERAL(4, 45, 18), // "nightMethodChanged"
QT_MOC_LITERAL(5, 64, 12), // "frameChanged"
QT_MOC_LITERAL(6, 77, 17), // "cloudFrameChanged"
QT_MOC_LITERAL(7, 95, 17), // "brightnessChanged"
QT_MOC_LITERAL(8, 113, 15), // "contrastChanged"
QT_MOC_LITERAL(9, 129, 17), // "saturationChanged"
QT_MOC_LITERAL(10, 147, 16), // "highLightChanged"
QT_MOC_LITERAL(11, 164, 13), // "shadowChanged"
QT_MOC_LITERAL(12, 178, 14), // "midtoneChanged"
QT_MOC_LITERAL(13, 193, 18), // "coefficientChanged"
QT_MOC_LITERAL(14, 212, 15), // "timeAxisChanged"
QT_MOC_LITERAL(15, 228, 16), // "beamCountChanged"
QT_MOC_LITERAL(16, 245, 10), // "frameCount"
QT_MOC_LITERAL(17, 256, 9), // "frameTime"
QT_MOC_LITERAL(18, 266, 14), // "QList<quint32>"
QT_MOC_LITERAL(19, 281, 8), // "timeList"
QT_MOC_LITERAL(20, 290, 12), // "QList<float>"
QT_MOC_LITERAL(21, 303, 8), // "timeAxis"
QT_MOC_LITERAL(22, 312, 8), // "sendPath"
QT_MOC_LITERAL(23, 321, 4), // "file"
QT_MOC_LITERAL(24, 326, 6), // "finish"
QT_MOC_LITERAL(25, 333, 15), // "distanceChanged"
QT_MOC_LITERAL(26, 349, 8), // "distance"
QT_MOC_LITERAL(27, 358, 13), // "sendVideoData"
QT_MOC_LITERAL(28, 372, 6), // "width_"
QT_MOC_LITERAL(29, 379, 7), // "height_"
QT_MOC_LITERAL(30, 387, 16), // "QVector<quint8>&"
QT_MOC_LITERAL(31, 404, 8), // "picture_"
QT_MOC_LITERAL(32, 413, 12), // "sendBeamData"
QT_MOC_LITERAL(33, 426, 15), // "QVector<float>&"
QT_MOC_LITERAL(34, 442, 5), // "beam_"
QT_MOC_LITERAL(35, 448, 9), // "setParams"
QT_MOC_LITERAL(36, 458, 28), // "RhythmUI::AcousticImageModes"
QT_MOC_LITERAL(37, 487, 16), // "filterModelType_"
QT_MOC_LITERAL(38, 504, 8), // "beamMax_"
QT_MOC_LITERAL(39, 513, 8), // "beamMin_"
QT_MOC_LITERAL(40, 522, 13), // "beamLimitMax_"
QT_MOC_LITERAL(41, 536, 11), // "peakFactor_"
QT_MOC_LITERAL(42, 548, 13), // "dynamicRange_"
QT_MOC_LITERAL(43, 562, 9), // "distance_"
QT_MOC_LITERAL(44, 572, 13), // "seekBeamIndex"
QT_MOC_LITERAL(45, 586, 9), // "position_"
QT_MOC_LITERAL(46, 596, 14), // "seekPercentage"
QT_MOC_LITERAL(47, 611, 14), // "openFileFinish"
QT_MOC_LITERAL(48, 626, 12), // "getVideoData"
QT_MOC_LITERAL(49, 639, 28), // "std::map<int,unsigned char*>"
QT_MOC_LITERAL(50, 668, 31), // "std::map<int,RhythmUI::picture>"
QT_MOC_LITERAL(51, 700, 13), // "getCameraData"
QT_MOC_LITERAL(52, 714, 16), // "QMap<int,QImage>"
QT_MOC_LITERAL(53, 731, 7), // "camera_"
QT_MOC_LITERAL(54, 739, 18), // "getVideoFromThread"
QT_MOC_LITERAL(55, 758, 22), // "QVector<unsigned char>"
QT_MOC_LITERAL(56, 781, 17), // "getBeamFromThread"
QT_MOC_LITERAL(57, 799, 13), // "cloudPicture_"
QT_MOC_LITERAL(58, 813, 6), // "float&"
QT_MOC_LITERAL(59, 820, 11), // "getTimeAxis"
QT_MOC_LITERAL(60, 832, 11), // "beamMaxList"
QT_MOC_LITERAL(61, 844, 6), // "getMax"
QT_MOC_LITERAL(62, 851, 6), // "getMin"
QT_MOC_LITERAL(63, 858, 15), // "getDynamicRange"
QT_MOC_LITERAL(64, 874, 14), // "changeTimeLine"
QT_MOC_LITERAL(65, 889, 8), // "timeline"
QT_MOC_LITERAL(66, 898, 12), // "currentFrame"
QT_MOC_LITERAL(67, 911, 7), // "setData"
QT_MOC_LITERAL(68, 919, 4), // "data"
QT_MOC_LITERAL(69, 924, 17), // "setDataWithCamera"
QT_MOC_LITERAL(70, 942, 12), // "addVideoData"
QT_MOC_LITERAL(71, 955, 12), // "addImageData"
QT_MOC_LITERAL(72, 968, 11), // "addBeamData"
QT_MOC_LITERAL(73, 980, 12), // "getFinalData"
QT_MOC_LITERAL(74, 993, 6), // "video_"
QT_MOC_LITERAL(75, 1000, 18), // "getFinalCameraData"
QT_MOC_LITERAL(76, 1019, 9), // "clearData"
QT_MOC_LITERAL(77, 1029, 16), // "resetRhythmVideo"
QT_MOC_LITERAL(78, 1046, 12), // "setImageMode"
QT_MOC_LITERAL(79, 1059, 4), // "mode"
QT_MOC_LITERAL(80, 1064, 15), // "setDynamicRange"
QT_MOC_LITERAL(81, 1080, 6), // "range_"
QT_MOC_LITERAL(82, 1087, 13), // "setPeakFactor"
QT_MOC_LITERAL(83, 1101, 6), // "value_"
QT_MOC_LITERAL(84, 1108, 17), // "setMaxLimiteValue"
QT_MOC_LITERAL(85, 1126, 13), // "setBeamConfig"
QT_MOC_LITERAL(86, 1140, 12), // "dynamicRange"
QT_MOC_LITERAL(87, 1153, 4), // "peak"
QT_MOC_LITERAL(88, 1158, 9), // "maxLimite"
QT_MOC_LITERAL(89, 1168, 5), // "start"
QT_MOC_LITERAL(90, 1174, 4), // "stop"
QT_MOC_LITERAL(91, 1179, 4), // "play"
QT_MOC_LITERAL(92, 1184, 4), // "full"
QT_MOC_LITERAL(93, 1189, 12), // "nightVersion"
QT_MOC_LITERAL(94, 1202, 11), // "nightMethod"
QT_MOC_LITERAL(95, 1214, 10), // "brightness"
QT_MOC_LITERAL(96, 1225, 8), // "contrast"
QT_MOC_LITERAL(97, 1234, 10), // "saturation"
QT_MOC_LITERAL(98, 1245, 9), // "highLight"
QT_MOC_LITERAL(99, 1255, 6), // "shadow"
QT_MOC_LITERAL(100, 1262, 7), // "midtone"
QT_MOC_LITERAL(101, 1270, 11) // "coefficient"

    },
    "RhythmVideo\0fullChanged\0\0nightVersionChanged\0"
    "nightMethodChanged\0frameChanged\0"
    "cloudFrameChanged\0brightnessChanged\0"
    "contrastChanged\0saturationChanged\0"
    "highLightChanged\0shadowChanged\0"
    "midtoneChanged\0coefficientChanged\0"
    "timeAxisChanged\0beamCountChanged\0"
    "frameCount\0frameTime\0QList<quint32>\0"
    "timeList\0QList<float>\0timeAxis\0sendPath\0"
    "file\0finish\0distanceChanged\0distance\0"
    "sendVideoData\0width_\0height_\0"
    "QVector<quint8>&\0picture_\0sendBeamData\0"
    "QVector<float>&\0beam_\0setParams\0"
    "RhythmUI::AcousticImageModes\0"
    "filterModelType_\0beamMax_\0beamMin_\0"
    "beamLimitMax_\0peakFactor_\0dynamicRange_\0"
    "distance_\0seekBeamIndex\0position_\0"
    "seekPercentage\0openFileFinish\0"
    "getVideoData\0std::map<int,unsigned char*>\0"
    "std::map<int,RhythmUI::picture>\0"
    "getCameraData\0QMap<int,QImage>\0camera_\0"
    "getVideoFromThread\0QVector<unsigned char>\0"
    "getBeamFromThread\0cloudPicture_\0float&\0"
    "getTimeAxis\0beamMaxList\0getMax\0getMin\0"
    "getDynamicRange\0changeTimeLine\0timeline\0"
    "currentFrame\0setData\0data\0setDataWithCamera\0"
    "addVideoData\0addImageData\0addBeamData\0"
    "getFinalData\0video_\0getFinalCameraData\0"
    "clearData\0resetRhythmVideo\0setImageMode\0"
    "mode\0setDynamicRange\0range_\0setPeakFactor\0"
    "value_\0setMaxLimiteValue\0setBeamConfig\0"
    "dynamicRange\0peak\0maxLimite\0start\0"
    "stop\0play\0full\0nightVersion\0nightMethod\0"
    "brightness\0contrast\0saturation\0highLight\0"
    "shadow\0midtone\0coefficient"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RhythmVideo[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      50,   14, // methods
      11,  408, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      21,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  264,    2, 0x06 /* Public */,
       3,    0,  265,    2, 0x06 /* Public */,
       4,    0,  266,    2, 0x06 /* Public */,
       5,    0,  267,    2, 0x06 /* Public */,
       6,    0,  268,    2, 0x06 /* Public */,
       7,    0,  269,    2, 0x06 /* Public */,
       8,    0,  270,    2, 0x06 /* Public */,
       9,    0,  271,    2, 0x06 /* Public */,
      10,    0,  272,    2, 0x06 /* Public */,
      11,    0,  273,    2, 0x06 /* Public */,
      12,    0,  274,    2, 0x06 /* Public */,
      13,    0,  275,    2, 0x06 /* Public */,
      14,    0,  276,    2, 0x06 /* Public */,
      15,    3,  277,    2, 0x06 /* Public */,
      14,    1,  284,    2, 0x06 /* Public */,
      22,    1,  287,    2, 0x06 /* Public */,
      24,    0,  290,    2, 0x06 /* Public */,
      25,    1,  291,    2, 0x06 /* Public */,
      27,    3,  294,    2, 0x06 /* Public */,
      32,    3,  301,    2, 0x06 /* Public */,
      35,    7,  308,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      44,    1,  323,    2, 0x0a /* Public */,
      46,    1,  326,    2, 0x0a /* Public */,
      47,    0,  329,    2, 0x0a /* Public */,
      48,    2,  330,    2, 0x0a /* Public */,
      51,    2,  335,    2, 0x0a /* Public */,
      54,    1,  340,    2, 0x0a /* Public */,
      56,    3,  343,    2, 0x0a /* Public */,
      59,    1,  350,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      61,    0,  353,    2, 0x02 /* Public */,
      62,    0,  354,    2, 0x02 /* Public */,
      63,    0,  355,    2, 0x02 /* Public */,
      64,    2,  356,    2, 0x02 /* Public */,
      67,    1,  361,    2, 0x02 /* Public */,
      69,    1,  364,    2, 0x02 /* Public */,
      70,    1,  367,    2, 0x02 /* Public */,
      71,    1,  370,    2, 0x02 /* Public */,
      72,    1,  373,    2, 0x02 /* Public */,
      73,    1,  376,    2, 0x02 /* Public */,
      75,    1,  379,    2, 0x02 /* Public */,
      76,    0,  382,    2, 0x02 /* Public */,
      77,    0,  383,    2, 0x02 /* Public */,
      78,    1,  384,    2, 0x02 /* Public */,
      80,    1,  387,    2, 0x02 /* Public */,
      82,    1,  390,    2, 0x02 /* Public */,
      84,    1,  393,    2, 0x02 /* Public */,
      85,    4,  396,    2, 0x02 /* Public */,
      89,    0,  405,    2, 0x02 /* Public */,
      90,    0,  406,    2, 0x02 /* Public */,
      91,    0,  407,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 18,   16,   17,   19,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,   26,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, 0x80000000 | 30,   28,   29,   31,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, 0x80000000 | 33,   28,   29,   34,
    QMetaType::Void, 0x80000000 | 36, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::UInt,   37,   38,   39,   40,   41,   42,   43,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   45,
    QMetaType::Void, QMetaType::Float,   45,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 49, 0x80000000 | 50,   31,   34,
    QMetaType::Void, 0x80000000 | 52, 0x80000000 | 50,   53,   34,
    QMetaType::Void, 0x80000000 | 55,   31,
    QMetaType::Void, 0x80000000 | 55, 0x80000000 | 58, 0x80000000 | 58,   57,   38,   39,
    QMetaType::Void, 0x80000000 | 20,   60,

 // methods: parameters
    QMetaType::Float,
    QMetaType::Float,
    QMetaType::Float,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   65,   66,
    QMetaType::Void, QMetaType::QVariant,   68,
    QMetaType::Void, QMetaType::QVariant,   68,
    QMetaType::Void, QMetaType::QVariant,   68,
    QMetaType::Void, QMetaType::QVariant,   68,
    QMetaType::Void, QMetaType::QVariant,   68,
    QMetaType::Void, QMetaType::QVariant,   74,
    QMetaType::Void, QMetaType::QVariant,   53,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   79,
    QMetaType::Void, QMetaType::Float,   81,
    QMetaType::Void, QMetaType::Float,   83,
    QMetaType::Void, QMetaType::Float,   83,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float, QMetaType::Float,   79,   86,   87,   88,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      92, QMetaType::Bool, 0x00495103,
      93, QMetaType::Bool, 0x00495103,
      94, QMetaType::UChar, 0x00495103,
      95, QMetaType::Float, 0x00495103,
      96, QMetaType::Float, 0x00495103,
      97, QMetaType::Float, 0x00495103,
      98, QMetaType::Float, 0x00495103,
      99, QMetaType::Float, 0x00495103,
     100, QMetaType::Float, 0x00495103,
     101, QMetaType::Float, 0x00495103,
      21, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       5,
       6,
       7,
       8,
       9,
      10,
      11,
      12,

       0        // eod
};

void RhythmVideo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RhythmVideo *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->fullChanged(); break;
        case 1: _t->nightVersionChanged(); break;
        case 2: _t->nightMethodChanged(); break;
        case 3: _t->frameChanged(); break;
        case 4: _t->cloudFrameChanged(); break;
        case 5: _t->brightnessChanged(); break;
        case 6: _t->contrastChanged(); break;
        case 7: _t->saturationChanged(); break;
        case 8: _t->highLightChanged(); break;
        case 9: _t->shadowChanged(); break;
        case 10: _t->midtoneChanged(); break;
        case 11: _t->coefficientChanged(); break;
        case 12: _t->timeAxisChanged(); break;
        case 13: _t->beamCountChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QList<quint32>(*)>(_a[3]))); break;
        case 14: _t->timeAxisChanged((*reinterpret_cast< QList<float>(*)>(_a[1]))); break;
        case 15: _t->sendPath((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->finish(); break;
        case 17: _t->distanceChanged((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 18: _t->sendVideoData((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< QVector<quint8>(*)>(_a[3]))); break;
        case 19: _t->sendBeamData((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< QVector<float>(*)>(_a[3]))); break;
        case 20: _t->setParams((*reinterpret_cast< RhythmUI::AcousticImageModes(*)>(_a[1])),(*reinterpret_cast< const float(*)>(_a[2])),(*reinterpret_cast< const float(*)>(_a[3])),(*reinterpret_cast< const float(*)>(_a[4])),(*reinterpret_cast< const float(*)>(_a[5])),(*reinterpret_cast< const float(*)>(_a[6])),(*reinterpret_cast< const quint32(*)>(_a[7]))); break;
        case 21: _t->seekBeamIndex((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 22: _t->seekPercentage((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 23: _t->openFileFinish(); break;
        case 24: _t->getVideoData((*reinterpret_cast< std::map<int,unsigned char*>(*)>(_a[1])),(*reinterpret_cast< std::map<int,RhythmUI::picture>(*)>(_a[2]))); break;
        case 25: _t->getCameraData((*reinterpret_cast< QMap<int,QImage>(*)>(_a[1])),(*reinterpret_cast< std::map<int,RhythmUI::picture>(*)>(_a[2]))); break;
        case 26: _t->getVideoFromThread((*reinterpret_cast< QVector<unsigned char>(*)>(_a[1]))); break;
        case 27: _t->getBeamFromThread((*reinterpret_cast< QVector<unsigned char>(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 28: _t->getTimeAxis((*reinterpret_cast< QList<float>(*)>(_a[1]))); break;
        case 29: { float _r = _t->getMax();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        case 30: { float _r = _t->getMin();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        case 31: { float _r = _t->getDynamicRange();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        case 32: _t->changeTimeLine((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 33: _t->setData((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 34: _t->setDataWithCamera((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 35: _t->addVideoData((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 36: _t->addImageData((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 37: _t->addBeamData((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 38: _t->getFinalData((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 39: _t->getFinalCameraData((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 40: _t->clearData(); break;
        case 41: _t->resetRhythmVideo(); break;
        case 42: _t->setImageMode((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 43: _t->setDynamicRange((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 44: _t->setPeakFactor((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 45: _t->setMaxLimiteValue((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 46: _t->setBeamConfig((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const float(*)>(_a[2])),(*reinterpret_cast< const float(*)>(_a[3])),(*reinterpret_cast< const float(*)>(_a[4]))); break;
        case 47: _t->start(); break;
        case 48: _t->stop(); break;
        case 49: _t->play(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint32> >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<float> >(); break;
            }
            break;
        case 26:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<unsigned char> >(); break;
            }
            break;
        case 27:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<unsigned char> >(); break;
            }
            break;
        case 28:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::fullChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::nightVersionChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::nightMethodChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::frameChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::cloudFrameChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::brightnessChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::contrastChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::saturationChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::highLightChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::shadowChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::midtoneChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::coefficientChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::timeAxisChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)(int , int , QList<quint32> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::beamCountChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)(QList<float> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::timeAxisChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::sendPath)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::finish)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)(quint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::distanceChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)(const unsigned int & , const unsigned int & , QVector<quint8> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::sendVideoData)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)(const unsigned int & , const unsigned int & , QVector<float> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::sendBeamData)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (RhythmVideo::*)(RhythmUI::AcousticImageModes , const float & , const float & , const float & , const float & , const float & , const quint32 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RhythmVideo::setParams)) {
                *result = 20;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RhythmVideo *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->full(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->nightVersion(); break;
        case 2: *reinterpret_cast< quint8*>(_v) = _t->nightMethod(); break;
        case 3: *reinterpret_cast< float*>(_v) = _t->brightness(); break;
        case 4: *reinterpret_cast< float*>(_v) = _t->contrast(); break;
        case 5: *reinterpret_cast< float*>(_v) = _t->saturation(); break;
        case 6: *reinterpret_cast< float*>(_v) = _t->highLight(); break;
        case 7: *reinterpret_cast< float*>(_v) = _t->shadow(); break;
        case 8: *reinterpret_cast< float*>(_v) = _t->midtone(); break;
        case 9: *reinterpret_cast< float*>(_v) = _t->coefficient(); break;
        case 10: *reinterpret_cast< bool*>(_v) = _t->timeAxis(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RhythmVideo *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setFull(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setNightVersion(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setNightMethod(*reinterpret_cast< quint8*>(_v)); break;
        case 3: _t->setBrightness(*reinterpret_cast< float*>(_v)); break;
        case 4: _t->setContrast(*reinterpret_cast< float*>(_v)); break;
        case 5: _t->setSaturation(*reinterpret_cast< float*>(_v)); break;
        case 6: _t->setHighLight(*reinterpret_cast< float*>(_v)); break;
        case 7: _t->setShadow(*reinterpret_cast< float*>(_v)); break;
        case 8: _t->setMidtone(*reinterpret_cast< float*>(_v)); break;
        case 9: _t->setCoefficient(*reinterpret_cast< float*>(_v)); break;
        case 10: _t->setTimeAxis(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject RhythmVideo::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickFramebufferObject::staticMetaObject>(),
    qt_meta_stringdata_RhythmVideo.data,
    qt_meta_data_RhythmVideo,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RhythmVideo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RhythmVideo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RhythmVideo.stringdata0))
        return static_cast<void*>(this);
    return QQuickFramebufferObject::qt_metacast(_clname);
}

int RhythmVideo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickFramebufferObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 50)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 50)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void RhythmVideo::fullChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RhythmVideo::nightVersionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RhythmVideo::nightMethodChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void RhythmVideo::frameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void RhythmVideo::cloudFrameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void RhythmVideo::brightnessChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void RhythmVideo::contrastChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void RhythmVideo::saturationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void RhythmVideo::highLightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void RhythmVideo::shadowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void RhythmVideo::midtoneChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void RhythmVideo::coefficientChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void RhythmVideo::timeAxisChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void RhythmVideo::beamCountChanged(int _t1, int _t2, QList<quint32> _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void RhythmVideo::timeAxisChanged(QList<float> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void RhythmVideo::sendPath(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void RhythmVideo::finish()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void RhythmVideo::distanceChanged(quint32 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void RhythmVideo::sendVideoData(const unsigned int & _t1, const unsigned int & _t2, QVector<quint8> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void RhythmVideo::sendBeamData(const unsigned int & _t1, const unsigned int & _t2, QVector<float> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void RhythmVideo::setParams(RhythmUI::AcousticImageModes _t1, const float & _t2, const float & _t3, const float & _t4, const float & _t5, const float & _t6, const quint32 & _t7)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
