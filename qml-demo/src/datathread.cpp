#include "datathread.h"

DataThread::DataThread()
{
    qDebug() << "创建 DataThread 线程";
    qRegisterMetaType<ReportFrame>("ReportFrame");
    qRegisterMetaType<QMap<quint64, QJsonObject>>("QMap<quint64, QJsonObject>");
    qRegisterMetaType<QMap<quint64, QByteArray>>("QMap<quint64, QByteArray>");
    qRegisterMetaType<QMap<quint64, float*>>("QMap<quint64, float*>");
    qRegisterMetaType<QMap<quint64,unsigned char*>>("QMap<quint64,unsigned char*>");
    qRegisterMetaType<std::map<int, float*>>("std::map<int, float*>");
    qRegisterMetaType<std::map<int, unsigned char*>>("std::map<int, unsigned char*>");

    qRegisterMetaType<QMap<quint64, QVector<float>>>("QMap<quint64, QVector<float>>&");
    qRegisterMetaType<QMap<quint64, QVector<quint8>>>("QMap<quint64, QVector<quint8>>&");
    qRegisterMetaType<QMap<QString,QMap<quint64,QVector<float>>>>("QMap<QString,QMap<quint64,QVector<float>>>&");
    qRegisterMetaType<QMap<QString,QMap<quint64,QJsonObject>>>("QMap<QString,QMap<quint64,QJsonObject>>&");
    qRegisterMetaType<QMap<QString,QMap<quint64,QJsonObject>>>("QMap<QString,QMap<quint64,QJsonObject>>&");
}

DataThread::~DataThread()
{
    qDebug() << "析构线程 ~";
}

void DataThread::getFinal(){
    qDebug() << "sort final " << beam_.size() ;

    sortFinalData();
}

void DataThread::sortFinalData()
{
    // std::map<int, unsigned char*> _picture_map;
    // std::map<int, float*> _beam_map;
    QJsonObject _prpd;
    QJsonObject _eigenvalue;
    QJsonObject _leq;
    quint32 time_len{0};

    quint64 startTs{0}; // 硬件时钟的数据
    quint64 startTs_sound{0}; // 无硬件时钟的时间

    // quint64 startTs_{0};
    if( beam_.size() > 0 ){
        QMap<quint64,QVector<float>>::iterator beam_iterator = beam_.begin();
        startTs = beam_iterator.key();
        while( beam_iterator != beam_.end() ){
            time_len = beam_iterator.key() - startTs;
            _beam_map.insert( beam_iterator.key() - startTs , beam_iterator.value() );
            beam_iterator++;
        }
    }

    if( video_.size() > 0 ){
        QMap<quint64,QVector<quint8>>::iterator video_iterator = video_.begin();
        while( video_iterator != video_.end() ){
            _picture_map.insert( video_iterator.key() - startTs , video_iterator.value() );
            video_iterator++;
        }
    }

    QMap<quint64,QVector<float>> spectra = spectra_["sound"];
    if( spectra.size() > 0 ){
        QMap<quint64,QVector<float>>::iterator spectra_iterator = spectra.begin();
        startTs = startTs>0?startTs:spectra_iterator.key();
        while( spectra_iterator != spectra.end() ){
            time_len = spectra_iterator.key() - startTs;
            _spectra_map.insert( spectra_iterator.key() - startTs , spectra_iterator.value() );
            spectra_iterator++;
        }
    }

    QMap<quint64,QVector<float>> spectra_local_ = spectra_local["sound"];
    if( spectra_local_.size() > 0 ){
        QMap<quint64,QVector<float>>::iterator spectra_local_iterator = spectra_local_.begin();
        while( spectra_local_iterator != spectra_local_.end() ){
            _spectra_local_map.insert( spectra_local_iterator.key() - startTs , spectra_local_iterator.value() );
            spectra_local_iterator++;
        }
    }

    if( camera_.size() > 0 ){
        QMap<quint64,QByteArray>::iterator camera_iterator = camera_.begin();
        startTs = camera_iterator.key();
        while( camera_iterator != camera_.end() ){
            time_len = camera_iterator.key() - startTs;
            _camera_map.insert( camera_iterator.key() - startTs , camera_iterator.value() );
            camera_iterator++;
        }
    }

    if( raw_.size() > 0 ){
        QMap<quint64,QVector<float>>::iterator raw_iterator = raw_.begin();
        startTs_sound = raw_iterator.key();
        int key = raw_iterator.key()>startTs_sound?raw_iterator.key()-startTs_sound:0;
        _raw_map.insert( key , raw_iterator.value() );
        raw_iterator++;
        while( raw_iterator != raw_.end() ){
            _raw_map.insert( raw_iterator.key() - startTs_sound , raw_iterator.value() );
            raw_iterator++;
        }
    }

    QMap<quint64,QJsonObject> eigenvalue = eigenvalue_["sound"];
    if( eigenvalue.size() > 0 ){
        QMap<quint64,QJsonObject>::iterator ev_iterator = eigenvalue.begin();
        startTs_sound = startTs_sound==0? ev_iterator.key():startTs_sound;
        while( ev_iterator != eigenvalue.end() ){
            _eigenvalue.insert( QString::number(ev_iterator.key() - startTs_sound) , ev_iterator.value() );
            ev_iterator++;
        }
    }

    if( leq_["sound"].size() > 0 ){
        QMap<quint64,QJsonObject>::iterator leq_iterator = leq_["sound"].begin();
        while( leq_iterator != leq_["sound"].end() ){
            _leq.insert( QString::number(leq_iterator.key() - startTs_sound ) , leq_iterator.value() );
            leq_iterator++;
        }
    }

    if( prpd_.size() > 0 ){
        QMap<quint64,QJsonObject>::iterator prpd_iterator = prpd_.begin();
        while( prpd_iterator != prpd_.end() ){
            _prpd.insert( QString::number(prpd_iterator.key() - startTs_sound ) , prpd_iterator.value() );
            prpd_iterator++;
        }
    }

    emit sendFinalData( _picture_map , _camera_map , _beam_map , _spectra_map , _spectra_local_map , _raw_map , _prpd , _leq , _eigenvalue , time_len );
}

