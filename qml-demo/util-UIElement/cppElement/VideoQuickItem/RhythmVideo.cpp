#include "RhythmVideo.h"
#include "RhythmVideoGLES.h"
#include "RhythmGradientBar.h"

//#include <synchapi.h>
#include <iostream>
#include <QDebug>
#include <ctime>
#include <cmath>
#include <limits>
#include <memory>
#include <QVector>

// #include "InfoWidget.h"
// #include "EigenValueTableWidget.h"

#include <QOpenGLFramebufferObject>
#include <QThread>
#include <QQuickItem>

#include <QElapsedTimer>

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif

class VideoRender:public QQuickFramebufferObject::Renderer {
public:
    VideoRender();
    ~VideoRender();

    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *) override;

private:
    bool isRun{false};
    RhythmVideoGLES m_render;

    // QElapsedTimer etimer;
};

VideoRender::VideoRender()
{
    m_render.initializeGL();
    // etimer.start();
}

VideoRender::~VideoRender(){
}

void VideoRender::render()
{
    m_render.paintGL();
}

void VideoRender::synchronize(QQuickFramebufferObject *item)
{
    RhythmVideo *shader = qobject_cast<RhythmVideo *>(item);

    isRun = shader->getStatus();

    if( m_render.night() != shader->nightVersion() ){
        m_render.setNight( shader->nightVersion() );
    }
    if( m_render._method() != shader->nightMethod() ){
        m_render.setMethod( shader->nightMethod() );
    }

    // 夜视算法参数
    if( m_render._brightness() != shader->brightness() ){
        m_render.setBrightness( shader->brightness() );
    }
    if( m_render._contrast() != shader->contrast() ){
        m_render.setContrast( shader->contrast() );
    }
    if( m_render._saturation() != shader->saturation() ){
        m_render.setSaturation( shader->saturation() );
    }
    if( m_render._highLight() != shader->highLight() ){
        m_render.setHighLight( shader->highLight() );
    }
    if( m_render._shadow() != shader->shadow() ){
        m_render.setShadow( shader->shadow() );
    }
    if( m_render._midtone() != shader->midtone() ){
        m_render.setMidtone( shader->midtone() );
    }

    if( m_render._coefficient() != shader->coefficient() ){
        m_render.setCoefficient( shader->coefficient() );
    }

    if( !shader->get_ui_image().isNull() ){
        m_render.setFrameMaxMin( shader->get_rgbMax() , shader->get_rgbMin() );
        m_render.setFrame( shader->get_ui_width(),shader->get_ui_height(), shader->get_ui_image() );
    }
    if( shader->get_ui_frame() != nullptr ){
        m_render.setFrame( shader->get_ui_width(),shader->get_ui_height(), shader->get_ui_frame() );
    }
    if( shader->get_ui_cloudFrame() != nullptr ){
        m_render.setCloudFrame( shader->get_ui_cloudWidth(),shader->get_ui_cloudHeight(), shader->get_ui_cloudFrame() );
    }

    // 渲染事件
    // m_render.setRotate(shader->xRotate(), shader->yRotate(), shader->zRotate());
    // shader->updateFPS(m_render.getFPS());
}

QOpenGLFramebufferObject *VideoRender::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

RhythmVideo::Renderer * RhythmVideo::createRenderer() const
{
    return new VideoRender();
}

RhythmVideo::RhythmVideo(QQuickItem *parent) : QQuickFramebufferObject(parent)
{
    // m_prhythmVideoGLES = new RhythmVideoGLES();
    // connect(this, &RhythmVideo::frameChanged, [=](){update();});
    // connect(this, &RhythmVideo::cloudFrameChanged, [=](){update();});

    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() };

    QSettings settings_var { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() + "-var" };

    delayFrameCount = settings.value("mainboard/delayFrameCount",0).toUInt();
    delayFrame.reserve(delayFrameCount);

    beamFPS = settings_var.value("soundcam/acousFps",100).toUInt();

    showCloudCycle = beamFPS / 50;

    qRegisterMetaType<std::map< int, unsigned char*>>("std::map< int, unsigned char*>");
    qRegisterMetaType<std::map<int,RhythmUI::picture>>("std::map<int,RhythmUI::picture>");
    qRegisterMetaType<RhythmUI::AcousticImageModes>("RhythmUI::AcousticImageModes");
    qRegisterMetaType<QVector<quint8>>("QVector<quint8>&");
    qRegisterMetaType<QVector<unsigned char>>("QVector<unsigned char>");
    qRegisterMetaType<QList<float>>("QList<float>");

    connect(this, &RhythmVideo::fullChanged, [&](){
        update();
    });

    m_picturMapIterator = m_map_pictures.begin();
    m_cloudPictureMapIterator = m_map_cloudPicture.begin();

    m_vec_pictureTemp.resize( m_ui_width * m_ui_height * 3 );
    m_vec_cloudPictureTemp.resize( m_ui_cloudWidth * m_ui_cloudHeight * 3 );

    // QThread* thread = new QThread();
    QThread* thread2 = new QThread();

    // beamThread = new VideoThread();
    videoRunThread = new VideoThread();
    //beamThread->setParams(m_e_filterModelType,m_f_beamMax,m_f_beamMin,m_f_beamLimitMax,m_f_peakFactor,m_f_dynamicRange,m_i_distance);
    // beamThread->moveToThread(thread);
    videoRunThread->moveToThread(thread2);

    /*
    connect( beamThread, &VideoThread::finished, thread, &QThread::quit);
    connect( beamThread, &VideoThread::finished, beamThread, &DataThread::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    */
    connect( videoRunThread, &VideoThread::finished, thread2, &QThread::quit);
    connect( videoRunThread, &VideoThread::finished, videoRunThread, &VideoThread::deleteLater);
    connect( thread2, &QThread::finished, thread2, &QThread::deleteLater);
    /*
    connect( this , &RhythmVideo::sendBeamData , beamThread , &VideoThread::setBeam );
    connect( beamThread , &VideoThread::sendBeam , this , &RhythmVideo::getBeamFromThread );
    connect( this , &RhythmVideo::setParams , beamThread , &VideoThread::setParams );
    */
    connect( this , &RhythmVideo::sendVideoData , videoRunThread , &VideoThread::setVideo );
    connect( videoRunThread , &VideoThread::sendVideo , this , &RhythmVideo::getVideoFromThread );

    // thread->start( QThread::NormalPriority );
    thread2->start( QThread::NormalPriority );

}

