#include "openrsd.h"

OpenRSD::OpenRSD()
{
}

OpenRSD::~OpenRSD()
{
     qDebug() << "析构 tdms 线程 ~ ";
}

// 按照设备分为各通道 不区分时传入 "/" 区分通道时候 传入通道名
void OpenRSD::openChannelData( QString f , QString ch , QList<rsd_defs_soundcam::DatasetTypes> dataTypes ){

    data.clear();

    rcf_ = new RhythmCompositeFile(f);
    if(rcf_->isOpen()){

        metaReader = new MetadataReader(rcf_,ch);
        QJsonObject deviceInfo = metaReader->readDeviceInfo();
        data["deviceInfo"] = deviceInfo;

        QStringList contentModules = metaReader->readContentModules();
        if( contentModules.size() > 0 ){

        }
        QStringList sourceType = metaReader->readContentSourceTypes();
        if( sourceType.size() > 0 ){

        }

        QJsonObject deviceConfig = metaReader->readMeasurmentConfig();
        if( !deviceConfig.isEmpty() ){

        }

        sreader = new SoundcamSliceReader(rcf_,ch);
        if( dataTypes.isEmpty() ){
            sreader->loadDataOfAllTypes();
        }else{
            sreader->loadData( dataTypes );
        }

        unsigned int len{0};

        // 时间轴 窜在beam时候用beam 计算 不存在beam时用 timelocal计算
        bool timeline{false};
        QList<quint32> timeList;
        int frameCount{0};
        int timeLength{0};
        int ts{0};

        // video
        quint16 height_picture_map{0};
        quint16 width_picture_map{0};
        unsigned int len2{0};

        quint32 bandWidth;

        QMap<quint64, QSharedPointer<DataAcoustic>> beams_ = sreader->mapBeams();
        int distance{3000};
        if( !beams_.isEmpty() ){
            QMap<quint64, QSharedPointer<DataAcoustic>>::const_iterator i2 = beams_.constBegin();
            ts = i2.key();
            m_frequencyMax = i2.value()->maxFreq();
            m_frequencyMin = i2.value()->minFreq();

            while (i2 != beams_.constEnd()) {
                distance = i2.value()->distance();
                len2 = i2.key() - ts;
                timeLength = i2.key() - ts;
                timeList.append( i2.key() - ts );
                frameCount++;
                timeline = true;

                _beam_map.insert(std::pair<int, float*>( i2.key() - ts ,i2.value()->buffer ));
                ++i2;
            }
        }

        QMap<quint64, QSharedPointer<DataVideo>> pics_ = sreader->mapPictures();
        if( !pics_.isEmpty() ){
            QMap<quint64, QSharedPointer<DataVideo>>::const_iterator i1 = pics_.constBegin();
            while (i1 != pics_.constEnd()) {
                _picture_map.insert(std::pair<int, unsigned char*>( i1.key() - ts , i1.value()->buffer ));

                width_picture_map = i1.value()->hRes();
                height_picture_map = i1.value()->vRes();
                ++i1;
            }

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
        QList<QSharedPointer<SoundcamSubAudio>> single_local = sreader->listAudioLocal();
        if( !single_local.isEmpty() ){
            m_dt = single_local[0]->dt();
            for( int i = 0; i < single_local.size(); i++ ){
                m_adata_len = single_local[i]->bufferLength();
                a_time = single_local[i]->timestamp() + i * 1000.0 * m_dt * m_adata_len;
                _locS_data.insert(std::pair<int, qint32*>( a_time ,single_local[i]->buffer() ));
                count++;

                if( !timeline ){
                    timeLength = a_time;
                    timeList.append( a_time );
                    frameCount++;
                }

            }
        }

        TimeLine timeLine;
        timeLine.timeLength = timeLength;
        timeLine.timeList = timeList;
        timeLine.frameCount = frameCount;
        timeLine.channelId = "sound";

        AudioData audioData;
        audioData._locS_data = _locS_data;
        audioData.dt = m_dt ;
        audioData.len = m_adata_len;
        audioData.time = a_time;
        audioData.count = count;
        audio_data.setValue(audioData);
        timeLine.dt = m_dt;

        QList<QSharedPointer<SoundcamSubAudio>> single = sreader->listAudioSingle();
        if( !single.isEmpty() ){

            double r_dt{0.0};
            quint32 m_rdata_len{0};
            quint64 rtime{0};
            quint32 rcount{0};

            r_dt = single[0]->dt();
            for( int i = 0; i< single.size(); i++ ){
                m_rdata_len = single[i]->bufferLength();
                rtime = single[i]->timestamp() + i * r_dt * 1000.0 * m_rdata_len;
                _raw_map.insert(std::pair<int, qint32*>( rtime , single[i]->buffer() ));
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

            // timeLine.dt = r_dt;
        }

        data["audio"] = audio_data;

        if( !pics_.isEmpty() ){
            data["video"] = video_data;
        }

        float df{0.0};
        QList<QSharedPointer<DataSpectrum>> spec_ = sreader->listSpectrumGlobal();
        if( !spec_.isEmpty() ){
            int spec_dt = timeLength / spec_.size();
            df = spec_[0]->deltaF();
            for(int i = 0; i < spec_.size(); i++ ){
                 len = spec_[i]->bufferLength;
                 m_pdata_time = spec_[i]->timestamp() + spec_dt * (float)i ;
                 _map_spectrum.insert(std::pair<int, float*>(m_pdata_time, spec_[i]->buffer));

            }
        }

        QList<QSharedPointer<DataSpectrum>> spec_local = sreader->listSpectrumLocal();
        if( !spec_local.isEmpty() ){
            int spec_dt = timeLength / spec_.size();
            for( int i = 0; i < spec_local.size(); i++ ){
                 m_pdata_time = spec_local[i]->timestamp() + spec_dt * (float)i;
                 _local_map_spectrum.insert(std::pair<int, float*>(m_pdata_time,spec_local[i]->buffer));

            }
        }

        bandWidth = round(df * len);

        // 频谱数据
        SpectraData spectraData;
        spectraData._p_data = _map_spectrum;
        spectraData.len = len;
        spectraData.m_frequencyMax = m_frequencyMax;
        spectraData.m_frequencyMin = m_frequencyMin;
        spectraData.m_pdata_time = m_pdata_time;
        spectraData.df = df;
        spectra_data.setValue(spectraData);

        // 频谱切片数据
        SpectrumData spectrumData;
        spectrumData._map_spectrum = _map_spectrum;
        spectrumData._local_map_spectrum = _local_map_spectrum;
        spectrumData.len = len;
        spectrumData.m_frequencyMax = m_frequencyMax;
        spectrumData.m_frequencyMin = m_frequencyMin;
        spectrumData.df = df;
        spectrum_data.setValue(spectrumData);

        data["spectra"] = spectra_data;
        data["spectrum"] = spectrum_data;

        time_line.setValue( timeLine );
        data["timeLine"] = time_line;
        data["bandWidth"] = bandWidth;
        data["reprocess"] = 0;

        emit sendData(data);

    }else{
        qDebug()<<"file invalid ," << rcf_->filePath();
    }
}
