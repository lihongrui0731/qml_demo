#include "datamanager.h"

#include <QFile>
#include <QList>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QSharedDataPointer>
#include <QImage>

#ifndef Q_OS_LINUX
#include <malloc.h>
#endif

DataManager::DataManager()
{
    fm = new fileManager();
    getConfig();

    jsonToObjectType = QList<QString>()<<"PRPD"<<"timedata"<<"spectra";
    jsonToStringType = QList<QString>()<<"ev-sp"<<"Level_A";

    qRegisterMetaType<ReportFrame>("ReportFrame");
    qRegisterMetaType<QMap<quint64, QVector<float>>>("QMap<quint64, QVector<float>>");
    qRegisterMetaType<QMap<quint64, QVector<quint8>>>("QMap<quint64, QVector<quint8>>");
    qRegisterMetaType<QVector<quint8>>("QVector<quint8>");
    qRegisterMetaType<std::map<int,qint32 *>>("std::map<int,qint32 *>");
    // timer.start();
}

DataManager::~DataManager()
{
    clearData();
    delete fm;
    fm = nullptr;
    qDebug() << "析构 dm";
}

void DataManager::proReady(){

    qDebug() << "data Thread create";

    QThread* thread = new QThread();
    if( dataThread != nullptr ){
        delete dataThread;
    }

    dataThread = new DataThread();
    dataThread->setRecordTime( sound_record_time ,vibration_record_time );
    dataThread->moveToThread(thread);

    connect( dataThread, &DataThread::finished, thread, &QThread::quit);
    connect( dataThread, &DataThread::finished, dataThread, &DataThread::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);

    connect( this , &DataManager::sortFinalData,  dataThread , &DataThread::sortFinalData );
    //connect( dataThread , &DataThread::sendFinalData , this , &DataManager::getFinalDataFromThread , Qt::UniqueConnection );
    connect( dataThread , &DataThread::sendFinalData , this , &DataManager::getFinalDataFromThread );
    connect( this , &DataManager::getFinal , dataThread, &DataThread::getFinal );

    connect( this , &DataManager::addAudio , dataThread, &DataThread::addAudio );
    connect( this , &DataManager::addRaw , dataThread, &DataThread::addRaw );

    connect( this , &DataManager::addPicture , dataThread, &DataThread::addPicture );
    connect( this , &DataManager::addBeam , dataThread, &DataThread::addBeam );
    connect( this , &DataManager::addSpectra , dataThread, &DataThread::addSpectra );
    connect( this , &DataManager::addSpectraLocal , dataThread, &DataThread::addSpectraLocal );
    connect( this , &DataManager::addSpectraJson , dataThread, &DataThread::addSpectraJson );
    connect( this , &DataManager::addPRPD , dataThread, &DataThread::addPRPD );
    connect( this , &DataManager::addEv , dataThread, &DataThread::addEv );
    connect( this , &DataManager::addTimedata , dataThread, &DataThread::addTimedata );
    connect( this , &DataManager::addCamera , dataThread, &DataThread::addCamera ); 
    connect( this , &DataManager::addLeq , dataThread, &DataThread::addLeq );
    thread->start( QThread::NormalPriority );
}

