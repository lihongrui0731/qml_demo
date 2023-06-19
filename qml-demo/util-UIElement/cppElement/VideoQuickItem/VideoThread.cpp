#include "VideoThread.h"
#include <QMetaType>
#include <QVariant>

VideoThread::VideoThread()
{
    qRegisterMetaType<std::map< int, unsigned char*>>("std::map< int, unsigned char*>");
    qRegisterMetaType<std::map<int,RhythmUI::picture>>("std::map<int,RhythmUI::picture>");
    qRegisterMetaType<QMap<int, QImage>>("QMap<int, QImage>");
    qRegisterMetaType<RhythmUI::AcousticImageModes>("RhythmUI::AcousticImageModes");

    qRegisterMetaType<QVector<quint8>>("QVector<quint8>&");
    qRegisterMetaType<QVector<float>>("QVector<float>&");
    //qRegisterMetaType<float>("float&");
    qRegisterMetaType<QVector<unsigned char>>("QVector<unsigned char>");
}

VideoThread::~VideoThread()
{
    qDebug() << "析构 video thread";
}

void VideoThread::run(){
    if( m_type == 1 ){
        sortData();
    }else{
        sortFinalData();
    }
}

void VideoThread::setVideoData(
    const unsigned int &width_pic, const unsigned int &height_pic, const std::map<int, unsigned char *> pictures_,
    const unsigned int &width_beam, const unsigned int &height_beam, const std::map<int, float*> beam_
        ){

    m_type = 1;
    m_pictures_ = pictures_;
    m_beam_ = beam_;
    _constItor = m_pictures_.begin();
    _beamItor = m_beam_.begin();

    m_picture_witdh = width_pic;
    m_picture_height = height_pic;
    m_beam_witdh = width_beam;
    m_beam_height = height_beam;
}

void VideoThread::setCameraData(
    const unsigned int &width_pic, const unsigned int &height_pic,const unsigned int &bytesPerLine_ , QMap<int, QByteArray> camera_,
    const unsigned int &width_beam, const unsigned int &height_beam, std::map<int, float*> beam_
        ){

    m_type = 1;

    m_camera_ = camera_;
    m_beam_ = beam_;
    _cameraItor = m_camera_.begin();
    _beamItor = m_beam_.begin();

    m_picture_witdh = width_pic;
    m_picture_height = height_pic;
    m_beam_witdh = width_beam;
    m_beam_height = height_beam;
    bytesPerLine = bytesPerLine_;

}


void VideoThread::setFinalVideoData(
    const unsigned int &width_pic, const unsigned int &height_pic, QMap<quint64, QVector<quint8>> pictures_,
    const unsigned int &width_beam, const unsigned int &height_beam, QMap<quint64, QVector<float>> beam_
        ){
    m_type = 2;
    m_pictures__ = pictures_;
    m_beam__ = beam_;
    _constItor_ = m_pictures__.begin();
    _beamItor_ = m_beam__.begin();

    m_picture_witdh = width_pic;
    m_picture_height = height_pic;
    m_beam_witdh = width_beam;
    m_beam_height = height_beam;
}


void VideoThread::setFinalCameraData(
    const unsigned int &width_pic, const unsigned int &height_pic,const unsigned int &bytesPerLine_ ,  QMap<quint64, QByteArray> camera_,
    const unsigned int &width_beam, const unsigned int &height_beam, QMap<quint64, QVector<float>> beam_
        ){
    m_type = 2;
    m_camera__ = camera_;
    m_beam__ = beam_;
    _cameraItor_ = m_camera__.begin();
    _beamItor_ = m_beam__.begin();
    bytesPerLine = bytesPerLine_;
    m_picture_witdh = width_pic;
    m_picture_height = height_pic;
    m_beam_witdh = width_beam;
    m_beam_height = height_beam;
}