RhythmVideo::~RhythmVideo()
{

      if( videoThread != nullptr ){
          delete videoThread;
          videoThread = nullptr;
      }
      if( videoRunThread != nullptr ){
          delete videoRunThread;
          videoRunThread = nullptr;
      }
      /*
      if( beamThread != nullptr ){
          delete beamThread;
          beamThread = nullptr;
      }
      */

      resetRhythmVideo();
      qDebug() << "析构 video ~";
      // m_Thread = nullptr;
}

void RhythmVideo::changeTimeLine( int timeline , int currentFrame ){
    m_i_currentTime = timeline;
    m_i_currentFrame = currentFrame;
    std::map<int, RhythmUI::picture>::iterator beam_iter;

    // qDebug() << m_map_cloudPicture.size() << m_map_pictures.size() << m_map_camera.size() << "-------------";
    // beam_iter = m_map_cloudPicture.upper_bound( timeline);
    beam_iter = m_map_cloudPicture.begin();
    for( int i = 0; i < currentFrame; i++ ){
        beam_iter++;
    }

    m_i_currentTime = beam_iter->first;
    if(beam_iter != m_map_cloudPicture.end()){
        // setVideoBeam( m_ui_cloudWidth,m_ui_cloudHeight, m_map_cloudPicture[beam_iter->first].picturedata);
        generateCloud(m_ui_cloudWidth,m_ui_cloudHeight,m_map_cloudPicture[beam_iter->first].picturedata ,m_map_cloudPicture[beam_iter->first].min , m_map_cloudPicture[beam_iter->first].max );
    }

    if( !m_map_pictures.empty() ){
        std::map<const int, unsigned char*>::iterator pic_iter;
        pic_iter = m_map_pictures.upper_bound( m_i_currentTime );
        if( pic_iter != m_map_pictures.begin() && ( pic_iter->first - 1 > m_i_currentTime ) ){
            pic_iter--;
        }
        if(pic_iter != m_map_pictures.end() )
        {
            if( m_i_currentCloudTime != pic_iter->first ){
                m_i_currentCloudTime = pic_iter->first;
                set_ui_frame(m_map_pictures[pic_iter->first]);
            }
        }
    }

    if( !m_map_camera.isEmpty() ){
        QMap<int, QImage>::Iterator cam_iter;

        cam_iter = m_map_camera.upperBound( m_i_currentTime + ( delayFrameCount + 2 ) * 1000 / beamFPS  );
        if( cam_iter != m_map_camera.begin() && ( cam_iter.key() -1 > m_i_currentTime) ){
            cam_iter--;
        }
        if( cam_iter != m_map_camera.end() ){
            if( m_i_currentCloudTime != cam_iter.key() ){
                m_i_currentCloudTime = cam_iter.key();
                m_image = cam_iter.value();

                m_i_rgbMax = 0;
                m_i_rgbMin = 255;

                const uchar * bits = m_image.constBits();
                int byteSize = m_image.sizeInBytes();
                for( int i = 0; i < byteSize; i++ ){
                    if( m_i_rgbMax < (quint8)bits[i] ){
                        m_i_rgbMax = (quint8)bits[i];
                    }
                    if(m_i_rgbMin > (quint8)bits[i] ){
                        m_i_rgbMin = (quint8)bits[i];
                    }
                }
            }
        }
    }

    update();
}

