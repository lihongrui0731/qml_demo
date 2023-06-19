/** ********************************************************************************************
 *  @brief
 * 这个类主要用来绘制视频的，通过设置云图数据和视频数据，进行视频播放；
 *
 *
 *  @author guweicao                                                              @date 2021/5/5
 *  ********************************************************************************************/
#ifndef RUISENVIDEO_H
#define RUISENVIDEO_H

#include <QWidget>
#include <QTimerEvent>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QColor>
#include <QWindow>
#include <QFuture>
#include <QElapsedTimer>

#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QThread>
#include <QMetaType>
#include <QImage>

#include <QVariant>
#include <QVariantMap>
#include <QList>
#include <QSettings>
#include <QCoreApplication>

#include "tools_shared.h"
// #include "report_frame.h"

#include "dataType.h"
#include "VideoThread.h"

class RhythmVideoGLES;
class RhythmGradientBar;

///声学成像合成画面控件
class RhythmVideo : public QQuickFramebufferObject{
    Q_OBJECT
    Q_PROPERTY(bool full READ full WRITE setFull NOTIFY fullChanged)
    Q_PROPERTY(bool nightVersion READ nightVersion WRITE setNightVersion NOTIFY nightVersionChanged)
    Q_PROPERTY(quint8 nightMethod READ nightMethod WRITE setNightMethod NOTIFY nightMethodChanged)

