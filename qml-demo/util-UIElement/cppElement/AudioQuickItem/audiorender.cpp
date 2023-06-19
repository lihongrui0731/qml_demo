#include "audiorender.h"
#include "src/datamanager.h"
#include <QFile>
#include <QStandardPaths>
#include <QtMath>
#include <qendian.h>
#ifdef Q_OS_LINUX
#include <malloc.h>
#endif
AudioRender::AudioRender(): m_pushTimer(new QTimer(this))
{
    m_format = initializeAudio(QAudioDeviceInfo::defaultOutputDevice());
    m_audioItem = new AudioQuickItem( m_b_data );
}

AudioRender::~AudioRender(){

    //if( raw_.size() == 0 ){
        std::map<int, qint32*>::iterator iter = m_i_data.begin();
        while( iter != m_i_data.end() ){
            free( iter->second );
            iter++;
        }
    //}

    delete m_audioOutput;
    m_audioOutput = nullptr;

    delete m_audioItem;
    m_audioItem = nullptr;

    delete m_pushTimer;
    m_pushTimer = nullptr;
}

void AudioRender::setFinalData( QVariant data ){
    rawFinalData t = data.value<rawFinalData>();

    raw_ = t.raw;
    int ts = 0;
    int c = 0;
    //int in_count = 0;
    // QVector<float> in_;
    QMap<quint64, QVector<float>>::Iterator iter = raw_.begin();

    sesitivity = MicSesitivity * ( 1.0 / 32.0 );

    arma::Col<float> tmp(32,arma::fill::zeros);
    while( iter != raw_.end() ){
        // m_i_data.insert( std::pair<int,qint32*> ( iter.key() , reinterpret_cast< qint32* >(iter.value().data()) ) );
        /*
        if( ts == 0 ){
            ts = iter.key();
        }
        */
        // in_ += iter.value();
        // in_count++;
        // if( in_count == ratio ){
        // if( in_count == ratio ){

        m_i_data.insert( std::pair<int,qint32*> ( iter.key() , downFrame( tmp , iter.value().data() , iter.value().size()) ) );
        m_i_len = iter.value().size() / ratio;
        c++;
        //    in_count = 0;
        //    in_.clear();
        //    ts = 0;
        // }

        iter++;
    }

    m_i_count = c;

    if( m_outputCSV ){
        QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
        QString dir2 = appData + "/audioData";

        QDir dataDir{appData};
        if(!dataDir.exists("audioData"))
            dataDir.mkpath("audioData");

        std::map<int, qint32*>::iterator iter_ = m_i_data.begin();
        while( iter_ != m_i_data.end() ){
            csvOutput( dir2 , iter_->second , m_i_len );
            iter_++;
        }
    }

    if( generateData(0) ){
        m_audioItem->writeData( m_b_data.constData() , m_b_data.count() );
        m_i_timeline = m_i_data.begin()->first;
    }
    emit finish();
}

void AudioRender::setRawData( QVariant data ){
    RawData t = data.value<RawData>();

    int c = 0;
    int in_count = 0;
    QVector<float> in_;
    arma::Col<float> tmp(32,arma::fill::zeros);
    int ts = 0;
    std::map<int, qint32*>::iterator iter = t._raw_data.begin();

    sesitivity = 1.0/32.0;
    while( iter != t._raw_data.end() ){
        if( ts == 0 ){
            ts = iter->first;
        }
        for( int i = 0; i < t.len; i++ ){
            in_.append( iter->second[i] );
        }
        in_count++;
        if( in_count == ratio ){
            m_i_data.insert( std::pair<int,qint32*> ( ts , downFrame( tmp , in_.data() , in_.size()) ) );
            m_i_len = t.len;
            c++;
            in_count = 0;
            in_.clear();
            ts = 0;
        }
        iter++;
    }

    // qDebug() << m_i_data.size() << m_i_len << "bbbbbbbbbbbbbbbbbbbbb";

    if( m_outputCSV ){
        QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
        QString dir2 = appData + "/audioData";

        QDir dataDir{appData};
        if(!dataDir.exists("audioData"))
            dataDir.mkpath("audioData");

        std::map<int, qint32*>::iterator iter_ = m_i_data.begin();
        while( iter_ != m_i_data.end() ){
            csvOutput( dir2 , iter_->second , m_i_len );
            iter_++;
        }
    }

    m_i_count = c;
    // m_f_durationSeconds = t.dt;
    // m_sampleRate = static_cast<quint32>(1/t.dt);

    if( generateData(0) ){
        m_audioItem->writeData( m_b_data.constData() , m_b_data.count() );
        m_i_timeline = m_i_data.begin()->first;
    }
    emit finish();
}

