#include "CLineChart.h"
#include "float.h"

CLineChart::CLineChart(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setFlag( QQuickItem::ItemHasContents, true );
    connect( this, &QQuickPaintedItem::widthChanged, this, &CLineChart::updateCustomPlotSize );
    connect( this, &QQuickPaintedItem::heightChanged, this, &CLineChart::updateCustomPlotSize );

    initializePlot();
}

CLineChart::~CLineChart()
{
    if( m_ppixMap != nullptr ){
        delete m_ppixMap;
        m_ppixMap = nullptr;
    }

    if( m_pcustomPlot != nullptr ){
        delete m_pcustomPlot;
        m_pcustomPlot = nullptr;
    }

#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif
}

void CLineChart::initializePlot()
{
    if( m_ppixMap != nullptr ){
        delete m_ppixMap;
        m_ppixMap = nullptr;
    }

    if( m_pcustomPlot != nullptr ){
        delete m_pcustomPlot;
        m_pcustomPlot = nullptr;
    }

   // updateCustomPlotSize();

    m_ppixMap = new QPixmap(100,100);
    // m_ppixMap->setDevicePixelRatio(1);
    m_ppixMap->fill(QColor(m_bgcolor));

    QPen _pen;
    _pen.setColor(Qt::lightGray);
    _pen.setStyle(Qt::DotLine);
    _pen.setWidth( 1.0/m_dpi );

    QPen _axisPen(Qt::lightGray);

    m_pcustomPlot = new QCustomPlot();
    m_pcustomPlot->setOpenGl(false);
    // m_pcustomPlot->setBufferDevicePixelRatio(1);
    m_pcustomPlot->plotLayout()->setMargins(QMargins(0,0,0,0));
    m_pcustomPlot->plotLayout()->setMinimumMargins( QMargins(0,0,0,0) );

    m_pcustomPlot->setBackground(*m_ppixMap);

    QFont _font;
    _font.setPixelSize(m_fontSize);
    _font.setBold(false);
    _font.setFamily( qApp->font().family() );

    m_pcustomPlot->xAxis->setTickLabelFont(_font);
    m_pcustomPlot->yAxis->setTickLabelFont(_font);

    m_pcustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
    m_pcustomPlot->axisRect()->setMinimumMargins( QMargins(5.0 / m_dpi,15.0 / m_dpi,5.0 / m_dpi ,2.0 / m_dpi) );

    m_pcustomPlot->addGraph(m_pcustomPlot->xAxis , m_pcustomPlot->yAxis); // blue line
    m_pcustomPlot->graph(0)->setPen(QPen(QColor(10, 10, 200)));

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
    m_pcustomPlot->xAxis->setLabelFont( _font );
    m_pcustomPlot->xAxis->setLabelColor( Qt::black );
    m_pcustomPlot->xAxis->setAntialiased(true);

    m_pcustomPlot->yAxis->setPadding(0);
    m_pcustomPlot->yAxis->setVisible(true);
    m_pcustomPlot->yAxis->setTickLabelPadding( 2 / m_dpi );
    m_pcustomPlot->yAxis->setSubTicks(false);
    m_pcustomPlot->yAxis->setTicks(true);
    m_pcustomPlot->yAxis->setTickLabels(true);
    m_pcustomPlot->yAxis->grid()->setAntialiasedZeroLine(false);
    m_pcustomPlot->yAxis->grid()->setZeroLinePen(_axisPen);
    m_pcustomPlot->yAxis->setTickPen(_axisPen);
    m_pcustomPlot->yAxis->setTickLabelColor(Qt::black);
    m_pcustomPlot->yAxis->grid()->setPen(_pen);
    m_pcustomPlot->yAxis->setBasePen(_axisPen);
    m_pcustomPlot->yAxis->setTickLength(0, 3/m_dpi);

    if( m_yAxisTitle != "" ){
        m_pcustomPlot->yAxis->setLabel( m_yAxisTitle );
    }
    m_pcustomPlot->yAxis->setLabelFont( _font );
    m_pcustomPlot->yAxis->setLabelColor( Qt::black );
    m_pcustomPlot->yAxis->setLabelPadding(0);
    m_pcustomPlot->yAxis->setAntialiased(true);

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
    m_pcustomPlot->yAxis2->setTickLabelColor(Qt::black);
    m_pcustomPlot->yAxis2->setTickPen(_axisPen);
    m_pcustomPlot->yAxis2->grid()->setPen(_pen);
    m_pcustomPlot->yAxis2->setBasePen(_axisPen);

    m_pcustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);

    m_pcustomPlot->setInteraction(QCP::iRangeDrag,true);

    /*
    QList<QCPAxis*> axes;
    axes << m_pcustomPlot->xAxis;
    m_pcustomPlot->axisRect()->setRangeZoomAxes(axes);
    m_pcustomPlot->graph(0)->rescaleKeyAxis(true);
    */

    QPen linePen(Qt::red);
    linePen.setWidth(1/m_dpi);

    m_ptracer = new QCPItemTracer(m_pcustomPlot);
    m_ptracer->setStyle(QCPItemTracer::tsCircle);
    m_ptracer->setPen(linePen);

    QFont lfont;
    lfont.setPixelSize( m_fontSize );
    lfont.setBold(true);

    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);

    m_plabel = new QCPItemText(m_pcustomPlot);

    m_plabel->setClipToAxisRect(false);
    m_plabel->setPadding(QMargins(5, 5, 5, 5));
    m_plabel->setPen(linePen);
    m_plabel->setBrush(brush);
    m_plabel->position->setParentAnchor(m_ptracer->position);
    m_plabel->setFont(lfont);
    m_plabel->setColor(Qt::red);
    m_plabel->setText("");

    m_ptracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
    m_ptracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
    m_ptracer->setSize(4 / m_dpi);
    m_plabel->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 垂直线
    m_plineV = new QCPItemStraightLine(m_pcustomPlot);//垂直线
    m_plineV->setLayer("overlay");
    m_plineV->setPen(linePen);
    m_plineV->setClipToAxisRect(true);
    m_plineV->point1->setCoords(0, m_yAxisMin);
    m_plineV->point2->setCoords(0, m_yAxisMax);

    m_ptracer->setVisible(false);
    m_plineV->setVisible(false);
    m_plabel->setVisible(false);
    m_pvisible = false;

    setRange();
}

