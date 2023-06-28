#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QObject>
#include <QQuickItem>
#include <QApplication>
#include <QCoreApplication>
#include <QVariantMap>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QThread>
#include <QMetaType>
#include <QMap>

#include "tdmsdata.h"

class DataThread : public QThread
{
    Q_OBJECT
public:
    DataThread();
    ~DataThread();

    void setRecordTime( quint64 sRecordTime , quint64 vRecordTime ){
         sound_record_time = sRecordTime;
         vibration_record_time = vRecordTime;
    }

    QMap<quint64,QJsonObject> getTimedata( QString channelId ){
        QMap<quint64,QJsonObject> data;
        if( timedata_.contains(channelId) ){
            data = timedata_[channelId];
        }
        return data;
    }

    QMap<quint64,QJsonObject> getEv( QString channelId ){
        QMap<quint64,QJsonObject> data;
        if( eigenvalue_.contains(channelId) ){
            data = eigenvalue_[channelId];
        }
        return data;
    }

    QMap<quint64,QJsonObject> getSpectraJson( QString channelId ){
        QMap<quint64,QJsonObject> data;
        if( spectra_json.contains(channelId) ){
            data = spectra_json[channelId];
        }
        return data;
    }

    quint32 getSpectraLen( QString channelId ){
        quint32 data{0};
        if( spectra_len.contains(channelId) ){
             data = spectra_len[channelId];
        }
        return data;
    }

private:
    QMap<quint64, QVector<quint8>> _picture_map;
    QMap<quint64, QVector<float>> _beam_map;
    QMap<quint64, QVector<float>> _spectra_map;
    QMap<quint64, QVector<float>> _spectra_local_map;
    QMap<quint64, QVector<float>> _raw_map;
    QMap<quint64, QByteArray> _camera_map;

    QMap<quint64,QVector<quint8>> video_;
    QMap<quint64,QVector<float>> beam_;
    QMap<quint64,QJsonObject> prpd_;
    QMap<quint64,QVector<float>> raw_;

    QMap<quint64,QByteArray> camera_;

    QMap<QString,QMap<quint64,QVector<float>>> spectra_; //老格式
    QMap<QString,QMap<quint64,QVector<float>>> spectra_local; //老格式
    QMap<QString,QMap<quint64,QJsonObject>> spectra_json; //新格式
    QMap<QString,quint32> spectra_len;
    QMap<QString,quint32> spectraLocal_len;

    QMap<QString,QMap<quint64,QJsonObject>> timedata_; // 时域数据
    QMap<QString,QMap<quint64,QJsonObject>> eigenvalue_; // 实时保存特征值 用于切换

    QMap<QString,QMap<quint64,QJsonObject>> leq_; // 实时保存leq 用于切换

    quint64 sound_record_time{10000};
    quint64 vibration_record_time{10000};

signals:
    void sendEigenvalueByType( const QString &json , QString channelId , QString type );
    void sendSpectraByChannel( QList<QString> data , QString channelId );
    void sendTimedataByChannel( QList<float> data , float dt , QString channelId , int len , float max );
    void sendFinalData( QMap<quint64 , QVector<quint8>> picture, QMap<quint64 , QByteArray> camera , QMap<quint64, QVector<float>> beam,QMap<quint64, QVector<float>> spectra , QMap<quint64, QVector<float>> spectra_local , QMap<quint64, QVector<float>> raw, QJsonObject prpd , QJsonObject leq, QJsonObject ev, quint32 time_len );

public slots:
    void getEigenvalueDataThread( QMap<quint64,QJsonObject> ev_, QString channelId , QString type );
    void getSpectraDataThread( QMap<quint64,QJsonObject > spa_ , quint32 spa_len , QString channelId );
    void getTimedataThread( QMap<quint64,QJsonObject> data_, QString channelId );

    void sortFinalData();
    void getFinal();

    void addAudio( quint64 time , QVector<float> data );
    void addRaw( quint64 time , QVector<float> data );
    void addPicture( quint64 time ,QVector<quint8> data );
    void addBeam( quint64 time ,QVector<float> data );
    void addSpectra( QString channelId , quint64 time ,QVector<float> data );
    void addSpectraLocal( QString channelId , quint64 time ,QVector<float> data );
    void addSpectraJson( QString channelId , quint64 time ,QJsonObject data );
    void addPRPD( quint64 time , QJsonObject data );
    void addEv( QString channelId , quint64 time , QJsonObject data );
    void addLeq( QString channelId , quint64 time , QJsonObject data );
    void addTimedata( QString channelId , quint64 time , QJsonObject data );
    void addCamera( quint64 time , QByteArray data );
};
#endif // DATATHREAD_H
