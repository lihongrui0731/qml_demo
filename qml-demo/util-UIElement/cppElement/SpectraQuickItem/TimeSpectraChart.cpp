#include "TimeSpectraChart.h"
#include "RhythmSpectraGLES.h"

#include <QDebug>
#include <cmath>
#include <ctime>
//#include <limits>
//#include <climits>
//#include <cfloat>
//#include <iostream>

#include <QOpenGLFramebufferObject>
#include <QThread>
#include <QQuickItem>
#include <QVector>

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif

class SpectraRender : public QQuickFramebufferObject::Renderer {
public:
    SpectraRender();
    ~SpectraRender();

    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *) override;

private:
    bool isRun{false};
    RhythmSpectraGLES m_render;
};

SpectraRender::SpectraRender()
{
    m_render.initializeGL();
}

SpectraRender::~SpectraRender(){
}

void SpectraRender::render()
{
    // qDebug() << " render spectra ";
    m_render.paintGL();
}

void SpectraRender::synchronize(QQuickFramebufferObject *item)
{
    // qDebug() << "synchronize spectra ----------------------------------";

    TimeSpectraChart *shader = qobject_cast<TimeSpectraChart *>(item);
    isRun = shader->getStatus();
    shader->set_scaleSize( m_render.getScaleSize() );
    if( shader->get_ui_frame() != nullptr ){
        m_render.setFrame( shader->get_ui_height(), shader->get_ui_width(), shader->get_ui_frame() );
    }
}

QOpenGLFramebufferObject *SpectraRender::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);

    return new QOpenGLFramebufferObject(size, format);
}

QQuickFramebufferObject::Renderer * TimeSpectraChart::createRenderer() const
{
    return new SpectraRender();
}

TimeSpectraChart::TimeSpectraChart(QQuickItem *parent): QQuickFramebufferObject(parent){

    // m_prhythmSpectraGLES = new RhythmSpectraGLES();
    // connect(m_prhythmSpectraGLES, &RhythmSpectraGLES::scaleAndMove, this, &TimeSpectraChart::scaleAndMove);
    qRegisterMetaType<std::map<int,float*>>("std::map<int,float*>");

    connect(this, &TimeSpectraChart::frameChanged, [=](){update();});
    connect(this, &TimeSpectraChart::fullChanged, [&](){
        update();
    });

    // m_ptimerCursor = new TimeCursorLabel(this);

    /*
    m_ptimerCursor = new TimerCursorLabel(this);

    connect(m_ptimerCursor, &TimerCursorLabel::currentCursor, this, [&](const float& position_)->void{
        emit currentCursorChanged(position_);
    });
    */

    m_uint_width = 100;
    m_uint_height = m_method != 2?m_data_len/m_cal_count:m_data_len;

    resetSpectra();
    // startTimer(1000 / 1);
}

TimeSpectraChart::~TimeSpectraChart()
{

//    delete m_prhythmSpectraGLES;
//    m_prhythmSpectraGLES = nullptr;
    if( m_pmainHBoxLayout != nullptr ){
        delete m_pmainHBoxLayout;
        m_pmainHBoxLayout = nullptr;
    }
    if( m_ptimerCursor != nullptr ){
        delete m_ptimerCursor;
        m_ptimerCursor = nullptr;
    }
    if( m_frame != nullptr ){
        delete m_frame;
        m_frame = nullptr;
        m_pdata = nullptr;
    }

    if( spectraThread != nullptr ){
        delete spectraThread;
        spectraThread = nullptr;
    }

    m_map_spectra.clear();
    spectra_temp.clear();

    /*
    if( m_map_spectra.size() > 0 ){
        std::map<int, float*>::iterator iter = m_map_spectra.begin();
        while( iter != m_map_spectra.end() ){
            free(iter->second);
            iter++;
        }
    }
    */

#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif

    qDebug() << "析构 ~ TimeSpectra";
   //  m_Thread = nullptr;
}


