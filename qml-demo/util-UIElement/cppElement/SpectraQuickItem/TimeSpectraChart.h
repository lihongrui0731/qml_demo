/** *****************************************************************************************************
 *  @brief
 *
 *
 *  @author guweicao                                                                      @date 2021/5/30
 ********************************************************************************************************/
#ifndef TIMESPECTRACHART_H
#define TIMESPECTRACHART_H

#include <limits>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QVariant>
#include <QVariantMap>

#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QThread>
#include <QMetaType>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtCore/qmath.h>
#include "TimeCursorLabel.h"
#include "SpectraThread.h"

#include "dataType.h"
//#include "report_frame.h"

class RhythmSpectraGLES;

class TimeSpectraChart: public QQuickFramebufferObject{
    Q_OBJECT
    Q_PROPERTY(bool ln READ ln WRITE setLn NOTIFY lnChanged)

    Q_PROPERTY(bool adaption READ adaption WRITE setAdaption NOTIFY adaptionChanged)

    Q_PROPERTY(bool full READ full WRITE setFull NOTIFY fullChanged)
    Q_PROPERTY(int frameWidth READ frameWidth WRITE setFrameWidth NOTIFY frameWidthChanged)
    Q_PROPERTY(float amplitudeMax READ amplitudeMax WRITE setAmplitudeMax NOTIFY amplitudeMaxChanged)
    Q_PROPERTY(float amplitudeMin READ amplitudeMin WRITE setAmplitudeMin NOTIFY amplitudeMinChanged)