void AudioRender::setData( QVariant data ){
    AudioData t = data.value<AudioData>();

    // double dt = t.dt;
    m_i_len = t.len;
    m_i_data = t._locS_data;
    m_i_count = t.count;

    m_f_durationSeconds = t.dt;;
    m_sampleRate = static_cast<quint32>(1/t.dt);

    std::map<int, qint32*>::iterator iter = m_i_data.begin();
    while( iter != m_i_data.end() ){
        for( int i = 0; i < m_i_len; i++ ){
            iter->second[i] = iter->second[i] * m_i_enlarge;
        }
        iter++;
    }

    if( m_outputCSV ){
        QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
        QString dir2 = appData + "/audioData";

        QDir dataDir{appData};
        if(!dataDir.exists("audioData"))
            dataDir.mkpath("audioData");

        std::map<int, qint32*>::iterator iter_ = m_i_data.begin();
        while( iter_ != m_i_data.end() ){
            csvOutput( dir2 , iter_->second , m_i_len );
            iter_++;
        }
    }

    if( generateData(0) ){
        m_audioItem->writeData( m_b_data.constData() , m_b_data.count() );
        m_i_timeline = m_i_data.begin()->first;
    }
    emit finish();
};

void AudioRender::addData( QVariant data ){
    floatData data_ = data.value<floatData>();
    qint32 *audio = reinterpret_cast<qint32 *>(data_.data.data());
    m_i_len = data_.data.size();
    addData( audio , data_.data.size() );
}

void AudioRender::addData( qint32 * data , quint32 len ){

    qint32 * data_ =  new qint32[len];
    for( quint32 i = 0; i < m_i_len; i ++ ){
        data_[i] =  data[i] * m_i_enlarge;
    }
    m_b_data.append( (char*)data_ , len  * sizeof(qint32) );
    m_i_count++;
    m_audioItem->writeData((char*)data_ , len  * sizeof(qint32));
    delete[] data_;
}

// 手动拖动时间轴触发
void AudioRender::changeTimeLine( quint32 timeline ){
    //测试
    if( timeline == 0 ){
        return;
    }

    bool res = generateData( timeline );
    if( !res ){
        return;
    }

    if( run ){
        // stop();
        //m_audioItem->clearData(); 清除会抖动
        if( m_b_data.count() > 0 ){
            m_audioItem->writeData( m_b_data.constData() , m_b_data.count() );
            if( audioState == QAudio::IdleState ){
                m_audioOutput->start(m_audioItem);
            }
        }

        //m_audioItem = new AudioQuickItem( m_b_data );
        //play();
    }else{
        m_audioItem->clearData();
        if( m_b_data.count() > 0 )
            m_audioItem->writeData( m_b_data.constData() , m_b_data.count() );
        //m_audioItem = new AudioQuickItem( m_b_data );
    }
}

bool AudioRender::generateData( quint32 timeline ){
    std::map<int, qint32*>::iterator iter;

    iter = m_i_data.upper_bound( timeline );

    //声音向后取一帧 所以不需要减
    /*
    if( iter != m_i_data.begin() && (iter->first - 1) > (int)timeline ){
        iter--;
    }
    */

    if( ( m_i_timeline != 0 && (int)m_i_timeline == iter->first ) || iter == m_i_data.end() ){
        return false;
    }

    m_i_timeline = iter->first;
    m_b_data.clear();
    //m_b_data = QByteArray( (char*)data_ , m_i_len * count_ * sizeof(qint32) );
    m_b_data = QByteArray( (char*)iter->second , m_i_len * sizeof(qint32) );
    return true;
}

