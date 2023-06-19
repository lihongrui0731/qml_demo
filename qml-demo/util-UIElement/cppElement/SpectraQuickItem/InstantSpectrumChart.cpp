#include "InstantSpectrumChart.h"

#include <QTimerEvent>
#include <QBrush>

#include <QThread>

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif

InstantSpectrumChart::InstantSpectrumChart(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    initializePlot();

    m_const_spectrumItor = m_map_spectrum.begin();
    m_const_local_spectrumItor = m_map_local_spectrum.begin();
}

void InstantSpectrumChart::initializePlot()
{
    m_ppixMap = new QPixmap(100 , 100);
    m_ppixMap->fill(Qt::black);

    QPen _pen;
    _pen.setColor(Qt::lightGray);
    _pen.setStyle(Qt::DotLine);

    QPen _axisPen(Qt::lightGray);

    m_pcustomPlot = new QCustomPlot();
    m_pcustomPlot->setOpenGl(true);
    m_pcustomPlot->plotLayout()->setMargins(QMargins(0,0,0,0));    
    m_pcustomPlot->plotLayout()->setMinimumMargins( QMargins(0,0,0,0) );

    // m_pcustomPlot->plotLayout()->;
    m_pcustomPlot->setBackground(*m_ppixMap);

    QFont _font;
    _font.setPointSize(8);
    _font.setFamily( qApp->font().family() );
    m_pcustomPlot->xAxis->setTickLabelFont(_font);
    m_pcustomPlot->yAxis2->setTickLabelFont(_font);

    m_pcustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    m_pcustomPlot->addGraph(m_pcustomPlot->yAxis2, m_pcustomPlot->xAxis); // blue line
    m_pcustomPlot->graph(0)->setPen(QPen(QColor(200, 10, 10)));

    m_pcustomPlot->addGraph(m_pcustomPlot->yAxis2, m_pcustomPlot->xAxis); // red line
    m_pcustomPlot->graph(1)->setPen(QPen(QColor(10, 10, 200)));

    m_pcustomPlot->xAxis->setSubTicks(false);
    m_pcustomPlot->xAxis->grid()->setAntialiasedZeroLine(false);
    m_pcustomPlot->xAxis->grid()->setSubGridVisible(false);
    m_pcustomPlot->xAxis->setTickLength(0, 1);
    m_pcustomPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_pcustomPlot->xAxis->setTickLabelColor(Qt::white);
    m_pcustomPlot->xAxis->setTickPen(_axisPen);
    m_pcustomPlot->xAxis->grid()->setPen(_pen);
    m_pcustomPlot->xAxis->setBasePen(_axisPen);

    m_pcustomPlot->yAxis->setTickLabels(false);
    m_pcustomPlot->yAxis->setTicks(false);

    m_pcustomPlot->yAxis->grid()->setAntialiasedZeroLine(false);
    m_pcustomPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_pcustomPlot->yAxis->grid()->setPen(_pen);
    m_pcustomPlot->yAxis->setBasePen(_axisPen);

    m_pcustomPlot->xAxis2->setVisible(true);
    m_pcustomPlot->xAxis2->setTickLabels(false);
    m_pcustomPlot->xAxis2->setTicks(false);
    m_pcustomPlot->xAxis2->grid()->setPen(_pen);
    m_pcustomPlot->xAxis2->setBasePen(_axisPen);
    m_pcustomPlot->xAxis2->setTickLength(0, 0);
    m_pcustomPlot->xAxis2->setLabelPadding(0);
    m_pcustomPlot->xAxis2->setTickLabelPadding(0);
    m_pcustomPlot->xAxis2->setPadding(0);
    m_pcustomPlot->yAxis2->setVisible(true);
    m_pcustomPlot->yAxis2->setSubTicks(false);
    m_pcustomPlot->yAxis2->setTickLength(0, 3);
    m_pcustomPlot->yAxis2->setTickLabelColor(Qt::white);
    m_pcustomPlot->yAxis2->setTickPen(_axisPen);
    m_pcustomPlot->yAxis2->grid()->setPen(_pen);
    m_pcustomPlot->yAxis2->setBasePen(_axisPen);

    setRange();
}