void TimeSpectraChart::addData( QVariant data ){

    floatData data_ = data.value<floatData>();
    QVector<float> spectra = data_.data;

    if( m_adaption ){
        float max_ = 0;
        for( int i=0; i<spectra.size(); i++ ){
            if( spectra.value(i) > max_ ){
                max_ = spectra.value(i);
            }
        }

        if( max_ > max_temp || max_temp_pos <= 0 ){
            max_temp = max_;
            max_temp_pos = m_uint_width;
        }

        if( max_temp > m_f_amplitudeMax || max_pos <=0 ){
            max_pos = max_temp_pos;
            if( max_temp > 1 ){
                m_f_amplitudeMax = ceil(max_temp)+1 > 100?100:ceil(max_temp)+1;
            }else{
                int i = 0;
                float max_temp_ = max_temp;
                while( max_temp_ < 1 ){
                    max_temp_ = max_temp_ * 10;
                    i++;
                }
                m_f_amplitudeMax = (ceil(max_temp_) ) * pow(10,-i);
            }
            max_temp = 0.0;
            emit amplitudeMaxChanged();
            redata();
        }

        spectra_temp.append(spectra);
        if( (unsigned int)spectra_temp.size() > m_uint_width ){
            spectra_temp.removeFirst();
            max_pos--;
        }
    }

    int len = data_.data.size();
 //   float dt_ = data_.dt * 1000;
 //   qDebug() << dt_;

    //  m_uint_width = 10000 / static_cast<quint32>(dt_);
    addSpectraData( len , spectra.data() );
}

void TimeSpectraChart::addJsonData( QJsonObject jsonObj ){
    /*
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    */

    quint16 count = jsonObj.value("frameCount").toInt();
    // quint32 len = jsonObj.value("frameLength").toInt();
    quint32 len =  static_cast<quint32>( jsonObj.value("bandWidth").toDouble() / jsonObj.value("deltaFreq").toDouble() );

    QJsonArray valueArr = jsonObj.value("values").toArray();

    // quint32 len_total = 0;

    QVector<float> spectra( len );
    for( int i = 0; i < count; i++ ){
        QJsonArray v_arr = valueArr[i].toArray();
        for( quint32 j = 0; j < len; j++ ){
            spectra[j] = v_arr[j].toDouble();
        }

        if( len != m_data_len ) {
            m_data_len = len;
            resetSpectra();
        }

        // len_total += len;
        if( m_adaption ){
            float max_ = 0;
            for( int i=0; i<spectra.size(); i++ ){
                if( spectra.value(i) > max_ ){
                    max_ = spectra.value(i);
                }
            }

            if( max_ > max_temp || max_temp_pos <= 0 ){
                max_temp = max_;
                max_temp_pos = m_uint_width;
            }

            if( max_temp > m_f_amplitudeMax || max_pos <=0 ){
                max_pos = max_temp_pos;
                if( max_temp > 1 ){
                    m_f_amplitudeMax = ceil(max_temp)+1 > 100?100:ceil(max_temp)+1;
                }else{
                    int i = 0;
                    float max_temp_ = max_temp;
                    while( max_temp_ < 1 ){
                        max_temp_ = max_temp_ * 10;
                        i++;
                    }
                    m_f_amplitudeMax = (ceil(max_temp_) ) * pow(10,-i);
                }
                max_temp = 0.0;
                emit amplitudeMaxChanged();
                redata();
            }

            spectra_temp.append(spectra);
            if( (unsigned int)spectra_temp.size() > m_uint_width ){
                spectra_temp.removeFirst();
                max_pos--;
                max_temp_pos--;
            }
        }
        addSpectraData( len , spectra.data() );
    }
    spectra.clear();
}

