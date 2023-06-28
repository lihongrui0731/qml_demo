#ifndef DATAMANAGER_H
#define DATAMANAGER_H

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
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QMetaType>
#include <QSharedPointer>
#include <QElapsedTimer>

#include "tdms_to_map.h"
#include "tdmsdata.h"
#include "data_with_timestamp.h"
#include "report_frame.h"
#include "capture_controller.h"

#include "filemanager.h"
#include "configmanager.h"
#include "datathread.h"
#include <QImage>
#include "dataType.h"

class DataManager:public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(int soundLength READ soundLength NOTIFY soundLengthChanged)
    Q_PROPERTY(int vibrationLength READ vibrationLength NOTIFY vibrationLengthChanged)

public:
    DataManager();
    ~DataManager();
public:
    // void addData( QVariantMap d );

    Q_INVOKABLE void proReady();

    Q_INVOKABLE void finishData();
    Q_INVOKABLE void clearData();
    Q_INVOKABLE void releaseMomery();

    Q_INVOKABLE void openSoundData(QString file);
    Q_INVOKABLE void openVibrationData(QString file);

    Q_INVOKABLE void setChannel( QString channels_ );

    // 实时保存的特征值数据
    Q_INVOKABLE void getEigenvalueData( QString channelID , QString type );
    Q_INVOKABLE void getSpectraData( QString channelID );

    // 实时保存的时域数据
    Q_INVOKABLE void getTimedata( QString channelID );

    Q_INVOKABLE void getConfig();

    // Q_INVOKABLE void getFinalData1();

    //保存文件的特征值数据
    Q_INVOKABLE void getEigenvaluesByType( QString channel , QString type );
    Q_INVOKABLE void getSpectraFromFile( QString channel );
    Q_INVOKABLE void getTimedataFromFile( QString channel );

    Q_INVOKABLE quint64 getSoundRecordTime(){ return sound_record_time; }
    Q_INVOKABLE quint64 getVibrationRecordTime(){ return vibration_record_time; }

    Q_INVOKABLE void exportEvToCSV( QString path ,QString channelId , QString channelName , QString dataType , QVariantList evTypes , bool reprocess = false );
    Q_INVOKABLE bool saveEvToCSV( QString path ,QString channelId , QString channelName , QString dataType , QVariantList evTypes , bool reprocess = false );

private:
    // QElapsedTimer timer;

    QList<QString> jsonToObjectType;
    QList<QString> jsonToStringType;

    TdmsData *tdmsData{nullptr};
    DataThread *dataThread{nullptr};

    QThread* openThread{nullptr};

    QVariantMap soundData;
    QVariantMap vibrationData;

    QJsonObject channels;

    DataThread *spectraDataThread{nullptr};
    DataThread *eigenvalueDataThread{nullptr};
    DataThread *timedataThread{nullptr};

    QMap<quint64,QJsonObject> prpd_;
    /*
    QMap<quint64,QVector<quint8>> video_;
    QMap<quint64,QVector<float>> beam_;
    QMap<quint64,QVector<float>> raw_;

    QMap<QString,QMap<quint64,QVector<float>>> spectra_; //老格式
    QMap<QString,QMap<quint64,QVector<float>>> spectra_local; //老格式
    QMap<QString,QMap<quint64,QJsonObject>> spectra_json; //新格式
    QMap<QString,quint32> spectra_len;
    QMap<QString,quint32> spectraLocal_len;

    QMap<QString,QMap<quint64,QJsonObject>> timedata_; // 时域数据
    QMap<QString,QMap<quint64,QJsonObject>> eigenvalue_; // 实时保存特征值 用于切换
    */

    QMap<QString,EigenvalueData> eigenvalue_temp; //打开文件时 振动多通道特征值临时存储
    QMap<QString,SpectraJsonData> spectra_temp; // 打开文件时 振动多通道spectra临时存储
    QMap<QString,QJsonObject> timedata_temp; // 打开文件时 振动多通道时域数据 临时存储

    quint64 sound_record_time{10000};
    quint64 vibration_record_time{10000};

    QString deviceId_{ "sc01" };
    QString channelId_{ "sc01" };
    QString sourceType_{ "sound" };
    QString deviceType_{ "soundcam" };
    QString deviceChannel_{ "AI0" };

    fileManager *fm{nullptr};

    int soundLength(){
        return sound_record_time;
    }

    int vibrationLength(){
        return vibration_record_time;
    }

    void reprocess();