void CLineChart::setCurrentPos( int current_ ){
    if( current_ < 0 || m_pcustomPlot->graph(0)->data().data()->size() <= 0 ){
        if( m_pvisible ){
            m_plabel->setVisible(false);
            m_ptracer->setVisible(false);
            m_plineV->setVisible(false);
            m_pvisible = false;
            update();
        }
        return;
    }

    QSharedPointer<QCPGraphDataContainer> tmpContainer;
    tmpContainer = m_pcustomPlot->graph(0)->data();
    float x = (tmpContainer->constBegin() + current_)->mainKey();
    float y = (tmpContainer->constBegin() + current_)->mainValue();

    m_pvisible = true;
    m_plabel->setVisible(true);
    m_ptracer->setVisible(true);

    m_plineV->point1->setCoords(x, m_yAxisMin);
    m_plineV->point2->setCoords(x, m_yAxisMax);
    m_plineV->setVisible(true);

    m_ptracer->position->setCoords(x, y);

    float xToPos_ = m_pcustomPlot->xAxis->coordToPixel( x );
    float yToPos_ = m_pcustomPlot->yAxis->coordToPixel( y );

    float labelX = 25 / m_dpi;
    float labelY = -70 / m_dpi;

    if( xToPos_ / width() > 0.6 ){
        labelX = -100 / m_dpi;
    }

    if( yToPos_ > height() / 2 ){
        labelY = -70 / m_dpi;
    }else{
        labelY = 10 / m_dpi;
    }

    m_plabel->position->setCoords(labelX, labelY);

    if( QString::number(y).length() > 4 ){
        m_plabel->setText(tr("X:%1\nY:%2").arg(x).arg( QString::number(y,'e', 4) ));
    }else{
        m_plabel->setText(tr("X:%1\nY:%2").arg(x).arg( y ));
    }
    update();

}

