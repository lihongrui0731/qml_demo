/** ***********************************************************************************
  * @brief
  * this is show the frequence, this is a gradient color bar
  * 主要用来显示频率的渐变条
  *
  * @author guweicao                     @date 2021/5/5             @copyright Rhythm
  ************************************************************************************/
#ifndef FREQUENCY_LEGEND_BAR_H
#define FREQUENCY_LEGEND_BAR_H

#include <QWidget>
#include <QPaintEvent>
#include <QLinearGradient>
#include <QPainter>

class FrequencyLegendBar : public QWidget
{
    Q_OBJECT

public:

    explicit FrequencyLegendBar(QWidget *parent = nullptr);
    virtual ~FrequencyLegendBar() override;

    /**
     * @brief 设置图例中的最大值
     */
    void setMaxValue(float maxValue)
    {
        max_ = maxValue;
    }

    /**
     * @brief 设置最小值数值
     */
    void setMinValue(float minValue)
    {
        min_ = minValue;
    }

protected:

    void paintEvent(QPaintEvent* event) override;

private:

    float max_{};
    float min_{};

    QString format(float value)
    {
        return QString::number(value, 'f', 0);
    }

};

#endif // FREQUENCY_LEGEND_BAR_H
