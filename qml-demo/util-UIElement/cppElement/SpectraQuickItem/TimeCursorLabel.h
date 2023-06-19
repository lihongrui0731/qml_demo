#ifndef TIME_CURSOR_LABEL_H
#define TIME_CURSOR_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QQuickItem>
#include <QScreen>
#include <QPen>
#include <QApplication>
#include <QDesktopWidget>
#include <QQuickPaintedItem>
#include <QTimer>
#include <QStyleOption>
#include <QList>
#include "math.h"
#include <QElapsedTimer>

/**
 * @brief 管理时频图上的时间戳标线，和当前播放位置，并负责从指定时间戳开始播放
 */
class TimeCursorLabel : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(int currentFrame READ currentFrame NOTIFY currentFrameChanged)
    Q_PROPERTY(int frameCount READ frameCount NOTIFY frameCountChanged)
    Q_PROPERTY(double timelineX READ timelineX NOTIFY timelineXChanged)
    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)

public:
    explicit TimeCursorLabel(QQuickItem *parent = nullptr):QQuickPaintedItem(parent)
    {
    //    QPalette _pe;
    //    _pe.setColor(QPalette::WindowText,Qt::white);
    //    QApplication::setPalette(_pe);

        timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
        connect(timer, SIGNAL(timeout()), this, SLOT(changeTimeCursor()));
        resetTimerCursor();
    }

    virtual ~TimeCursorLabel() override
    {
        // 若尚在播放当中则停止 timer
        resetTimerCursor();
        delete timer;
        timer = nullptr;
    }

    void setFrequency( int frequency_ ){
        m_frequency = frequency_;
        update();
        //rePaint();
    }

    Q_INVOKABLE void setFontSize( int fontSize_ ){
        m_fontSize = fontSize_;
    }

    Q_INVOKABLE void setDuration(float duration_)
    {
        m_f_duration = duration_ / 1000;
    }

    Q_INVOKABLE void setInterval(int inteval_)
    {
        m_i_interval = inteval_;
    }

    Q_INVOKABLE void resetTimerCursor()
    {
        stop();
        m_i_frameCount = 0;
        m_i_currentFrame = 0;
        m_i_vectorPosition = -1;
        m_timeLinePositonXF = -1;
        m_s_currentText = "";
        m_vector_markXList.clear();
        m_timeList.clear();
        run = false;
        update();
    }

    Q_INVOKABLE void fullChanged(){
        getPosFrequency();

        update();
    }

    Q_INVOKABLE void setFrameCount(const int& frameCount , const float& frameTime , QList<quint32> timeList )
    {
        m_i_frameCount = frameCount;
        m_i_currentFrame = 0;
        m_i_frameTime = frameTime;
        m_i_interval = static_cast<quint8>( qRound(m_i_frameTime / (m_i_frameCount-1)) );

        m_timeList = timeList;

        update();
        emit frameCountChanged();
    }

    Q_INVOKABLE void setFrequencyMax( float max )
    {
        m_frequencyMax = max;
        getPosFrequency();
        update();
    }

    Q_INVOKABLE void setFrequencyMin( float min )
    {
        m_frequencyMin = min;
        getPosFrequency();
        update();
    }

    Q_INVOKABLE int getFrequencyMax()
    {
        return m_frequencyMax;
    }

    Q_INVOKABLE int getFrequencyMin()
    {
        return m_frequencyMin;
    }

    Q_INVOKABLE double timelineX(){
        return m_timeLinePositonXF;
    }

    Q_INVOKABLE void setFrequency(float min , float max )
    {
        m_frequencyMax = max;
        m_frequencyMin = min;
        getPosFrequency();
        update();
    }

    Q_INVOKABLE int frameCount(){
       return m_i_frameCount;
    }

    Q_INVOKABLE float getClickTimeLine()
    {
        //quint32 v = static_cast<quint32>(qRound(static_cast<float>(m_i_currentFrame) / static_cast<float>(m_i_frameCount) * m_i_frameTime));
        quint32 v = m_timeList.value(m_i_currentFrame);
        return v;
    }

    int fontSize() { return m_fontSize; }
    int currentFrame() { return m_i_currentFrame; }

    Q_INVOKABLE void play()
    {

        run = true;

        timer->start( m_i_interval );
        eTimer.start();

        /*
        if ( m_i_timerID <= 0 )
            m_i_timerID = startTimer(m_i_interval);
        */
    }

    Q_INVOKABLE void stop()
    {
        timer->stop();
        lastElTimer = 0;
        run = false;
        /*
        if ( m_i_timerID >= 0 )
            killTimer(m_i_timerID);
        m_i_timerID = 0;
        */
    }

    int frequency(){
        return m_frequency;
    }

    /**
     * @brief 新增一个标记点
     */
    void insertMark()
    {
        if (  m_vector_markXList.indexOf(m_timeLinePositonXF) == -1) {
            m_vector_markXList.push_back(m_timeLinePositonXF);
            std::sort(m_vector_markXList.begin(), m_vector_markXList.end(),
                      [&](const double& start, const double& end)->bool{ return start <= end; });
        }
        update();
    }

    /**
     * @brief 删除所有标记点
     */
    void removeMarks()
    {
        QVector<int>::iterator _markItor = m_vector_markXList.begin();
        while ( _markItor != m_vector_markXList.end() ) {
            if ( *_markItor == m_timeLinePositonXF ) {
                m_vector_markXList.erase(_markItor);
                continue;
            }
            _markItor++;
        }
        update();
    }

    /**
     * @brief seek 到下一个标记点
     */
    void nextMark()
    {
        if ( m_vector_markXList.size() > 0 ) {
            m_i_vectorPosition++;
            m_i_vectorPosition  %= m_vector_markXList.size();
            m_timeLinePositonXF = m_vector_markXList.at(m_i_vectorPosition);
            emit currentCursor( static_cast<float>(m_timeLinePositonXF)/static_cast<float>(width()) );
        }
        update();
    }

    /**
     * @brief seek 到上一个标记点
     */
    void lastMark()
    {
        if ( m_vector_markXList.size() > 0 ) {
            m_i_vectorPosition += (m_vector_markXList.size()-1);
            m_i_vectorPosition %= m_vector_markXList.size();
            m_timeLinePositonXF = m_vector_markXList.at(m_i_vectorPosition);
            emit currentCursor( static_cast<float>(m_timeLinePositonXF)/static_cast<float>(width()) );
        }
        update();
    }