void DataThread::addAudio( quint64 time , QVector<float> data ){
    raw_.insert( time , data );

    QList<quint64> raw_key = raw_.keys();
    for( quint64 key : qAsConst(raw_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            raw_[key].clear();
            raw_.remove(key);
        }
    }
}
void DataThread::addRaw( quint64 time , QVector<float> data ){
    raw_.insert( time , data );
    QList<quint64> raw_key = raw_.keys();
    for( quint64 key : qAsConst(raw_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            raw_[key].clear();
            raw_.remove(key);
        }
    }
}
void DataThread::addPicture( quint64 time ,QVector<quint8> data ){
    video_.insert( time , data );

    QList<quint64> video_key = video_.keys();
    for( quint64 key : qAsConst(video_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            video_[key].clear();
            video_.remove(key);
        }
    }
}

void DataThread::addBeam( quint64 time ,QVector<float> data ){
    beam_.insert( time , data );

    QList<quint64> beam_key = beam_.keys();
    for( quint64 key : qAsConst(beam_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            beam_[key].clear();
            beam_.remove(key);
        }
    }
}

void DataThread::addSpectra( QString channelId , quint64 time ,QVector<float> data ){
    QMap<quint64,QVector<float>> spectra__ = spectra_.value(channelId);
    spectra__.insert( time , data );
    quint32 len = data.count();

    QList<quint64> spectra_key = spectra__.keys();
    for( quint64 key : qAsConst(spectra_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            spectra__[key].clear();
            spectra__.remove(key);
        }
    }
    spectra_.insert( channelId, spectra__ );
    spectra_len.insert( channelId, len );
}
void DataThread::addSpectraLocal( QString channelId , quint64 time ,QVector<float> data ){
    QMap<quint64,QVector<float>> spectra__ = spectra_local.value(channelId);
    spectra__.insert( time , data );
    quint32 len = data.count();

    QList<quint64> spectra_key = spectra__.keys();
    for( quint64 key : qAsConst(spectra_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            spectra__[key].clear();
            spectra__.remove(key);
        }
    }
    spectra_local.insert( channelId, spectra__ );
    spectraLocal_len.insert( channelId, len );

}
void DataThread::addSpectraJson( QString channelId , quint64 time ,QJsonObject data ){
    //振动新格式
    QMap<quint64,QJsonObject> spectra__json = spectra_json.value(channelId);
    spectra__json.insert( time , data );
    quint32 len = data.count();

    if( len == 0 ){
        return;
    }

    QList<quint64> spectra_json_key = spectra__json.keys();
    for( quint64 key : qAsConst(spectra_json_key) ){
        if( key < time - vibration_record_time && time > sound_record_time ){
            spectra__json.remove(key);
        }
    }

    spectra_json.insert( channelId, spectra__json );
    spectra_len.insert( channelId, len );
}
void DataThread::addPRPD( quint64 time , QJsonObject data ){
    prpd_.insert( time , data );
    QList<quint64> prpd_key = prpd_.keys();
    for( quint64 key : qAsConst(prpd_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            prpd_.remove(key);
        }
    }
}
void DataThread::addEv( QString channelId , quint64 time , QJsonObject data ){
    quint16 record_time;
    if( channelId == "sound" ){
        record_time = sound_record_time;
    }else{
        record_time = vibration_record_time;
    }

    QMap<quint64,QJsonObject> eigenvalue__ = eigenvalue_.value(channelId);
    eigenvalue__.insert( time , data );

    QList<quint64> eigenvalue_key = eigenvalue__.keys();
    for( quint64 key : qAsConst(eigenvalue_key) ){
        if( key < time - record_time && time > record_time  ){
            eigenvalue__.remove(key);
        }
    }
    eigenvalue_.insert( channelId, eigenvalue__ );
}


