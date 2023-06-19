/** ********************************************************************************************
 *  @brief 用于显示声成像云图中的颜色图例
 *
 *  @author guweicao                                                              @date 2021/5/5
 *  ********************************************************************************************/
#ifndef RHYTHMGRADIENTBAR_H
#define RHYTHMGRADIENTBAR_H

#include <QPainter>
#include <QPainterPath>
#include <QQuickItem>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QQuickPaintedItem>

class RhythmGradientBar : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(float maxDB READ maxDB WRITE setMaxdB NOTIFY maxDBChanged)
    Q_PROPERTY(float dBrange READ dBrange WRITE setRange NOTIFY dBrangeChanged)
    Q_PROPERTY(float minDB READ minDB WRITE setMindB NOTIFY minDBChanged)
    Q_PROPERTY(bool showRange READ showRange WRITE setShowRange NOTIFY showRangeChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

public:
    explicit RhythmGradientBar(QQuickItem *parent = nullptr);
    virtual ~RhythmGradientBar() override;

    /** ********************************************
     * @brief setMaxdB
     * @param maxdb_    @brief set max dB
     ***********************************************/
    Q_INVOKABLE void setMaxdB(const float maxdb_);

    /** ********************************************
     * @brief setRange
     * @param range_   @brief set range dB
     ***********************************************/
    Q_INVOKABLE void setRange(const float range_);

    /** ********************************************
     * @brief setMindB
     * @param mindb_   @brief set min dB
     ***********************************************/
    Q_INVOKABLE void setMindB(const float mindb_);

    Q_INVOKABLE void setShowRange( bool showRange_ );
    Q_INVOKABLE void setFontSize( int fontSize_ );
    Q_INVOKABLE void repaint(){update();};

    /** ********************************************
     * @brief setMinMaxB
     * @param mindb_
     * @param maxdb_
     ***********************************************/
    Q_INVOKABLE void setMinMaxB(const float mindb_, const float maxdb_);

    float maxDB() const {return m_maxDB;}
    float dBrange() const {return m_dBrange;}
    float minDB() const {return m_minDB;}
    bool showRange() const { return m_showRange; }

    int fontSize() const { return m_fontSize; }

    QString format( float f );
private:
  //  void resizeEvent(QResizeEvent*event_) ;
  //   void paintEvent(QPaintEvent*event_) ;
    void paint(QPainter *painter) override;

    QColor generateColor(float value , float min , float max );

    float m_maxDB;
    float m_dBrange;
    float m_minDB;
    bool m_showRange{true};
    int m_fontSize{10};
signals:
    void maxDBChanged();
    void dBrangeChanged();
    void minDBChanged();
    void showRangeChanged();
    void fontSizeChanged();
};

#endif // RHYTHMVIDEOGRADIENTBAR_H