void CLineChart::setLinePos( float pos_ ){
    if( isChangeSize ){
        return;
    }
    m_linePos = pos_;
    if( m_linePos == -1 ){
        if( m_pvisible ){
            m_plabel->setVisible(false);
            m_ptracer->setVisible(false);
            m_plineV->setVisible(false);
            m_pvisible = false;
            update();
        }
        return;
    }

    float x = m_pcustomPlot->xAxis->pixelToCoord( m_linePos );
    if( x > m_xAxisMax || x < m_xAxisMin ){
        if( m_pvisible ){
            m_plabel->setVisible(false);
            m_ptracer->setVisible(false);
            m_plineV->setVisible(false);
            m_pvisible = false;
            update();
        }
        return;
    }

    m_pvisible = true;

    m_plabel->setVisible(true);
    m_ptracer->setVisible(true);

    CLineChart::point p = getYbyX(x);

    m_plineV->point1->setCoords(p.x, m_yAxisMin);
    m_plineV->point2->setCoords(p.x, m_yAxisMax);
    m_plineV->setVisible(true);

    m_ptracer->position->setCoords(p.x, p.y);

    float yToPos_ = m_pcustomPlot->yAxis->coordToPixel( p.y );

    float labelX = 25 / m_dpi;
    float labelY = -70 / m_dpi;

    if( pos_ / width() > 0.6 ){
        labelX = -100 / m_dpi;
    }

    if( yToPos_ > height() / 2 ){
        labelY = -70 / m_dpi;
    }else{
        labelY = 10 / m_dpi;
    }

    m_plabel->position->setCoords(labelX, labelY);

    if( QString::number(p.y).length() > 4 ){
        m_plabel->setText(tr("X:%1\nY:%2").arg(p.x).arg( QString::number(p.y,'e', 4) ));
    }else{
        m_plabel->setText(tr("X:%1\nY:%2").arg(p.x).arg( p.y ));
    }
    update();
}

void CLineChart::setFontSize( int fontSize_){
    if( m_fontSize != fontSize_ ){
        m_fontSize = fontSize_;
        QFont _font = qApp->font();
        _font.setBold(true);
        _font.setPixelSize(m_fontSize);

        m_pcustomPlot->xAxis->setTickLabelFont(_font);
        m_pcustomPlot->yAxis->setTickLabelFont(_font);

        m_pcustomPlot->xAxis->setLabelFont( _font );
        m_pcustomPlot->yAxis->setLabelFont( _font );

        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    }
}

void CLineChart::setDpi( float dpi_ ){

    if( m_dpi != dpi_ ){
        m_dpi = dpi_;

        initializePlot();
        //m_pcustomPlot->setBufferDevicePixelRatio(1);
        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
        return;

        m_pcustomPlot->xAxis->setTickLength(0, 3/m_dpi);
        m_pcustomPlot->yAxis->setTickLength(0, 3/m_dpi);
        m_pcustomPlot->xAxis->setTickLabelPadding(0 / m_dpi);
        m_pcustomPlot->yAxis->setTickLabelPadding(2 / m_dpi);
        m_pcustomPlot->axisRect()->setMinimumMargins( QMargins(5 / m_dpi ,15 / m_dpi ,5 / m_dpi ,2 / m_dpi) );

        // m_pcustomPlot->setBufferDevicePixelRatio(1);
        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    }
}

void CLineChart::setRange(){
    m_pcustomPlot->xAxis->setRange(m_xAxisMin , m_xAxisMax );
    setXticks();
    setYticks();
}

