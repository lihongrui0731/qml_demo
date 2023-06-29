#ifndef CLINECHART_H
#define CLINECHART_H

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
#include <QDateTime>
#include "qcustomplot.h"
#include "math.h"
#include <QTimerEvent>
#include <QBrush>

#include "dataType.h"

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif


/**
 * @brief 绘制瞬时频谱分布曲线 单独
 */
class CLineChart : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(int frameLen READ frameLen WRITE setFrameLength NOTIFY frameLengthChanged);
    Q_PROPERTY(float xAxisMin READ xAxisMin WRITE setxAxisMin NOTIFY xAxisMinChanged);
    Q_PROPERTY(float xAxisMax READ xAxisMax WRITE setxAxisMax NOTIFY xAxisMaxChanged);
    Q_PROPERTY(float yAxisMin READ yAxisMin WRITE setyAxisMin NOTIFY yAxisMinChanged);
    Q_PROPERTY(float yAxisMax READ yAxisMax WRITE setyAxisMax NOTIFY yAxisMaxChanged);

    Q_PROPERTY(float xAxisUnit READ xAxisUnit WRITE setxAxisUnit NOTIFY xAxisUnitChanged);
    Q_PROPERTY(float yAxisUnit READ yAxisUnit WRITE setyAxisUnit NOTIFY yAxisUnitChanged);

    Q_PROPERTY(QString xAxisTitle READ xAxisTitle WRITE setxAxisTitle NOTIFY xAxisTitleChanged);
    Q_PROPERTY(QString yAxisTitle READ yAxisTitle WRITE setyAxisTitle NOTIFY yAxisTitleChanged);

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged );
    Q_PROPERTY(float linePos READ linePos WRITE setLinePos NOTIFY linePosChanged );
    Q_PROPERTY(float dt READ dt WRITE setDt NOTIFY dtChanged );
    Q_PROPERTY(float df READ df WRITE setDf NOTIFY dfChanged ); // 分辨率

    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(float dpi READ dpi WRITE setDpi NOTIFY dpiChanged )

    Q_PROPERTY(bool isDB READ isDB WRITE setDB NOTIFY isDBChanged )
    Q_PROPERTY(bool isLG READ isLG WRITE setLG NOTIFY isLGChanged )
    Q_PROPERTY(QString bgColor READ bgcolor WRITE setBgcolor NOTIFY bgcolorChanged );
    Q_PROPERTY(QString textColor READ textColor WRITE setTextColor NOTIFY textColorChanged);

    struct point {
        float x;
        float y;
    };

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
    explicit CLineChart(QQuickItem *parent = nullptr);
    virtual ~CLineChart() override;

    Q_INVOKABLE void exportCSV( QString path , QString channelName , QString dataType );
    Q_INVOKABLE bool saveCSV( QString path , QString channelName , QString dataType );

    Q_INVOKABLE void paint( QPainter* painter ) override;

    /** ********************************************************************************************
     * @brief resetCurveLine
     * this reset all the datas for drawing curve lines
     * **********************************************************************************************/
    Q_INVOKABLE void resetCurveLine();

    Q_INVOKABLE void timeLineChanged( int size );
    Q_INVOKABLE void timeLineChanged();

    // 绘制实时刷新的数据
    Q_INVOKABLE void addJsonData( QJsonObject data );
    Q_INVOKABLE void addData( QList<float> data , int len );

    // 绘制连续数据
    Q_INVOKABLE void addJsonBatchData( QJsonObject data );

    Q_INVOKABLE void changeSize( float x , int v );

    Q_INVOKABLE void rePaint(){
        setRange();

        m_ppixMap->fill(QColor(m_bgcolor));
        m_pcustomPlot->setBackground(*m_ppixMap);

        if( m_xAxisTitle != "" ){
            m_pcustomPlot->xAxis->setLabel( m_xAxisTitle );
        }
        if( m_yAxisTitle != "" ){
            m_pcustomPlot->yAxis->setLabel( m_yAxisTitle );
        }

        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);

        m_pcustomPlot->xAxis->rescale(true);
        XminToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMin );
        XmaxToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMax );
    }

    Q_INVOKABLE void reflash(){
         update();
    }

    Q_INVOKABLE void reSize(){
        m_scaleRange = 0; // 放大倍数
        m_scalePoint = 0; // 放大中心点
        setRange();

        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);

        m_pcustomPlot->xAxis->rescale(true);
        XminToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMin );
        XmaxToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMax );
        update();
    }

    Q_INVOKABLE void clearData(){
        _vector_keys.clear();
        _vector_datas.clear();
        m_pcustomPlot->graph(0)->data()->clear();
    }

    Q_INVOKABLE void reInitial(){

        if( m_pcustomPlot != nullptr ){
            m_pcustomPlot->clearPlottables();
            delete m_pcustomPlot;
            m_pcustomPlot = nullptr;
        }
        m_scaleRange = 0; // 放大倍数
        m_scalePoint = 0; // 放大中心点
        initializePlot();

        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);

        m_pcustomPlot->xAxis->rescale(true);
        m_pcustomPlot->yAxis->rescale(true);

        XminToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMin );
        XmaxToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMax );
        if( _vector_datas.size() > 0 ){
            m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas);
        }
        update();
    }

    Q_INVOKABLE void reload();

    Q_INVOKABLE void move( float orgX , float newX );
    Q_INVOKABLE void yAxisSelfChange( float max , float min , int num );

    int frameLen(){ return m_len; }
    float xAxisMin(){ return m_xAxisMin; }
    float xAxisMax(){ return m_xAxisMax; }
    float yAxisMin(){ return m_yAxisMin; }
    float yAxisMax(){ return m_yAxisMax; }

    float xAxisUnit(){ return m_xAxisUnit; }
    float yAxisUnit(){ return m_yAxisUnit; }

    QString xAxisTitle(){ return m_xAxisTitle; }
    QString yAxisTitle(){ return m_yAxisTitle; }

    QString bgcolor(){ return m_bgcolor; }
    QString textColor() {return m_textColor;}

    bool isDB(){ return m_isDB; }
    bool isLG(){ return m_isLG; }

    int fontSize() const { return m_fontSize; }
    float dpi() const { return m_dpi; }
    float dt() const {return m_dt;}
    float df() const {return m_df;}
    float linePos() const { return m_linePos; }
    float currentIndex() const { return m_i_currentTime; }

    bool checkRange();

    void setxAxisMin( float xAxisMin_ ){ if( xAxisMin_ == m_xAxisMin) return; m_xAxisMin = xAxisMin_; }
    void setxAxisMax( float xAxisMax_ ){ if( xAxisMax_ == m_xAxisMax) return; m_xAxisMax = xAxisMax_; }
    void setxAxisUnit( float xAxisUnit_ ){ if( xAxisUnit_ == m_xAxisUnit) return; m_xAxisUnit = xAxisUnit_; }
    void setyAxisUnit( float yAxisUnit_ ){ if( yAxisUnit_ == m_yAxisUnit) return; m_yAxisUnit = yAxisUnit_; }

    void setyAxisMin( float yAxisMin_ ){ if( yAxisMin_ == m_yAxisMin) return; m_yAxisMin = yAxisMin_; }
    void setyAxisMax( float yAxisMax_ ){ if( yAxisMax_ == m_yAxisMax) return; m_yAxisMax = yAxisMax_; }

    void setxAxisTitle( QString title_ ){ m_xAxisTitle = title_; }
    void setyAxisTitle( QString title_ ){ m_yAxisTitle = title_; }

    void setFrameLength( int len ){
        m_len = len;
    }
    void setCurrentIndex( int index_ ){
        m_i_currentTime = index_;
    }

    void setLinePos( float pos_ );
    Q_INVOKABLE void setCurrentPos( int current_ );

    void setDt( float dt_ ){
        m_dt =  dt_;
    }
    void setDf( float df_ ){
        m_df = df_;
    }

    void setDB( bool isDB_ ){
        m_isDB = isDB_;
        reload();
    }
    void setLG( bool isLG_ ){
        m_isLG = isLG_;
        changeScaleType();
    }

    void changeScaleType();
    void setBgcolor( QString bgcolor_ ){
        m_bgcolor = bgcolor_;
    }
    void setTextColor(QString color) {
        m_textColor = color;
    }

    void setFontSize( int fontSize_ );
    void setDpi( float dpi );
    void setData( const size_t& dataSize_ , float *data );
    // 累计数据 右侧进入
    void setRBatchData( const size_t& dataSize_ , float *data , int dataLength );