void TimeSpectraChart::setData( QVariant data ){
    SpectraData t = data.value<SpectraData>();

    if( t.m_pdata_time ) m_pdata_time = t.m_pdata_time;
    if( t.m_frequencyMax ) m_frequencyMax = t.m_frequencyMax;
    if( t.m_frequencyMax ) m_frequencyMin = t.m_frequencyMin;

    //m_map_spectra = t._p_data;
    m_data_size = static_cast<unsigned int>(t._p_data.size());

    m_uint_width = t._p_data.size();
    m_uint_height = m_method !=2? t.len / m_cal_count : t.len;
    m_data_len = t.len;

    //保留原始数据
    std::map<int, float*>::iterator iter = t._p_data.begin();
    while( iter != t._p_data.end() ){
        QVector<float> v_;
        v_.reserve(m_data_len);
        for( unsigned int i = 0; i < m_data_len; i++ ){
            v_.append( iter->second[i] );
        }

        // free( iter->second );
        m_map_spectra.insert(iter->first,v_);
        iter++;
    }

    // m_map_spectra = t._p_data;
    // 调用子线程

    if( spectraThread != nullptr ){
        delete spectraThread;
    }
    spectraThread = new SpectraThread();
    connect( spectraThread , &SpectraThread::sendSpectraData , this , &TimeSpectraChart::getSpectraData );

    spectraThread->setSpectraData( m_f_amplitudeMax , m_f_amplitudeMin , m_uint_width , m_data_len , m_map_spectra , m_method );
    spectraThread->start( QThread::LowPriority );

    // setSpectraData(static_cast<unsigned int>(m_map_spectra.size()), t.len , m_map_spectra);
    if( t.m_pdata_time ) emit frameCountChanged(m_map_spectra.size() , m_pdata_time);
    // emit finish();
}

void TimeSpectraChart::setAmplitudeMax( float amplitudeMax_ ){
    if( m_f_amplitudeMax == amplitudeMax_ ){
        return;
    }
    m_f_amplitudeMax = amplitudeMax_;
}
void TimeSpectraChart::setAmplitudeMin( float amplitudeMin_ ){
    if( m_f_amplitudeMin == amplitudeMin_ ){
        return;
    }
    m_f_amplitudeMin = amplitudeMin_;
}

void TimeSpectraChart::setFrequence( float max , float min , bool isRun ){
    m_f_amplitudeMax = max;
    m_f_amplitudeMin = min;

    if( !isRun ){
        if( isReloadColor ){
            return;
        }
        isReloadColor = true;

        if( spectraThread != nullptr ){
            delete spectraThread;
        }
        spectraThread = new SpectraThread();
        connect( spectraThread , &SpectraThread::sendSpectraData , this , &TimeSpectraChart::getSpectraData  );

        spectraThread->setSpectraData( m_f_amplitudeMax , m_f_amplitudeMin , m_uint_width , m_data_len , m_map_spectra , m_method );
        spectraThread->start( QThread::LowPriority );
    }
}

void TimeSpectraChart::getSpectraData( unsigned char *data ){
    if( m_pdata != nullptr ){
        free(m_pdata);
        m_pdata = nullptr;
    }

    m_pdata = data;
    set_ui_frame( m_pdata );

    spectraThread->quit();
    delete spectraThread;
    spectraThread = nullptr;
    isReloadColor = false;

    emit finish();
}

void TimeSpectraChart::setInteval(const int& inteval_)
{
    m_ptimerCursor->setInterval(inteval_);
}

void TimeSpectraChart::resetSpectra()
{

   // m_ptimerCursor->resetTimerCursor();
    if( m_pdata != nullptr ){
        delete m_pdata;
        m_pdata = nullptr;
    }

/*
    if( m_map_spectra.size() > 0 ){
        std::map<int, float*>::iterator iter = m_map_spectra.begin();
        while( iter != m_map_spectra.end() ){
            if( iter->second != nullptr ) {
                free(iter->second);
                iter->second = nullptr;
            }
            iter++;
        }
    }
*/

    m_map_spectra.clear();
    spectra_temp.clear();

    max_temp = 0;
    max_pos = 0;
    max_temp_pos = 0;

    m_uint_height = m_method != 2?m_data_len/m_cal_count:m_data_len;
    size_t _size = m_uint_width*m_uint_height*3* sizeof(unsigned char);

    m_pdata = static_cast<unsigned char*>( malloc(_size) );
    memset(m_pdata, 0, _size);

    set_ui_frame( m_pdata );


#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif

}

void TimeSpectraChart::redata(){
    clearData();
    size_t _size = m_uint_width*m_uint_height*3* sizeof(unsigned char);
    m_pdata = static_cast<unsigned char*>(malloc(_size));
    memset(m_pdata, 0, _size);

    for( int i=0; i< spectra_temp.size(); i++ ){
        addSpectraData(m_data_len,spectra_temp[i].data());
    }
}