void CLineChart::setXticks(){
    QVector<double> ticks;
    QVector<QString> labels;

    int x_len = 10 * pow(2,m_scaleRange) > ( m_xAxisMax - m_xAxisMin ) ? ( m_xAxisMax - m_xAxisMin ):10 * pow(2,m_scaleRange);
    m_xAxisUnit = x_len == ( m_xAxisMax - m_xAxisMin )? 1: ( m_xAxisMax - m_xAxisMin ) / x_len;

    for( int i = 0; i <= x_len; i++ ){
        ticks << i * m_xAxisUnit + m_xAxisMin;
        labels << QString::number( i * m_xAxisUnit + m_xAxisMin );
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    m_pcustomPlot->xAxis->setTicker(textTicker);
}

void CLineChart::setYticks(){
    m_pcustomPlot->yAxis->setRange(m_yAxisMin , m_yAxisMax );
    // set yAxis labels
    QVector<double> ticksY;
    QVector<QString> labelsY;

    m_yAxisUnit = (m_yAxisMax - m_yAxisMin ) / 5;
    for( int i = 0; i < 6; i++ ){
        ticksY << i * m_yAxisUnit + m_yAxisMin;
        labelsY << QString::number( i * m_yAxisUnit + m_yAxisMin );
    }

    if( m_isLG ){
        m_pcustomPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        // m_pcustomPlot->yAxis->setRange(0,m_yAxisMax);
        if( m_yAxisMax < 0 ){
            if( m_yAxisMax < -1 )
                m_pcustomPlot->yAxis->setRange(m_yAxisMin,-1);
            else
                m_pcustomPlot->yAxis->setRange(m_yAxisMin,0);
        }else{
            m_pcustomPlot->yAxis->setRange(0,m_yAxisMax);
        }
        logTicker->setSubTickCount(5);
        logTicker->setLogBase(10);
        m_pcustomPlot->yAxis->setTicker(logTicker);

    }else{
        m_pcustomPlot->yAxis->setScaleType(QCPAxis::stLinear);
        QSharedPointer<QCPAxisTickerText> textTickerY(new QCPAxisTickerText);
        textTickerY->addTicks(ticksY, labelsY);
        m_pcustomPlot->yAxis->setTicker(textTickerY);
    }

}

bool CLineChart::checkRange(){
    float xminToPos_ = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMin );
    float xmaxToPos_ = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMax );
    if( xmaxToPos_ < XmaxToPos ||  xminToPos_ > XminToPos  ){
        return false;
    }
    return true;
}

void CLineChart::updateCustomPlotSize()
{
    if (m_pcustomPlot !=nullptr)
    {
        m_pcustomPlot->setGeometry(0, 0, (int)width(), (int)height());
        m_pcustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
    }
}

void CLineChart::paint( QPainter* painter )
{
    if (m_pcustomPlot !=nullptr )
    {
        updateCustomPlotSize();

        QPixmap picture( width() * qApp->devicePixelRatio(), height() * qApp->devicePixelRatio());
        picture.setDevicePixelRatio(qApp->devicePixelRatio());
        picture.fill(Qt::transparent);
        QCPPainter qcpPainter( &picture );
        m_pcustomPlot->toPainter( &qcpPainter );

        painter->drawPixmap( QPoint(), picture );
    }
}

void CLineChart::resetCurveLine()
{
    m_pcustomPlot->graph(0)->setVisible(false);
    m_pcustomPlot->graph(0)->data().data()->clear();
    m_plabel->setVisible(false);
    m_ptracer->setVisible(false);
    m_plineV->setVisible(false);
    m_pvisible = false;

    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    _vector_keys.clear();
    _vector_datas.clear();

    update();
}

void CLineChart::addData( QList<float> data , int len ){
    m_len = len;
    setData( m_len , data.toVector().data() );

    update();
}

