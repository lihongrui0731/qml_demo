/****************************************************************************
** Meta object code from reading C++ file 'CLineChart.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../qml-demo/util-UIElement/cppElement/ChartQuickItem/CLineChart.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CLineChart.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CLineChart_t {
    QByteArrayData data[77];
    char stringdata0[773];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CLineChart_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CLineChart_t qt_meta_stringdata_CLineChart = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CLineChart"
QT_MOC_LITERAL(1, 11, 13), // "drawFrequency"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "finish"
QT_MOC_LITERAL(4, 33, 18), // "frameLengthChanged"
QT_MOC_LITERAL(5, 52, 15), // "xAxisMinChanged"
QT_MOC_LITERAL(6, 68, 15), // "xAxisMaxChanged"
QT_MOC_LITERAL(7, 84, 15), // "yAxisMinChanged"
QT_MOC_LITERAL(8, 100, 15), // "yAxisMaxChanged"
QT_MOC_LITERAL(9, 116, 16), // "xAxisUnitChanged"
QT_MOC_LITERAL(10, 133, 16), // "yAxisUnitChanged"
QT_MOC_LITERAL(11, 150, 9), // "dtChanged"
QT_MOC_LITERAL(12, 160, 9), // "dfChanged"
QT_MOC_LITERAL(13, 170, 15), // "fontSizeChanged"
QT_MOC_LITERAL(14, 186, 10), // "dpiChanged"
QT_MOC_LITERAL(15, 197, 14), // "linePosChanged"
QT_MOC_LITERAL(16, 212, 19), // "currentIndexChanged"
QT_MOC_LITERAL(17, 232, 14), // "bgcolorChanged"
QT_MOC_LITERAL(18, 247, 16), // "textColorChanged"
QT_MOC_LITERAL(19, 264, 17), // "xAxisTitleChanged"
QT_MOC_LITERAL(20, 282, 17), // "yAxisTitleChanged"
QT_MOC_LITERAL(21, 300, 11), // "isDBChanged"
QT_MOC_LITERAL(22, 312, 11), // "isLGChanged"
QT_MOC_LITERAL(23, 324, 9), // "exportCSV"
QT_MOC_LITERAL(24, 334, 4), // "path"
QT_MOC_LITERAL(25, 339, 11), // "channelName"
QT_MOC_LITERAL(26, 351, 8), // "dataType"
QT_MOC_LITERAL(27, 360, 7), // "saveCSV"
QT_MOC_LITERAL(28, 368, 5), // "paint"
QT_MOC_LITERAL(29, 374, 9), // "QPainter*"
QT_MOC_LITERAL(30, 384, 7), // "painter"
QT_MOC_LITERAL(31, 392, 14), // "resetCurveLine"
QT_MOC_LITERAL(32, 407, 15), // "timeLineChanged"
QT_MOC_LITERAL(33, 423, 4), // "size"
QT_MOC_LITERAL(34, 428, 11), // "addJsonData"
QT_MOC_LITERAL(35, 440, 4), // "data"
QT_MOC_LITERAL(36, 445, 7), // "addData"
QT_MOC_LITERAL(37, 453, 12), // "QList<float>"
QT_MOC_LITERAL(38, 466, 3), // "len"
QT_MOC_LITERAL(39, 470, 16), // "addJsonBatchData"
QT_MOC_LITERAL(40, 487, 10), // "changeSize"
QT_MOC_LITERAL(41, 498, 1), // "x"
QT_MOC_LITERAL(42, 500, 1), // "v"
QT_MOC_LITERAL(43, 502, 7), // "rePaint"
QT_MOC_LITERAL(44, 510, 7), // "reflash"
QT_MOC_LITERAL(45, 518, 6), // "reSize"
QT_MOC_LITERAL(46, 525, 9), // "clearData"
QT_MOC_LITERAL(47, 535, 9), // "reInitial"
QT_MOC_LITERAL(48, 545, 6), // "reload"
QT_MOC_LITERAL(49, 552, 4), // "move"
QT_MOC_LITERAL(50, 557, 4), // "orgX"
QT_MOC_LITERAL(51, 562, 4), // "newX"
QT_MOC_LITERAL(52, 567, 15), // "yAxisSelfChange"
QT_MOC_LITERAL(53, 583, 3), // "max"
QT_MOC_LITERAL(54, 587, 3), // "min"
QT_MOC_LITERAL(55, 591, 3), // "num"
QT_MOC_LITERAL(56, 595, 13), // "setCurrentPos"
QT_MOC_LITERAL(57, 609, 8), // "current_"
QT_MOC_LITERAL(58, 618, 8), // "frameLen"
QT_MOC_LITERAL(59, 627, 8), // "xAxisMin"
QT_MOC_LITERAL(60, 636, 8), // "xAxisMax"
QT_MOC_LITERAL(61, 645, 8), // "yAxisMin"
QT_MOC_LITERAL(62, 654, 8), // "yAxisMax"
QT_MOC_LITERAL(63, 663, 9), // "xAxisUnit"
QT_MOC_LITERAL(64, 673, 9), // "yAxisUnit"
QT_MOC_LITERAL(65, 683, 10), // "xAxisTitle"
QT_MOC_LITERAL(66, 694, 10), // "yAxisTitle"
QT_MOC_LITERAL(67, 705, 12), // "currentIndex"
QT_MOC_LITERAL(68, 718, 7), // "linePos"
QT_MOC_LITERAL(69, 726, 2), // "dt"
QT_MOC_LITERAL(70, 729, 2), // "df"
QT_MOC_LITERAL(71, 732, 8), // "fontSize"
QT_MOC_LITERAL(72, 741, 3), // "dpi"
QT_MOC_LITERAL(73, 745, 4), // "isDB"
QT_MOC_LITERAL(74, 750, 4), // "isLG"
QT_MOC_LITERAL(75, 755, 7), // "bgColor"
QT_MOC_LITERAL(76, 763, 9) // "textColor"

    },
    "CLineChart\0drawFrequency\0\0finish\0"
    "frameLengthChanged\0xAxisMinChanged\0"
    "xAxisMaxChanged\0yAxisMinChanged\0"
    "yAxisMaxChanged\0xAxisUnitChanged\0"
    "yAxisUnitChanged\0dtChanged\0dfChanged\0"
    "fontSizeChanged\0dpiChanged\0linePosChanged\0"
    "currentIndexChanged\0bgcolorChanged\0"
    "textColorChanged\0xAxisTitleChanged\0"
    "yAxisTitleChanged\0isDBChanged\0isLGChanged\0"
    "exportCSV\0path\0channelName\0dataType\0"
    "saveCSV\0paint\0QPainter*\0painter\0"
    "resetCurveLine\0timeLineChanged\0size\0"
    "addJsonData\0data\0addData\0QList<float>\0"
    "len\0addJsonBatchData\0changeSize\0x\0v\0"
    "rePaint\0reflash\0reSize\0clearData\0"
    "reInitial\0reload\0move\0orgX\0newX\0"
    "yAxisSelfChange\0max\0min\0num\0setCurrentPos\0"
    "current_\0frameLen\0xAxisMin\0xAxisMax\0"
    "yAxisMin\0yAxisMax\0xAxisUnit\0yAxisUnit\0"
    "xAxisTitle\0yAxisTitle\0currentIndex\0"
    "linePos\0dt\0df\0fontSize\0dpi\0isDB\0isLG\0"
    "bgColor\0textColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CLineChart[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
      19,  294, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      21,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  214,    2, 0x06 /* Public */,
       3,    0,  215,    2, 0x06 /* Public */,
       4,    0,  216,    2, 0x06 /* Public */,
       5,    0,  217,    2, 0x06 /* Public */,
       6,    0,  218,    2, 0x06 /* Public */,
       7,    0,  219,    2, 0x06 /* Public */,
       8,    0,  220,    2, 0x06 /* Public */,
       9,    0,  221,    2, 0x06 /* Public */,
      10,    0,  222,    2, 0x06 /* Public */,
      11,    0,  223,    2, 0x06 /* Public */,
      12,    0,  224,    2, 0x06 /* Public */,
      13,    0,  225,    2, 0x06 /* Public */,
      14,    0,  226,    2, 0x06 /* Public */,
      15,    0,  227,    2, 0x06 /* Public */,
      16,    0,  228,    2, 0x06 /* Public */,
      17,    0,  229,    2, 0x06 /* Public */,
      18,    0,  230,    2, 0x06 /* Public */,
      19,    0,  231,    2, 0x06 /* Public */,
      20,    0,  232,    2, 0x06 /* Public */,
      21,    0,  233,    2, 0x06 /* Public */,
      22,    0,  234,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      23,    3,  235,    2, 0x02 /* Public */,
      27,    3,  242,    2, 0x02 /* Public */,
      28,    1,  249,    2, 0x02 /* Public */,
      31,    0,  252,    2, 0x02 /* Public */,
      32,    1,  253,    2, 0x02 /* Public */,
      32,    0,  256,    2, 0x02 /* Public */,
      34,    1,  257,    2, 0x02 /* Public */,
      36,    2,  260,    2, 0x02 /* Public */,
      39,    1,  265,    2, 0x02 /* Public */,
      40,    2,  268,    2, 0x02 /* Public */,
      43,    0,  273,    2, 0x02 /* Public */,
      44,    0,  274,    2, 0x02 /* Public */,
      45,    0,  275,    2, 0x02 /* Public */,
      46,    0,  276,    2, 0x02 /* Public */,
      47,    0,  277,    2, 0x02 /* Public */,
      48,    0,  278,    2, 0x02 /* Public */,
      49,    2,  279,    2, 0x02 /* Public */,
      52,    3,  284,    2, 0x02 /* Public */,
      56,    1,  291,    2, 0x02 /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   24,   25,   26,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   24,   25,   26,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   33,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   35,
    QMetaType::Void, 0x80000000 | 37, QMetaType::Int,   35,   38,
    QMetaType::Void, QMetaType::QJsonObject,   35,
    QMetaType::Void, QMetaType::Float, QMetaType::Int,   41,   42,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   50,   51,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Int,   53,   54,   55,
    QMetaType::Void, QMetaType::Int,   57,

 // properties: name, type, flags
      58, QMetaType::Int, 0x00495003,
      59, QMetaType::Float, 0x00495003,
      60, QMetaType::Float, 0x00495003,
      61, QMetaType::Float, 0x00495003,
      62, QMetaType::Float, 0x00495003,
      63, QMetaType::Float, 0x00495003,
      64, QMetaType::Float, 0x00495003,
      65, QMetaType::QString, 0x00495003,
      66, QMetaType::QString, 0x00495003,
      67, QMetaType::Int, 0x00495103,
      68, QMetaType::Float, 0x00495103,
      69, QMetaType::Float, 0x00495103,
      70, QMetaType::Float, 0x00495103,
      71, QMetaType::Int, 0x00495103,
      72, QMetaType::Float, 0x00495103,
      73, QMetaType::Bool, 0x00495003,
      74, QMetaType::Bool, 0x00495003,
      75, QMetaType::QString, 0x00495003,
      76, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       2,
       3,
       4,
       5,
       6,
       7,
       8,
      17,
      18,
      14,
      13,
       9,
      10,
      11,
      12,
      19,
      20,
      15,
      16,

       0        // eod
};