void RhythmVideo::openFileFinish(){
    emit finish();
}

void RhythmVideo::addVideoData( QVariant data ){
    charData data_ = data.value<charData>();
    emit sendVideoData(m_ui_width , m_ui_height , data_.data);
    //update();
}

void RhythmVideo::addImageData( QVariant data ){

    byteArrayData data_ = data.value<byteArrayData>();
    m_ui_width = data_.attr.value("width");
    m_ui_height = data_.attr.value("height");
    bytesPerLine = data_.attr.value("bytesPerLine");

    m_i_rgbMax = data_.attr.value("max");
    m_i_rgbMin = data_.attr.value("min");

    QImage img_(reinterpret_cast<const uchar *>( data_.data.data()), m_ui_width , m_ui_height , bytesPerLine , QImage::Format_RGB888 );

    QRect rect(0,0,m_ui_width,m_ui_height);
    m_image = img_.copy(rect);
    m_image.convertTo(QImage::Format_RGB888);

    //update();
}

void RhythmVideo::addBeamData( QVariant data ){
    cloudCycle++;
    if( cloudCycle >= showCloudCycle ){
        cloudCycle = 0;
        floatData data_ = data.value<floatData>();

        //emit sendBeamData(m_ui_cloudWidth , m_ui_cloudHeight , data_.data);
        setVideoBeam(m_ui_cloudWidth,m_ui_cloudHeight,data_.data.constData() );
        update();
    }else{
        return;
    }
}

void RhythmVideo::setDataWithCamera( QVariant data ){
    CameraData t = data.value<CameraData>();
    m_time_len = t.m_time_len;
    m_i_distance = t.m_distance;

    setVideoBeam(m_ui_cloudWidth, m_ui_cloudHeight, t._beam_map);

    QImage img_ = QImage::fromData(t._camera_map.begin().value());

    QRect rect(0,0,img_.width(),img_.height());
    m_image = img_.copy(rect);
    m_image.convertTo(QImage::Format_RGB888);

    QList<quint32> timeList;
    std::map<int,float*>::iterator iter = t._beam_map.begin();
    while( iter != t._beam_map.end() ){
        timeList.append( iter->first );
        iter++;
    }

    emit distanceChanged( m_i_distance );
    emit beamCountChanged( t._beam_map.size() , m_time_len , timeList );

    m_ui_width = m_image.width();
    m_ui_height = m_image.height();
    bytesPerLine = m_image.bytesPerLine();

    // 调用子线程
    if( videoThread != nullptr ){
        delete videoThread;
    }
    videoThread = new VideoThread();
    connect( videoThread , &VideoThread::sendCameraData , this , &RhythmVideo::getCameraData );
    connect( videoThread , &VideoThread::sendTimeAxis , this , &RhythmVideo::getTimeAxis );

    videoThread->setCameraData(m_ui_width, m_ui_height, bytesPerLine, t._camera_map,m_ui_cloudWidth, m_ui_cloudHeight, t._beam_map);
    videoThread->setTimeAxis( m_timeAxis );
    videoThread->start( QThread::NormalPriority );

    // update();
}

void RhythmVideo::setData( QVariant data ){
    VideoData t = data.value<VideoData>();

    m_time_len = t.m_time_len;
    m_i_distance = t.m_distance;
    setVideoData(t.width_picture_map, t.height_picture_map, t._picture_map);
    setVideoBeam(m_ui_cloudWidth, m_ui_cloudHeight, t._beam_map);

    QList<quint32> timeList;

    std::map<int,float*>::iterator iter = t._beam_map.begin();
    while( iter != t._beam_map.end() ){
        timeList.append( iter->first );
        iter++;
    }

    emit distanceChanged( m_i_distance );
    emit beamCountChanged( t._beam_map.size() , m_time_len , timeList );

    // 调用子线程
    if( videoThread != nullptr ){
        delete videoThread;
    }
    videoThread = new VideoThread();
    connect( videoThread , &VideoThread::sendVideoData , this , &RhythmVideo::getVideoData );
    connect( videoThread , &VideoThread::sendTimeAxis , this , &RhythmVideo::getTimeAxis );

    videoThread->setVideoData(t.width_picture_map, t.height_picture_map, t._picture_map,m_ui_cloudWidth, m_ui_cloudHeight, t._beam_map);
    videoThread->setTimeAxis( m_timeAxis );
    videoThread->start( QThread::NormalPriority );

    // update();
}