QAudioFormat AudioRender::initializeAudio( const QAudioDeviceInfo &deviceInfo ){
    QAudioFormat format;
    format.setSampleRate(m_sampleRate);
    format.setChannelCount(1);
    format.setSampleSize(32);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    if (!deviceInfo.isFormatSupported(format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        format = deviceInfo.nearestFormat(format);
    }

    m_audioOutput = new QAudioOutput(format, this);

    connect( m_audioOutput , &QAudioOutput::stateChanged , [=]( QAudio::State state){
        audioState = state;
    });

    qreal voiceValue_ = QAudio::convertVolume(m_audioOutput->volume(),
                                                QAudio::LinearVolumeScale,
                                                QAudio::LogarithmicVolumeScale);

    m_i_volumeValue = voiceValue_ * 100;
    return format;
}

void AudioRender::deviceChanged(int index)
{;
    Q_UNUSED(index);
    m_audioOutput->stop();
    m_audioOutput->disconnect(this);
    // initializeAudio(m_deviceBox->itemData(index).value<QAudioDeviceInfo>());
}

void AudioRender::volumeChanged(int value)
{
    qreal linearVolume = QAudio::convertVolume(value / qreal(100),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);

    m_audioOutput->setVolume(linearVolume);
}

int AudioRender::getVolume(){
    return m_i_volumeValue;
}

void AudioRender::volumeClose( bool b ){
    m_b_noVolume = b;
    if( m_b_noVolume ){
        volumeChanged(0);
    }else{
        volumeChanged(m_i_volumeValue);
    }
}

void AudioRender::play(){
    if( run ){
        return;
    }
    /*
    if( m_b_data.count() > 0 ){
        m_audioItem->writeData( m_b_data.constData() , m_b_data.count());
    }
    */
    m_audioOutput->start(m_audioItem);
    run = true;
}

void AudioRender::start(){
    std::map<int, qint32*>::iterator iter = m_i_data.begin();
    while( iter != m_i_data.end() ){
        free( iter->second );
        iter++;
    }
    m_i_data.clear();
}

void AudioRender::stop(){
    run = false;
    m_audioOutput->stop();
    // m_audioItem->clearData();
}

void AudioRender::reset(){
    bool run_ = run;
    if( m_b_data.count() > 0 ){
        stop();
        //if( generateData(0) )
        m_audioItem->clearData();
        m_audioItem->writeData( m_b_data.constData() , m_b_data.count() );

        if( run_ ){
            play();
        }
    }
}

qint32 * AudioRender::downFrame( arma::Col<float> &tmp ,  float *data_ , int len ){
    typedef float re;
    typedef arma::Col<re> rvec;

    sp::resampling< re > rs(1, ratio);

    rvec x = rvec( reinterpret_cast<re *>(data_) , len , false );
    x = arma::join_cols( tmp, x );
    tmp = x.tail_rows(32);

    rvec y( x.n_rows * 1 / ratio );

    rs.upfirdown(x, y);
    y.shed_row(8);
    y = y / sesitivity * m_i_enlarge;

    qint32 *p_y = static_cast<qint32* >(malloc( sizeof(qint32) * y.n_rows ));

    arma::Col<qint32> y_ = arma::conv_to<arma::Col<qint32>>::from(y);
    for( int i = 0; i < y_.n_elem; i++){
        p_y[i] = static_cast<qint32>(y_(i));
    }

    return p_y;
}

void AudioRender::csvOutput( QString p, qint32 *data_ , int len ){

    QString path = p + "/audio.csv";

    QFile file(path);
    if( !file.open( QIODevice::Append )) {
        return;
    }

    QTextStream out( &file );
    for( int i = 0; i < len; i++ ){
        out<< data_[i]<< "\n";
    }

    out.flush();
    file.close();
}

