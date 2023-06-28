#include "tdmsdata.h"

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

TdmsData::TdmsData()
{
    // timer.start();
}

TdmsData::~TdmsData()
{
     qDebug() << "析构 tdms 线程 ~ ";
}

void TdmsData::saveSoundTdmsData(){
}

void TdmsData::openSoundData( QString f ){
    tdms.openSOCFile( f );
    // spectra
    QMap<quint64, DataWithTimestamp*>* spectList = tdms.p_spectFrames;
    unsigned int len{0};

    // 时间轴 窜在beam时候用beam 计算 不存在beam时用 spectra计算
    bool timeline{false};
    QList<quint32> timeList;
    int frameCount{0};
    int timeLength{0};

    // video
    quint16 height_picture_map{0};
    quint16 width_picture_map{0};
    unsigned int len2{0};

    if( !tdms.p_videoFrames->isEmpty() ){
        QMap<quint64, DataWithTimestamp*>* videoList = tdms.p_videoFrames;
        // std::map<int, unsigned char*> _picture_map;
        // std::map<int, float*> _beam_map;
        QMap<quint64, DataWithTimestamp*>::const_iterator i1 = videoList->constBegin();
        while (i1 != videoList->constEnd()) {
            DataVideo * videoFrame = static_cast<DataVideo *>(i1.value());
            _picture_map.insert(std::pair<int, unsigned char*>( i1.key() , reinterpret_cast<unsigned char*>(videoFrame->buffer) ));

            width_picture_map = videoFrame->hRes();
            height_picture_map = videoFrame->vRes();
            ++i1;
        }
    }

    // beam
    // 存在beam时 用beam 计算时间轴
    QMap<quint64, DataWithTimestamp*>* beamList = tdms.p_acousFrames;
    QMap<quint64, DataWithTimestamp*>::const_iterator i2 = beamList->constBegin();

    int distance{3000};
    while (i2 != beamList->constEnd()) {
        DataAcoustic * beamFrame = static_cast<DataAcoustic *>(i2.value());
        distance = beamFrame->distance();
        len2 = i2.key();
        timeLength = i2.key();
        timeList.append( i2.key() );
        frameCount++;
        timeline = true;

        _beam_map.insert(std::pair<int, float*>( i2.key() ,reinterpret_cast<float*>(beamFrame->buffer) ));
        ++i2;
    }

if( !tdms.p_videoFrames->isEmpty() ){
    VideoData pictureData;
    pictureData.width_picture_map = width_picture_map;
    pictureData.height_picture_map = height_picture_map;
    pictureData._picture_map = _picture_map;
    pictureData._beam_map = _beam_map;
    pictureData.m_time_len = len2;

    pictureData.m_distance = distance;
    video_data.setValue(pictureData);
}

    // audio
    quint32 m_adata_len{0};
    double m_dt{0};
    quint64 a_time{0};
    quint32 count{0};
    QMap<quint64, DataWithTimestamp*>* audioList = tdms.p_audioFrames;
    QMap<quint64, DataWithTimestamp*>::const_iterator i3 = audioList->constBegin();

    while( i3 != audioList->constEnd()) {
        DataAudio * audioFrame = static_cast<DataAudio *>( i3.value() );
        m_adata_len = audioFrame->bufferLength;
        m_dt = audioFrame->dtVal();
        a_time = audioFrame->timestamp() * m_dt;

        qint32 * data_temp = new qint32[m_adata_len];

        for( quint32 n = 0; n < m_adata_len; n++ ){
            data_temp[n] = audioFrame->bufferA[n] + audioFrame->bufferB[n];
        }

        _locS_data.insert(std::pair<int, qint32*>( i3.key() ,data_temp ));
        count++;
        ++i3;
    }

    AudioData audioData;
   // t4._locS_dataNeg = _locS_dataNeg;
   // t4._locS_dataPos = _locS_dataPos;

    audioData._locS_data = _locS_data;
    audioData.dt = m_dt ;
    audioData.len = m_adata_len;
    audioData.time = a_time;
    audioData.count = count;
    audio_data.setValue(audioData);

    // std::map<std::string, float*> _p_data;

    // std::map<std::string, float*> _map_spectrum;
    // std::map<std::string, float*> _local_map_spectrum;

    QMap<quint64, DataWithTimestamp*>::const_iterator i = spectList->constBegin();
    while (i != spectList->constEnd()) {
        DataSpectrum * spectrumFrame = static_cast<DataSpectrum *>(i.value());
     //   _p_data.insert(std::pair<std::string, float*>( std::string(5 - std::to_string(i.key()).length(), '0') + std::to_string(i.key()) , reinterpret_cast<float*>(spectrumFrame->buffer) ));
        len = spectrumFrame->bufferLength;
        m_pdata_time = i.key();

        //_map_spectrum.insert(std::pair<std::string, float*>( std::string(5 - std::to_string(i.key()).length(), '0') + std::to_string(i.key()) , reinterpret_cast<float*>(spectrumFrame->buffer) ));
        _map_spectrum.insert(std::pair<int, float*>(i.key(), reinterpret_cast<float*>(spectrumFrame->buffer)));

        m_frequencyMax = 0;
        m_frequencyMin = 0;

        if( !timeline ){
            timeLength = i.key();
            timeList.append( i.key() );
            frameCount++;
        }

        if( spectrumFrame->bufferLength  > 1024 ){
            len = spectrumFrame->bufferLength / 2;
            float* local = spectrumFrame->buffer;
            for( unsigned int i = 0 ; i < len; i++ ){
                local++;
            }

            //_local_map_spectrum.insert(std::pair<std::string, float*>( std::string(5 - std::to_string(i.key()).length(), '0') + std::to_string(i.key()) , reinterpret_cast<float*>(local) ));
            _local_map_spectrum.insert(std::pair<int, float*>( i.key() , reinterpret_cast<float*>(local) ));
            m_frequencyMax = spectrumFrame->frequencyMax;
            m_frequencyMin = spectrumFrame->frequencyMin;
        }
        //addSpectraData(static_cast<unsigned int>(len) , reinterpret_cast<float*>(spectrumFrame->buffer) );
        ++i;
    }

    // 频谱数据
    SpectraData spectraData;
    spectraData._p_data = _map_spectrum;
    spectraData.len = len;
    spectraData.m_frequencyMax = m_frequencyMax;
    spectraData.m_frequencyMin = m_frequencyMin;
    spectraData.m_pdata_time = m_pdata_time;

    spectra_data.setValue(spectraData);

    // 频谱切片数据
    SpectrumData spectrumData;
    spectrumData._map_spectrum = _map_spectrum;
    spectrumData._local_map_spectrum = _local_map_spectrum;
    spectrumData.len = len;
    spectrumData.m_frequencyMax = m_frequencyMax;
    spectrumData.m_frequencyMin = m_frequencyMin;

    spectrum_data.setValue(spectrumData);

    TimeLine timeLine;
    timeLine.timeLength = timeLength;
    timeLine.timeList = timeList;
    timeLine.frameCount = frameCount;
    timeLine.channelId = "sound";
    timeLine.dt = m_dt;

    QList<QJsonObject> prpd_ = tdms.channelDataFrames().value("sc01_PRPD");
    QJsonObject prpd_data{};
    if( prpd_.size() > 0 ){
        for( int i = 0; i < prpd_.size(); i++ ){
            prpd_data.insert(QString::number(prpd_[i]["timeStamp"].toInt()),prpd_[i]["values"]);
        }
    }

    data["systemtyp"] = tdms.systemtyp();
    data["audio"] = audio_data;
    data["spectra"] = spectra_data;
    data["spectrum"] = spectrum_data;
    if( !tdms.p_videoFrames->isEmpty() ){
        data["video"] = video_data;
    }
    data["prpd"] = prpd_data;

    QList<QJsonObject> leq_ = tdms.channelDataFrames().value("sc01_Level_A");
    QJsonArray leq_data;
    if( leq_.size() > 0 ){
        for( int i = 0; i < leq_.size(); i++ ){
            leq_data.append( leq_[i] );
        }
        data["leq"] = leq_data;
    }

    // 外置相机
    QList<QJsonObject> camera_ = tdms.channelDataFrames().value("sc01_camera");

    if( camera_.size() > 0 ){
        float dt = camera_[0]["dt"].toDouble();
        for( int i = 0; i < camera_.size(); i++ ){
            QJsonArray arr = camera_[i]["values"].toArray();
            QByteArray byte;
            for( int j = 0; j < arr.size(); j++ ){
                byte.append( arr[j].toInt() );
            }
            _camera_map.insert( dt*1000*i , byte );
        }

        CameraData cameraData;
        cameraData._camera_map = _camera_map;
        cameraData._beam_map = _beam_map;
        cameraData.m_time_len = len2;

        cameraData.m_distance = distance;
        camera_data.setValue(cameraData);
        data["camera"] = camera_data;
    }

    QStringList grpList { tdms.p_tdmsMap_->tdmsChannels.uniqueKeys() };
    if( grpList.contains("rawdata") ){
        // rawData
        double r_dt{0.0};
        quint32 m_rdata_len{0};
        quint64 rtime{0};
        quint32 rcount{0};

        QMap<quint64, DataWithTimestamp*>* rawdataList = tdms.p_rawDataFrames;
        QMap<quint64, DataWithTimestamp*>::const_iterator ir = rawdataList->constBegin();
        while (ir != rawdataList->constEnd()) {
            DataRawData * rawDataFrame = static_cast<DataRawData *>(ir.value());
            r_dt = rawDataFrame->dtVal();
            m_rdata_len = rawDataFrame->bufferLength;
            rtime = rawDataFrame->timestamp() * r_dt;
            _raw_map.insert(std::pair<int, qint32 *>( ir.key() , reinterpret_cast<qint32 *>(rawDataFrame->buffer) ));
            ++ir;
            rcount++;
        }

        RawData rawData;
        rawData._raw_data = _raw_map;
        rawData.dt = r_dt ;
        rawData.len = m_rdata_len;
        rawData.time = rtime;
        rawData.count = rcount;
        raw_data.setValue(rawData);
        data["rawData"] = raw_data;

        timeLine.dt = r_dt;

    }

    time_line.setValue( timeLine );
    data["timeLine"] = time_line;

    // 判断老程序与新程序
    QList<QJsonObject> ev_ = tdms.channelDataFrames().value("sc01_ev-sp");
    if( ev_.size() > 0 ){
        //特征值

        EigenvalueData eigenvalueData;
        eigenvalueData.m_eigenvalue = ev_;
        eigenvalueData.channelId = "sound";
        eigenvalue_data.setValue(eigenvalueData);

        data["eigenvalue"] = eigenvalue_data;
        data["reprocess"] = 0;

//        data["systemtyp"] = tdms.m_systemtyp;
//        data["reprocess"] = 1;

    }else{
        // 调用计算模块
        data["systemtyp"] = tdms.m_systemtyp;
        data["reprocess"] = 1;

    }

    emit sendData(data);
}