// 绘制一次性折线图
void CLineChart::addJsonData( QJsonObject jsonObj ){

    if( isChangeSize ){
        return;
    }

    /*
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    */

    QJsonArray valueArr = jsonObj.value("values").toArray();

    QVector<float> spectra;
    spectra.reserve(m_len);
    for( int j = 0; j < valueArr.size() && j < m_len; j++ ){
        spectra.append( valueArr[j].toDouble() );
    }

    setData( m_len , spectra.data() );

    /*
    if( m_yflash ){
       // m_pcustomPlot->xAxis->scaleRange( pow(2, -m_scaleRange ) , m_scalePoint);
        setLinePos( m_linePos );
        m_yflash = false;
    }
    */
    update();
}

// 绘制批量数据
void CLineChart::addJsonBatchData( QJsonObject jsonObj ){

    if( isChangeSize ){
        return;
    }

    QJsonArray valueArr = jsonObj.value("values").toArray();

    QVector<float> data;
    data.reserve(m_len);
    for( int j = 0; j < valueArr.size() && j < m_len; j++ ){
        data.append( valueArr[j].toDouble() );
    }

    setRBatchData( m_len , data.data() , data.size() );
    update();
}



void CLineChart::yAxisSelfChange( float max , float min , int num ){

    if( min < m_yAxisMin || ( min/num ) > m_yAxisMin ){
        if( min == 0 ){
            m_yAxisMin = 0;
        }else if( abs(min) > 1 ){
            m_yAxisMin = floor(min) - 1;
        }else{
            int i = 0;
            while( abs(min) < 1 ){
                min = min * 10;
                i++;
            }
            m_yAxisMin = (floor(min) - 1) * pow(10,-i);
        }
    }

    if( max > m_yAxisMax || ( max*num ) < m_yAxisMax ){
        if( max == 0 ){
            m_yAxisMax = 0;
        }else if( abs(max) > 1 ){
            m_yAxisMax = ceil(max) + ceil( abs(max) * 0.5 );
            m_yAxisMax += 5 - ( static_cast<int>(m_yAxisMax - m_yAxisMin) % 5 );
        }else{
            int i = 0;
            while( abs(max) < 1 ){
                max = max * 10;
                i++;
            }
            m_yAxisMax = (ceil(max) + 1) * pow(10,-i);
        }
    }

    m_yflash = true;
    //if( !m_isLG ){
        setYticks();
    //}
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}

// 绘制从右向左的折线图
void CLineChart::setRBatchData( const size_t& dataSize_ , float* data , int dataLength ){
    _vector_keys = QVector<double>( dataLength, 0.0);
    _vector_datas = QVector<double>( dataLength, 0.0);

    float start_ =  m_xAxisMin + ( dataSize_ - dataLength ) * m_df;

    for ( int _i = 0; _i < dataLength; _i++ ) {
        if ( !std::isinf(data[_i]) ) {
            _vector_keys[_i] = start_ + static_cast<double>(_i) * m_df;
            _vector_datas[_i] = static_cast<double>(data[_i]);
        }
    }
    if ( dataLength > 0 ) {
        m_pcustomPlot->graph(0)->setVisible(true);
        if( m_pcustomPlot->graph(0)->data().data()->size() > 0 ){
            m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas , true );
        }else{
            m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas);
        }

    } else {
        m_pcustomPlot->graph(0)->setVisible(false);
    }

    m_pcustomPlot->replot( QCustomPlot::rpQueuedReplot );
}

