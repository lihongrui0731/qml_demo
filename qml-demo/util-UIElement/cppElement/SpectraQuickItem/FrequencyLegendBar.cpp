#include "FrequencyLegendBar.h"

#include <QDebug>
#include <QFont>

FrequencyLegendBar::FrequencyLegendBar(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(20);

    QFont f { font() };
    f.setPixelSize(9);
    setFont(f);

    setMaxValue(80.1f);
    setMinValue(0.0f);
}

FrequencyLegendBar::~FrequencyLegendBar()
{
}

void FrequencyLegendBar::paintEvent(QPaintEvent*)
{
    QRect r( rect() );
    QPainter painter(this);

    QLinearGradient lg( r.topLeft(), r.bottomRight());
    lg.setColorAt(0.00, Qt::red);
    lg.setColorAt(0.25, Qt::yellow);
    lg.setColorAt(0.50, Qt::green);
    lg.setColorAt(0.75, Qt::blue);
    lg.setColorAt(1.00, Qt::black);
    painter.setBrush(lg);
    painter.drawRect(r);

    painter.setPen(QColor(Qt::black));
    painter.drawText(r, Qt::AlignHCenter | Qt::AlignTop, format(max_));
    painter.setPen(QColor(Qt::white));
    painter.drawText(r, Qt::AlignHCenter | Qt::AlignBottom, format(min_));
}