signals:
    void tdmsSoundData( QString f );
    void tdmsVibrationData( QString f );

    void sendSoundData( QVariantMap data , double dt );
    void sendVibrationData( QVariantMap data );
    void sendSoundTimeLine( QString channelID , int frameCount , int timeLength , QList<quint32> timeList );
    void sendVibrationTimeLine( QString channelID , int frameCount , int timeLength , QList<quint32> timeList );

    // 实时保存数据
    void addAudio( quint64 time , QVector<float> data );
    void addRaw( quint64 time , QVector<float> data );

    void addPicture( quint64 time ,QVector<quint8> data );
    void addBeam( quint64 time ,QVector<float> data );
    void addCamera( quint64 time ,QByteArray data );
    void addSpectra( QString channelId , quint64 time ,QVector<float> data );
    void addSpectraLocal( QString channelId , quint64 time ,QVector<float> data );
    void addSpectraJson( QString channelId , quint64 time ,QJsonObject data );
    void addPRPD( quint64 time , QJsonObject data );
    void addEv( QString channelId , quint64 time , QJsonObject data );
    void addLeq( QString channelId , quint64 time , QJsonObject data );

    void addTimedata( QString channelId , quint64 time , QJsonObject data );

    void pictureChanged( QVariant d );
    void beamChanged( QVariant d );
    void cameraChanged( QVariant d );
    void audioChanged( QVariant d );

    void spectraChanged( QString channelId , QVariant d );
    void spectraLocalChanged( QString channelId , QVariant d );
    void spectraJsonChanged( QString channelId , QJsonObject d );

    void timedataChanged( QString channelId , QJsonObject d );

    void prpdChanged( QJsonObject d );
    void eigenvalueChanged( QString channelId , QString s );
    void leqChanged( QString channelId , QString s );

    void getEigenvalueByType( QMap<quint64,QJsonObject> ev_, QString channelId , QString type );
    void getSpectraByChannel( QMap<quint64,QJsonObject> spa_, quint32 spa_len , QString channelId );
    void getTimedataByChannel( QMap<quint64,QJsonObject> data_, QString channelId );


    //void sendEigenvalueByType( QString json ,  QString channelId , QString type );
    void sendEigenvaluesJson( QString json ,  QString channelId , QString type );
    void sendSpectra( QList<QString> data , QString channelId );
    void sendTimedataFromThread( QList<float> data , float dt , QString channelId , int len , float max );


    void soundLengthChanged();
    void vibrationLengthChanged();

    void sendFinalVideo( QVariant video );
    void sendFinalCamera( QVariant camera );
    void sendFinalSpectra( QString channelId , QVariant spectra );
    void sendFinalSpectraLocal( QString channelId , QVariant spectra );
    void sendFinalPRPD( QJsonObject prpd);
    void sendFinalLeq( QString channelId , QJsonObject leq);
    void sendFinalEV( QString channelId , QString ev , quint32 timeLen );
    void sendFinalRawData( QVariant raw );
    void startAllow();
    void sendChannel( QString channelId );
    void deviceInfoChanged( QString deviceInfo_ );

    void soundFinished();
    void vibrationFinished( QString channelId );

    void soundReady( const bool isReady );
    void vibrationReady( const bool isReady );

    void soundMeasuring( const bool isMeasuring );
    void vibrationMeasuring( const bool isMeasuring );

    void deviceStatus( const QString& devId , QString sourceType , QString info );

    void sortFinalData(
        QMap<quint64,QVector<quint8>> video,
        QMap<quint64,QVector<float>> beam,
        QMap<quint64,QJsonObject> prpd,
        QMap<quint64,QVector<float>> spectra,
        QMap<quint64,QJsonObject> eigenvalue,
        QMap<quint64,QVector<float>> spectra_local,
        QMap<quint64,QVector<float>> raw
    );

    void getFinal();

    void sendEigenvalueByType( QString channelId_ , QString type_ , QString ev_ , QString dateTime , double dt , int frameCount );
    void sendSpectraFromFile( QString channelId_ , QJsonObject spec_ , int frameCount );
    // void sendTimedataFromFile( QString channelId_ , QString timedata_ );
    void sendTimedataFromFile( QString channelId_ , double dt_ , QList<float> list , int len_ , float max_);

    void sendDataToCal( QSharedPointer<SampleFrame> );
    void sendEvFromCal( QJsonObject d );

    void startReprocess( QSharedPointer<sovi::Plan> plan , std::map<int,qint32 *> audio , int len , double dt , QString systemtyp , QString dataType);

    void reprocessEv();
    void reprocessEvFinish();
    void reprocessPrpdFinish( QJsonObject prpd );
public slots:
    void getSoundData( QVariantMap data );
    void getVibrationData( QVariantMap data );

    // void getActualTimeData( ReportFrame data );

    Q_INVOKABLE void addData( const ReportFrame &data );
    void receiveReprocessData( const ReportFrame &data );

    // void getEigenvalueDataThread( QMap<quint64,QJsonObject> ev_, QString channelId  , QString type );
    void getEigenvalueDataJson(const QString &json ,  QString channelId , QString type );
    void getSpectra( QList<QString> data , QString channelId );
    void getTimedataFromThread( QList<float> data , float dt , QString channelId ,int len , float max );

    void getFinalDataFromThread( QMap<quint64, QVector<quint8>> _picture_map , QMap<quint64, QByteArray> _camera_map , QMap<quint64, QVector<float>> _beam_map , QMap<quint64, QVector<float>> spectra_map , QMap<quint64, QVector<float>> spectra_local_map , QMap<quint64, QVector<float>> raw_map , QJsonObject _prpd, QJsonObject _leq , QJsonObject _ev , quint32 time_len );

    void getFinalData( QSharedPointer<ChannelSessionInfo> csi );

    void reprocessStopped();
};

#endif // DATAMANAGER_H