//    /**
//     * @brief setText
//     * @param text_   @brief set the time line title
//     */
//    void setText(const QString& text_)
//    {
//        m_s_currentText = text_;
//    }

public slots:

    Q_INVOKABLE void changeTimeCursor(){
        int t_ = m_i_interval;
        if( eTimer.isValid() ){
            t_ = eTimer.elapsed() < m_i_interval ? m_i_interval:eTimer.elapsed();
            int i_ = ceil( ( t_ + surplusTime - lastElTimer ) / m_i_interval );
            if( i_ == 0){
                return;
            }
            surplusTime = ( t_ + surplusTime - lastElTimer ) % m_i_interval;
            // if(  t_ - lastElTimer > 200 ){
            // 判断是跳帧 还是停止后重新开始
            if( lastElTimer == 0 ){
                m_i_currentFrame++;
            }else{
                m_i_currentFrame += i_;
            }
        }else{
            m_i_currentFrame++;
        }
        lastElTimer = t_;

        if ( m_i_currentFrame >= m_i_frameCount ){
            m_i_currentFrame = 0;
        }

        float _ratio = static_cast<float>(m_i_currentFrame)/static_cast<float>(m_i_frameCount-1.0);
        m_timeLinePositonXF = static_cast<double>( static_cast<double>(width() ) * _ratio) ;

        m_s_currentText = QString::number((m_timeList.value(m_i_currentFrame) - m_i_frameTime)/1000, 'f', 2) + "s";
        update();

        emit currentFrameChanged(m_i_currentFrame);
    }

    Q_INVOKABLE void setCurrentFrame(const int& currentFrame_)
    {
        m_i_currentFrame = currentFrame_;
        double _positionRatio = static_cast<double>(m_i_currentFrame)/static_cast<double>(m_i_frameCount-1.0);
        m_timeLinePositonXF = static_cast<double>(width() )*_positionRatio;

        //m_s_currentText = QString::number(_positionRatio*m_f_duration - m_f_duration, 'g', 6);
        m_s_currentText = QString::number((m_timeList.value(m_i_currentFrame) - m_i_frameTime)/1000, 'f', 2) + "s";
        //emit currentCursor( static_cast<float>(m_timeLinePositonXF)/static_cast<float>(width()) );

        update();
        if( !run ){
            emit currentFrameChanged(m_i_currentFrame);
        }
        //qDebug() << m_i_currentFrame << "-1" ;

    }

    Q_INVOKABLE void setPostion(const float& position_x)
    {
        m_timeLinePositonXF = position_x;

        double _positionRatio = static_cast<double>(m_timeLinePositonXF)/static_cast<double>(width());
        m_i_currentFrame = static_cast<int>((m_i_frameCount-1.0)*_positionRatio);
        //m_s_currentText = QString::number(_positionRatio*m_f_duration - m_f_duration, 'g', 6);

        if( m_i_currentFrame == 0 && m_s_currentText == "" ){
        }else{
            m_s_currentText = QString::number((m_timeList.value(m_i_currentFrame) - m_i_frameTime)/1000, 'f', 2) + "s";
        }
       //emit currentCursor( static_cast<float>(m_timeLinePositonXF)/static_cast<float>(width()) );
        update();
        emit currentFrameChanged( m_i_currentFrame );
    }

signals:
    void fontSizeChanged();
    void currentCursor(const float& position_);
    void currentFrameChanged( double currentFrame );
    void frameCountChanged();
    void timelineXChanged();
    void frequencyChanged();
//    void currentPosition(const float& poistion);

