#include "fontoutlinelabel.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include <QByteArray>
#include <QBuffer>

FontOutlineLabel::FontOutlineLabel()
{
    m_outLineColor = QColor(60,179,113);
    m_contentColor = QColor(Qt::white);
    m_fontOutLineWidth = 2;
    m_y =  0;
    m_x = 0;
    m_fontBold = true;
    m_fontSize = 14;
    m_font.setPixelSize(m_fontSize);
    m_font.setBold(m_fontBold);
    m_text = QString("");
}

FontOutlineLabel::~FontOutlineLabel(){

}

void FontOutlineLabel::setText(const QString &strText)
{
    m_text = strText;
    update();
}

void FontOutlineLabel::setStartPos(int &x, int &y)
{
    m_x = x;
    m_y = y;
}

void FontOutlineLabel::setStartPosX(int &x )
{
    m_x = x;
}

void FontOutlineLabel::setStartPosY(int &y )
{
    m_y = y;
}

void FontOutlineLabel::setFontOutLineColor(const QColor &color)
{
    m_outLineColor = color;
}

void FontOutlineLabel::setFontOutLineWidth(int wide)
{
    m_fontOutLineWidth = wide;
}

void FontOutlineLabel::setFontContentColor(const QColor &color)
{
    m_contentColor = color;
    update();
}

void FontOutlineLabel::setFontBold(bool fontBold){
    m_fontBold = fontBold;
    m_font.setBold(m_fontBold);
}
void FontOutlineLabel::setFontSize(int fontSize){
    m_fontSize = fontSize;
    m_font.setPixelSize(m_fontSize);
}

void FontOutlineLabel::setWidth( int w ){
    m_w = w;
}

void FontOutlineLabel::setHeight( int h ){
    m_h = h;
}

void FontOutlineLabel::paint(QPainter* painter)
{
    if( painter == nullptr ) return;
    painter->save();

    QStyleOption opt;
   // opt.initFrom(this);
   // style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QPainterPath path;
    path = pathText(m_x, m_y, m_font, m_text);

    QPen pen;
    pen.setColor(m_outLineColor);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(m_fontOutLineWidth);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->strokePath(path, pen);
    painter->fillPath(path, QBrush(m_contentColor));

    painter->restore();

    // update();
}

// 处理换行文字
QPainterPath FontOutlineLabel::pathText(int x, int y, QFont font, const QString &text)
{
    QStringList textList = text.split("\n");
    int len = textList.length();

    QPainterPath path;

    qreal lineHeight = QFontMetricsF(font).height();

    int lineCount = 1;
    QByteArray content = text.toUtf8();
    QBuffer buff(&content);
    buff.open(QBuffer::ReadOnly);

    while(!buff.atEnd()) {

        QByteArray line = buff.readLine();

        qreal width = QFontMetricsF(font).horizontalAdvance(line);

        path.addText(x + ( m_w - width ) / 2 , y + ( m_h - len * lineHeight ) / 2 + lineCount * lineHeight - 3, font, line);
        lineCount++;
    }
    return path;
}

