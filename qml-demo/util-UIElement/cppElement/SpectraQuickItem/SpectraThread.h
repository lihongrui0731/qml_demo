#ifndef SPECTRATHREAD_H
#define SPECTRATHREAD_H

#include <QObject>
#include <QQuickItem>
#include <QMetaType>
#include <QVariant>
#include <QCoreApplication>
#include <QThread>
#include <QList>
#include <QMap>

class SpectraThread : public QThread
{
    Q_OBJECT

    struct color{
        explicit color(int red_, int green_, int blue_):
            red(red_), green(green_), blue(blue_) {}
        int red{255};
        int green{255};
        int blue{255};
    };
public:
    SpectraThread();
    ~SpectraThread();

    void setSpectraData(float amplitudeMax , float amplitudeMin , const unsigned int & width, const unsigned int& height, const QMap<int, QVector<float>>& spectra , quint8 method );

protected:
    virtual void run() override;

private:
    QMap<int, QVector<float>> m_spectra;

    quint8 m_method{0}; // 0 每4个值平方和开根 ， 1 最大值 ，2 不处理
    quint8 m_cal_count{4}; // 合并计算的数值个数

    unsigned char *m_spectra_data{nullptr};
    SpectraThread::color generateColor( float value );

    float m_amplitudeMin{0};
    float m_amplitudeMax{80};

    unsigned int m_uint_width;
    unsigned int m_uint_height;
    unsigned int m_data_len;
    /// 待绘制的内容

    void sortSpectraData();

signals:
    //void sendData( QVariant p_data );
    void sendSpectraData( unsigned char * data );

};

#endif // SPECTRATHREAD_H
