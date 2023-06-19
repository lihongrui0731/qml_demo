#include "RhythmGradientBar.h"

RhythmGradientBar::RhythmGradientBar(QQuickItem *parent)
 :QQuickPaintedItem(parent)
{
    setMaxdB(0.0f);
    setRange(3.0f);
    setMindB(0.0f);
}

RhythmGradientBar::~RhythmGradientBar()
{
}

void RhythmGradientBar::setMaxdB(const float maxdb_)
{
   // m_pmaxDBLabel->setText(QStringLiteral("%1 dB").arg( QString::number( maxdb_, 'f', 1)));
   // repaint();
   if (maxdb_ == m_maxDB) return ;
   m_maxDB = maxdb_;
   // emit maxDBChanged();

}

void RhythmGradientBar::setRange(const float range_)
{
    if (range_ == m_dBrange) return ;
    m_dBrange = range_;
   // emit dBrangeChanged();

//    m_pdBrangeLabel->setText(QStringLiteral("dyn\n%1 dB").arg( QString::number( range_, 'f', 1)));
}

void RhythmGradientBar::setMindB(const float mindb_)
{
    if (mindb_ == m_minDB) return ;
    m_minDB = mindb_;
 //    emit minDBChanged();
    //m_pminDbLabel->setText(QStringLiteral("%1 dB").arg( QString::number( mindb_, 'f', 1)));
}


void RhythmGradientBar::setShowRange(bool showRange_)
{
    if (showRange_ == m_showRange) return ;
    m_showRange = showRange_;
 //    emit minDBChanged();
    //m_pminDbLabel->setText(QStringLiteral("%1 dB").arg( QString::number( mindb_, 'f', 1)));
}

void RhythmGradientBar::setFontSize(int fontSize_){
    if (fontSize_ == m_fontSize) return ;
    m_fontSize = fontSize_;
    update();
}

void RhythmGradientBar::setMinMaxB(const float mindb_, const float maxdb_)
{
    if (mindb_ == m_minDB && maxdb_ == m_maxDB ) return ;
    m_minDB = mindb_;
    m_maxDB = maxdb_;
 //   emit minDBChanged();
 //   emit minDBChanged();

    //m_pminDbLabel->setText(QStringLiteral("%1 dB").arg( QString::number( mindb_, 'f', 1)));
    //m_pmaxDBLabel->setText(QStringLiteral("%1 dB").arg( QString::number( maxdb_, 'f', 1)));
    //repaint();
}

/*
void RhythmGradientBar::resizeEvent(QResizeEvent*event_)
{
    m_pmaxDBLabel->move(20, 0);
    m_pdBrangeLabel->move(20, event_->size().height()/2-25);
    m_pminDbLabel->move(20, event_->size().height()-30);
   // QWidget::resizeEvent(event_);
    update();
};
*/

QColor RhythmGradientBar::generateColor(float value , float min , float max )
{
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

    return QColor(r, g, b);
}

QString RhythmGradientBar::format(float f ){
    QString s;
    if( f > 1 || f == 0 ){
        s = QString::number(f , 'f', 0);
    }else if( f > 0.1 ){
        s = QString::number(f , 'f', 1);
    }else if( f > 0.001 ){
        s = QString::number( f*1000 , 'f', 0) + "\n" + "m";
    }else if( f > 0.0001 ) {
        s = QString::number( f*1000 , 'f', 1) + "\n" + "m";
    }else if( f > 0.000001 ) {
        s = QString::number(f*1000000 , 'f', 0) + "\n" + "μ";
    }else{
        s = QString::number(f*1000000 , 'f', 1) + "\n" + "μ";
    }
    return s;
}

void RhythmGradientBar::paint(QPainter *painter)
{
    if( painter == nullptr ) return;
    painter->save();

    int w = 10;
    int leftw = width() - w;
    int toph = height() / 10;
    int h = height() - toph * 2 ;

    Q_UNUSED(leftw);
    Q_UNUSED(h);

    QLinearGradient _linerGradient(0 , 0 , width() , height());

    for( quint8 i = 0; i < 255; i++  ){
        _linerGradient.setColorAt( static_cast<float>(i)/255.0 ,  generateColor( 255-i , 0 , 255 ));
    }
/*
    _linerGradient.setColorAt(0.00,  generateColor( 0 , 0 , 255 ));
    _linerGradient.setColorAt(0.25, Qt::yellow);
    _linerGradient.setColorAt(0.50, Qt::green);
    _linerGradient.setColorAt(0.75, Qt::blue);
    _linerGradient.setColorAt(1.00, Qt::black);
*/
  //  _linerGradient.setSpread(QGradient::PadSpread);
    painter->setPen(Qt::NoPen);
    painter->setBrush( _linerGradient );

    QRect _rect(0 ,0 ,width() ,height());
    _rect.setWidth(width());
    _rect.setHeight(height());
    painter->drawRect(_rect);

    painter->restore();
    painter->save();

    QFont font;
    font.setPixelSize(m_fontSize);
    font.setBold(false);

    painter->setFont(font);
    painter->setPen(Qt::black);

    if( m_showRange ){
        painter->drawText(0, height()/2, width(), 40, Qt::AlignCenter, QString::number( m_dBrange , 'f', 1) + "\n" + "dB" );
    }
    painter->restore();

    painter->save();
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(0, 2, width(), 26, Qt::AlignCenter, format(m_maxDB));
    painter->drawText(0, height() - 26, width(), 26, Qt::AlignCenter, format( m_minDB ));

    painter->restore();

}