void DataThread::addLeq( QString channelId , quint64 time , QJsonObject data ){
    quint16 record_time;
    if( channelId == "sound" ){
        record_time = sound_record_time;
    }else{
        record_time = vibration_record_time;
    }

    QMap<quint64,QJsonObject> leq__ = leq_.value(channelId);
    leq__.insert( time , data );

    QList<quint64> eigenvalue_key = leq__.keys();
    for( quint64 key : qAsConst(eigenvalue_key) ){
        if( key < time - record_time && time > record_time  ){
            leq__.remove(key);
        }
    }
    leq_.insert( channelId, leq__ );
}


void DataThread::addTimedata( QString channelId , quint64 time , QJsonObject data ){
    QMap<quint64,QJsonObject> timedata_json = timedata_.value(channelId);
    timedata_json.insert( time , data );

    QList<quint64> spectra_json_key = timedata_json.keys();
    for( quint64 key : qAsConst(spectra_json_key) ){
        if( key < time - vibration_record_time && time > vibration_record_time  ){
            timedata_json.remove(key);
        }
    };
    timedata_.insert( channelId, timedata_json );
}

void DataThread::addCamera( quint64 time , QByteArray data ){
    if( beam_.isEmpty() ){
        return;
    }
    camera_.insert(time,data);
    QList<quint64> camera_key = camera_.keys();
    for( quint64 key : qAsConst(camera_key) ){
        if( key < time - sound_record_time && time > sound_record_time ){
            camera_.remove(key);
        }
    }
}

void DataThread::getTimedataThread( QMap<quint64,QJsonObject> data_, QString channelId ){

    QList<float> list_;
    double dt_{0.0};
    int len{0};
    float max{0.0};

    QMap<quint64,QJsonObject>::iterator iter = data_.begin();
    while( iter != data_.end() ){
        QJsonObject obj_ = iter.value();
        dt_ = obj_.value("dt").toDouble();
        QJsonArray arr_ = obj_.value("values").toArray();

        for( int i = 0; i < arr_.size(); i++ ){
            list_.append(arr_[i].toDouble());
            if( abs(arr_[i].toDouble()) > max ){
                max = abs(arr_[i].toDouble());
            }
            len++;
        }
        iter++;
    }

    emit sendTimedataByChannel( list_ , dt_ , channelId , len , max );
}

void DataThread::getSpectraDataThread( QMap<quint64,QJsonObject> spa_, quint32 spa_len , QString channelId ){
    QMap<quint64,QJsonObject>::iterator spa_iter = spa_.begin();
    int startTs = spa_iter.key();
    Q_UNUSED (startTs);
    Q_UNUSED (spa_len);

    QList<QString> _spectra_data;
    quint32 count = 0;
    while( spa_iter != spa_.end() ){
        //_spectra_data.insert(std::pair<int,float*>( spa_iter.key() - startTs , spa_iter.value()) );

        QJsonDocument JsonDoc = QJsonDocument( spa_iter.value() );
        QString s = JsonDoc.toJson(QJsonDocument::Compact);

        _spectra_data.append(s);
        count++;
        spa_iter++;
    }

    emit sendSpectraByChannel( _spectra_data , channelId );
}

void DataThread::getEigenvalueDataThread( QMap<quint64,QJsonObject> ev_, QString channelId , QString type ){
    QMap<quint64,QJsonObject>::iterator ev_iter = ev_.begin();
    QJsonArray varr;
    // int frameCount = obj.value("frameCount").toInt();
    while( ev_iter != ev_.end() ){
        varr.append( ev_iter.value() );
        ev_iter++;
    }

    QJsonDocument document;
    document.setArray(varr);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);

    emit sendEigenvalueByType(json_str , channelId , type);
}


/* 无法满足振动需求
void DataThread::getEigenvalueDataThread( QMap<quint64,QJsonObject> ev_, QString channelId , QString type ){
    QMap<quint64,QJsonObject>::iterator ev_iter = ev_.begin();
    QJsonObject obj = ev_iter.value();
    QJsonObject values = obj.value("values").toObject();
    QJsonArray varr = values[type].toArray();
    // int frameCount = obj.value("frameCount").toInt();
    ev_iter++;
    while( ev_iter != ev_.end() ){
        QJsonObject obj_ = ev_iter.value();
        QJsonObject values_ = obj_.value("values").toObject();
      //  quint32 frameCount_ = obj_.value("frameCount").toInt();
      //  frameCount += frameCount_;

        // values_iter.key();
        QJsonArray arr_ = values_[ type ].toArray();
        for( int i = 0; i< arr_.count(); i++ ){
            varr.append(arr_[i]);
        }
        ev_iter++;
    }

    values.insert( type , varr );
    // obj.insert("frameCount" , frameCount );
    obj.insert("values" , values);

    QJsonDocument document;
    document.setObject(obj);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);

    emit sendEigenvalueByType(json_str , channelId , type);
}*/