void TdmsData::openVibrationData( QString f ){
    tdms.openSOCFile( f );

    QMultiMap<QString, QString> channelFrameTypes_ = tdms.channelFrameTypes();
    QList<QString> channels = channelFrameTypes_.uniqueKeys();

    QHash<QString, QList<QJsonObject>> channelDataFrames_ = tdms.channelDataFrames();

    for (const QString& channel : qAsConst(channels) ){ // 通道
        QString channelName = "ch" + QString::number(channel.rightRef(1).toInt()) ;

        QVariantMap data_;

        data_["channelId"] = channelName;
        QList<QString> frameTypes = channelFrameTypes_.values(channel);

        for ( const QString& frameType : qAsConst(frameTypes) ){
            // QList<QJsonObject> data = channelDataFrames_.value(frameType);
            //数据

            if( frameType.contains("ev-sp" , Qt::CaseSensitive) ){
                // 特征值
                EigenvalueData eigenvalueData;
                eigenvalueData.m_eigenvalue = channelDataFrames_.value(frameType);
                eigenvalueData.channelId = channelName;

                eigenvalue_data.setValue(eigenvalueData);
                data_["eigenvalue"] = eigenvalue_data;
            }else if( frameType.contains("spectra" , Qt::CaseSensitive) ){
                SpectraJsonData spectraJsonData;
                spectraJsonData.m_spectra = channelDataFrames_.value(frameType);
                spectraJsonData.channelId = channelName;

                spectra_json_data.setValue(spectraJsonData);
                data_["spectra"] = spectra_json_data;

                int time_{0};
                TimeLine timeLine;
                timeLine.frameCount = channelDataFrames_.value(frameType).size();
                for( int k = 0; k < timeLine.frameCount; k++ ){
                    QJsonObject spectra_json = channelDataFrames_.value(frameType)[k];

                    time_ += (int)(spectra_json["dt"].toDouble() * spectra_json["frameCount"].toInt() * 1000);
                    timeLine.timeList.append( time_ );
                    timeLine.timeLength = time_;
                }
                timeLine.channelId = channelName;
                time_line.setValue(timeLine);
                data_["timeLine"] = time_line;
            }else if( frameType.contains("timedata" , Qt::CaseSensitive) ){
                TimedataJson timedata_;
                timedata_.m_timedata = channelDataFrames_.value(frameType);
                timedata_.channelId = channelName;
                timedata_data.setValue(timedata_);
                data_["timedata"] = timedata_data;
            }
        }

        emit sendData( data_ );
    }
}
