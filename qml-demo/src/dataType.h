#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <QCoreApplication>
#include <QMetaType>
#include <QVariant>
#include <QVariantMap>
#include <QMargins>
#include <QJsonObject>
#include <QImage>
#include <QByteArray>

struct imageData{
    QImage data;
    QString channelId;
    quint32 timestamp;
    QHash<QString,QString> attr;
};
Q_DECLARE_METATYPE(imageData);
struct byteArrayData{
    QByteArray data;
    QString channelId;
    quint32 timestamp;
    QHash<QString,float> attr;
    QHash<QString,QString> property;
};
Q_DECLARE_METATYPE(byteArrayData);

struct floatData{
    QVector<float> data;
    QString channelId;
    quint32 timestamp;
    float dt;
};
Q_DECLARE_METATYPE(floatData);
struct charData{
    QVector<quint8> data;
    QString channelId;
    quint32 timestamp;
};
Q_DECLARE_METATYPE(charData);
struct stringData{
    QString data;
    QString channelId;
    quint32 timestamp;
};
Q_DECLARE_METATYPE(stringData);
struct jsonData{
    QJsonObject data;
    QString channelId;
    quint32 timestamp;
};
Q_DECLARE_METATYPE(jsonData);


typedef struct LeqData{
    QVector<float> _leq;
    double dt;
} LeqData;
Q_DECLARE_METATYPE( LeqData )

typedef struct CameraData{
    QMap<int , QByteArray> _camera_map;
    std::map<int, float*> _beam_map;
    quint64 m_time_len;
    quint64 m_distance;
    double dt;
} CameraData;
Q_DECLARE_METATYPE( CameraData )

typedef struct TimeLine{
    QList<quint32> timeList;
    int frameCount;
    int timeLength;
    QString channelId;
    double dt;
} TimeLine;
Q_DECLARE_METATYPE( TimeLine )

typedef struct RawData{
    quint32 count;
    unsigned int len;
    std::map<int, qint32*> _raw_data;
    double dt;
    float time;
} RawData;
Q_DECLARE_METATYPE( RawData )

//音频
typedef struct AudioData{
     // std::map<int, qint32*> _locS_dataNeg;
     // std::map<int, qint32*> _locS_dataPos;
     quint32 count;
     unsigned int len;
     std::map<int, qint32*> _locS_data;
     double dt;
     float time;
} AudioData;
Q_DECLARE_METATYPE( AudioData )

// spectra
typedef struct SpectraData{
     //std::map<std::string, float*> _p_data;
     std::map<int, float*> _p_data;
     quint64 len;
     quint64 m_pdata_time;
     float m_frequencyMax;
     float m_frequencyMin;
     float df;
} SpectraData;
Q_DECLARE_METATYPE( SpectraData )

// spectra 切片
typedef struct SpectrumData{
     std::map<int, float*> _map_spectrum;
     std::map<int, float*> _local_map_spectrum;
     quint64 len;
     float m_frequencyMax;
     float m_frequencyMin;
     float df;
} SpectrumData;
Q_DECLARE_METATYPE( SpectrumData )

// spectra 切片
typedef struct SpectraJsonData{
    QList<QJsonObject> m_spectra;
    QString channelId;
} SpectraJsonData;
Q_DECLARE_METATYPE( SpectraJsonData )

// spectra 切片
typedef struct TimedataJson{
    QList<QJsonObject> m_timedata;
    QString channelId;
} TimedataJson;
Q_DECLARE_METATYPE( TimedataJson )

// picture
typedef struct VideoData{
     quint64 width_picture_map;
     quint64 height_picture_map;
     std::map<int, unsigned char*> _picture_map;
     std::map<int, float*> _beam_map;
     quint64 m_time_len;
     quint64 m_distance;

} VideoData;
Q_DECLARE_METATYPE( VideoData )

// 特征值
typedef struct EigenvalueData{
    QList<QJsonObject> m_eigenvalue;
    QString channelId;
} EigenvalueData;
Q_DECLARE_METATYPE( EigenvalueData )

struct videoFinalData{
    QMap<quint64, QVector<quint8>> picture;
    QMap<quint64, QVector<float>> beam;
    quint32 time_len;
};
Q_DECLARE_METATYPE(videoFinalData);

struct cameraFinalData{
    QMap<quint64, QByteArray> picture;
    QMap<quint64, QVector<float>> beam;
    quint32 time_len;
};
Q_DECLARE_METATYPE(cameraFinalData);

struct spectraFinalData{
    QMap<quint64, QVector<float>> spectra;
};
Q_DECLARE_METATYPE(spectraFinalData);

struct rawFinalData{
    QMap<quint64, QVector<float>> raw;
};
Q_DECLARE_METATYPE(rawFinalData);

namespace RhythmDataType
{
}

#endif // DATA_TYPE_H
