/** *************************************************************
  * @brief
  * this is show the frequence curve line
  * @author guweicao
  * @copyright Rhythem
  * @date 20121/5/5
  ***************************************************************/
#ifndef INSTANT_SPECTRUM_CHART_H
#define INSTANT_SPECTRUM_CHART_H

#include <QWidget>
#include <QPixmap>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>

#include <QApplication>
#include <QThread>
#include <QMetaType>

#include <QVariant>
#include <QVariantMap>
#include <QVector>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "qcustomplot.h"
#include "math.h"
//#include "tdmsthread.h"

#include "dataType.h"

/**
 * @brief 绘制瞬时频谱分布曲线
 */
class InstantSpectrumChart : public QQuickPaintedItem
{
    Q_OBJECT

public:
    Q_PROPERTY(int frameLen READ frameLen WRITE setFrameLength NOTIFY frameLengthChanged)
    Q_PROPERTY(float amplitudeMax READ amplitudeMax WRITE setAmplitudeMax NOTIFY amplitudeMaxChanged)
    Q_PROPERTY(float amplitudeMin READ amplitudeMin WRITE setAmplitudeMin NOTIFY amplitudeMinChanged)
    Q_PROPERTY(float frequenceWidth READ frequenceWidth WRITE setFrequenceWidth NOTIFY frequenceWidthChanged)

    Q_PROPERTY(float xAxisMin READ xAxisMin WRITE setxAxisMin NOTIFY xAxisMinChanged)
    Q_PROPERTY(float xAxisMax READ xAxisMax WRITE setxAxisMax NOTIFY xAxisMaxChanged)
    Q_PROPERTY(float yAxisMin READ yAxisMin WRITE setyAxisMin NOTIFY yAxisMinChanged)
    Q_PROPERTY(float yAxisMax READ yAxisMax WRITE setyAxisMax NOTIFY yAxisMaxChanged)

    Q_PROPERTY(float xAxisUnit READ xAxisUnit WRITE setxAxisUnit NOTIFY xAxisUnitChanged)
    Q_PROPERTY(float yAxisUnit READ yAxisUnit WRITE setyAxisUnit NOTIFY yAxisUnitChanged)

    Q_PROPERTY(bool ln READ ln WRITE setLn NOTIFY lnChanged)

    struct points {
        points(const int& dataSize ) {
            xAxis = new QVector<double>(dataSize);
            yAxis = new QVector<double>(dataSize);
        }
        QVector<double>* xAxis{nullptr};
        QVector<double>* yAxis{nullptr};
        ~points() {
            if ( xAxis )
                delete xAxis;
            if ( yAxis )
                delete yAxis;
        }
    };

public:
    explicit InstantSpectrumChart(QQuickItem *parent = nullptr);
    virtual ~InstantSpectrumChart() override;

    void paint( QPainter* painter ) override;

public:
    /** ********************************************************************************************
     * @brief resetCurveLine
     * this reset all the datas for drawing curve lines
     * **********************************************************************************************/
    Q_INVOKABLE void resetCurveLine();

    /** ********************************************************************************************
     * @brief spectrumPlay
     ***********************************************************************************************/
    Q_INVOKABLE void spectrumPlay();

    /** ********************************************************************************************
     * @brief spectrumStop
     ***********************************************************************************************/
    Q_INVOKABLE void spectrumStop();

    Q_INVOKABLE void timeLineChanged( int size );
    Q_INVOKABLE void timeLineChanged();

    Q_INVOKABLE void setData( QVariant data );
    Q_INVOKABLE void addData( QVariant data );
    Q_INVOKABLE void addLocalData( QVariant data );

    Q_INVOKABLE void addJsonData( QJsonObject data );

    Q_INVOKABLE void setRange();
    Q_INVOKABLE void reflash(){
        update();
    };

    Q_INVOKABLE void setFinalSpectraData( QVariant data_map );
    Q_INVOKABLE void setFinalSpectraLocalData( QVariant data_map );

    /** ********************************************************************************************
     * @brief setSpectraData
     * @param dataSize_       @brief set every string data size
     * @param datamap_        @brief set the spectra map
     ***********************************************************************************************/
    void setSpectraData(const size_t& dataSize_, const std::map<int, float*> datamap_);

    /** ********************************************************************************************
     * @brief setSpectraData
     * @param dataSize_      @brief set current curve data size
     * @param data           @brief set current curve data
     ***********************************************************************************************/
    void setSpectraData(const size_t& dataSize_, const float* data);

    /** ********************************************************************************************
     * @brief setLocalSpectraData draw local spectra freq curve
     * @param dataSize_               @brief set local spectra string data size
     * @param datamap_                @brief set loca spectra map
     ***********************************************************************************************/
    void setLocalSpectraData(const size_t& dataSize_, const std::map<int, float*> datamap_);

