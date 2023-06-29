#include "scatterchart.h"
#include "math.h"
#include <QElapsedTimer>

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif

ScatterChart::ScatterChart( QQuickItem *parent ) : QQuickPaintedItem(parent),m_index(-1)
{
    qRegisterMetaType<QCPColorMap*>("QCPColorMap*");

    setFlag( QQuickItem::ItemHasContents, true );
    // connect( this, &QQuickPaintedItem::widthChanged, this, &ScatterChart::updateCustomPlotSize );
    // connect( this, &QQuickPaintedItem::heightChanged, this, &ScatterChart::updateCustomPlotSize );

    initializePlot();
    initHeatMap();

    thread = new QThread();
    scatterThread = new ScatterThread();
    scatterThread->setParams( m_line_count );
    scatterThread->moveToThread(thread);

    connect( scatterThread, &ScatterThread::finished, thread, &QThread::quit);
    connect( scatterThread, &ScatterThread::finished, scatterThread, &ScatterThread::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    connect( this , &ScatterChart::addDataInThread, scatterThread , &ScatterThread::addData , Qt::QueuedConnection );

    connect( scatterThread , &ScatterThread::sendData , this , &ScatterChart::getDataFromThread , Qt::QueuedConnection );

    thread->start( QThread::LowPriority );

}

ScatterChart::~ScatterChart(){
    if( m_pcustomPlot != nullptr ){
        delete m_pcustomPlot;
        m_pcustomPlot = nullptr;
    }
    if( m_ppixMap != nullptr ){
        delete m_ppixMap;
        m_ppixMap = nullptr;
    }

    if( scatterThread != nullptr ){
        scatterThread->quit();
        delete scatterThread;
        scatterThread = nullptr;
    }



#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif

}

void ScatterChart::initializePlot(){
    if( m_ppixMap != nullptr ){
        delete m_ppixMap;
        m_ppixMap = nullptr;
    }

    if( m_pcustomPlot != nullptr ){
        delete m_pcustomPlot;
        m_pcustomPlot = nullptr;
    }

    m_ppixMap = new QPixmap(100, 100);
    m_ppixMap->fill(m_background);

    QPen _pen;
    _pen.setColor(Qt::lightGray);
    _pen.setStyle(Qt::DotLine);

    QPen _axisPen(Qt::lightGray);

    m_pcustomPlot = new QCustomPlot();
    m_pcustomPlot->setOpenGl(b_useOpengl);
    //m_pcustomPlot->setBufferDevicePixelRatio(1);
    m_pcustomPlot->plotLayout()->setRowSpacing(0);
    m_pcustomPlot->plotLayout()->setMargins(QMargins(0,0,0,0));
    m_pcustomPlot->plotLayout()->setMinimumMargins( QMargins(0,0,0,0) );
    m_pcustomPlot->setBackground(*m_ppixMap);

    QFont _font = qApp->font();
    _font.setPixelSize(m_fontSize);
    _font.setBold(false);

    m_pcustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
    m_pcustomPlot->axisRect()->setMinimumMargins( QMargins(5.0 / m_dpi ,15.0 / m_dpi ,5.0 / m_dpi ,2.0 / m_dpi) );

    m_pcustomPlot->xAxis->setTickLabelFont(_font);
    m_pcustomPlot->yAxis->setTickLabelFont(_font);

    m_pcustomPlot->xAxis->setSubTicks(false);
    m_pcustomPlot->xAxis->setTickLabelPadding(2.0 / m_dpi);
    m_pcustomPlot->xAxis->grid()->setAntialiasedZeroLine(false);
    m_pcustomPlot->xAxis->grid()->setSubGridVisible(false);
    m_pcustomPlot->xAxis->setTickLength(0, 3.0/m_dpi);
    m_pcustomPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_pcustomPlot->xAxis->setTickLabelColor(Qt::black);
    m_pcustomPlot->xAxis->setTickPen(_axisPen);
    m_pcustomPlot->xAxis->grid()->setPen(_pen);
    m_pcustomPlot->xAxis->setBasePen(_axisPen);

    if( m_xAxisTitle != "" ){
        m_pcustomPlot->xAxis->setLabel( m_xAxisTitle );
    }
    m_pcustomPlot->xAxis->setLabelPadding(0);
    m_pcustomPlot->xAxis->setLabelFont( _font );
    m_pcustomPlot->xAxis->setLabelColor( m_textColor );
    m_pcustomPlot->xAxis->setAntialiased(true);

    m_pcustomPlot->yAxis->setVisible(true);
    m_pcustomPlot->yAxis->setTicks(true);
    m_pcustomPlot->yAxis->setTickLabels(true);
    m_pcustomPlot->yAxis->setTickLabelPadding(2.0 / m_dpi);

    m_pcustomPlot->yAxis->grid()->setAntialiasedZeroLine(false);
    m_pcustomPlot->yAxis->grid()->setZeroLinePen(_axisPen);
    m_pcustomPlot->yAxis->setTickPen(_axisPen);
    m_pcustomPlot->yAxis->setTickLabelColor(m_textColor);
    m_pcustomPlot->yAxis->grid()->setPen(_pen);
    m_pcustomPlot->yAxis->setBasePen(_axisPen);
    m_pcustomPlot->yAxis->setTickLength(0, 3.0/m_dpi);
    m_pcustomPlot->yAxis->setPadding(0);
    m_pcustomPlot->yAxis->setAntialiased(true);
    if( m_yAxisTitle != "" ){
        m_pcustomPlot->yAxis->setLabel( m_yAxisTitle );
    }
    m_pcustomPlot->yAxis->setLabelPadding(0);
    m_pcustomPlot->yAxis->setLabelFont( _font );
    m_pcustomPlot->yAxis->setLabelColor( m_textColor );

    m_pcustomPlot->xAxis2->setVisible(false);
    m_pcustomPlot->xAxis2->setTickLabels(false);
    m_pcustomPlot->xAxis2->setTicks(false);
    m_pcustomPlot->xAxis2->grid()->setPen(_pen);
    m_pcustomPlot->xAxis2->setBasePen(_axisPen);
    m_pcustomPlot->xAxis2->setTickLength(0, 0);
    m_pcustomPlot->xAxis2->setLabelPadding(0);
    m_pcustomPlot->xAxis2->setTickLabelPadding(0);
    m_pcustomPlot->xAxis2->setPadding(0);

    m_pcustomPlot->yAxis2->setVisible(false);
    m_pcustomPlot->yAxis2->setTicks(false);
    m_pcustomPlot->yAxis2->setSubTicks(false);
    m_pcustomPlot->yAxis2->setTickLength(0, 0);
    m_pcustomPlot->yAxis2->setTickLabelColor(m_textColor);
    m_pcustomPlot->yAxis2->setTickPen(_axisPen);
    m_pcustomPlot->yAxis2->grid()->setPen(_pen);
    m_pcustomPlot->yAxis2->setBasePen(_axisPen);
    setRange();
}

void ScatterChart::initHeatMap(){

    m_heatmap = new QCPColorMap(m_pcustomPlot->xAxis, m_pcustomPlot->yAxis);  // 构造一个颜色图
    m_heatmap->data()->setSize(200,120);
    m_heatmap->data()->setRange( QCPRange(0.5,199.5) , QCPRange(0.5,119.5) );

    initData();
    m_i_currentTime = 0;

    m_colorScale = new QCPColorScale(m_pcustomPlot);  // 构造一个色条
    m_colorScale->setType(QCPAxis::atRight);   // 水平显示
    m_colorScale->setBarWidth(10.0/m_dpi);

    QVector<double> ticks;
    QVector<QString> labels;
    for( int i = 0; i <= m_line_count; i += m_line_count / 4 ){
        //ticksY << 0 << 5 << 10 << 15 << 20 << 25 ;
        //labelsY << ""<<""<<""<<""<<""<<"";
        ticks << i;
        labels << QString::number(i);
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    m_colorScale->axis()->setTicker(textTicker);

    QFont font = qApp->font();
    font.setPixelSize(m_fontSize);
    font.setBold(false);

    m_colorScale->axis()->setTickLabelFont(font);
    m_colorScale->axis()->setTickLabels(true);
    m_colorScale->axis()->setTickLength(3.0 / m_dpi);

    m_pcustomPlot->plotLayout()->addElement(0, 1, m_colorScale); // 在颜色图下面显示
    m_heatmap->setColorScale(m_colorScale);
    QCPColorGradient gradient;  // 色条使用的颜色渐变
    // gradient.setColorStopAt(0.0, QColor("#f6efa6"));
    for( quint8 i = 0; i <= m_line_count; i++ ){
        color color_ = generateColor( m_line_count+1 , 0 , i );
        gradient.setColorStopAt(1.0 / m_line_count * i, QColor( color_.red , color_.green , color_.blue ) );
    }

    m_heatmap->setGradient(gradient);
    //    colorMap->rescaleDataRange();        // 自动计算数据范围，数据范围决定了哪些数据值映射到QCPColorGradient的颜色渐变当中
    m_heatmap->setDataRange(QCPRange(0, m_line_count));     // 为了保持与echart的例子一致，我们这里手动设置数据范围
    m_heatmap->setInterpolate(false);

    // 保持色条与轴矩形边距一致
    QCPMarginGroup *marginGroup = new QCPMarginGroup(m_pcustomPlot);
    m_pcustomPlot->axisRect()->setMarginGroup(QCP::msTop | QCP::msBottom, marginGroup);
    m_colorScale->setMarginGroup(QCP::msTop | QCP::msBottom, marginGroup);

    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);

}

void ScatterChart::setRange(){

    // set all axis range
    m_pcustomPlot->xAxis->setRange(0, 200);
    m_pcustomPlot->yAxis->setRange(0, 120);

    // set xAxis labels
    QVector<double> ticks;
    QVector<QString> labels;

    for( int i = 0; i <= 200; i += 20 ){
        ticks << i;
        labels << QString::number( static_cast<float>((float)i/10.0) );
    }

    // labels << "  100" << "" << "80" << "" << "60" << "" << QStringLiteral("dB(A)") << "" << "20" << "" << "0" <<""<<"-20";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    m_pcustomPlot->xAxis->setTicker(textTicker);

    // set yAxis labels
    QVector<double> ticksY;
    QVector<QString> labelsY;

    for( int i = 0; i <= 120; i += 20 ){
        ticksY << i;
        labelsY << QString::number(i);
    }

    QSharedPointer<QCPAxisTickerText> textTickerY(new QCPAxisTickerText);
    textTickerY->addTicks(ticksY, labelsY);
    m_pcustomPlot->yAxis->setTicker(textTickerY);
}


ScatterChart::color ScatterChart::generateColor( float max ,float min , float value)
{
    const float float_epsilon { std::numeric_limits<float>::epsilon() };
    Q_UNUSED(float_epsilon);
    if ( std::isinf( value ))
        return color(0, 0, 0);

    if (value < min) value = min;
    else if (value > max) value = max;

    float dv = max - min;

    float v = (value - min) / dv;
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

void ScatterChart::paint(QPainter *painter){
    if (m_pcustomPlot != nullptr )
    {
        updateCustomPlotSize();
        QPixmap picture( width() * qApp->devicePixelRatio() , height() * qApp->devicePixelRatio() );
        picture.setDevicePixelRatio(qApp->devicePixelRatio());
        picture.fill(Qt::transparent);
        QCPPainter qcpPainter( &picture );
        m_pcustomPlot->toPainter( &qcpPainter );

        painter->drawPixmap( QPoint(), picture );
    }
}

void ScatterChart::setFontSize( int fontSize_){
    if( m_fontSize != fontSize_ ){
        m_fontSize = fontSize_;
        QFont _font = qApp->font();
        _font.setPixelSize(m_fontSize);
        _font.setBold(false);

        m_pcustomPlot->xAxis->setTickLabelFont(_font);
        m_pcustomPlot->yAxis->setTickLabelFont(_font);
        m_colorScale->axis()->setTickLabelFont(_font);

        m_pcustomPlot->xAxis->setLabelFont( _font );
        m_pcustomPlot->yAxis->setLabelFont( _font );
        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    }
}

void ScatterChart::setDpi( float dpi_ ){
    if( m_dpi != dpi_ ){
        m_dpi = dpi_;
        initializePlot();
        initHeatMap();
        //m_pcustomPlot->setBufferDevicePixelRatio(1);
        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
        return;

        m_pcustomPlot->xAxis->setTickLength(0, 3.0/m_dpi);
        m_pcustomPlot->yAxis->setTickLength(0, 3.0/m_dpi);
        m_colorScale->setBarWidth(10.0/m_dpi);
        m_colorScale->axis()->setTickLength(3.0 / m_dpi);
        m_pcustomPlot->xAxis->setTickLabelPadding(0.0 / m_dpi);
        m_pcustomPlot->yAxis->setTickLabelPadding(2.0 / m_dpi);
        m_pcustomPlot->axisRect()->setMinimumMargins( QMargins(5.0 / m_dpi ,15.0 / m_dpi ,5.0 / m_dpi ,2.0 / m_dpi) );
        // m_pcustomPlot->setBufferDevicePixelRatio(1);
        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    }
}

void ScatterChart::changeTimeLine( int timeline ){
    if( m_data.size() < 1 ){
        return;
    }

    QMap<quint32 , QJsonObject>::Iterator iter = m_data.upperBound( timeline );

    if( iter != m_data.begin() && (int)(iter.key() - 1) > timeline ){
        iter--;
    }

    if( m_i_currentTime == (unsigned int)iter.key() ){
        return;
    }

    if( iter != m_data.end() )
    {
        m_i_currentTime = iter.key();
        addData( iter.value() );
    }

    // qDebug() << "prpd changeTimeLine";
}

void ScatterChart::updateCustomPlotSize()
{
    if (m_pcustomPlot !=nullptr )
    {
        m_pcustomPlot->setGeometry(0, 0, (int)width(), (int)height());
        m_pcustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
    }
}

void ScatterChart::initData(){
    for (int x = 0; x < 200; ++x) {
        for (int y = 0; y < 120; ++y) {
            m_heatmap->data()->setAlpha(x, y, 0);  // z为0，设置为透明
        }
    }
}


void ScatterChart::addData( QJsonObject data ){
    emit addDataInThread( m_heatmap , data );

    /*

    initData();

    QJsonParseError json_error;
    QJsonDocument jdoc(QJsonDocument::fromJson( data.toLatin1(), &json_error ));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject jsonObj = jdoc.object();
    QJsonArray jsonVals = jsonObj["values"].toArray();
    QJsonObject yc;
    QList<QString> yc_keys;
    for( quint8 i = 0; i < jsonVals.count(); i++ ){
         quint8 x = static_cast<quint8>(jsonVals[i].toObject()["x"].toDouble() * 10);
         yc = jsonVals[i].toObject()["yc"].toObject();
         yc_keys = yc.keys();
         for( QString key : qAsConst(yc_keys) ){
            quint8 color_ = yc[key].toInt() > m_line_count?m_line_count:yc[key].toInt();
            quint8 y = key.toUInt();
            // m_data.at(color_)->xAxis->append( jsonVals[i].toObject()["x"].toDouble() );
            //  m_data.at(color_)->yAxis->append( key.toDouble() );
            m_heatmap->data()->setAlpha( x , y , 255 );
            m_heatmap->data()->setCell( x , y , (double)color_ );
        }
    }
    update();

    */
}

void ScatterChart::getDataFromThread( QCPColorMap * heatMap ){
    m_heatmap = heatMap;
    update();
}

void ScatterChart::reset(){
    m_data.clear();
    initData();
    // m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    update();
}

void ScatterChart::setData( QJsonObject data ){
    /*
    QJsonParseError json_error;
    QJsonDocument jdoc(QJsonDocument::fromJson( data.toLatin1(), &json_error ));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject jsonObj = jdoc.object();
    */
    // QList<QString> keys = jsonObj.keys();
    QList<QString> keys = data.keys();
    if( keys.size() < 1 ){
        return;
    }

    for( const QString &key : qAsConst(keys) ){
        m_data.insert( key.toUInt() , data[key].toObject() );
    }

    QMap<quint32 , QJsonObject>::Iterator iter = m_data.begin();
    addData( iter.value() );
}