void CLineChart::setData( const size_t& dataSize_ , float* data ){
    _vector_keys = QVector<double>( dataSize_, 0.0);
    _vector_datas = QVector<double>( dataSize_, 0.0);
    _vector_org_datas = QVector<double>( dataSize_, 0.0);

    for ( size_t _i = 0; _i < dataSize_; _i++ ) {
        if ( !std::isinf(data[_i]) ) {
            _vector_keys[_i] = static_cast<double>(_i) * m_df;
            _vector_org_datas[_i] = static_cast<double>(data[_i]);
            if( m_isDB ){
                 _vector_datas[_i] = 20 * log10( static_cast<double>(data[_i]) );
            }else{
                _vector_datas[_i] = static_cast<double>(data[_i]);
            }
        }
    }
    if ( dataSize_ > 0 ) {
        m_pcustomPlot->graph(0)->setVisible(true);
        if( m_pcustomPlot->graph(0)->data().data()->size() > 0 ){
            m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas , true );
        }else{
            m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas);
        }

    } else {
        m_pcustomPlot->graph(0)->setVisible(false);
    }

    m_pcustomPlot->replot( QCustomPlot::rpQueuedReplot );
}

void CLineChart::changeScaleType(){
    setYticks();
    m_pcustomPlot->replot( QCustomPlot::rpQueuedReplot );
}

void CLineChart::reload(){

    int dataSize_ = _vector_org_datas.size();
    for ( int _i = 0; _i < dataSize_; _i++ ) {
        if ( !std::isinf(_vector_org_datas[_i]) ) {
            if( m_isDB ){
                _vector_datas[_i] = 20 * log10( static_cast<double>(_vector_org_datas[_i]) );
            }else{
                _vector_datas[_i] = static_cast<double>(_vector_org_datas[_i]);
            }
        }
    }
    if ( dataSize_ > 0 ) {
        m_pcustomPlot->graph(0)->setVisible(true);
        if( m_pcustomPlot->graph(0)->data().data()->size() > 0 ){
            m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas , true );
        }else{
            m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas);
        }

    } else {
        m_pcustomPlot->graph(0)->setVisible(false);
    }

    m_pcustomPlot->replot( QCustomPlot::rpQueuedReplot );
}


CLineChart::point CLineChart::getYbyX( float &x ){
   if( m_pcustomPlot->graph(0)->data().data()->size() <= 0){
        CLineChart::point p;
        p.x = 0;
        p.y = 0;
        return p;
   }
   double y = 0;
   double maxY = -DBL_MAX;
   double maxX = x;

   QSharedPointer<QCPGraphDataContainer> tmpContainer;
   tmpContainer = m_pcustomPlot->graph(0)->data();
   //使用二分法快速查找所在点数据！！！敲黑板，下边这段是重点
   int low = 0, high = tmpContainer->size();

   double unit = ( m_xAxisMax - m_xAxisMin ) / high;
   int l = m_len / 100 / pow(2,m_scaleRange);
   l = l < 1?1:l;

   while(high > low)
   {
       int middle = (low + high) / 2;
       if(x < tmpContainer->constBegin()->mainKey() ||
               x > (tmpContainer->constEnd()-1)->mainKey())
            break;

       if(x == (tmpContainer->constBegin() + middle)->mainKey())
       {
           maxY = (tmpContainer->constBegin() + middle)->mainValue();
           break;
       }
       if(x > (tmpContainer->constBegin() + middle)->mainKey())
       {
           low = middle;
       }
       else if(x < (tmpContainer->constBegin() + middle)->mainKey())
       {
           high = middle;
       }

       if( high - low <= l )
       {   //差值计算所在位置数据

           /*
           y = (tmpContainer->constBegin()+low)->mainValue() + ( (x - (tmpContainer->constBegin() + low)->mainKey()) *
               ((tmpContainer->constBegin()+high)->mainValue() - (tmpContainer->constBegin()+low)->mainValue()) ) /
               ((tmpContainer->constBegin()+high)->mainKey() - (tmpContainer->constBegin()+low)->mainKey());
           */
           for( int i = low; i < high; i++ ){
               y = (tmpContainer->constBegin()+i)->mainValue();
               if( y > maxY ){
                   maxY = y;
                   maxX = m_xAxisMin + i*unit;
                }
           }
           break;
       }
   }

   CLineChart::point p;
   p.x = maxX;
   p.y = maxY;
   return p;
}

