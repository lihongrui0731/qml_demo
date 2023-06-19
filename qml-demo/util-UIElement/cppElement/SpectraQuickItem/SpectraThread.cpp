#include "SpectraThread.h"
#include <QMetaType>
#include <QVariant>
#include <cmath>

SpectraThread::SpectraThread()
{
    qRegisterMetaType<std::map<int,float*>>("std::map<int,float*>");
    qDebug() << "spectra thread 线程";
}

SpectraThread::~SpectraThread(){
    qDebug() << "析构 spectra thread";
    if( m_spectra_data != nullptr ){
        delete m_spectra_data;
        m_spectra_data = nullptr;
    }
    m_spectra.clear();
    //等待tdms释放
}

void SpectraThread::run(){
    sortSpectraData();
}

SpectraThread::color SpectraThread::generateColor(float value)
{
    // const float float_epsilon { std::numeric_limits<float>::epsilon() };
    // Q_UNUSED(float_epsilon);

    if ( std::isinf( value ))
        return color(0, 0, 0);

    float _amplitudeMax = m_method == 0?m_amplitudeMax*2:m_amplitudeMax;

    if (value < m_amplitudeMin) value = m_amplitudeMin ;
    else if (value > _amplitudeMax) value = _amplitudeMax;

    float dv = _amplitudeMax - m_amplitudeMin;
    float v = (value - m_amplitudeMin) / dv;
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

    return color(r, g, b);
}

void SpectraThread::setSpectraData( float amplitudeMax , float amplitudeMin , const unsigned int & width, const unsigned int& height, const QMap<int, QVector<float>>& spectra , quint8 method ){
    m_amplitudeMax = amplitudeMax;
    m_amplitudeMin = amplitudeMin;
    m_uint_width = width;
    m_method = method;

    m_uint_height = m_method !=2 ?height/m_cal_count:height;
    m_data_len = height;
    m_spectra = spectra;
}

void SpectraThread::sortSpectraData()
{
    size_t _count = m_uint_width * m_uint_height * sizeof (unsigned char)*3;

    unsigned char* data = static_cast<unsigned char*>(malloc(_count* sizeof(unsigned char)));
    size_t _coloum = 0; // remember the coloum
    QMap<int, QVector<float>>::const_iterator _spectraIterator = m_spectra.begin(); //

    while ( _spectraIterator != m_spectra.end() ) {
        if( m_method == 0 ){ // 每四项做平方和
            QList<float> temp;
            size_t row__{0};
            for(size_t _row = 0; _row < m_data_len; _row++ ) {
                temp.append( _spectraIterator.value()[_row] );
                if( temp.size() >= m_cal_count ){
                    float sum{0};
                    for( float t:temp ){
                        sum += pow(t,2);
                    }
                    float d_ = sqrt(sum);

                    const color& _color = generateColor(d_);
                    size_t _position    = _coloum * m_uint_height + row__;
                    data[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                    data[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                    data[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                    temp.clear();
                    row__++;
                }
            }

        }else if( m_method == 1 ){ // 每四项取最大值
            float max_{0.0};
            size_t row__{0};
            for(size_t _row = 0; _row < m_data_len; _row++ ) {
                max_ = _spectraIterator.value()[_row] > max_?_spectraIterator.value()[_row]:max_;
                if( (_row+1)%m_cal_count == 0 ){
                    const color& _color = generateColor(max_);
                    size_t _position    = _coloum * m_uint_height + row__;
                    data[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                    data[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                    data[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
                    max_ = 0.0;
                    row__++;
                }
            }

        }else if( m_method == 2 ){ // 不做处理
            for(size_t _row = 0; _row < m_uint_height; _row++ ) {
                const color& _color = generateColor(_spectraIterator.value()[_row]);

                size_t _position    = _coloum * m_uint_height + _row;
                data[_position*3]   = static_cast<unsigned char>(_color.red);   // red value
                data[_position*3+1] = static_cast<unsigned char>(_color.green); // green value
                data[_position*3+2] = static_cast<unsigned char>(_color.blue);  // blue value
            }
        }
        _coloum++;
        _spectraIterator++;
    }

    emit sendSpectraData(data);
}