void RhythmVideo::getFinalCameraData( QVariant camera_ ){
    resetRhythmVideo();
    cameraFinalData c = camera_.value<cameraFinalData>();

    m_time_len = c.time_len;

    m_ui_videoSize = c.picture.size();
    m_ui_videoBeamSize = c.beam.size();
    m_vec_cloudPictureTemp.resize( m_ui_cloudWidth * m_ui_cloudHeight * 3);
    // m_vec_pictureTemp.resize( m_ui_width * m_ui_height * 3);

    QImage img_ = QImage( reinterpret_cast<uchar *>( c.picture.begin().value().data() ), m_ui_width ,m_ui_height, bytesPerLine , QImage::Format_RGB888 );
    QRect rect(0,0,img_.width(),img_.height());
    m_image = img_.copy(rect);
    m_image.convertTo(QImage::Format_RGB888);

    QMap<quint64,QVector<float>>::Iterator beamIter = c.beam.begin();
    QList<quint32> timeList;
    while( beamIter != c.beam.end() ){
        timeList.append(beamIter.key());
        beamIter++;
    }
    emit beamCountChanged( c.beam.size() , m_time_len , timeList );

    if( videoThread != nullptr ){
        delete videoThread;
    }
    videoThread = new VideoThread();
    connect( videoThread , &VideoThread::sendCameraData , this , &RhythmVideo::getCameraData );
    connect( videoThread , &VideoThread::sendTimeAxis , this , &RhythmVideo::getTimeAxis );

    videoThread->setFinalCameraData(m_ui_width,m_ui_height,bytesPerLine,c.picture,m_ui_cloudWidth,m_ui_cloudHeight,c.beam);
    videoThread->setTimeAxis( m_timeAxis );
    videoThread->start( QThread::NormalPriority );
}

void RhythmVideo::getFinalData( QVariant video_ ){
    resetRhythmVideo();

    videoFinalData v = video_.value<videoFinalData>();
    m_time_len = v.time_len;

    m_ui_videoSize = v.picture.size();
    m_ui_videoBeamSize = v.beam.size();
    m_vec_cloudPictureTemp.resize( m_ui_cloudWidth * m_ui_cloudHeight * 3);
    m_vec_pictureTemp.resize( m_ui_width * m_ui_height * 3);
    // setVideoData(m_ui_width, m_ui_height, v._picture_map);
    // setVideoBeam(m_ui_cloudWidth, m_ui_cloudHeight, v._beam_map);

    QMap<quint64,QVector<float>>::Iterator beamIter = v.beam.begin();
    QList<quint32> timeList;
    while( beamIter != v.beam.end() ){
        timeList.append(beamIter.key());
        beamIter++;
    }

    emit beamCountChanged( v.beam.size() , m_time_len , timeList );

    if( videoThread != nullptr ){
        delete videoThread;
    }
    videoThread = new VideoThread();
    connect( videoThread , &VideoThread::sendVideoData , this , &RhythmVideo::getVideoData );
    connect( videoThread , &VideoThread::sendTimeAxis , this , &RhythmVideo::getTimeAxis );

    videoThread->setFinalVideoData(m_ui_width,m_ui_height,v.picture,m_ui_cloudWidth,m_ui_cloudHeight,v.beam);
    videoThread->setTimeAxis( m_timeAxis );
    videoThread->start( QThread::NormalPriority );
}

void RhythmVideo::getCameraData( QMap<int, QImage> camera_, std::map<int , RhythmUI::picture> beam_ ){
    m_map_camera = camera_;
    m_map_cloudPicture = beam_;
    m_cloudPictureMapIterator = m_map_cloudPicture.end();
    m_cloudPictureMapIterator--;
    if ( m_map_cloudPicture.size() > 0 )
        setVideoBeam( m_ui_cloudWidth, m_ui_cloudHeight, m_cloudPictureMapIterator->second.picturedata );
        //generateCloud(m_ui_cloudWidth, m_ui_cloudHeight, m_cloudPictureMapIterator->second.picturedata, m_cloudPictureMapIterator->second.min, m_cloudPictureMapIterator->second.max);

    videoThread->quit();
    delete videoThread;
    videoThread = nullptr;
    update();

    emit finish();
}

void RhythmVideo::getVideoData( std::map< int, unsigned char*> picture_ , std::map<int , RhythmUI::picture> beam_ ){

    m_map_pictures = picture_;
    m_picturMapIterator = m_map_pictures.end();
    m_picturMapIterator--;
    if ( m_map_pictures.size() > 0 )
        set_ui_frame( m_picturMapIterator->second );

    m_map_cloudPicture = beam_;
    m_cloudPictureMapIterator = m_map_cloudPicture.end();
    m_cloudPictureMapIterator--;
    if ( m_map_cloudPicture.size() > 0 )
        setVideoBeam( m_ui_cloudWidth, m_ui_cloudHeight, m_cloudPictureMapIterator->second.picturedata );
        //generateCloud(m_ui_cloudWidth, m_ui_cloudHeight, m_cloudPictureMapIterator->second.picturedata, m_cloudPictureMapIterator->second.min, m_cloudPictureMapIterator->second.max);

    videoThread->quit();
    delete videoThread;
    videoThread = nullptr;

    update();

    emit finish();
}

void RhythmVideo::play(){
    if( timer != 0 ){
        killTimer(timer);
        timer = 0;
    }
    timer = startTimer( m_time_len / m_ui_videoSize );
}