void CLineChart::changeSize( float x, int v){
    if( isChangeSize ){
        return;
    }
    isChangeSize = true;

    float x_ = m_pcustomPlot->xAxis->pixelToCoord( x );

    if( x_ < m_xAxisMin || x_ > m_xAxisMax ){
        isChangeSize = false;
        return;
    }

    if( m_scaleRange >= 6 && v > 0  ){
       isChangeSize = false;
       return;
    }

    if( m_scaleRange <= 0 && v < 0 ){
        isChangeSize = false;
        return;
    }
    m_scalePoint = x_;
    m_scaleRange += v;

    setXticks();

    if( m_pcustomPlot != nullptr ){
        m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
        m_pcustomPlot->xAxis->scaleRange( pow( 2, -v ) , m_scalePoint);
    }

    if( m_scaleRange == 0 &&  m_pcustomPlot != nullptr ){
        m_pcustomPlot->xAxis->rescale(true);
        m_scalePoint = 0;
        XminToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMin );
        XmaxToPos = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMax );
    }
    update();
    isChangeSize = false;

}

void CLineChart::exportCSV( QString path_ , QString channelName , QString dataType ){

    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QString dir = appData + "/temp/";

    QDir dataDir{appData + "/"};
    if(!dataDir.exists("temp"))
        dataDir.mkpath("temp");
    QString path = dir + "/data.csv";

    QFile file( path );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        qDebug() << "error";
        return;
    }

    QTextStream out( &file );
    double * arr = _vector_datas.data();

    for( int i = 0; i < _vector_datas.size(); i++ ){
        out<< arr[i]<< "\n";
    }

    out.flush();
    file.close();

    QDateTime date_;

    path_ = path_.mid(8) + "/"+channelName+"_"+dataType+"_";
    path_ += date_.currentDateTime().toString("yyyy.MM.ddThh.mm.ss.zzz")+".csv";
    QFile::copy(path, path_);

}


bool CLineChart::saveCSV( QString path_ , QString channelName , QString dataType ){

    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QString dir = appData + "/RhythmData/" + path_;

    QDir dataDir{appData + "/RhythmData/"};
    if(!dataDir.exists(path_))
        dataDir.mkpath(path_);
    QDateTime date_;
    QString path = dir;
    path += "/"+channelName+"_"+dataType+"_" + date_.currentDateTime().toString("yyyy.MM.ddThh.mm.ss.zzz")+".csv";
    QFile file( path );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        qDebug() << "error";
        return false;
    }

    QTextStream out( &file );
    double * arr = _vector_datas.data();
    for( int i = 0; i < _vector_datas.size(); i++ ){
        out<< arr[i]<< "\n";
    }
    out.flush();
    file.close();
    return true;
}


void CLineChart::move( float orgX , float newX ){
    float ox_ = m_pcustomPlot->xAxis->pixelToCoord( orgX );
    float nx_ = m_pcustomPlot->xAxis->pixelToCoord( newX );
    float move_ = ox_ - nx_;
    if( (ox_ - nx_) < 0 ){
        float xminToPos_ = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMin );
        if(  xminToPos_ >= XminToPos ){
            return;
        }

        if( move_ > (XminToPos - xminToPos_) ){
            move_ = XminToPos - xminToPos_;
        }

    }else{
        float xmaxToPos_ = m_pcustomPlot->xAxis->coordToPixel( m_xAxisMax );
        if( xmaxToPos_ <= XmaxToPos ){
            return;
        }

        if( move_ > (xmaxToPos_ - XmaxToPos) ){
            move_ = xmaxToPos_ - XmaxToPos;
        }
    }

    m_scalePoint += move_;
    m_pcustomPlot->xAxis->moveRange(move_);
    m_scaleMove += move_;

    update();
}

void CLineChart::timeLineChanged( int size ){
    Q_UNUSED(size);
}
void CLineChart::timeLineChanged(){

}