protected:
/*
    void mousePressEvent(QMouseEvent* event) override
    {
        m_timeLinePositonXF = event->pos().x();
        double _positionRatio = static_cast<double>(m_timeLinePositonXF)/static_cast<double>(width());
        m_i_currentFrame = static_cast<int>(m_i_frameCount*_positionRatio);
        m_s_currentText = QString::number(_positionRatio*m_f_duration, 'g', 6);
        m_s_currentText = m_s_currentText.left(7);
        emit currentCursor( static_cast<float>(m_timeLinePositonXF)/static_cast<float>(width()) );
        update();
    }

    void mouseMoveEvent(QMouseEvent*event) override
    {
        m_timeLinePositonXF = event->pos().x();
        m_timeLinePositonXF = m_timeLinePositonXF > 0 ? m_timeLinePositonXF : 0;
        m_timeLinePositonXF = m_timeLinePositonXF < width() ? m_timeLinePositonXF : width();
        double _positionRatio = static_cast<double>(m_timeLinePositonXF)/static_cast<double>(width());
        m_i_currentFrame = static_cast<int>(m_i_frameCount*_positionRatio);
        m_s_currentText = QString::number(_positionRatio*m_f_duration, 'g', 6);
        m_s_currentText = m_s_currentText.left(7);
        emit currentCursor( static_cast<float>(m_timeLinePositonXF)/static_cast<float>(width()) );
        update();
    }
*/

    void timerEvent(QTimerEvent* event) override
    {
        Q_UNUSED( event );
        if ( m_i_frameCount > 0 ) {
            if ( m_i_currentFrame > m_i_frameCount ){
                m_i_currentFrame = 0;
            }
            float _ratio = static_cast<float>(m_i_currentFrame)/static_cast<float>(m_i_frameCount);
            m_timeLinePositonXF = static_cast<int>( static_cast<float>(width()) * _ratio) ;

            m_s_currentText = QString::number(static_cast<double>(_ratio*m_f_duration), 'f', 2);

            emit currentCursor( _ratio );
            m_i_currentFrame++;
        } else {
            stop();
        }

        update();
    }

    void paint(QPainter* painter) override
    {
        if( painter == nullptr ) return;

        painter->save();
        QPen _pen;

        // draw make line and set pen style
        _pen.setColor(Qt::green);
        _pen.setStyle(Qt::SolidLine);
        painter->setPen(_pen);
        foreach( auto _markItor, m_vector_markXList )
            painter->drawLine(_markItor, 0, _markItor, height());

        painter->restore();
        painter->save();

        // draw time line and set pen style;
        _pen.setColor(Qt::lightGray);
        _pen.setStyle(Qt::SolidLine);
        _pen.setWidth(2 / m_dpi);

        QFont font = qApp->font();
        font.setPixelSize(m_fontSize);
        font.setBold(false);
        painter->setFont(font);
        painter->setPen(_pen);

        // frequencyMax
        painter->drawLine(0,static_cast<int>(pos_frequencyMax), width(), static_cast<int>(pos_frequencyMax));
        // frequencyMin
        painter->drawLine(0, static_cast<int>(pos_frequencyMin), width(), static_cast<int>(pos_frequencyMin));

        painter->drawLine(m_timeLinePositonXF, 0, m_timeLinePositonXF, height());

        QPainterPath path;

        QFont font_ = qApp->font();
        font_.setPixelSize(m_fontSize);
        font_.setBold(true);

        // draw text
        int _textWidth = 60 / m_dpi;
        if ( m_timeLinePositonXF + _textWidth <= width() ) {
            path.addText(m_timeLinePositonXF+5, 20, font_, m_s_currentText);
        } else {
            path.addText(m_timeLinePositonXF-_textWidth-5, 20, font_, m_s_currentText);
        }

        painter->strokePath(path, _pen);
        painter->fillPath(path, QBrush(QColor(Qt::black)));

        painter->restore();

    }

    void getPosFrequency(){
        pos_frequencyMin = height()/m_frequency*(m_frequency - m_frequencyMin/1000);
        pos_frequencyMax = height()/m_frequency*(m_frequency - m_frequencyMax/1000);
    }

private:

    int m_frequency{100};

    QString m_s_currentText{""};
    double m_timeLinePositonXF{-1};
    QVector<int> m_vector_markXList;
    int m_i_vectorPosition{-1};
    int m_i_timerID{0};
    int m_i_frameCount{0};
    int m_i_currentFrame{1};
    float m_f_duration{10};
    int m_i_interval{10};
    float m_i_frameTime{0};
    QList<quint32> m_timeList;

    float m_frequencyMin{0};
    float m_frequencyMax{0};
    int pos_frequencyMin{-1};
    int pos_frequencyMax{-1};
    int m_fontSize{16};
    float m_dpi{static_cast<float>(qApp->devicePixelRatio())};
    bool run{false};
    QTimer *timer{nullptr};

    QElapsedTimer eTimer;

    int lastElTimer{0};
    int surplusTime{0}; // 余数
};

#endif // TIME_CURSOR_LABEL_H