void VideoThread::sortFinalData(){
    // 处理picture

    if( !m_pictures__.empty() ){
        const quint64 _count = m_picture_witdh * m_picture_height;
        // std::map<int, unsigned char *>::iterator _constItor = m_pictures_.begin();

        while( _constItor_ != m_pictures__.end() ){
            unsigned char* _data = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * _count * 3));
            unsigned char* pic_ = _constItor_.value().data();
            for ( quint64 _i = 0; _i < _count; _i++ ) {
                // qDebug() << _i << _constItor->second[_i];
                _data[_i* 3] = pic_[_i];
                _data[_i* 3+1] = pic_[_i];
                _data[_i* 3+2] = pic_[_i];
            }
            m_map_pictures.insert(std::pair<int, unsigned char*>( _constItor_.key(), _data ));
            _constItor_++;
        }
    }

    if( !m_camera__.isEmpty() ){
        while( _cameraItor_ != m_camera__.end() ){
            QImage img = QImage( reinterpret_cast<uchar *>( _cameraItor_.value().data()) , m_picture_witdh , m_picture_height , bytesPerLine, QImage::Format_RGB888 );
            QRect rect(0,0,img.width(),img.height());
            QImage image = img.copy(rect);

            m_map_camera.insert(_cameraItor_.key() , image);
            _cameraItor_++;
        }
    }

    quint64 _count_beam = m_beam_witdh * m_beam_height;
  //  std::map<int, float*>::iterator _beamItor = m_beam_.begin();

    while ( _beamItor_ != m_beam__.end() ){
        float* beam_ = _beamItor_.value().data();

        RhythmUI::picture _picture{RhythmUI::picture()};
        _picture.min = beam_[0]; // initialize min value
        _picture.max = beam_[0]; // initialize max value

        _picture.picturedata = static_cast<float*>(malloc(sizeof (float)* _count_beam));

        float _sum = 0.0f;
        quint64 _i = 0;
        while ( _i < _count_beam ) {
            _picture.min = beam_[_i] < _picture.min ? beam_[_i] : _picture.min;
            _picture.max = beam_[_i] > _picture.max ? beam_[_i] : _picture.max;
            _picture.picturedata[_i] = beam_[_i];
            _sum += beam_[_i];
            _i ++;
        }

        if( isTimeAxis ){
            beamMaxList.append( _picture.max );
        }

        _picture.ave = _sum/static_cast<float>(_count_beam);
        m_map_cloudPicture[_beamItor_.key()] = _picture;

        _beamItor_++;
    }

    if( isTimeAxis ){
        emit sendTimeAxis( beamMaxList );
    }

    if( !m_pictures__.empty() ){
        emit sendVideoData( m_map_pictures , m_map_cloudPicture );
    }else if( !m_camera__.empty() ){
        emit sendCameraData( m_map_camera , m_map_cloudPicture );
    }
}

void VideoThread::sortData(){
    // 处理picture

    if( !m_pictures_.empty() ){
        const quint64 _count = m_picture_witdh * m_picture_height;
        // std::map<int, unsigned char *>::iterator _constItor = m_pictures_.begin();
        while( _constItor != m_pictures_.end() ){
            unsigned char* _data = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * _count * 3));
            for ( quint64 _i = 0; _i < _count; _i++ ) {
                // qDebug() << _i << _constItor->second[_i];
                _data[_i* 3] = _constItor->second[_i];
                _data[_i* 3+1] = _constItor->second[_i];
                _data[_i* 3+2] = _constItor->second[_i];
            }
            m_map_pictures.insert(std::pair<int, unsigned char*>( _constItor->first, _data ));
            _constItor++;
        }
    }

    if( !m_camera_.empty() ){
        while( _cameraItor != m_camera_.end() ){
            QImage img = QImage::fromData( _cameraItor.value() );
            if( img.isNull() ) continue;

            QRect rect(0,0,img.width(),img.height());
            QImage image = img.copy(rect);
            m_map_camera.insert(_cameraItor.key() , image);
            _cameraItor++;
        }
    }

    quint64 _count_beam = m_beam_witdh * m_beam_height;
    while ( _beamItor != m_beam_.end() ){

        RhythmUI::picture _picture{RhythmUI::picture()};
        _picture.min = _beamItor->second[0]; // initialize min value
        _picture.max = _beamItor->second[0]; // initialize max value

        _picture.picturedata = static_cast<float*>(malloc(sizeof (float)* _count_beam));

        float _sum = 0.0f;
        quint64 _i = 0;
        while ( _i < _count_beam ) {
            _picture.min = _beamItor->second[_i] < _picture.min ? _beamItor->second[_i] : _picture.min;
            _picture.max = _beamItor->second[_i] > _picture.max ? _beamItor->second[_i] : _picture.max;
            _picture.picturedata[_i] = _beamItor->second[_i];
            _sum += _beamItor->second[_i];
            _i ++;
        }

        if( isTimeAxis ){
            beamMaxList.append( _picture.max );
        }

        _picture.ave = _sum/static_cast<float>(_count_beam);
        m_map_cloudPicture[_beamItor->first] = _picture;

        _beamItor++;
    }

    if( isTimeAxis ){
        emit sendTimeAxis( beamMaxList );
    }

    if( !m_pictures_.empty() ){
        emit sendVideoData( m_map_pictures , m_map_cloudPicture );
    }else if( !m_camera_.empty() ){
        emit sendCameraData( m_map_camera , m_map_cloudPicture );
    }
}