void RhythmVideo::clearData(){

}

void RhythmVideo::resetRhythmVideo()
{
    // clear all the data
    m_picturMapIterator = m_map_pictures.begin();
    while ( m_picturMapIterator != m_map_pictures.end() ) {
        free(m_picturMapIterator->second);
        m_picturMapIterator++;
    }
    m_map_pictures.clear();
    m_picturMapIterator = m_map_pictures.begin();

    // clear all the old data
    m_cloudPictureMapIterator = m_map_cloudPicture.begin();
    while ( m_cloudPictureMapIterator != m_map_cloudPicture.end() ) {
        free( m_cloudPictureMapIterator->second.picturedata );
        m_cloudPictureMapIterator++;
    }

    m_map_camera.clear();

    m_cloudPictureMapIterator = m_map_cloudPicture.begin();
    m_map_cloudPicture.clear();

    m_ui_videoSize = 0;
    m_ui_videoBeamSize = 0;

    m_vec_pictureTemp.clear();
    m_vec_cloudPictureTemp.clear();

    m_frame = nullptr;
    m_cloudFrame = nullptr;


#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif
}

// 新版本色谱获取
RhythmVideo::color RhythmVideo::generateColor( float value ){
    value = qBound(m_f_beamMin , value , m_f_beamMax);

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
            b = 128.0 * (1.0 + v / _8th);
        } else if (v < 3.0 * _8th) {
            b = 255;
            g = 128.0 * (v - _8th) / _8th;
        } else if (v < 5.0 * _8th) {
            r = 128.0 * (v - 3.0 * _8th) / _8th;
            g = 255;
            b = 128.0 * (5.0 * _8th - v) / _8th;
        } else if (v < 7.0 * _8th) {
            r = 255;
            g = 128.0 * (7.0 * _8th - v) / _8th;
        } else {
            r = 128.0 * (9.0 * _8th - v) / _8th;
        }
    }
    return color(r, g, b);
}

/*
 * 老版本色谱获取
RhythmVideo::color RhythmVideo::generateColor(const float &value) const
{
    static const float float_epsilon { std::numeric_limits<float>::epsilon() };
    float division      = (m_f_beamMax - m_f_beamMin)/4.0f;            // first division
    float divisionone   = m_f_beamMin*0.25f + m_f_beamMax*0.75f;       // second division
    float divisiontwo   = (m_f_beamMin + m_f_beamMax)*0.5f;            // third division
    float divisionthree = m_f_beamMin*0.75f + m_f_beamMax*0.25f;       // forth division

    if( value >= m_f_beamMax) // top splite
        return color(255, 0, 0);
    else if( divisionone < value && value < m_f_beamMax)
        return color(255, static_cast<int>((m_f_beamMax - value)/division * 255.0f), 0);
    else if( fabsf(value - divisionone) <= float_epsilon)  // second splite
        return color(255, 255, 0);
    else if( divisiontwo < value && value < divisionone)
        return color(static_cast<int>((value - divisiontwo)/division*255), 255, 0);
    else if( fabsf(value - divisiontwo) <= float_epsilon ) // third splite
        return color(0, 255, 0);
    else if( divisionthree < value && value < divisiontwo)
        return color( 0, static_cast<int>((value-divisionthree)/division*255), static_cast<int>((divisiontwo - value)/division*255));
    else if( fabsf(value - divisionthree) <= float_epsilon )  // fourth splite
        return color(0, 0, 255);
    else if( m_f_beamMin < value && value< divisionthree)
        return color( 0, 0, static_cast<int>((value - m_f_beamMin)/division * 255.0f));
    else if ( value <= m_f_beamMin ) // last splite
        return color(0, 0, 0);

    return color(255, 255, 255);
}
*/

void RhythmVideo::generateCloud(const unsigned int& width_, const unsigned int& height_, const float* data_, const float& min_, const float& max_)
{
    Q_UNUSED( min_ );
    m_f_beamMax = max_;
    m_f_beamMin = qMax((float)0 , max_ - m_f_dynamicRange);
    int _i = 0;
    int _count = static_cast<int>(width_) * static_cast<int>(height_);
    while ( _i < _count ) {
        const color& _color = generateColor(data_[_i]);  // generate the colors
        int _pos = _i*3;
        m_vec_cloudPictureTemp[_pos]   = static_cast<unsigned char>(_color.red);
        m_vec_cloudPictureTemp[_pos+1] = static_cast<unsigned char>(_color.green);
        m_vec_cloudPictureTemp[_pos+2] = static_cast<unsigned char>(_color.blue);
        _i += 1;
    }

    // 声学成像延时
    if( delayFrameCount > 0 && isRun ){
        delayFrame.append( m_vec_cloudPictureTemp );
        if( delayFrame.size() > delayFrameCount ){
            delayFrame.removeFirst();
        }
        set_ui_cloudFrame(&delayFrame[0][0]);
    }else{
        set_ui_cloudFrame(&m_vec_cloudPictureTemp[0]);
    }
}

