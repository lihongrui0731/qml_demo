#ifndef FONTOUTLINELABEL_H
#define FONTOUTLINELABEL_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QPainterPath>

/******
 * 描边字体类
 * *****/
class FontOutlineLabel:public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged )
    Q_PROPERTY(int startPosX READ startPosX WRITE setStartPosX NOTIFY startPosChanged)
    Q_PROPERTY(int startPosY READ startPosY WRITE setStartPosY NOTIFY startPosChanged)
    Q_PROPERTY(QColor fontOutLineColor READ fontOutLineColor WRITE setFontOutLineColor NOTIFY fontOutLineColorChanged)
    Q_PROPERTY(int fontOutLineWidth READ fontOutLineWidth WRITE setFontOutLineWidth NOTIFY fontOutLineWidthChanged)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontContentColor NOTIFY fontColorChanged)

    Q_PROPERTY(bool fontBold READ fontBold WRITE setFontBold NOTIFY fontChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontChanged)

    Q_PROPERTY(int w READ width_ WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int h READ height_ WRITE setHeight NOTIFY heightChanged)

public:
    FontOutlineLabel();
    ~FontOutlineLabel();

public:
    Q_INVOKABLE void setText(const QString& strText);//设置显示的文本
    Q_INVOKABLE void setStartPos(int &x,int &y);//设置字体开始写的起始点
    Q_INVOKABLE void setFontOutLineColor(const QColor &color);//设置字体轮廓线的颜色
    Q_INVOKABLE void setFontOutLineWidth(int wide = 1);//设置字体轮廓线的线宽
    Q_INVOKABLE void setFontContentColor(const QColor &color);//设置字体填充的颜色
    Q_INVOKABLE void setStartPosX(int &x );
    Q_INVOKABLE void setStartPosY(int &y);
    Q_INVOKABLE void setFontBold(bool fontBold);
    Q_INVOKABLE void setFontSize(int fontSize);
    Q_INVOKABLE void setWidth(int w);
    Q_INVOKABLE void setHeight(int h);

protected:
    void paint(QPainter* painter) override;

private:
    QColor m_outLineColor;//字体轮廓线颜色
    QColor m_contentColor;//字体填充颜色
    int m_fontOutLineWidth;//字体轮廓线的线宽
    int m_y;//字体起始点的横坐标
    int m_x;//字体起始点的纵坐标
    QFont m_font;//字体
    bool m_fontBold;
    int m_fontSize;
    QString m_text;//显示文本
    int m_w; //组件宽度
    int m_h; //组件高度

    QString text(){
        return m_text;
    }

    int startPosX(){
        return m_x;
    }
    int startPosY(){
        return m_y;
    }
    QColor fontOutLineColor(){
        return m_outLineColor;
    }
    int fontOutLineWidth(){
        return m_fontOutLineWidth;
    }
    QColor fontColor(){
        return m_contentColor;
    }
    bool fontBold(){
        return m_fontBold;
    }
    int fontSize(){
        return m_fontSize;
    }
    int width_(){
        return m_w;
    }
    int height_(){
        return m_h;
    }

    QPainterPath pathText(int x, int y, QFont font, const QString &text);

signals:
    void textChanged();
    void fontChanged();
    void startPosChanged();
    void fontOutLineColorChanged();
    void fontOutLineWidthChanged();
    void fontColorChanged();
    void widthChanged();
    void heightChanged();
};

#endif // FONTOUTLINELABEL_H