void VideoThread::setVideo(const unsigned int &width_, const unsigned int &height_, QVector<quint8> &picture_)
{
    if( isSetVideo ){
        return;
    }
    isSetVideo = true;

    int _count = static_cast<int>(width_) * static_cast<int>(height_) * 3;
    if( pictureData.capacity() != _count ){
        picture_temp.resize( _count );
        pictureData.resize( _count );
    }

    int _size = static_cast<int>(width_) * static_cast<int>(height_);
    for ( int _i = 0; _i < _size; _i++ ) {
        int _position = _i*3;
        picture_temp[_position] = picture_[_i];
        picture_temp[_position+1] = picture_[_i];
        picture_temp[_position+2] = picture_[_i];
    }

    pictureData.swap(picture_temp);
    if ( pictureData.size() == _count ){
        emit sendVideo( pictureData );
    }

    isSetVideo = false;
}

void VideoThread::setBeam(const unsigned int &width_, const unsigned int &height_, QVector<float> &beam_)
{
    if( isSetBeam ){
        return;
    }
    isSetBeam = true;
    if ( m_e_filterModelType == RhythmUI::AcousticImageModes::ImageModeAuto ) {
        // 自动模式
        // initialize the min data and the max data
        float _min = beam_[0];
        float _max = beam_[0];
        // copy all the data
        size_t _size = width_*height_;

        size_t _i = 0;
        while ( _i < _size ) {
            _min = beam_[_i] < _min ? beam_[_i] : _min;
            _max = beam_[_i] > _max ? beam_[_i] : _max;
            _i ++;
        }
        generateCloud(width_, height_, beam_.constData(), _min, _max);
    } else if ( m_e_filterModelType == RhythmUI::AcousticImageModes::ImageModeFixedThreshold ) {
        // 阈值模式
        generateCloud(width_, height_, beam_.constData(), 0, m_f_beamLimitMax);
    } else if ( m_e_filterModelType == RhythmUI::AcousticImageModes::ImageModeAverageThreshold ) {
        // 去噪模式
        // initialize the min data and the max data
        float _min = beam_[0];
        float _max = beam_[0];
        // copy all the data
        size_t _count = width_*height_;
        size_t _i = 0;
        float _sum = 0.0f;
        while ( _i < _count ) {
            _min = beam_[_i] < _min ? beam_[_i] : _min;
            _max = beam_[_i] > _max ? beam_[_i] : _max;
            _sum += beam_[_i];
            _i ++;
        }
        float _ave = _sum/_count + m_f_peakFactor;
        _max = _ave >= _max ? _ave : _max;
        generateCloud(width_, height_, beam_.constData(), _min, _max);
    } else if ( m_e_filterModelType == RhythmUI::AcousticImageModes::ImageModeTransient ) {
    }

    emit sendBeam( beamData , m_f_beamMax , m_f_beamMin );
    isSetBeam = false;
}

void VideoThread::generateCloud(const unsigned int& width_, const unsigned int& height_, const float* data_, const float& min_, const float& max_)
{
    int _size = static_cast<int>(width_) * static_cast<int>(height_) * 3;

    QVector<unsigned char> beam_temp;
    if( _size != beamData.capacity() ){
        beam_temp.resize( _size );
        beamData.resize( _size );
    }

    Q_UNUSED( min_ );
    m_f_beamMax = max_;
    m_f_beamMin = ( max_ - m_f_dynamicRange ) < 0?0 : ( max_ - m_f_dynamicRange );
    int _i = 0;
    int _count = static_cast<int>(width_) * static_cast<int>(height_);
    while ( _i < _count ) {
        const color _color = generateColor( data_[_i]);  // generate the colors
        int _pos = _i*3;
        beam_temp[_pos]   = static_cast<unsigned char>(_color.red);
        beam_temp[_pos+1] = static_cast<unsigned char>(_color.green);
        beam_temp[_pos+2] = static_cast<unsigned char>(_color.blue);
        _i += 1;
    }

    beamData.swap(beam_temp);
}

// 新版本色谱获取
VideoThread::color VideoThread::generateColor(const float& value ){
    // if (value < m_f_beamMin) value = m_f_beamMin;
    //  else if (value > m_f_beamMax) value = m_f_beamMax;

    float dv = m_f_beamMax - m_f_beamMin;
    //const c = {r: 0, g: 0, b: 0};
    float v = (value - m_f_beamMin) / dv;
    float _8th = 0.125;
    quint8 r = 0;
    quint8 g = 0;
    quint8 b = 0;

    if ( value <= m_f_beamMin ){ // last splite
        return color(0, 0, 0);
    }else{
        if (v < _8th) {
            b = 128 * (1 + v / _8th);
        } else if (v < 3 * _8th) {
            b = 255;
            g = 128 * (v - _8th) / _8th;
        } else if (v < 5 * _8th) {
            r = 128 * (v - 3 * _8th) / _8th;
            g = 255;
            b = 128 * (5 * _8th - v) / _8th;
        } else if (v < 7 * _8th) {
            r = 255;
            g = 128 * (7 * _8th - v) / _8th;
        } else {
            r = 128 * (9 * _8th - v) / _8th;
        }
    }
    return color(r, g, b);
}