void RhythmVideo::getTimeAxis( QList<float> beamMaxList_ ){
    beamMaxList = beamMaxList_;
    emit timeAxisChanged( beamMaxList );
}

void RhythmVideo::getBeamFromThread( QVector<unsigned char> cloudPicture_ , float &beamMax_ , float &beamMin_ ){
    // m_vec_cloudPictureTemp = cloudPicture_;
    m_f_beamMax = beamMax_;
    m_f_beamMin = beamMin_;
    // QVector<unsigned char> data_(cloudPicture_);

    m_vec_cloudPictureTemp.fill(0);
    m_vec_cloudPictureTemp.swap(cloudPicture_);
    set_ui_cloudFrame(m_vec_cloudPictureTemp.data());

    //set_ui_cloudFrame( m_vec_cloudPictureTemp.data() );
    update();
}

void RhythmVideo::getVideoFromThread( QVector<unsigned char> picture_ ){
    //m_vec_pictureTemp = picture_;
   //  QVector<unsigned char> data_(picture_);
    // data_.swap(picture_);
    m_vec_pictureTemp.fill(0);
    m_vec_pictureTemp.swap(picture_);
    set_ui_frame( m_vec_pictureTemp.data() );
   // update();
}

void RhythmVideo::setImageMode(const int& mode)
{
    m_e_filterModelType = static_cast< RhythmUI::AcousticImageModes >(mode);
    switch ( m_e_filterModelType ) {
    case RhythmUI::AcousticImageModes::ImageModeAuto:
       // m_pinfoWidget->setDetectionModel(QStringLiteral("自动模式"));
        break;
    case RhythmUI::AcousticImageModes::ImageModeFixedThreshold:
      //  m_pinfoWidget->setDetectionModel(QStringLiteral("阈值模式"));
        break;
    case RhythmUI::AcousticImageModes::ImageModeAverageThreshold:
      //  m_pinfoWidget->setDetectionModel(QStringLiteral("去噪模式"));
        break;
    case RhythmUI::AcousticImageModes::ImageModeTransient:
      //  m_pinfoWidget->setDetectionModel(QStringLiteral("瞬态模式"));
        break;
    }

    // emit setParams(m_e_filterModelType,m_f_beamMax,m_f_beamMin,m_f_beamLimitMax,m_f_peakFactor,m_f_dynamicRange,m_i_distance);

}

void RhythmVideo::setDynamicRange(const float& range_)
{
    m_f_dynamicRange = range_;
    // m_prhythemVideoGradientBar->setRange(m_f_dynamicRange);
    // emit setParams(m_e_filterModelType,m_f_beamMax,m_f_beamMin,m_f_beamLimitMax,m_f_peakFactor,m_f_dynamicRange,m_i_distance);

}

void RhythmVideo::setPeakFactor(const float& value_)
{
    m_f_peakFactor = value_;
    // emit setParams(m_e_filterModelType,m_f_beamMax,m_f_beamMin,m_f_beamLimitMax,m_f_peakFactor,m_f_dynamicRange,m_i_distance);

}

void RhythmVideo::setMaxLimiteValue(const float &value_)
{
    m_f_beamLimitMax = value_;
    // m_prhythemVideoGradientBar->setMaxdB(m_f_beamLimitMax);
    // emit setParams(m_e_filterModelType,m_f_beamMax,m_f_beamMin,m_f_beamLimitMax,m_f_peakFactor,m_f_dynamicRange,m_i_distance);

}

void RhythmVideo::setBeamConfig( const int& mode , const float& dynamicRange , const float& peak , const float& maxLimite ){

    setImageMode( mode );
    setDynamicRange( dynamicRange );
    setPeakFactor( peak );
    setMaxLimiteValue( maxLimite );

    emit setParams(m_e_filterModelType,m_f_beamMax,m_f_beamMin,m_f_beamLimitMax,m_f_peakFactor,m_f_dynamicRange,m_i_distance);

    if( !isRun ){
        changeTimeLine( m_i_currentTime , m_i_currentFrame );
    }
}

void RhythmVideo::setVideoData(const unsigned int &width_, const unsigned int &height_, const unsigned char* const picture_)
{
    int _count = static_cast<int>(width_) * static_cast<int>(height_) * 3;
    if ( m_ui_width != width_ || m_ui_height != height_ ) {
        m_ui_width = width_;
        m_ui_height = height_;
        m_vec_pictureTemp.resize(_count);
    }

   // emit sendVideoData( width_ , height_ , picture_ );
    /*
    int _size = static_cast<int>(width_) * static_cast<int>(height_);
    for ( int _i = 0; _i < _size; _i++ ) {
        int _position = _i*3;
        m_vec_pictureTemp[_position] = picture_[_i];
        m_vec_pictureTemp[_position+1] = picture_[_i];
        m_vec_pictureTemp[_position+2] = picture_[_i];
    }

    if ( m_vec_pictureTemp.size() == _count )
        // m_prhythmVideoGLES->setFrame(m_ui_width, m_ui_height, &m_vec_pictureTemp[0]);
        // emit setFrame( m_ui_width, m_ui_height, &m_vec_pictureTemp[0] ) ;

        set_ui_frame( &m_vec_pictureTemp[0] );

    //m_prhythmVideoGLES->repaint();
    // update();
    */
}