    Q_PROPERTY(quint8 method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(quint8 calCount READ calCount WRITE setCalCount NOTIFY calCountChanged)
    Q_PROPERTY(quint32 frameHeight READ frameHeight WRITE setFrameHeight NOTIFY frameHeightChanged )

    struct color{
        explicit color(int red_, int green_, int blue_):
            red(red_), green(green_), blue(blue_) {}
        int red{255};
        int green{255};
        int blue{255};
    };

public:
    TimeSpectraChart(QQuickItem *parent = 0);
    virtual ~TimeSpectraChart();

    unsigned int get_ui_width() const {return m_uint_width;}
    unsigned int get_ui_height() const {return m_uint_height;}
    unsigned char* get_ui_frame() const { return m_frame;}
    quint8 method() const{ return m_method; }
    quint8 calCount() const { return m_cal_count; }

    bool getStatus() const{ return isRun; }
    bool adaption() const{ return m_adaption; }

    float amplitudeMax(){
        return m_f_amplitudeMax;
    }

    float amplitudeMin(){
        return m_f_amplitudeMin;
    }

    void set_ui_frame( unsigned char *v) { m_frame = v; emit frameChanged(); }

    void setAmplitudeMax( float amplitudeMax_ );
    void setAmplitudeMin( float amplitudeMin_ );

    Q_INVOKABLE void setFrequence( float max , float min , bool isRun );

    void setMethod( quint8 method_ ){
        m_method = method_;
    }

    void setCalCount( quint8 calCount_ ){
        m_cal_count = calCount_;
        m_uint_height = m_method != 2?m_data_len/m_cal_count:m_data_len;
    }

    void setAdaption( bool adaption_ ){
        m_adaption = adaption_;
    }

    void set_scaleSize( float v ){ scaleSize = v; }
    /**
     * @brief 相邻数据帧时间间隔
     */
    void setInteval(const int& inteval_);

    /**
     * @brief session数据集 末帧-首帧 时间戳差值
     */
    void setDuration(const float& duration_);

    /**
     * @brief setFrameCount
     * @param count
     */
    // void setFrameCount(const int& count);

    void setTimeControlerShow(const bool& show_);

    /**
     * @brief setMinLimmitFrequence
     * @param value
     */
    void setMinLimmitFrequence(const float& value)
    {
        m_f_minFrequenceLimmit = static_cast<int>(value);
    }

    /**
     * @brief setMaxLimmitFrequence
     * @param value
     */
    void setMaxLimmitFrequence(const float& value)
    {
        m_f_maxFrequenceLimmit = static_cast<int>(value);
    }

    // 绘图实现

    /**
     * @brief 重置画面
     */
    Q_INVOKABLE void resetSpectra();

    /**
     * @brief 传入一段时长内的多帧频谱数据集合
     * @param width
     * @param height
     * @param spectra - (map<string->float*>)
     */
    void setSpectraData(const unsigned int & width, const unsigned int& height, const std::map<int, float*>& spectra);

    /**
     * @brief 在尾部追加一帧频谱数据
     * @param height - 一帧内频点的数量
     * @param spctra - 一帧频谱数据
     */
    void addSpectraData(const unsigned int & height, const float *spectra);

    // 时间戳标记

    /**
     * @brief insertMark
     */
    void insertMark();

    /**
     * @brief removeMark
     */
    void removeMark();

    /**
     * @brief nextMark
     */
    void nextMark();

    /**
     * @brief lastMark
     */
    void lastMark();

    /**
     * @brief 从当前光标位置开始播放
     */
    void videoPlay();

    /**
     * @brief 停止播放
     */
    void videoStop();

    Renderer * createRenderer() const override;

    bool full() const {return m_full;}
    bool ln() const { return m_ln; }

    int frameWidth(){ return m_uint_width; }
    int frameHeight(){ return m_data_len; }

    void setFrameWidth( unsigned int width ){
        if( width > 0 )
        m_uint_width = width;
    }

    void setFrameHeight( unsigned int height ){
       m_data_len = height;
       m_uint_height = m_method != 2?m_data_len/m_cal_count:m_data_len;
    }

    void setLn(bool v){
        m_ln = v;
    }

    void setFull(bool v)
    {
        if (v == m_full) return ;
        m_full = v;
        emit fullChanged();
    }

    Q_INVOKABLE void addData( QVariant data );
    Q_INVOKABLE void addJsonData( QJsonObject data );

    Q_INVOKABLE void setData( QVariant data );

    Q_INVOKABLE void start(){
        resetSpectra();
        isRun = true;
    }
    Q_INVOKABLE void stop(){
        isRun = false;
    }

    Q_INVOKABLE int getClickTimeLine(const float& position_x){
        double _positionRatio = static_cast<double>(position_x)/static_cast<double>(width());
        return static_cast<int>(m_pdata_time*_positionRatio);
    }

    Q_INVOKABLE int getClickSize(const float& position_x){
        double _positionRatio = static_cast<double>(position_x)/static_cast<double>(width());
        return static_cast<int>(m_data_size*_positionRatio) - 1;
    }

    Q_INVOKABLE double getFrequencyMax(){
        return m_frequencyMax;
    }

    Q_INVOKABLE double getFrequencyMin(){
        return m_frequencyMin;
    }

    Q_INVOKABLE void resize( float x , float y , QString act );

    Q_INVOKABLE void setFinalSpectraData( QVariant data_map );

    Q_INVOKABLE void clearData(){
        if( m_pdata != nullptr ){
            free(m_pdata);
            m_pdata = nullptr;
        }
    };

public slots:

    /**
     * @brief seek 到指定位置
     * @param position_
     */
    // void setPosition(const float& position_);
    void getSpectraData( unsigned char *data );

signals:

    void currentCursorChanged(const float& position_);

    // void scaleAndMove(const float& move, const float& scale);
    void lnChanged();
    void adaptionChanged();
    void fullChanged();
    void frameChanged();
    void frameCountChanged( int frameCount , int frameTime );
    void frameWidthChanged();
    void frameHeightChanged();

    void amplitudeMaxChanged();
    void amplitudeMinChanged();

    void sendPath( QString file );

    void methodChanged();
    void calCountChanged();
    void finish();
protected:
    /// @todo 确认无用后删除
    void timerEvent(QTimerEvent*) override;
    //  void resizeEvent(QResizeEvent* event) override;

private:

    bool isRun{false};
    // ui members
    bool m_full;
    bool m_ln{false};
    bool m_adaption{false};
    bool isReloadColor{false};
    QSize m_size;
    QPointF m_pos;

    quint8 m_method{0}; //0 合并算法平方和开根 ，1 合并算法取最大值 ， 2 不处理
    quint8 m_cal_count{4};

    int m_pdata_time{0};

    float scaleSize{0};
    int m_data_size{0};

    QList<QVector<float>> spectra_temp;
    int max_pos{0}; //最大值的位置
    float max_temp{0.0}; //第二最大值的位置
    int max_temp_pos{0};

    SpectraThread *spectraThread{nullptr};

    QHBoxLayout                   *m_pmainHBoxLayout{nullptr};  // widgets layout
    RhythmSpectraGLES             *m_prhythmSpectraGLES{nullptr}; // data gl view widget

    TimeCursorLabel               *m_ptimerCursor{nullptr};

    unsigned char *m_frame{nullptr};

    // data members
    QMap<int, QVector<float>>          m_map_spectra;      // all the spectra data
    float                          m_f_amplitudeMax{80};
    float                          m_f_amplitudeMin{0};
    int                            m_f_minFrequenceLimmit{-1};
    int                            m_f_maxFrequenceLimmit{-1};

    float                          m_frequencyMax{0};
    float                          m_frequencyMin{0};

    float                          m_max{m_f_amplitudeMax};

    // 现有已绘制内容的宽度
    unsigned int m_uint_width{100};
    unsigned int m_data_len{1024};
    unsigned int m_uint_height{1024};

    // 待绘制的内容
    unsigned char *m_pdata{nullptr};

    //  tdmsThread * m_Thread;
    // procedures
    // 计算数值对应的颜色

    TimeSpectraChart::color generateColor( float value );
    void redata();

    // TimeSpectraChart::color generateColor(const float &value) const;
};

#endif // TIMESPECTRACHART_H