TimeSpectraChart::color TimeSpectraChart::generateColor(float value)
{
    const float float_epsilon { std::numeric_limits<float>::epsilon() };
    Q_UNUSED( float_epsilon );

    if ( std::isinf( value ))
        return color(0, 0, 0);

    float _frequenceMax = m_method == 0?m_f_amplitudeMax*sqrt(m_cal_count):m_f_amplitudeMax;

    if( m_ln ){
      //value = qLn( value );
    }

    if (value < m_f_amplitudeMin) value = m_f_amplitudeMin;
    else if (value > _frequenceMax) value = _frequenceMax;

    float dv = _frequenceMax - m_f_amplitudeMin;

    float v = (value - m_f_amplitudeMin) / dv;
    float _8th = 0.125;
    quint8 r = 0;
    quint8 g = 0;
    quint8 b = 0;

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

   // qDebug()<< r << "-" << g << "-" << b;

    return color(r, g, b);
}

/*
TimeSpectraChart::color TimeSpectraChart::generateColor(const float &value) const
{
    const float float_epsilon { std::numeric_limits<float>::epsilon() };

    if ( std::isinf( value ))
        return color(0, 0, 0);

    float division = (m_f_amplitudeMax - m_f_amplitudeMin) / 4.0f;
    float divisionone = m_f_amplitudeMin * 0.25f + m_f_amplitudeMax * 0.75f;
    float divisiontwo = (m_f_amplitudeMin + m_f_amplitudeMax) * 0.5f;
    float divisionthree = m_f_amplitudeMin * 0.75f + m_f_amplitudeMax * 0.25f;

    if( value >= m_f_amplitudeMax)
        return color(255, 0, 0);
    else if( divisionone < value && value < m_f_amplitudeMax)
        return color(255, static_cast<int>((m_f_amplitudeMax - value)/division * 255.0f), 0);
    else if( fabsf(value - divisionone) <= float_epsilon )  // second splite
        return color(255, 255, 0);
    else if( divisiontwo < value && value < divisionone)
        return color(static_cast<int>((value - divisiontwo)/division*255), 255, 0);
    else if( fabsf(value - divisiontwo) <= float_epsilon ) // third splite
        return color(0, 255, 0);
    else if( divisionthree < value && value < divisiontwo)
        return color( 0, static_cast<int>((value-divisionthree)/division*255), static_cast<int>((divisiontwo - value)/division*255));
    else if( fabsf(value - divisionthree) <= float_epsilon )  // fourth splite
        return color(0, 0, 255);
    else if( m_f_amplitudeMin < value && value< divisionthree)
        return color(0, 0, static_cast<int>((value - m_f_amplitudeMin)/division * 255.0f));
    else if ( value <= m_f_amplitudeMin ) // last splite
        return color(0, 0, 0);

    return color(0, 0, 0);
}
*/