void RhythmVideo::setVideoBeam(const unsigned int &width_, const unsigned int &height_, const float* const beam_)
{
    if ( m_ui_cloudWidth != width_ || m_ui_cloudHeight != height_ ) {
        m_vec_cloudPictureTemp.resize( static_cast<int>(width_) * static_cast<int>(height_) * 3);
        m_ui_cloudWidth = width_;
        m_ui_cloudHeight = height_;
    }

    // emit sendBeamData( width_ , height_ , beam_ );

    if ( m_e_filterModelType == RhythmUI::AcousticImageModes::ImageModeAuto ) {
        // 自动模式
        // initialize the min data and the max data
        float _min = beam_[0];
        float _max = beam_[0];
        // copy all the data
        size_t _size = width_*height_;
        size_t _i = 0;
        while ( _i < _size ) {
            _min = qMin(_min , beam_[_i]);
            _max = qMax( beam_[_i] , _max );
            _i ++;
        }
        generateCloud(width_, height_, beam_, _min, _max);
    } else if ( m_e_filterModelType == RhythmUI::AcousticImageModes::ImageModeFixedThreshold ) {
        // 阈值模式
        generateCloud(width_, height_, beam_, 0, m_f_beamLimitMax);
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
            _min = qMin( _min , beam_[_i] );
            _max = qMax( beam_[_i], _max);
            _sum += beam_[_i];
            _i ++;
        }
        float _ave = _sum/_count + m_f_peakFactor;
        _max = qMax( _ave , _max );

        generateCloud(width_, height_, beam_, _min, _max);
    } else if ( m_e_filterModelType == RhythmUI::AcousticImageModes::ImageModeTransient ) {
    }
    //m_prhythmVideoGLES->repaint();
    // update(); 
}

void RhythmVideo::setCameraData(const QByteArray& camera_){

}

void RhythmVideo::setCameraData(const QList<QByteArray>& camera_){

}

void RhythmVideo::setVideoData(const unsigned int& width_, const unsigned int& height_, const std::map<int, unsigned char *> &pictures_)
{
    m_ui_videoSize = pictures_.size();
    if ( m_ui_width != width_ || m_ui_height != height_ ) {
        m_ui_width = width_;
        m_ui_height = height_;
    }

    // clear all the data
    m_picturMapIterator = m_map_pictures.begin();
    while ( m_picturMapIterator != m_map_pictures.end() ) {
        free(m_picturMapIterator->second);
        m_picturMapIterator++;
    }
    m_map_pictures.clear();

    // copy all the data
    /*
    std::map<int, unsigned char *>::const_iterator _constItor = pictures_.begin();
    while ( _constItor != pictures_.end() ) {
        const size_t _count = width_ * height_ ;
        unsigned char* _data = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * _count* 3));
        for ( size_t _i = 0; _i < _count; _i++ ) {
            _data[_i* 3] = _constItor->second[_i];
            _data[_i* 3+1] = _constItor->second[_i];
            _data[_i* 3+2] = _constItor->second[_i];
        }
        m_map_pictures.insert(std::pair<int, unsigned char*>( _constItor->first, _data ));
        _constItor++;
    }

    m_picturMapIterator = m_map_pictures.begin();

    if ( m_map_pictures.size() > 0 ){}
        // emit setFrame( m_ui_width, m_ui_height, &m_vec_pictureTemp[0] ) ;
        set_ui_frame( m_picturMapIterator->second );
        //m_prhythmVideoGLES->setFrame(m_ui_width, m_ui_height, m_picturMapIterator->second);
        //m_prhythmVideoGLES->repaint();
 //   update();
    */
}

