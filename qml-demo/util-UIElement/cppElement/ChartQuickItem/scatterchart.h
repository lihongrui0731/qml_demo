#ifndef SCATTERCHART_H
#define SCATTERCHART_H

#include <QWidget>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QThread>
#include <QLabel>
#include <QtCharts>
#include <QVector>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <QMetaType>

#include "qcustomplot.h"
#include "math.h"
#include "scatterthread.h"

class ScatterChart : public QQuickPaintedItem{
    Q_OBJECT
    struct color{
        explicit color(int red_, int green_, int blue_):
            red(red_), green(green_), blue(blue_) {}
        int red{255};
        int green{255};
        int blue{255};
    };
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(float dpi READ dpi WRITE setDpi NOTIFY dpiChanged )
    Q_PROPERTY(QString xAxisTitle READ xAxisTitle WRITE setxAxisTitle NOTIFY xAxisTitleChanged);
    Q_PROPERTY(QString yAxisTitle READ yAxisTitle WRITE setyAxisTitle NOTIFY yAxisTitleChanged);
    Q_PROPERTY(bool useOpengl READ useOpengl WRITE setUseOpengl NOTIFY useOpenglChanged );
    Q_PROPERTY(QString bgColor READ bgColor WRITE setBgColor NOTIFY bgColorChanged);
    Q_PROPERTY(QString textColor READ textColor WRITE setTextColor NOTIFY textColorChanged);

public:
    explicit ScatterChart(QQuickItem *parent = nullptr);
    ~ScatterChart();

    void paint(QPainter *painter) override;

    Q_INVOKABLE void addData( QJsonObject data );
    Q_INVOKABLE void setData( QJsonObject data );
    Q_INVOKABLE void initData();
    Q_INVOKABLE void setFontSize( int fontSize_ );
    Q_INVOKABLE void setDpi( float dpi_ );
    Q_INVOKABLE void changeTimeLine( int timeline );
    Q_INVOKABLE void reset();
    Q_INVOKABLE void rePaint(){
        setRange();

        m_ppixMap->fill(QColor(m_background));
        m_pcustomPlot->setBackground(*m_ppixMap);

        if( m_xAxisTitle != "" ){
            m_pcustomPlot->xAxis->setLabel( m_xAxisTitle );
        }
        if( m_yAxisTitle != "" ){
            m_pcustomPlot->yAxis->setLabel( m_yAxisTitle );
        }

        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);

//        m_pcustomPlot->xAxis->rescale(true);
//        XminToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMin );
//        XmaxToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMax );
    }

    QString xAxisTitle(){ return m_xAxisTitle; }
    QString yAxisTitle(){ return m_yAxisTitle; }
    bool useOpengl(){ return b_useOpengl; }

    int fontSize() const { return m_fontSize; }
    float dpi() const { return m_dpi; }
    void setxAxisTitle( QString title_ ){ m_xAxisTitle = title_; }
    void setyAxisTitle( QString title_ ){ m_yAxisTitle = title_; }
    void setUseOpengl( bool useOpengl_ ){
        b_useOpengl = useOpengl_;
        m_pcustomPlot->setOpenGl(b_useOpengl);
    }

    QString bgColor(){ return m_background; }
    QString textColor() {return m_textColor;}
    void setBgColor( QString bgcolor_ ){
        m_background = bgcolor_;
    }
    void setTextColor(QString color) {
        m_textColor = color;
    }
public slots:
   // void generateData(int seriesCount, int rowCount, int colCount);

    void getDataFromThread( QCPColorMap* colorMap_ );

private:
    QCustomPlot* m_pcustomPlot{nullptr};
    QPixmap* m_ppixMap{nullptr};
    QCPColorMap *m_heatmap{nullptr};
    QCPColorScale *m_colorScale{nullptr};

    QMap<quint32 , QJsonObject> m_data;

    QString m_xAxisTitle{"时间（ms）"};
    QString m_yAxisTitle{"幅值（dB）"};

    int m_index;
    quint8 m_line_count{16};
    unsigned int m_i_currentTime{0};
    bool b_useOpengl{true};

    ScatterChart::color generateColor( float max , float min, float value );
    QString m_background;
    QString m_textColor{"#595b5d"};

    void initializePlot();
    void initHeatMap();
    void updateCustomPlotSize();
    void setRange();
    float m_dpi{static_cast<float>(qApp->devicePixelRatio())};
    int m_fontSize{ static_cast<int>( qApp->font().pixelSize() / m_dpi ) };

    ScatterThread *scatterThread{nullptr};
    QThread *thread{nullptr};
signals:
    void fontSizeChanged();
    void dpiChanged();
    void xAxisTitleChanged();
    void yAxisTitleChanged();
    void useOpenglChanged();
    void addDataInThread( QCPColorMap* , QJsonObject );
    void bgColorChanged();
    void textColorChanged();
};

#endif // SCATTERCHART_H