    /** ********************************************************************************************
     * @brief setLocalSpectraData draw spectra freq curve
     * @param dataSize_                @brief set local spectra data size
     * @param data                     @brief set lobal spectra data (float*)
     ***********************************************************************************************/
    void setLocalSpectraData(const size_t& dataSize_, const float* data);

    int frameLen(){ return m_len; }
    float amplitudeMax(){ return m_amplitudeMax; }
    float amplitudeMin(){ return m_amplitudeMin; }
    float frequenceWidth(){ return m_frequency_width; }

    float xAxisMin(){ return m_xAxisMin; }
    float xAxisMax(){ return m_xAxisMax; }
    float yAxisMin(){ return m_yAxisMin; }
    float yAxisMax(){ return m_yAxisMax; }

    float xAxisUnit(){ return m_xAxisUnit; }
    float yAxisUnit(){ return m_yAxisUnit; }
    bool ln(){ return m_ln; }

    void setxAxisMin( float xAxisMin_ ){ if( xAxisMin_ == m_xAxisMin) return; m_xAxisMin = xAxisMin_; setRange(); m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);}
    void setxAxisMax( float xAxisMax_ ){ if( xAxisMax_ == m_xAxisMax) return; m_xAxisMax = xAxisMax_; setRange(); m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);}
    void setyAxisMin( float yAxisMin_ ){ if( yAxisMin_ == m_yAxisMin) return; m_yAxisMin = yAxisMin_; setRange(); m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);}
    void setyAxisMax( float yAxisMax_ ){ if( yAxisMax_ == m_yAxisMax) return; m_yAxisMax = yAxisMax_; setRange(); m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);}

    void setxAxisUnit( float xAxisUnit_ ){ if( xAxisUnit_ == m_xAxisUnit) return; m_xAxisUnit = xAxisUnit_; setRange(); m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);}
    void setyAxisUnit( float yAxisUnit_ ){ if( yAxisUnit_ == m_yAxisUnit) return; m_yAxisUnit = yAxisUnit_; setRange(); m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);}

    void setFrameLength( int len ){
        m_len = len;
    }
    void setAmplitudeMax( float amplitudeMax_ ){
        m_amplitudeMax = amplitudeMax_;
    }
    void setLn( bool v){
        m_ln = v;
    }
    void setAmplitudeMin( float amplitudeMin_ ){
        m_amplitudeMin = amplitudeMin_;
    }
    void setFrequenceWidth( float frequenceWidth_ ){
        m_frequency_width = frequenceWidth_;
    }


public slots:

    /**
     * 在集合中定位指定位置的帧，绘制图形
     */
    void seekPercentage(const float& position);
    void scaleAndMove(const float& scale, const float& move);

signals:
    void sendPath( QString file );
    void drawFrequency();
    void finish();

    void frameLengthChanged();
    void amplitudeMaxChanged();
    void amplitudeMinChanged();
    void frequenceWidthChanged();

    void xAxisMinChanged();
    void xAxisMaxChanged();
    void yAxisMinChanged();
    void yAxisMaxChanged();
    void xAxisUnitChanged();
    void yAxisUnitChanged();
    void lnChanged();

protected:
   // void timerEvent(QTimerEvent*event_);
   // void resizeEvent(QResizeEvent*event_) override;

private:
    QCustomPlot* m_pcustomPlot{nullptr};
    QPixmap* m_ppixMap{nullptr};
    int m_i_timerID{0};

    // spectrum
    std::map<int, points*> m_map_spectrum;
    std::map<int, points*>::const_iterator m_const_spectrumItor;
    // local_spectrum
    std::map<int, points*> m_map_local_spectrum;
    std::map<int, points*>::const_iterator m_const_local_spectrumItor;

    float m_amplitudeMin{0.0};
    float m_amplitudeMax{80.0};
    float m_frequencyLimitMin{0};
    float m_frequencyLimitMax{0};
    bool m_ln{false};

    float m_xAxisMin{0.0};
    float m_xAxisMax{100.0};
    float m_yAxisMin{0.0};
    float m_yAxisMax{25.0};
    float m_xAxisUnit{20.0};
    float m_yAxisUnit{5.0};

    int m_len{1024};
    float m_frequency_width{24.0};

    int m_i_currentTime{0};
    int m_i_local_currentTime{0};

    float m_dpi{static_cast<float>(qApp->devicePixelRatio())};

    /**
     * @brief initializePlot
     * initialize all the points, for showing curve line
     */
    void initializePlot();
    void updateCustomPlotSize();

    SpectrumData spectrumData;
    // tdmsThread * m_Thread;
};

#endif // INSTANT_SPECTRUM_CHART_H