void DataManager::getConfig(){
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };

    QString soundConfig = fm->getConfig( appData + "/Config/sound_config.json" );
    QString vibrationConfig = fm->getConfig( appData + "/Config/vibration_config.json" );

    QJsonParseError json_error;
    QJsonDocument soundDoc(QJsonDocument::fromJson(soundConfig.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject soundArr = soundDoc.object();

    QJsonDocument vibDoc(QJsonDocument::fromJson(vibrationConfig.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject vibrationArr = vibDoc.object();

    sound_record_time = soundArr.value("s_record_time").toInt() * 1000;
    vibration_record_time = vibrationArr.value("v_record_time").toInt() * 1000;
}

void DataManager::getFinalData(QSharedPointer<ChannelSessionInfo> csi ){
    qDebug() << csi->sourceType() << "finish data todo ---------------------------------- ";
 //    return;

    if( csi->sourceType() == "sound" ){
        qDebug() << "soundcam data todo -----------------------------------";
        emit soundFinished();
        emit getFinal();
    }else{
        qDebug() << "--- ---" << csi->channelId() << channels[csi->channelId()].toString() ;

        emit vibrationFinished( channels[csi->channelId()].toString() );
    }
}

void DataManager::getFinalDataFromThread( QMap<quint64, QVector<quint8>> picture_map, QMap<quint64, QByteArray> camera_map, QMap<quint64, QVector<float>> beam_map, QMap<quint64, QVector<float>> spectra_map, QMap<quint64, QVector<float>> spectra_local_map , QMap<quint64, QVector<float>> raw_map , QJsonObject prpd , QJsonObject leq, QJsonObject ev , quint32 time_len ){
    //  VideoData videoData;
    //  QMap<quint64, QVector<quint8>> picture_map_ = picture_map;
    //  QMap<quint64, QVector<float>> beam_map_ = beam_map;
    QJsonObject prpd_ = prpd;
    //  quint32 time_len_ = time_len;

    videoFinalData videoData;
    if( !picture_map.isEmpty() ){
        videoData.beam = beam_map;
        videoData.picture = picture_map;
        videoData.time_len = time_len;
    }

    cameraFinalData cameraData;
    if( !camera_map.isEmpty() ){
        cameraData.beam = beam_map;
        cameraData.picture = camera_map;
        cameraData.time_len = time_len;
    }

    spectraFinalData spectraData;
    spectraData.spectra = spectra_map;

    spectraFinalData spectraLocalData;
    spectraLocalData.spectra = spectra_local_map;

    rawFinalData rawData;
    if( !raw_map.isEmpty() ){
        rawData.raw = raw_map;
    }

    /*
    dataThread->quit();
    delete dataThread;
    dataThread = nullptr;
    */

    QVariant video_;
    if( !picture_map.isEmpty() ){
        video_.setValue(videoData);
    }

    QVariant camera_;
    if( !camera_map.isEmpty() ){
        camera_.setValue(cameraData);
    }

    QVariant spectra_;
    spectra_.setValue(spectraData);

    QVariant spectra_local_;
    spectra_local_.setValue( spectraLocalData );

    QVariant raw_;
    if( !raw_map.isEmpty() ){
        raw_.setValue( rawData );
    }

    QJsonDocument ev_json( ev );

    if( !picture_map.isEmpty() ){
        emit sendFinalVideo( video_ );
    }
    if( !camera_map.isEmpty() ){
        emit sendFinalCamera( camera_ );
    }

    emit sendFinalEV( "sound" , ev_json.toJson(QJsonDocument::Compact) , time_len );
    emit sendFinalPRPD( prpd );
    emit sendFinalLeq( "sound" , leq );
    emit sendFinalSpectra( "sound" , spectra_ );
    emit sendFinalSpectraLocal( "sound" , spectra_local_ );
    if( !raw_map.isEmpty() ){
        emit sendFinalRawData( raw_ );
    }
    emit startAllow();
}

// 实时接收 一帧数据
void DataManager::addData( const ReportFrame &data ){

    sovi::ReportFrameDataTypes dataType = data.dataType();

    imageData d_img;
    byteArrayData d_byte;
    charData d_c;
    floatData d_f;
    stringData d_s;
    jsonData d_j;

    QVariant d;
    QString s;
    QJsonArray jsonArr;
    QJsonObject jsonObj;
    QJsonDocument JsonDoc;
    QByteArray byteArray;
    QString channelId_ = channels[data.channelId()].toString();

    quint64 time = data.equipTimestampMs();
    if( time == 0 ){
        QDateTime timestamp = data.timestamp();
        time = timestamp.toMSecsSinceEpoch();
    }

    QString frameType = data.frameType();

    switch( dataType ){
        case sovi::VectorUInt8:
            d_c.data = data.byteVector();
            if( d_c.data.size() == 0 ){
                return;
            }
            d_c.channelId = channelId_;
            d.setValue(d_c);
            break;
        case sovi::VectorInt32:
           break;
        case sovi::VectorSingle:
            d_f.data = data.singleVector();
            if( d_f.data.size() == 0 ){
                return;
            }
            d_f.channelId = channelId_;
            d_f.dt = data.attributeNumber("dt");
            d.setValue(d_f);
            break;
        case sovi::VectorSingle2D:
           break;
        case sovi::JsonArray:
            jsonArr = data.jsonArray();
            if( jsonArr.isEmpty() ){
                return;
            }
            JsonDoc = QJsonDocument(jsonArr);
            s = JsonDoc.toJson(QJsonDocument::Compact);
            break;
        case sovi::JsonObject:
            if( data.jsonObject().isEmpty() ){
                return;
            }
            if( jsonToStringType.contains(frameType) )  {
                jsonObj = data.jsonObject();
                JsonDoc = QJsonDocument(jsonObj);
                s = JsonDoc.toJson(QJsonDocument::Compact);
            }else{
                d_j.data = data.jsonObject();
                d_j.channelId = channelId_;
                d.setValue(d_j);
            }
            break;
        case sovi::ByteArray:
            if( data.byteArray().isEmpty() ){
                return;
            }
            if( data.attributeText("type") == "image" ){
                d_byte.data = data.byteArray();
                d_byte.channelId = channelId_;
                d_byte.attr.insert("width",data.attributeNumber("width"));
                d_byte.attr.insert("height",data.attributeNumber("height"));
                d_byte.attr.insert("bytesPerLine",data.attributeNumber("bytesPerLine"));
                d_byte.attr.insert("max",data.attributeNumber("max"));
                d_byte.attr.insert("min",data.attributeNumber("min"));
                d.setValue(d_byte);
            }
            break;
        default:
            break;
    }

    if( frameType == "audf" ){
        // 规定时间间隔的rawdata数据
        emit audioChanged( d );
        emit addAudio(time , d_f.data );
    }else if( frameType == "raw" ){
        emit audioChanged( d );
        emit addRaw(time , d_f.data );
    }else if( frameType == "picture" ){
        emit pictureChanged( d );
        emit addPicture(time , d_c.data );
    }else if( frameType == "beam" ){
        emit beamChanged( d );
        emit addBeam( time , d_f.data );
    }else if( frameType == "camera" ){
        emit cameraChanged( d );
        emit addCamera( time , d_byte.data);
    }else if( frameType == "spectra" ){
        if( channelId_ == "sound" ){
            emit spectraChanged( channelId_ , d );
            emit addSpectra(channelId_ , time , d_f.data);
        }else{
            //振动新格式
            emit spectraJsonChanged( channelId_ , d_j.data );
            emit addSpectraJson(channelId_ , time , d_j.data );
        }
    }else if( frameType == "spectra_local" ){
        emit spectraLocalChanged( channelId_ , d );
        emit addSpectraLocal( channelId_ , time , d_f.data );
    }else if( frameType == "PRPD" ){
        emit prpdChanged(d_j.data);
        emit addPRPD( time , d_j.data );
    }else if( frameType == "ev-sp" ){
        emit eigenvalueChanged( channelId_ , s );
        emit addEv( channelId_ , time , jsonObj );
    }else if( frameType == "Level_A" ){
        emit leqChanged( channelId_ , s );
        emit addLeq( channelId_ , time , jsonObj );
    }else if( frameType == "timedata" ){
        emit timedataChanged( channelId_ , d_j.data );
        emit addTimedata( channelId_ , time , d_j.data );
    }
    return;
}

//接收后处理数据
void DataManager::receiveReprocessData( const ReportFrame &data ){
    sovi::ReportFrameDataTypes dataType = data.dataType();

    QVariant d;
    QString s;
    QJsonArray jsonArr;
    QJsonObject jsonObj;
    QJsonDocument JsonDoc;
    QString channelId = data.channelId();
    QString channelId_ = channels[channelId].toString();
    QDateTime timestamp = data.timestamp();

    switch( dataType ){
        case sovi::VectorUInt8:
            break;
        case sovi::VectorInt32:
           break;
        case sovi::VectorSingle:
            break;
        case sovi::VectorSingle2D:
           break;
        case sovi::JsonArray:
            jsonArr = data.jsonArray();
            JsonDoc = QJsonDocument(jsonArr);
            s = JsonDoc.toJson(QJsonDocument::Compact);
            break;
        case sovi::JsonObject:
            jsonObj = data.jsonObject();
            JsonDoc = QJsonDocument(jsonObj);
            s = JsonDoc.toJson(QJsonDocument::Compact);
            break;
        default:
            break;
    }

    QString frameType = data.frameType();
    if( frameType == "ev-sp" ){

        if( eigenvalue_temp.contains(channelId_) ){
            EigenvalueData eigenvalueData_ = eigenvalue_temp.value(channelId_);
            QJsonObject obj_0 = eigenvalueData_.m_eigenvalue[0]["values"].toObject();
            int frameCount = eigenvalueData_.m_eigenvalue[0]["frameCount"].toInt();

            QList<QString> evType = jsonObj["values"].toObject().keys();
            for( const QString &ev : qAsConst(evType) ){
                QJsonArray arr_0 = obj_0.value(ev).toArray();
                QJsonArray arr_ = jsonObj["values"].toObject().value(ev).toArray();
                for( int i = 0; i < arr_.size(); i++ ){
                    arr_0.append( arr_[i] );
                }
                obj_0[ev] = arr_0;
            }

            frameCount += jsonObj["frameCount"].toInt();
            eigenvalueData_.m_eigenvalue[0]["values"] = obj_0;
            eigenvalueData_.m_eigenvalue[0]["frameCount"] = frameCount;
            eigenvalue_temp.insert( channelId_, eigenvalueData_);
        }else{
            EigenvalueData eigenvalueData_;
            eigenvalueData_.m_eigenvalue.append(jsonObj);
            eigenvalueData_.channelId = channelId_;
            eigenvalue_temp.insert( channelId_ , eigenvalueData_ );
        }
    }

    if( frameType == "PRPD" ){
        prpd_.insert( ( prpd_.size() * jsonObj["cycleTime"].toInt() + jsonObj["dt"].toDouble() * jsonObj["frameCount"].toInt() ) * 1000, jsonObj );
    }
}

void DataManager::reprocessStopped(){
    if( prpd_.size() > 0 ){
        QJsonObject prpd_json{};
        QMap<quint64,QJsonObject>::Iterator iter = prpd_.begin();
        while ( iter != prpd_.end() ) {
            prpd_json.insert( QString::number(iter.key()) , iter.value() );
            iter++;
        }

        emit reprocessPrpdFinish( prpd_json );
    }

    if( eigenvalue_temp.size() > 0 ){
        emit reprocessEvFinish();
    }
}

/***
* 获取历史的通道数据
**/
void DataManager::getEigenvalueData( QString channelID , QString type ){
    if( dataThread == nullptr ){
        return;
    }

    QMap<quint64,QJsonObject> ev_ = dataThread->getEv(channelID);

    //QMap<quint64,QJsonObject> ev_ = eigenvalue_.value(channelID);
    if( ev_.isEmpty() ){
        return;
    }

    if( eigenvalueDataThread != nullptr ){
       delete eigenvalueDataThread;
       eigenvalueDataThread = nullptr;
    }

    QThread *thread = new QThread();
    eigenvalueDataThread = new DataThread();
    eigenvalueDataThread->moveToThread(thread);

    connect( eigenvalueDataThread, &DataThread::finished, thread, &QThread::quit);
    connect( eigenvalueDataThread, &DataThread::finished, eigenvalueDataThread, &DataThread::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);

    connect( this , &DataManager::getEigenvalueByType,  eigenvalueDataThread , &DataThread::getEigenvalueDataThread );
    connect( eigenvalueDataThread , &DataThread::sendEigenvalueByType , this , &DataManager::getEigenvalueDataJson );

    thread->start( QThread::NormalPriority );

    emit getEigenvalueByType( ev_ , channelID , type );
}

void DataManager::getEigenvalueDataJson(const QString &json , QString channelId , QString type ){
    emit sendEigenvaluesJson( json , channelId , type );
};

/***
 *  获取振动通道历史频谱
 */
void DataManager::getSpectraData( QString channelID ){
    if( dataThread == nullptr ){
        return;
    }

    QMap<quint64,QJsonObject> spa_ = dataThread->getSpectraJson(channelID);
    quint32 spa_len = dataThread->getSpectraLen(channelID);

    if( spa_.isEmpty() ){
        return;
    }

    if( spectraDataThread != nullptr ){
       delete spectraDataThread;
       spectraDataThread = nullptr;
    }

    QThread *thread = new QThread();
    spectraDataThread = new DataThread();
    spectraDataThread->moveToThread(thread);

    connect( spectraDataThread, &DataThread::finished, thread, &QThread::quit);
    connect( spectraDataThread, &DataThread::finished, spectraDataThread, &DataThread::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);

    connect( this , &DataManager::getSpectraByChannel,  spectraDataThread , &DataThread::getSpectraDataThread );
    connect( spectraDataThread , &DataThread::sendSpectraByChannel , this , &DataManager::getSpectra , Qt::BlockingQueuedConnection );

    thread->start( QThread::NormalPriority );
    emit getSpectraByChannel( spa_ , spa_len , channelID );
}

void DataManager::getSpectra( QList<QString> data , QString channelId ){
    emit sendSpectra( data , channelId );
};

/***
 *  获取 振动时域历史数据
 */
void DataManager::getTimedata( QString channelID ){
    if( dataThread == nullptr ){
        return;
    }

    QMap<quint64,QJsonObject> td_ = dataThread->getTimedata(channelID);
    if( td_.isEmpty() ){
        return;
    }

    // QMap<quint64,QJsonObject> td_ = timedata_.value(channelID);
    if( timedataThread != nullptr ){
       delete timedataThread;
       timedataThread = nullptr;
    }

    QThread *thread = new QThread();
    timedataThread = new DataThread();
    timedataThread->moveToThread(thread);

    connect( timedataThread, &DataThread::finished, thread, &QThread::quit);
    connect( timedataThread, &DataThread::finished, timedataThread, &DataThread::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);

    connect( this , &DataManager::getTimedataByChannel,  timedataThread , &DataThread::getTimedataThread );
    connect( timedataThread , &DataThread::sendTimedataByChannel , this , &DataManager::getTimedataFromThread , Qt::BlockingQueuedConnection );

    thread->start( QThread::NormalPriority );
    emit getTimedataByChannel( td_ , channelID );
}

void DataManager::getTimedataFromThread( QList<float> data , float dt , QString channelId , int len , float max ){

    emit sendTimedataFromThread( data , dt , channelId , len , max);
}

void DataManager::releaseMomery(){
    #ifdef Q_OS_LINUX
        //linux 内存紧缩 实时释放
        malloc_trim(0);
    #endif

    qDebug() << "release all memory";
}

void DataManager::clearData(){
    prpd_.clear();

    /*
    video_.clear();
    beam_.clear();
    spectra_.clear(); //老格式
    spectra_json.clear(); //新格式
    spectra_local.clear();
    spectra_len.clear();
    spectraLocal_len.clear();
    eigenvalue_.clear();
    timedata_.clear();
    raw_.clear();
    */

    eigenvalue_temp.clear(); //打开文件时 临时存储
    spectra_temp.clear();
    timedata_temp.clear();
    soundData.clear();
    vibrationData.clear();

    finishData();

#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif
}

void DataManager::finishData(){
    if( tdmsData != nullptr ){
        delete tdmsData;
        tdmsData = nullptr;
    }
    if( dataThread != nullptr ){
        delete dataThread;
        dataThread = nullptr;
    }
    if( spectraDataThread != nullptr ){
        delete spectraDataThread;
        spectraDataThread = nullptr;
    }
    if( eigenvalueDataThread != nullptr ){
        delete eigenvalueDataThread;
        eigenvalueDataThread = nullptr;
    }
    if( timedataThread != nullptr ){
        delete timedataThread;
        timedataThread = nullptr;
    }
    if( openThread != nullptr ){
        openThread->quit();
        openThread->wait();
        delete openThread;
        openThread = nullptr;
    }
    if( fm != nullptr ){
        delete fm;
        fm = nullptr;
    }
}

void DataManager::getSoundData( QVariantMap data ){
    soundData = data;

    if( soundData.contains("eigenvalue") ){
        eigenvalue_temp.insert("sound",soundData["eigenvalue"].value<EigenvalueData>());
    }

    TimeLine timeLine = data["timeLine"].value<TimeLine>();
    emit sendSoundTimeLine( timeLine.channelId , timeLine.frameCount , timeLine.timeLength, timeLine.timeList );

    if( data.value("reprocess") == 1 ){
        emit reprocessEv();
        reprocess();
    }

    emit sendSoundData(soundData , timeLine.dt);
}

void DataManager::getVibrationData( QVariantMap data ){
    vibrationData = data;

    TimeLine timeLine = data["timeLine"].value<TimeLine>();
    emit sendVibrationTimeLine( timeLine.channelId , timeLine.frameCount , timeLine.timeLength, timeLine.timeList );

    EigenvalueData eigenvalueData = vibrationData["eigenvalue"].value<EigenvalueData>();
    eigenvalue_temp.insert(eigenvalueData.channelId,eigenvalueData);

    SpectraJsonData spectraJsonData = vibrationData["spectra"].value<SpectraJsonData>();
    spectra_temp.insert(spectraJsonData.channelId,spectraJsonData);

    TimedataJson timedataData = vibrationData["timedata"].value<TimedataJson>();
    timedata_temp.insert(timedataData.channelId,timedataData.m_timedata[0]);

    emit sendChannel( vibrationData["channelId"].toString() );
    emit sendVibrationData(vibrationData);
}

//打开文件切换 特征值
void DataManager::getEigenvaluesByType( QString channelId , QString type ){

    EigenvalueData eigenvalueData = eigenvalue_temp[channelId];
    QList<QJsonObject> values_ = eigenvalueData.m_eigenvalue;

    for( int i = 0; i < values_.count(); i++ ){
        QJsonObject obj = values_[i];
        QJsonObject obj_ev = obj.value("values").toObject();
        QJsonDocument jdoc = QJsonDocument(obj_ev);

        double dt = obj.value("dt").toDouble();
        int frameCount = obj.value("frameCount").toInt();
        QString baseTime = obj.value("baseTime").toString();

        emit sendEigenvalueByType( channelId ,type , jdoc.toJson(QJsonDocument::Compact) , baseTime , dt , frameCount );
    }
}

// 打开文件后 获取振动文件指定通道的 spectra
void DataManager::getSpectraFromFile( QString channelId ){
    SpectraJsonData spectraJsonData = spectra_temp[channelId];
    QList<QJsonObject> values_ = spectraJsonData.m_spectra;
    for( int i = 0; i < values_.count(); i++ ){
        QJsonObject obj = values_[i];
        int frameCount = obj.value("frameCount").toInt();
        emit sendSpectraFromFile( channelId , obj , frameCount );
    }
}

void DataManager::getTimedataFromFile( QString channelId ){
    QJsonObject obj = timedata_temp[channelId];

    double dt_ = obj.value("dt").toDouble();
    QJsonArray arr_ = obj.value("values").toArray();
    QList<float> list;
    list.reserve(arr_.size());

    float max_ = 0;
    for( int i= 0; i < arr_.size(); i++ ){
        list.append( arr_[i].toDouble() );
        if( abs(arr_[i].toDouble()) > max_ ){
            max_ = abs(arr_[i].toDouble());
        }
    }

    emit sendTimedataFromFile( channelId , dt_ , list , list.size() , max_ );
}

void DataManager::setChannel( QString channels_ ){
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(channels_.toLatin1(), &json_error));
    channels = jsonDoc.object();
}

void DataManager::openSoundData(QString f){

    openThread = new QThread();
    tdmsData = new TdmsData();
    tdmsData->moveToThread(openThread);

    connect( tdmsData, &TdmsData::finished, openThread, &QThread::quit);
    connect( tdmsData, &TdmsData::finished, tdmsData, &TdmsData::deleteLater);
    connect( openThread, &QThread::finished, openThread, &QThread::deleteLater);
    connect( this , &DataManager::tdmsSoundData, tdmsData , &TdmsData::openSoundData );
    connect( tdmsData , &TdmsData::sendData , this , &DataManager::getSoundData , Qt::QueuedConnection );

    openThread->start( QThread::LowPriority );
    emit tdmsSoundData( f );

}

void DataManager::openVibrationData(QString f){
    openThread = new QThread();
    tdmsData = new TdmsData();
    tdmsData->moveToThread(openThread);

    connect( tdmsData, &TdmsData::finished, openThread, &QThread::quit);
    connect( tdmsData, &TdmsData::finished, tdmsData, &TdmsData::deleteLater);
    connect( openThread, &QThread::finished, openThread, &QThread::deleteLater);

    connect( this , &DataManager::tdmsVibrationData, tdmsData , &TdmsData::openVibrationData );
    connect( tdmsData , &TdmsData::sendData , this , &DataManager::getVibrationData , Qt::BlockingQueuedConnection );

    openThread->start( QThread::LowPriority );
    emit tdmsVibrationData( f );
}

void DataManager::reprocess(){

    QString info{};

    QJsonObject channelInfo{};
    channelInfo.insert("channelId",channelId_);
    channelInfo.insert("sourceType",sourceType_);

    QJsonObject device{};
    device.insert("deviceType",deviceType_);
    device.insert("deviceId",deviceId_);
    channelInfo.insert("device",device);

    QJsonArray channels_;
    channels_.append(channelInfo);
    info = QJsonDocument( channels_ ).toJson(QJsonDocument::Compact);

    ConfigManager *cm = new ConfigManager();
    QSharedPointer<sovi::Plan> plan = cm->createPlan( info );

    if( soundData.keys().contains("rawData") ){
        emit startReprocess( plan , soundData["rawData"].value<RawData>()._raw_data , soundData["rawData"].value<RawData>().len ,soundData["rawData"].value<RawData>().dt , soundData["systemtyp"].toString() , "raw" );
    }else{
        emit startReprocess( plan , soundData["audio"].value<AudioData>()._locS_data , soundData["audio"].value<AudioData>().len ,soundData["audio"].value<AudioData>().dt , soundData["systemtyp"].toString() , "local");
    }
    delete cm;
}


void DataManager::exportEvToCSV( QString path_ , QString channelId , QString channelName , QString dataType ,  QVariantList evTypes , bool reprocess ){

    QStringList evType_;
    for( int i = 0; i < evTypes.size(); i++ ){
        evType_.append( evTypes[i].toString() );
    }

    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QString dir = appData + "/temp/";

    QDir dataDir{appData + "/"};
    if(!dataDir.exists("temp"))
        dataDir.mkpath("temp");
    QString path = dir + "/data.csv";

    QFile file( path );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        qDebug() << "error";
        return;
    }

    QHash<QString,QVector<float>> ev_;

    if( reprocess ){
        // 后处理
        QList<QJsonObject> ev1_ = eigenvalue_temp[channelId].m_eigenvalue;
        for( int i = 0; i < ev1_.size(); i++ ){
            QJsonObject ev_temp = ev1_[i]["values"].toObject();
            QList<QString> keys = ev_temp.keys();
            for( QString key : qAsConst(keys) ){
                 if( !evType_.contains(key) ){
                     continue;
                 }
                 QVector<float> vec_ = ev_[key];
                 QJsonArray arr_ = ev_temp[key].toArray();
                 for( int i2 = 0; i2 < arr_.size(); i2++ ){
                    vec_.append( arr_[i2].toDouble() );
                 }
                 ev_.insert(key,vec_);
            }
        }

    }else{
        // 实时数据
        QMap<quint64,QJsonObject> ev2_ = dataThread->getEv(channelId);
        QMap<quint64,QJsonObject>::Iterator iter = ev2_.begin();
        while( iter != ev2_.end() ){
            QJsonObject ev_temp = iter.value()["values"].toObject();
            QList<QString> keys = ev_temp.keys();
            for( QString key : qAsConst(keys) ){
                 if( !evType_.contains(key) ){
                    continue;
                 }
                 QVector<float> vec_ = ev_[key];
                 QJsonArray arr_ = ev_temp[key].toArray();
                 for( int i2 = 0; i2 < arr_.size(); i2++ ){
                    vec_.append( arr_[i2].toDouble() );
                 }
                 ev_.insert(key,vec_);
            }
            iter++;
        }
    }

    QTextStream out( &file );
    QHash<QString,QVector<float>>::Iterator iter = ev_.begin();
    int len = iter.value().size();
    while( iter != ev_.end() ){
        out << iter.key() << ",";
        iter++;
    }
    out << "\n";
    iter = ev_.begin();
    for( int i = 0; i < len; i++ ){
        while( iter != ev_.end() ){
            out << iter.value()[i] << ",";
            iter++;
        }
        out << "\n";
        iter = ev_.begin();
    }
    out.flush();
    file.close();

    QDateTime date_;

    path_ = path_.mid(8) + "/"+channelName+"_"+dataType+"_";
    path_ += date_.currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz")+".csv";
    QFile::copy(path, path_);

}


bool DataManager::saveEvToCSV( QString path_ , QString channelId , QString channelName , QString dataType , QVariantList evTypes, bool reprocess ){

    QStringList evType_;
    for( int i = 0; i < evTypes.size(); i++ ){
        evType_.append( evTypes[i].toString() );
    }

    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QString dir = appData + "/RhythmData/" + path_;

    QDir dataDir{appData + "/RhythmData/"};
    if(!dataDir.exists(path_))
        dataDir.mkpath(path_);
    QDateTime date_;
    QString path = dir;
    path += "/"+channelName+"_"+dataType+"_" + date_.currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz")+".csv";
    QFile file( path );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        qDebug() << "error";
        return false;
    }

    QHash<QString,QVector<float>> ev_;

    if( reprocess ){
        // 后处理
        QList<QJsonObject> ev1_ = eigenvalue_temp[channelId].m_eigenvalue;
        for( int i = 0; i < ev1_.size(); i++ ){
            QJsonObject ev_temp = ev1_[i]["values"].toObject();
            QList<QString> keys = ev_temp.keys();
            for( QString key : qAsConst(keys) ){
                 if( !evType_.contains(key) ){
                     continue;
                 }

                 QVector<float> vec_ = ev_[key];
                 QJsonArray arr_ = ev_temp[key].toArray();
                 for( int i2 = 0; i2 < arr_.size(); i2++ ){
                    vec_.append( arr_[i2].toDouble() );
                 }
                 ev_.insert(key,vec_);
            }
        }

    }else{
        // 实时数据
        QMap<quint64,QJsonObject> ev2_ = dataThread->getEv(channelId);
        QMap<quint64,QJsonObject>::Iterator iter = ev2_.begin();
        while( iter != ev2_.end() ){
            QJsonObject ev_temp = iter.value()["values"].toObject();
            QList<QString> keys = ev_temp.keys();
            for( QString key : qAsConst(keys) ){
                if( !evType_.contains(key) ){
                    continue;
                }

                QVector<float> vec_ = ev_[key];
                 QJsonArray arr_ = ev_temp[key].toArray();
                 for( int i2 = 0; i2 < arr_.size(); i2++ ){
                    vec_.append( arr_[i2].toDouble() );
                 }
                 ev_.insert(key,vec_);
            }
            iter++;
        }
    }

    QTextStream out( &file );
    QHash<QString,QVector<float>>::Iterator iter = ev_.begin();
    int len = iter.value().size();
    while( iter != ev_.end() ){
        out << iter.key() << ",";
        iter++;
    }
    out << "\n";
    iter = ev_.begin();
    for( int i = 0; i < len; i++ ){
        while( iter != ev_.end() ){
            out << iter.value()[i] << ",";
            iter++;
        }
        out << "\n";
        iter = ev_.begin();
    }

    out.flush();
    file.close();
    return true;
}