void CLineChart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CLineChart *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->drawFrequency(); break;
        case 1: _t->finish(); break;
        case 2: _t->frameLengthChanged(); break;
        case 3: _t->xAxisMinChanged(); break;
        case 4: _t->xAxisMaxChanged(); break;
        case 5: _t->yAxisMinChanged(); break;
        case 6: _t->yAxisMaxChanged(); break;
        case 7: _t->xAxisUnitChanged(); break;
        case 8: _t->yAxisUnitChanged(); break;
        case 9: _t->dtChanged(); break;
        case 10: _t->dfChanged(); break;
        case 11: _t->fontSizeChanged(); break;
        case 12: _t->dpiChanged(); break;
        case 13: _t->linePosChanged(); break;
        case 14: _t->currentIndexChanged(); break;
        case 15: _t->bgcolorChanged(); break;
        case 16: _t->textColorChanged(); break;
        case 17: _t->xAxisTitleChanged(); break;
        case 18: _t->yAxisTitleChanged(); break;
        case 19: _t->isDBChanged(); break;
        case 20: _t->isLGChanged(); break;
        case 21: _t->exportCSV((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 22: { bool _r = _t->saveCSV((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 23: _t->paint((*reinterpret_cast< QPainter*(*)>(_a[1]))); break;
        case 24: _t->resetCurveLine(); break;
        case 25: _t->timeLineChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->timeLineChanged(); break;
        case 27: _t->addJsonData((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 28: _t->addData((*reinterpret_cast< QList<float>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 29: _t->addJsonBatchData((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 30: _t->changeSize((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 31: _t->rePaint(); break;
        case 32: _t->reflash(); break;
        case 33: _t->reSize(); break;
        case 34: _t->clearData(); break;
        case 35: _t->reInitial(); break;
        case 36: _t->reload(); break;
        case 37: _t->move((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 38: _t->yAxisSelfChange((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 39: _t->setCurrentPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
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
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::drawFrequency)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::finish)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::frameLengthChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::xAxisMinChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::xAxisMaxChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::yAxisMinChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::yAxisMaxChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::xAxisUnitChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::yAxisUnitChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::dtChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::dfChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::fontSizeChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::dpiChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::linePosChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::currentIndexChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::bgcolorChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::textColorChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::xAxisTitleChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::yAxisTitleChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::isDBChanged)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (CLineChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLineChart::isLGChanged)) {
                *result = 20;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<CLineChart *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->frameLen(); break;
        case 1: *reinterpret_cast< float*>(_v) = _t->xAxisMin(); break;
        case 2: *reinterpret_cast< float*>(_v) = _t->xAxisMax(); break;
        case 3: *reinterpret_cast< float*>(_v) = _t->yAxisMin(); break;
        case 4: *reinterpret_cast< float*>(_v) = _t->yAxisMax(); break;
        case 5: *reinterpret_cast< float*>(_v) = _t->xAxisUnit(); break;
        case 6: *reinterpret_cast< float*>(_v) = _t->yAxisUnit(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->xAxisTitle(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->yAxisTitle(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->currentIndex(); break;
        case 10: *reinterpret_cast< float*>(_v) = _t->linePos(); break;
        case 11: *reinterpret_cast< float*>(_v) = _t->dt(); break;
        case 12: *reinterpret_cast< float*>(_v) = _t->df(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->fontSize(); break;
        case 14: *reinterpret_cast< float*>(_v) = _t->dpi(); break;
        case 15: *reinterpret_cast< bool*>(_v) = _t->isDB(); break;
        case 16: *reinterpret_cast< bool*>(_v) = _t->isLG(); break;
        case 17: *reinterpret_cast< QString*>(_v) = _t->bgcolor(); break;
        case 18: *reinterpret_cast< QString*>(_v) = _t->textColor(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<CLineChart *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setFrameLength(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setxAxisMin(*reinterpret_cast< float*>(_v)); break;
        case 2: _t->setxAxisMax(*reinterpret_cast< float*>(_v)); break;
        case 3: _t->setyAxisMin(*reinterpret_cast< float*>(_v)); break;
        case 4: _t->setyAxisMax(*reinterpret_cast< float*>(_v)); break;
        case 5: _t->setxAxisUnit(*reinterpret_cast< float*>(_v)); break;
        case 6: _t->setyAxisUnit(*reinterpret_cast< float*>(_v)); break;
        case 7: _t->setxAxisTitle(*reinterpret_cast< QString*>(_v)); break;
        case 8: _t->setyAxisTitle(*reinterpret_cast< QString*>(_v)); break;
        case 9: _t->setCurrentIndex(*reinterpret_cast< int*>(_v)); break;
        case 10: _t->setLinePos(*reinterpret_cast< float*>(_v)); break;
        case 11: _t->setDt(*reinterpret_cast< float*>(_v)); break;
        case 12: _t->setDf(*reinterpret_cast< float*>(_v)); break;
        case 13: _t->setFontSize(*reinterpret_cast< int*>(_v)); break;
        case 14: _t->setDpi(*reinterpret_cast< float*>(_v)); break;
        case 15: _t->setDB(*reinterpret_cast< bool*>(_v)); break;
        case 16: _t->setLG(*reinterpret_cast< bool*>(_v)); break;
        case 17: _t->setBgcolor(*reinterpret_cast< QString*>(_v)); break;
        case 18: _t->setTextColor(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CLineChart::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickPaintedItem::staticMetaObject>(),
    qt_meta_stringdata_CLineChart.data,
    qt_meta_data_CLineChart,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CLineChart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CLineChart::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLineChart.stringdata0))
        return static_cast<void*>(this);
    return QQuickPaintedItem::qt_metacast(_clname);
}

int CLineChart::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickPaintedItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 19;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CLineChart::drawFrequency()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CLineChart::finish()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CLineChart::frameLengthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CLineChart::xAxisMinChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CLineChart::xAxisMaxChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CLineChart::yAxisMinChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CLineChart::yAxisMaxChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void CLineChart::xAxisUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void CLineChart::yAxisUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void CLineChart::dtChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void CLineChart::dfChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void CLineChart::fontSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void CLineChart::dpiChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void CLineChart::linePosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void CLineChart::currentIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void CLineChart::bgcolorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void CLineChart::textColorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void CLineChart::xAxisTitleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void CLineChart::yAxisTitleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void CLineChart::isDBChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void CLineChart::isLGChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