void InstantSpectrumChart::setRange(){

    // set all axis range
    m_pcustomPlot->xAxis->setRange(m_xAxisMin, m_xAxisMax);
//    m_pcustomPlot->yAxis->setRange(0, 25);
    //m_pcustomPlot->xAxis2->setRange(m_xAxisMin, m_xAxisMax);
    m_pcustomPlot->yAxis2->setRange(m_yAxisMin, m_yAxisMax);

    // set xAxis labels
    QVector<double> ticks;
    QVector<QString> labels;

    float x_ = m_xAxisMin;
    while( x_ < m_xAxisMax ){
        ticks << x_;
        labels << "";
        x_ += m_xAxisUnit;
    }

    // labels << "  100" << "" << "80" << "" << "60" << "" << QStringLiteral("dB(A)") << "" << "20" << "" << "0" <<""<<"-20";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    m_pcustomPlot->xAxis->setTicker(textTicker);

    // set yAxis labels
    QVector<double> ticksY;
    QVector<QString> labelsY;

    float y_ = m_yAxisMin;
    while( y_ < m_yAxisUnit ){
        ticksY << y_;
        labelsY << "";
        y_ += m_yAxisUnit;
    }

    QSharedPointer<QCPAxisTickerText> textTickerY(new QCPAxisTickerText);
    textTickerY->addTicks(ticksY, labelsY);
    m_pcustomPlot->yAxis2->setTicker(textTickerY);

}

void InstantSpectrumChart::updateCustomPlotSize()
{
    if (m_pcustomPlot)
    {
        m_pcustomPlot->setGeometry(0, 0, (int)width(), (int)height());
        m_pcustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
    }
}

void InstantSpectrumChart::paint( QPainter* painter )
{
    if (m_pcustomPlot)
    {
        updateCustomPlotSize();

        QPixmap picture( width() * m_dpi , height() * m_dpi );
        picture.setDevicePixelRatio(m_dpi);
        QCPPainter qcpPainter( &picture );

        m_pcustomPlot->toPainter( &qcpPainter );

        painter->drawPixmap( QPoint(), picture );

    }
}

void InstantSpectrumChart::resetCurveLine()
{
    std::map<int, points*>::iterator _spect_const_itor = m_map_spectrum.begin();
    while ( _spect_const_itor != m_map_spectrum.end() ) {
        delete _spect_const_itor->second;
        _spect_const_itor++;
    }
    m_map_spectrum.clear();
    m_const_spectrumItor = m_map_spectrum.begin();

    std::map<int, points*>::iterator _local_spect_const_itor = m_map_local_spectrum.begin();
    while ( _local_spect_const_itor != m_map_local_spectrum.end() ) {
        delete _local_spect_const_itor->second;
        _local_spect_const_itor++;
    }
    m_map_local_spectrum.clear();
    m_const_local_spectrumItor = m_map_local_spectrum.begin();

    m_pcustomPlot->graph(0)->setVisible(false);
    m_pcustomPlot->graph(1)->setVisible(false);
    m_pcustomPlot->graph(0)->data().clear();
    m_pcustomPlot->graph(1)->data().clear();

}

void InstantSpectrumChart::setSpectraData(const size_t& dataSize_, const float* data)
{
    QVector<double> _vector_keys( dataSize_, 0.0);
    QVector<double> _vector_datas( dataSize_, 0.0);

    // add all the data into the line
    double frequency_ = double(dataSize_) / m_frequency_width;
    double b = m_xAxisMax + m_xAxisMin;

    double max = 0;

    for ( size_t _i = 0; _i < dataSize_; _i++ ) {
        if ( !std::isinf(data[_i]) ) {
            _vector_keys[_i] = static_cast<double>(_i) / frequency_ /*42.0*/ ;
            _vector_datas[_i] =  b - static_cast<double>(data[_i]) ;

            max = data[_i] > max ? data[_i]:max;
        }
    }

    if ( dataSize_ > 0 ) {
        m_pcustomPlot->graph(0)->setVisible(true);
        m_pcustomPlot->graph(0)->setData(_vector_keys, _vector_datas);
    } else {
        m_pcustomPlot->graph(0)->setVisible(false);
    }
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}