void RhythmVideo::setVideoBeam(const unsigned int &width_, const unsigned int &height_, const std::map<int, float*> &beam_)
{
    if ( m_ui_cloudWidth != width_ || m_ui_cloudHeight != height_ ) {
        m_ui_cloudWidth = width_;
        m_ui_cloudHeight = height_;
        m_vec_cloudPictureTemp.resize( m_ui_cloudWidth * m_ui_cloudHeight * 3);
    }

    m_ui_videoBeamSize = beam_.size();
    // clear all the old data
    m_cloudPictureMapIterator = m_map_cloudPicture.begin();
    while ( m_cloudPictureMapIterator != m_map_cloudPicture.end() ) {
        free( m_cloudPictureMapIterator->second.picturedata );
        m_cloudPictureMapIterator++;
    }
    m_map_cloudPicture.clear();

    // copy all the data
    /*
    size_t _count = width_*height_;
    std::map<int, float*>::const_iterator _beamItor = beam_.begin();
    while ( _beamItor != beam_.end() ) {
        picture _picture;
        _picture.min = _beamItor->second[0]; // initialize min value
        _picture.max = _beamItor->second[0]; // initialize max value
        _picture.picturedata = static_cast<float*>(malloc(sizeof (float)* _count));
        float _sum = 0.0f;
        size_t _i = 0;
        while ( _i < _count ) {
            _picture.min = _beamItor->second[_i] < _picture.min ? _beamItor->second[_i] : _picture.min;
            _picture.max = _beamItor->second[_i] > _picture.max ? _beamItor->second[_i] : _picture.max;
            _picture.picturedata[_i] = _beamItor->second[_i];
            _sum += _beamItor->second[_i];
            _i ++;
        }
        _picture.ave = _sum/static_cast<float>(_count);
        m_map_cloudPicture[_beamItor->first] = _picture;
        _beamItor++;
    }

    m_cloudPictureMapIterator = m_map_cloudPicture.begin();
    if ( m_map_cloudPicture.size() > 0 )
        generateCloud(width_, height_, m_cloudPictureMapIterator->second.picturedata, m_cloudPictureMapIterator->second.min, m_cloudPictureMapIterator->second.max);
   // m_prhythmVideoGLES->repaint();
   // update();
    */
}

void RhythmVideo::seekBeamIndex(const int& position_)
{
    float _position = static_cast<float>(position_ % m_ui_videoBeamSize) / static_cast<float>(m_ui_videoBeamSize);
    seekPercentage(_position);
}

void RhythmVideo::seekPercentage(const float& position_)
{
    int _videoPosition = static_cast<int>(position_ * m_ui_videoSize - 0.49999f);
    int _videoBeamPosition = static_cast<int>(position_ * m_ui_videoBeamSize - 0.49999f);

    std::map< int, unsigned char*>::iterator _pictureMapIterator = m_map_pictures.begin();
    std::map< int, RhythmUI::picture>::iterator _pictureClodMapIterator = m_map_cloudPicture.begin();
    std::advance(_pictureMapIterator, _videoPosition);
    std::advance(_pictureClodMapIterator, _videoBeamPosition);

    if ( _pictureMapIterator != m_map_pictures.end() && _pictureMapIterator != m_picturMapIterator ) {
        m_picturMapIterator = _pictureMapIterator;

        set_ui_frame( m_picturMapIterator->second);
    }

    if ( _pictureClodMapIterator != m_map_cloudPicture.end() && _pictureClodMapIterator != m_cloudPictureMapIterator ) {
        m_cloudPictureMapIterator = _pictureClodMapIterator;
        generateCloud(m_ui_cloudWidth, m_ui_cloudHeight, m_cloudPictureMapIterator->second.picturedata, m_cloudPictureMapIterator->second.min, m_cloudPictureMapIterator->second.max);
    }

}

void RhythmVideo::timerEvent(QTimerEvent* event_)
{
    Q_UNUSED( event_ );

    if ( m_picturMapIterator != m_map_pictures.end() ) {
        set_ui_frame(m_picturMapIterator->second);

        m_picturMapIterator++;
    } else if ( m_map_pictures.size() > 0 ) {
        m_picturMapIterator = m_map_pictures.begin();
    } else {
        m_picturMapIterator = m_map_pictures.end(); //
    }

    if ( m_cloudPictureMapIterator != m_map_cloudPicture.end() ) {
        generateCloud(m_ui_cloudWidth, m_ui_cloudHeight, m_cloudPictureMapIterator->second.picturedata, m_cloudPictureMapIterator->second.min, m_cloudPictureMapIterator->second.max);
        m_cloudPictureMapIterator++;
    } else if ( m_map_cloudPicture.size() > 0 ) {
        m_cloudPictureMapIterator = m_map_cloudPicture.begin();
    } else {
        m_cloudPictureMapIterator = m_map_cloudPicture.end(); //
    }

    update();
}

/*
void RhythmVideo::resizeEvent(QResizeEvent* event)
{
    double height = event->size().height();
    double _currentRatio = static_cast<double>(m_ui_width) / static_cast<double>(m_ui_height);
    QWidget::resize(static_cast<int>(height*_currentRatio)+230, static_cast<int>(height));

    m_prhythmVideoGLES->setGeometry(0, 0, static_cast<int>(height*_currentRatio), static_cast<int>(height));
    m_prhythemVideoGradientBar->setGeometry(static_cast<int>(height*_currentRatio)+10, 0, 100, static_cast<int>(height));
    //
    m_pinfoWidget->setGeometry(static_cast<int>(height*_currentRatio)+100, 0, 150, static_cast<int>(height));

    QWidget::resizeEvent(event);
}
*/