public slots:
  //  void scaleAndMove(const float& scale, const float& move);

signals:
    void drawFrequency();
    void finish();

    void frameLengthChanged();
    void xAxisMinChanged();
    void xAxisMaxChanged();
    void yAxisMinChanged();
    void yAxisMaxChanged();
    void xAxisUnitChanged();
    void yAxisUnitChanged();
    void dtChanged();
    void dfChanged();

    void fontSizeChanged();
    void dpiChanged();
    void linePosChanged();
    void currentIndexChanged();
    void bgcolorChanged();
    void textColorChanged();

    void xAxisTitleChanged();
    void yAxisTitleChanged();

    void isDBChanged();
    void isLGChanged();

private:
    QCustomPlot* m_pcustomPlot{nullptr};
    QPixmap* m_ppixMap{nullptr};
    QCPItemTracer *m_ptracer{nullptr}; //跟踪的点
    QCPItemText *m_plabel{nullptr};//显示的数值
    QCPItemStraightLine* m_plineV{nullptr};

    bool m_pvisible{false};

    int m_i_timerID{0};

    int m_scaleRange{0}; // 放大倍数
    float m_scalePoint{0.0}; // 放大中心点
    float m_scaleMove{0}; // 移动偏移

    float m_xAxisMin{0.0};
    float m_xAxisMax{4000.0};
    float m_yAxisMin{0.0};
    float m_yAxisMax{1.0};
    float m_xAxisUnit{400.0};
    float m_yAxisUnit{0.2};

    QString m_xAxisTitle;
    QString m_yAxisTitle;

    QString m_bgcolor{"#FFFFFF"};
    QString m_textColor{"#595b5d"};

    // x轴对应控件的位置
    float XmaxToPos{0};
    float XminToPos{0};

    int m_len{ static_cast<int>( ( m_xAxisMax - m_xAxisMin) / m_xAxisUnit) };

    int m_i_currentTime{0};
    int m_i_local_currentTime{0};

    bool m_yflash{false};
    bool isChangeSize{false};

    bool m_isLG{false};
    bool m_isDB{false};

    float m_dpi{static_cast<float>(qApp->devicePixelRatio())};
    int m_fontSize{static_cast<int>(qApp->font().pixelSize()>0? qApp->font().pixelSize() / m_dpi:10/m_dpi)};
    float m_dt{0.2};
    float m_linePos{0.0};
    float m_df{1};

    QVector<double> _vector_keys;
    QVector<double> _vector_datas;
    QVector<double> _vector_org_datas;

    void initializePlot();
    void updateCustomPlotSize();
    void setRange();
    void setXticks();
    void setYticks();

    point getYbyX( float &x );
};

#endif // CLINECHART_H