    Q_PROPERTY(float brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(float contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(float saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(float highLight READ highLight WRITE setHighLight NOTIFY highLightChanged)
    Q_PROPERTY(float shadow READ shadow WRITE setShadow NOTIFY shadowChanged)
    Q_PROPERTY(float midtone READ midtone WRITE setMidtone NOTIFY midtoneChanged)
    Q_PROPERTY(float coefficient READ coefficient WRITE setCoefficient NOTIFY coefficientChanged)

    Q_PROPERTY(bool timeAxis READ timeAxis WRITE setTimeAxis NOTIFY timeAxisChanged)

    struct color{
        explicit color(int red_, int green_, int blue_):
            red(red_), green(green_), blue(blue_)
        {}
        int red{255};
        int green{255};
        int blue{255};
    };

public:
    RhythmVideo(QQuickItem *parent = 0);
    virtual ~RhythmVideo() override;

    unsigned int get_ui_width() const {return m_ui_width;}
    unsigned int get_ui_height() const {return m_ui_height;}
    unsigned int get_ui_cloudWidth() const {return m_ui_cloudWidth;}
    unsigned int get_ui_cloudHeight() const {return m_ui_cloudHeight;}
    unsigned char * get_ui_frame() const { return m_frame;}
    quint8 get_rgbMax() const{ return m_i_rgbMax; }
    quint8 get_rgbMin() const{ return m_i_rgbMin; }

    QImage get_ui_image() const { return m_image; }
    unsigned char * get_ui_cloudFrame() const { return m_cloudFrame;}

    bool getStatus() const { return isRun; }

    void set_ui_frame( unsigned char *v ) {
        m_frame = v; /* frameChanged(); */
    }

    void set_ui_cameraFrame( QImage& image ){
        m_image = image;
    }

    void set_ui_cloudFrame( unsigned char *v ) {
        m_cloudFrame = v;  emit cloudFrameChanged();
    }

    void setNightVersion( bool nightVersion_ ){
        m_nightVersion = nightVersion_;
    }

    void setNightMethod( quint8 nightMethod_ ){
        m_method = nightMethod_;
    }

    void setBrightness( float brightness_ ){
        m_brightness = brightness_;
    }
    void setContrast( float contrast_ ){
        m_contrast = contrast_;
    }
    void setSaturation( float saturation_ ){
        m_saturation = saturation_;
    }
    void setHighLight( float highLight_ ){
        m_highLight = highLight_;
    }
    void setShadow( float shadow_ ){
        m_shadow = shadow_;
    }
    void setMidtone( float midtone_ ){
        m_midtone = midtone_;
    }
    void setCoefficient( float coefficient_ ){
        m_coefficient = coefficient_;
    }
    void setTimeAxis( bool isTimeAxis ){
        m_timeAxis = isTimeAxis;

    }

    bool timeAxis(){
        return m_timeAxis;
    }

    float brightness(){
        return m_brightness;
    }
    float contrast(){
        return m_contrast;
    }
    float saturation(){
        return m_saturation;
    }
    float highLight(){
        return m_highLight;
    }
    float shadow(){
        return m_shadow;
    }
    float midtone(){
        return m_midtone;
    }
    float coefficient(){
        return m_coefficient;
    }

    bool nightVersion(){
        return m_nightVersion;
    }

    quint8 nightMethod(){
        return m_method;
    }

    Q_INVOKABLE float getMax(){
        return m_f_beamMax;
    }

    Q_INVOKABLE float getMin(){
        return m_f_beamMin;
    }

    Q_INVOKABLE float getDynamicRange(){
        return m_f_dynamicRange;
    }

    Q_INVOKABLE void changeTimeLine( int timeline , int currentFrame );
    Q_INVOKABLE void setData( QVariant data );
    Q_INVOKABLE void setDataWithCamera( QVariant data );

    Q_INVOKABLE void addVideoData( QVariant data );
    Q_INVOKABLE void addImageData( QVariant data );
    Q_INVOKABLE void addBeamData( QVariant data );

    Q_INVOKABLE void getFinalData( QVariant video_ );
    Q_INVOKABLE void getFinalCameraData( QVariant camera_ );

    Q_INVOKABLE void clearData();

    /**
     * @brief 测量距离(单位mm)
     * @param distance
     * @todo 仅在info组件显示
     */
     void setMeasureDistance(const int distance);

    /**
     * @brief 频率范围上限
     * @param max
     * @todo 仅在info组件显示
     */
     void setMaxFrequence(const int max);

    /**
     * @brief 频率范围下限
     * @param min
     * @todo 仅在info组件显示
     */
     void setMinFrequence(const int min);

    /**
     * @brief 清除内容，复位状态
     */
    Q_INVOKABLE void resetRhythmVideo();

     void setCameraData(const QByteArray &camera);

     void setCameraData(const QList<QByteArray> &camera);

    /**
     * @brief 传入画面序列
     * @param width_    set picture width ( pixels )
     * @param height_   set picture height ( pixels )
     * @param pictures_ set picture data map ( all the pictures data in the map)
     */
    void setVideoData(const unsigned int &width_, const unsigned int &height_, const std::map<int, unsigned char *> &pictures_);

    /**
     * @brief 绘制单幅画面
     * @param width_        @brief set the picture width ( pixels)
     * @param height_       @brief set the picture height (pixels)
     * @param picture_      @brief set the capture data pointer (pixels)
     */
    void setVideoData(const unsigned int &width_, const unsigned int &height_, const unsigned char* const picture_);

    /**
     * @brief 传入声成像画面序列
     * @param width_        @brief set picture width ( pixels )
     * @param height_       @brief set picture height ( pixels )
     * @param beam_         @brief set picture data map ( all the pictures data in the map)
     */
    void setVideoBeam(const unsigned int &width_, const unsigned int &height_, const std::map<int, float*> &beam_);

    /**
     * @brief 绘制单幅声成像画面
     * @param width_        @brief set the beam width ( pixels )
     * @param height_       @brief set the bemm height ( pixels )
     * @param beam_         @brief set the beam data ( pixels )
     */
    void setVideoBeam(const unsigned int &width_, const unsigned int &height_, const float* const beam_);

    /**
     * @brief 绘制单幅声成像画面 (指定最大值、最小值)
     * @param width_         @brief set beam data with
     * @param height_        @brief set beam data height
     * @param beam_          @brief set beam data
     * @param min_           @brief set beam min value
     * @param max_           @brief set beam max value
     */
    void setVideoBeam(const unsigned int &width_, const unsigned int &height_, const float* const beam_, const float& min_, const float& max_);

    /** ********************************************************************************************************************************
     * @brief setFilterModelType
     * @param type_               @brief set the filter model type
     **********************************************************************************************************************************/

    // Q_INVOKABLE void setImageMode(const RhythmUI::AcousticImageModes& mode);
    Q_INVOKABLE void setImageMode(const int& mode);

    /** *******************************************************************************************************************************
     * @brief setDynamicRange  @brief
     * @param range_           @brief set the dynamic range value data
     **********************************************************************************************************************************/
    Q_INVOKABLE void setDynamicRange(const float& range_);

    /** *******************************************************************************************************************************
     * @brief setPeakFactor
     * @param value_           @brief set peak factor
     **********************************************************************************************************************************/
    Q_INVOKABLE void setPeakFactor(const float& value_);

    /** *******************************************************************************************************************************
     * @brief setMaxValue
     * @param value_           @brief set the maxLimite value
     **********************************************************************************************************************************/
    Q_INVOKABLE void setMaxLimiteValue(const float& value_);

    Q_INVOKABLE void setBeamConfig( const int& mode , const float& dynamicRange , const float& peak , const float& maxLimite );

    Q_INVOKABLE void start(){
        resetRhythmVideo();

        // m_picturMapIterator = m_map_pictures.begin();
        // m_cloudPictureMapIterator = m_map_cloudPicture.begin();

        m_vec_pictureTemp.resize( m_ui_width * m_ui_height * 3 );
        m_vec_cloudPictureTemp.resize( m_ui_cloudWidth * m_ui_cloudHeight * 3 );

        isRun = true;
    }
    Q_INVOKABLE void stop(){
        isRun = false;
        if( timer != 0 ){
            killTimer(timer);
            timer = 0;
        }
    }
    Q_INVOKABLE void play();

public slots:

    /**
     * @brief seek 到指定的位置
     * @param position_ ?毫秒时间戳
     */
    void seekBeamIndex(const int& position_);

    /**
     * @brief seek 到指定的位置
     * @param position_ (float) 0~1
     */
    void seekPercentage(const float& position_);

    void openFileFinish();
    void getVideoData( std::map<int, unsigned char*> picture_, std::map<int , RhythmUI::picture> beam_ );
    void getCameraData( QMap<int, QImage> camera_, std::map<int , RhythmUI::picture> beam_ );

    void getVideoFromThread( QVector<unsigned char> picture_ );
    void getBeamFromThread( QVector<unsigned char> cloudPicture_ , float &beamMax_ , float &beamMin_ );

    void getTimeAxis( QList<float> beamMaxList );

protected:

    /// @todo 确认无用后删除
    void timerEvent(QTimerEvent*) override;

    int timer{0};

    Renderer * createRenderer() const override;
    bool full() const {return m_full;}
    void setFull(bool v)
    {
        if (v == m_full) return ;
        m_full = v;
        emit fullChanged();
    }
    // void resizeEvent(QResizeEvent*) override;

signals:
    void fullChanged();
    void nightVersionChanged();
    void nightMethodChanged();
    void frameChanged();
    void cloudFrameChanged();

    void brightnessChanged();
    void contrastChanged();
    void saturationChanged();
    void highLightChanged();
    void shadowChanged();
    void midtoneChanged();
    void coefficientChanged();
    void timeAxisChanged();

    void beamCountChanged( int frameCount , int frameTime , QList<quint32> timeList );
    // beam 的max值 的曲线图
    void timeAxisChanged( QList<float> timeAxis );

    void sendPath( QString file );

    void finish();
    void distanceChanged( quint32 distance );

    void sendVideoData( const unsigned int &width_, const unsigned int &height_, QVector<quint8> &picture_ );
    void sendBeamData( const unsigned int &width_, const unsigned int &height_, QVector<float> &beam_ );

    void setParams( RhythmUI::AcousticImageModes filterModelType_ ,const float &beamMax_ ,const float &beamMin_ ,const float &beamLimitMax_ ,const float &peakFactor_ ,const float &dynamicRange_ ,const quint32 &distance_ );

private:
    bool isRun{false};
    float m_imgScale{0.75};

    bool m_timeAxis{false};

    bool m_full;
    bool m_nightVersion;
    quint8 m_method;

    QSize m_size;
    QPointF m_pos;

    // 声学成像延时帧数 设置
    quint8 delayFrameCount{0};
    QList<QVector<unsigned char>> delayFrame;

    QList<float> beamMaxList;

    quint8 beamFPS{100};

    unsigned int m_time_len{0};

    VideoThread *videoThread{nullptr};
    VideoThread *videoRunThread{nullptr};
    VideoThread *beamThread{nullptr};
    RhythmGradientBar                                *m_prhythemVideoGradientBar{nullptr};

    unsigned char *m_frame{nullptr};
    unsigned char *m_cloudFrame{nullptr};
    QImage m_image;

    quint8 showCloudCycle{5};
    quint8 cloudCycle{0};

    // 夜视算法参数
    float m_brightness{0.0};
    float m_contrast{0.0};
    float m_saturation{0.0};
    float m_highLight{0.0};
    float m_shadow{0.0};
    float m_midtone{0.0};
    float m_coefficient{0.0};

    // 摄像画面尺寸
    unsigned int m_ui_width{640};
    unsigned int m_ui_height{480};
    unsigned int bytesPerLine{0};
    // 声成像点阵尺寸
    unsigned int m_ui_cloudWidth{64};
    unsigned int m_ui_cloudHeight{48};

    int m_i_currentTime{0};
    int m_i_currentCloudTime{0};
    quint32 m_i_currentFrame{0};

    quint8 m_i_rgbMax{0};
    quint8 m_i_rgbMin{0};

    RhythmVideoGLES          *m_prhythmVideoGLES{nullptr};
    // tdmsVideoThread * m_Thread;

    std::map< int, unsigned char*>            m_map_pictures;
    std::map< int, unsigned char*>::iterator  m_picturMapIterator{m_map_pictures.begin()};
    std::map< int, RhythmUI::picture>                   m_map_cloudPicture;
    std::map< int, RhythmUI::picture>::iterator         m_cloudPictureMapIterator{m_map_cloudPicture.begin()};
    QMap<int, QImage>                   m_map_camera;
    QMap<int, QImage>::Iterator         m_map_cameraIterator{m_map_camera.begin()};

    float m_f_beamMax{13.9f};
    float m_f_beamMin{10.9f};
    float m_f_beamLimitMax{13.9f};
    float m_f_peakFactor{1.0f};
    float m_f_dynamicRange{1.0f};
    quint32 m_i_distance{1000};
    size_t m_ui_videoSize{0};
    size_t m_ui_videoBeamSize{0};

    int colorCount = 0;

    QVector<unsigned char> m_vec_pictureTemp;
    QVector<unsigned char> m_vec_cloudPictureTemp;

    QFuture<bool> m_future_picture;
    QFuture<bool> m_future_cloud;

    RhythmUI::AcousticImageModes m_e_filterModelType{RhythmUI::AcousticImageModes::ImageModeAuto};

    //QList<evBuffer *> m_list_eigenValue;

    /**
     * @brief generateColor
     * @param value
     * @return
     */
    //old color generateColor(const float& value) const;
    color generateColor( float value );

    /**
     * @brief generataCloud
     * @param width_            @brief set cloud picture width
     * @param height_           @brief set cloud picture height
     * @param data_             @brief set float vector
     * @param min_              @brief set min frequence
     * @param max_              @brief set max frequence
     */
    void generateCloud(const unsigned int &width_, const unsigned int &height_, const float *data_, const float &min_ , const float &max_);

};

#endif // RUISENVIDEO_H