void InstantSpectrumChart::setLocalSpectraData(const size_t& dataSize_, const float* data)
{
    QVector<double> _vector_keys(dataSize_, 0.0);
    QVector<double> _vector_datas(dataSize_, 0.0);

    // add local data in the line
    double frequency_ = double(dataSize_) / m_frequency_width;
    double b = m_xAxisMax + m_xAxisMin;
    for ( size_t _i = 0; _i < dataSize_ ; _i++ ) {
        if ( !std::isinf(data[_i]) ) {
            _vector_keys[_i] = static_cast<double>(_i) / frequency_ ;
            _vector_datas[_i] = b - static_cast<double>(data[_i]) ;
        }
    }
    if ( dataSize_ > 0 ) {
        m_pcustomPlot->graph(1)->setVisible(true);
        m_pcustomPlot->graph(1)->setData(_vector_keys, _vector_datas);
    } else {
        m_pcustomPlot->graph(1)->setVisible(false);
    }
    // m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}


void InstantSpectrumChart::setFinalSpectraData( QVariant data_map ){
    m_map_spectrum.clear();

    spectraFinalData spectra_ = data_map.value<spectraFinalData>();
    QMap<quint64, QVector<float>> spectra = spectra_.spectra;

    double frequency_ = (double)m_len / m_frequency_width;
    double b = m_xAxisMax + m_xAxisMin;
    QMap<quint64, QVector<float>>::Iterator spectra_itor = spectra.begin();

    while ( spectra_itor != spectra.end() ) {
        float * spectra_data_ = spectra_itor.value().data();
        points* _point = new points(m_len);
        // add all the data into the line
        for ( int _i = 0; _i < m_len ; _i++ ) {
            if ( !std::isinf(spectra_data_[_i]) ) {
                (*_point->xAxis)[_i] = static_cast<double>(_i) /frequency_ ;
                (*_point->yAxis)[_i] = b - static_cast<double>(spectra_data_[_i]) ;
             }
        }
        m_map_spectrum.insert(std::pair<int, points*>(spectra_itor.key(), _point));
        spectra_itor++;
    }

    m_const_spectrumItor = m_map_spectrum.begin();
    if ( m_const_spectrumItor != m_map_spectrum.end() )
        m_pcustomPlot->graph(0)->setData(*m_const_spectrumItor->second->xAxis, *m_const_spectrumItor->second->yAxis);
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}

void InstantSpectrumChart::setFinalSpectraLocalData( QVariant data_map ){
    m_map_local_spectrum.clear();

    spectraFinalData spectra_ = data_map.value<spectraFinalData>();
    QMap<quint64, QVector<float>> spectra = spectra_.spectra;

    double frequency_ = (double)m_len / m_frequency_width;
    double b = m_xAxisMax + m_xAxisMin;
    QMap<quint64, QVector<float>>::Iterator spectra_itor = spectra.begin();

    while ( spectra_itor != spectra.end() ) {
        float * spectra_data_ = spectra_itor.value().data();
        points* _point = new points(m_len);
        // add all the data into the line
        for ( int _i = 0; _i < m_len ; _i++ ) {
            if ( !std::isinf(spectra_data_[_i]) ) {
                (*_point->xAxis)[_i] = static_cast<double>(_i) /frequency_ ;
                (*_point->yAxis)[_i] = b - static_cast<double>(spectra_data_[_i]) ;
             }
        }
        m_map_local_spectrum.insert(std::pair<int, points*>(spectra_itor.key(), _point));
        spectra_itor++;
    }

    m_const_local_spectrumItor = m_map_local_spectrum.begin();
    if ( m_const_local_spectrumItor != m_map_local_spectrum.end() )
        m_pcustomPlot->graph(1)->setData(*m_const_local_spectrumItor->second->xAxis, *m_const_local_spectrumItor->second->yAxis);
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}

void InstantSpectrumChart::setSpectraData(const size_t& dataSize_, const std::map<int, float*> datamap_)
{
    std::map<int, points*>::iterator _spect_const_itor = m_map_spectrum.begin();
    while ( _spect_const_itor != m_map_spectrum.end() ) {
        delete _spect_const_itor->second;
        _spect_const_itor++;
    }

    m_map_spectrum.clear();

    double frequency_ = double(dataSize_) / m_frequency_width;
    double b = m_xAxisMax + m_xAxisMin;
    std::map<int, float*>::const_iterator _const_itor = datamap_.begin();

    while ( _const_itor != datamap_.end() ) {
        points* _point = new points(dataSize_);
        // add all the data into the line
        for ( size_t _i = 0; _i < dataSize_ ; _i++ ) {
            if ( !std::isinf(_const_itor->second[_i]) ) {
                (*_point->xAxis)[_i] = static_cast<double>(_i) /frequency_ ;
                (*_point->yAxis)[_i] = b - static_cast<double>(_const_itor->second[_i]) ;
             }
        }
        m_map_spectrum.insert(std::pair<int, points*>(_const_itor->first, _point));
        _const_itor++;
    }

    m_const_spectrumItor = m_map_spectrum.begin();
    if ( m_const_spectrumItor != m_map_spectrum.end() )
        m_pcustomPlot->graph(0)->setData(*m_const_spectrumItor->second->xAxis, *m_const_spectrumItor->second->yAxis);

    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}

void InstantSpectrumChart::setLocalSpectraData(const size_t& dataSize_, const std::map<int, float*> datamap_)
{
    if( datamap_.size() == 0 ){
        return;
    }

    std::map<int, points*>::iterator _spect_const_itor = m_map_local_spectrum.begin();
    while ( _spect_const_itor != m_map_local_spectrum.end() ) {
        delete _spect_const_itor->second;
        _spect_const_itor++;
    }

    m_map_local_spectrum.clear();
    double frequency_ = double(dataSize_) / m_frequency_width;
    double b = m_xAxisMax + m_xAxisMin;
    std::map<int, float*>::const_iterator _const_itor = datamap_.begin();
    while ( _const_itor != datamap_.end() ) {
        points *_point = new points(dataSize_);
        // add all the data into the line
        for ( size_t _i = 0; _i < dataSize_ ; _i++ ) {
            if ( !std::isinf(_const_itor->second[_i]) ) {
                (*_point->xAxis)[_i] = static_cast<double>(_i) /frequency_ ;
                (*_point->yAxis)[_i] = b - static_cast<double>(_const_itor->second[_i]) ;
            }
        }
        m_map_local_spectrum.insert(std::pair<int, points*>(_const_itor->first, _point));
        _const_itor++;
    }
    m_const_local_spectrumItor = m_map_local_spectrum.begin();
    if ( m_const_local_spectrumItor != m_map_local_spectrum.end() )
        m_pcustomPlot->graph(1)->setData(*m_const_local_spectrumItor->second->xAxis, *m_const_local_spectrumItor->second->yAxis);
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}

void InstantSpectrumChart::spectrumPlay()
{
//    if ( m_i_timerID <= 0 && ( m_map_spectrum.size() || m_map_local_spectrum.size() ) )
//        m_i_timerID = QWidget::startTimer(10); // 25 frames every second
}

void InstantSpectrumChart::spectrumStop()
{
//    if ( m_i_timerID > 0 )
//        QWidget::killTimer(m_i_timerID);
//    m_i_timerID = 0;
}

void InstantSpectrumChart::seekPercentage(const float& position_)
{
    int _videoPosition = static_cast<int>(position_*m_map_spectrum.size() - 0.49999f);
    int _videoBeamPosition = static_cast<int>(position_*m_map_local_spectrum.size() - 0.49999f);

    std::map<int, points*>::iterator _spectrumMapIterator = m_map_spectrum.begin();
    std::map<int, points*>::iterator _loacalspectrumMapIterator = m_map_local_spectrum.begin();
    std::advance(_spectrumMapIterator, _videoPosition);
    std::advance(_loacalspectrumMapIterator, _videoBeamPosition);
    if ( _spectrumMapIterator != m_map_spectrum.end() ) {
        m_pcustomPlot->graph(0)->setVisible(true);
        m_pcustomPlot->graph(0)->setData(*_spectrumMapIterator->second->xAxis, *_spectrumMapIterator->second->yAxis);
    } else {
        m_pcustomPlot->graph(0)->setVisible(false);
    }
    if ( _loacalspectrumMapIterator != m_map_local_spectrum.end() ) {
        m_pcustomPlot->graph(1)->setVisible(true);
        m_pcustomPlot->graph(1)->setData(*_loacalspectrumMapIterator->second->xAxis, *_loacalspectrumMapIterator->second->yAxis);
    } else {
        m_pcustomPlot->graph(1)->setVisible(false);
    }
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
}

void InstantSpectrumChart::timeLineChanged( int timeline )
{
//  int len = static_cast<int>( m_map_spectrum.size() / 1000.0 * (size + 1));
    m_const_spectrumItor = m_map_spectrum.upper_bound( timeline );
    m_const_local_spectrumItor = m_map_local_spectrum.upper_bound( timeline );

    if( m_const_spectrumItor != m_map_spectrum.begin() && m_const_spectrumItor->first - 1 > timeline ){
        m_const_spectrumItor--;
    }
    if( m_const_local_spectrumItor != m_map_local_spectrum.begin() && m_const_local_spectrumItor->first - 1 > timeline ){
        m_const_local_spectrumItor--;
    }


    if ( m_const_spectrumItor != m_map_spectrum.end() ) {
       if( m_i_local_currentTime != m_const_spectrumItor->first ){
            m_pcustomPlot->graph(0)->setData(*m_const_spectrumItor->second->xAxis, *m_const_spectrumItor->second->yAxis);
            m_i_currentTime = m_const_spectrumItor->first;
        }
    }
    if ( m_const_local_spectrumItor != m_map_local_spectrum.end() ) {
        if( m_i_local_currentTime != m_const_local_spectrumItor->first ){
            m_pcustomPlot->graph(1)->setData(*m_const_local_spectrumItor->second->xAxis, *m_const_local_spectrumItor->second->yAxis);
            m_i_local_currentTime = m_const_local_spectrumItor->first;
        }
    }
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    // QWidget::timerEvent(event_);
    update();
}

void InstantSpectrumChart::timeLineChanged()
{

    if ( m_const_spectrumItor != m_map_spectrum.end() ) {
        m_pcustomPlot->graph(0)->setData(*m_const_spectrumItor->second->xAxis, *m_const_spectrumItor->second->yAxis);
        m_const_spectrumItor++;
    } else {
        m_const_spectrumItor = m_map_spectrum.begin();
    }

    if ( m_const_local_spectrumItor != m_map_local_spectrum.end() ) {
        m_pcustomPlot->graph(1)->setData(*m_const_local_spectrumItor->second->xAxis, *m_const_local_spectrumItor->second->yAxis);
        m_const_local_spectrumItor++;
    } else {
        m_const_local_spectrumItor = m_map_local_spectrum.begin();
    }
    m_pcustomPlot->replot(QCustomPlot::rpQueuedReplot);
    // QWidget::timerEvent(event_);
    update();
}


void InstantSpectrumChart::scaleAndMove(const float& scale, const float& move)
{
    float _range = 25.0f/scale;
    float _midle = 12.5f;
    float _ratio = 12.5f/scale;
    float _movedValue = _ratio*move;
    m_pcustomPlot->yAxis2->setRange(_midle - _range/2.0f - _movedValue, _midle + _range/2.0f - _movedValue);
}

void InstantSpectrumChart::addData( QVariant data ){
    floatData data_ = data.value<floatData>();
    float * spectrum = data_.data.data();
    int len = data_.data.count();

    setSpectraData( len , spectrum );
    update();
}

void InstantSpectrumChart::addLocalData( QVariant data ){
    floatData data_ = data.value<floatData>();
    float * spectrum = data_.data.data();
    int len = data_.data.count();

    setLocalSpectraData( len , spectrum );
    // update();
}


void InstantSpectrumChart::addJsonData( QJsonObject jsonObj ){
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
    quint16 len = jsonObj.value("frameLength").toInt();

    QJsonArray valueArr = jsonObj.value("values").toArray();

    QVector<float> spectra(len);
    for( quint16 i = 0; i < count; i++ ){
        QJsonArray v_arr = valueArr[i].toArray();
        for( int j = 0; j < len && j < m_len; j++ ){
            spectra[j] = v_arr[j].toDouble();
        }
        setSpectraData( m_len , spectra.data() );
    }
    spectra.clear();
    update();
}

void InstantSpectrumChart::setData( QVariant data ){
    SpectrumData t = data.value<SpectrumData>();
    m_frequencyLimitMax = t.m_frequencyMax;
    m_frequencyLimitMin = t.m_frequencyMin;

    setSpectraData( t.len , t._map_spectrum );
    setLocalSpectraData( t.len , t._local_map_spectrum );
    // setSpectraData(static_cast<unsigned int>(_p_data.size()), len , _p_data);
    update();
    emit drawFrequency();
    emit finish();
}

InstantSpectrumChart::~InstantSpectrumChart()
{
    resetCurveLine();

    delete m_pcustomPlot;
    m_pcustomPlot = nullptr;
    delete m_ppixMap;
    m_ppixMap = nullptr;

    m_const_spectrumItor = m_map_spectrum.begin();
    while( m_const_spectrumItor!= m_map_spectrum.end() ){
        free( m_const_spectrumItor->second );
        m_const_spectrumItor++;
    }

    m_const_local_spectrumItor = m_map_local_spectrum.begin();
    while( m_const_local_spectrumItor!= m_map_local_spectrum.end() ){
        free( m_const_local_spectrumItor->second );
        m_const_local_spectrumItor++;
    }

   //  m_Thread = nullptr;
}