void TimeSpectraChart::setSpectraData(const unsigned int & width, const unsigned int& height, const std::map<int, float*>& spectra)
{
    // 弃用 改为子线程执行
    size_t _count = width * height * sizeof (unsigned char)*3;

    unsigned char* data = static_cast<unsigned char*>(malloc(_count* sizeof(unsigned char)));
    size_t _coloum = 0; // remember the coloum
    std::map<int, float*>::const_iterator _spectraIterator = spectra.begin(); //

    while ( _spectraIterator != spectra.end() ) {
        for(size_t _row = 0; _row < height; _row++ ) {
            // qDebug() << "*************" << _row;

            const color& _color = generateColor(_spectraIterator->second[_row]);

            size_t _position    = _coloum*height + _row;
            data[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
            data[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
            data[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
        }
        _coloum++;
        _spectraIterator++;
    }

    m_uint_width = width;
    m_uint_height = height;
    // m_prhythmSpectraGLES->setFrame(height, width, data);

    m_pdata = data;
    set_ui_frame( m_pdata );
    // free(data);

    // m_prhythmSpectraGLES->repaint();
    // update();
}


void TimeSpectraChart::addSpectraData(const unsigned int& height, const float* spectra)
{
    m_data_len = height;
    m_uint_height = m_method !=2? height/m_cal_count:height;

    size_t _copySize = (m_uint_width - 1 ) * m_uint_height * 3 * sizeof(unsigned char);
    int size_ = m_uint_height * 3 * sizeof(unsigned char);

    memmove(&m_pdata[0], &m_pdata[size_], _copySize); // move the data to before height*3

    size_t _column = (m_uint_width - 1 ); // remember the column
    size_t _startPosition = _column*m_uint_height;

    if( m_method == 0 ){
        QVector<float> temp;
        //temp.reserve(m_cal_count);
        size_t row__{0};

        for( unsigned int _row = 0; _row < height; _row++ ) {
            temp.append( spectra[_row] );
            if( temp.size() >= m_cal_count ){
                float sum{0};
                for( float t:temp ){
                    sum += pow(t,2);
                }
                float d_ = sqrt(sum);

                const color& _color = generateColor(d_);
                size_t _position = _startPosition + row__;

                if( scaleSize < 100.0f ){
                    if ( (static_cast<int>(row__) >= m_f_minFrequenceLimmit - 1 && static_cast<int>(row__) <= m_f_minFrequenceLimmit + 1) ||
                         (static_cast<int>(row__) >= m_f_maxFrequenceLimmit - 1 && static_cast<int>(row__) <= m_f_maxFrequenceLimmit + 1)) {
                        m_pdata[_position*3]   = 255;   // red value
                        m_pdata[_position*3+1] = 255;   // green value
                        m_pdata[_position*3+2] = 255;   // blue value
                    } else {
                        m_pdata[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                        m_pdata[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                        m_pdata[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                    }

                } else {
                    if ( static_cast<int>(row__) != m_f_minFrequenceLimmit && static_cast<int>(row__) != m_f_maxFrequenceLimmit) {
                        m_pdata[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                        m_pdata[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                        m_pdata[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                    } else {
                        m_pdata[_position*3]   = 255;   // red value
                        m_pdata[_position*3+1] = 255;   // green value
                        m_pdata[_position*3+2] = 255;   // blue value
                    }
                }
                temp.clear();
                row__++;
            }

        }
    }else if( m_method == 1 ){
        float max_{0.0};
        size_t row__{0};
        for( unsigned int _row = 0; _row < height; _row++ ) {
            max_ = spectra[_row] > max_?spectra[_row]:max_;
            if( (_row+1)%m_cal_count == 0 ){
                const color& _color = generateColor(max_);
                size_t _position    = _startPosition + row__;

                if( scaleSize < 100.0f ){
                    if ( (static_cast<int>(row__) >= m_f_minFrequenceLimmit - 1 && static_cast<int>(row__) <= m_f_minFrequenceLimmit + 1) ||
                         (static_cast<int>(row__) >= m_f_maxFrequenceLimmit - 1 && static_cast<int>(row__) <= m_f_maxFrequenceLimmit + 1)) {
                        m_pdata[_position*3]   = 255;   // red value
                        m_pdata[_position*3+1] = 255;   // green value
                        m_pdata[_position*3+2] = 255;   // blue value
                    } else {
                        m_pdata[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                        m_pdata[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                        m_pdata[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                    }
                } else {
                    if ( static_cast<int>(row__) != m_f_minFrequenceLimmit && static_cast<int>(row__) != m_f_maxFrequenceLimmit) {
                        m_pdata[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                        m_pdata[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                        m_pdata[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                    } else {
                        m_pdata[_position*3]   = 255;   // red value
                        m_pdata[_position*3+1] = 255;   // green value
                        m_pdata[_position*3+2] = 255;   // blue value
                    }
                }

                max_ = 0.0;
                row__++;
            }
        }

    }else if( m_method == 2 ){
        for(size_t _row = 0; _row < height; _row++ ) {
            const color& _color = generateColor(spectra[_row]);
            size_t _position = _startPosition + _row;
            // if ( m_prhythmSpectraGLES->getScaleSize() < 100.0f ) {
            if( scaleSize < 100.0f ){
                if ( (static_cast<int>(_row) >= m_f_minFrequenceLimmit - 1 && static_cast<int>(_row) <= m_f_minFrequenceLimmit + 1) ||
                     (static_cast<int>(_row) >= m_f_maxFrequenceLimmit - 1 && static_cast<int>(_row) <= m_f_maxFrequenceLimmit + 1)) {
                    m_pdata[_position*3]   = 255;   // red value
                    m_pdata[_position*3+1] = 255;   // green value
                    m_pdata[_position*3+2] = 255;   // blue value
                } else {
                    m_pdata[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                    m_pdata[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                    m_pdata[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                }
            } else {
                if ( static_cast<int>(_row) != m_f_minFrequenceLimmit && static_cast<int>(_row) != m_f_maxFrequenceLimmit) {
                    m_pdata[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                    m_pdata[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                    m_pdata[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                } else {
                    m_pdata[_position*3]   = 255;   // red value
                    m_pdata[_position*3+1] = 255;   // green value
                    m_pdata[_position*3+2] = 255;   // blue value
                }
            }
        }
    }

    _column++;
    // m_prhythmSpectraGLES->setFrame(height, m_uint_width, m_pdata);

    set_ui_frame(m_pdata);

   // m_prhythmSpectraGLES->repaint();
   // update();
}

// 实时结束获取最终数据
void TimeSpectraChart::setFinalSpectraData( QVariant data_map ){
    spectraFinalData spectra_ = data_map.value<spectraFinalData>();
    m_uint_width = spectra_.spectra.size();

    resetSpectra();

    QMap<quint64, QVector<float>>::Iterator spectra_itor = spectra_.spectra.begin();
    while ( spectra_itor != spectra_.spectra.end() ) {
        QVector<float> v_;
        v_.reserve( m_data_len );
        for( unsigned int i = 0; i < m_data_len; i++ ){
            v_.append( spectra_itor.value().data()[i] );
        }
        m_map_spectra.insert(spectra_itor.key(),v_);
        spectra_itor++;
    }

    if( spectraThread != nullptr ){
        delete spectraThread;
    }
    spectraThread = new SpectraThread();
    connect( spectraThread , &SpectraThread::sendSpectraData , this , &TimeSpectraChart::getSpectraData );

    spectraThread->setSpectraData( m_f_amplitudeMax , m_f_amplitudeMin , m_uint_width , m_data_len , m_map_spectra , m_method );
    spectraThread->start( QThread::LowPriority );

}

void TimeSpectraChart::resize( float x , float y , QString act ){
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(act);
    // 放大缩小
}

void TimeSpectraChart::timerEvent(QTimerEvent* event_)
{
    Q_UNUSED(event_);
    int len = static_cast<int>(m_uint_width) * 1024;
    float *pic_ = new float[len];

    for( int i = 0; i < len; i++ ){
        pic_[i] = i % 255;
    }

    addSpectraData(1024, pic_);
    delete[] pic_;
}

/*
void TimeSpectraChart::setPosition(const float& position_)
{
    m_ptimerCursor->setPostion(position_);
    emit currentCursorChanged(position_);
}
*/

void TimeSpectraChart::insertMark()
{
   //zz m_ptimerCursor->insertMark();
}

void TimeSpectraChart::removeMark()
{
   //zz m_ptimerCursor->removeMarks();
}

void TimeSpectraChart::nextMark()
{
   //zz m_ptimerCursor->nextMark();
}

void TimeSpectraChart::lastMark()
{
   //zz m_ptimerCursor->lastMark();
}

void TimeSpectraChart::videoPlay()
{
   //zz m_ptimerCursor->play();
}

void TimeSpectraChart::videoStop()
{
   //zz m_ptimerCursor->stop();
}

void TimeSpectraChart::setDuration(const float& duration_)
{
    Q_UNUSED( duration_ );
   //zz m_ptimerCursor->setDuration(duration_);
}

void TimeSpectraChart::setTimeControlerShow(const bool& show_)
{
   //zz m_ptimerCursor->se
    Q_UNUSED( show_ );
}

/*
void TimeSpectraChart::resizeEvent(QResizeEvent* event)
{
    m_prhythmSpectraGLES->resize(event->size());
    m_ptimerCursor->resize(event->size());
    QWidget::resizeEvent(event);
}
*/
