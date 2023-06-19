#ifndef RHYTHMVIDEOGLES_H
#define RHYTHMVIDEOGLES_H

#include <QImage>
#include <QElapsedTimer>
#include "OpenGLESBase.h"

class RhythmVideoGLES : public OpenGLESBase
{
public:
    RhythmVideoGLES();
    virtual ~RhythmVideoGLES();
public:
    void setFrame(const unsigned int &width, const unsigned int &height, const unsigned char *frame);
    void setFrame(const unsigned int &width, const unsigned int &height, const QImage &image);

    void setFrameMaxMin( quint8 Max_ , quint8 Min_ ){
        rgbMax = (GLfloat)Max_ / 255.0;
        rgbMin = (GLfloat)Min_ / 255.0;
        // qDebug() << "rgbMax - rgbMin:" << rgbMax << rgbMin;
    }

    void setCloudFrame(const unsigned int &width, const unsigned int &height, const unsigned char *frame);
    void setNight( bool b){
        isNight = b;
    }
    bool night(){ return isNight; }
    float _brightness(){ return (float)brightness; }
    float _contrast(){ return (float)contrast; }
    float _saturation(){ return (float)saturation; }
    float _highLight(){ return (float)highLight; }
    float _shadow(){ return (float)shadow; }
    float _midtone(){ return (float)midtone; }
    float _coefficient(){ return (float)coefficient; }
    quint8 _method(){ return (quint8)method; }

    void paintGL();

    void setBrightness( float brightness_ ){ brightness = (GLfloat)brightness_; }
    void setContrast( float contrast_ ){ contrast = (GLfloat)contrast_; }
    void setSaturation( float saturation_ ){ saturation = (GLfloat)saturation_; }
    void setHighLight( float highLight_ ){ highLight = (GLfloat)highLight_; }
    void setShadow( float shadow_ ){ shadow = (GLfloat)shadow_; }
    void setMidtone( float midtone_ ){ midtone = (GLfloat)midtone_; }
    void setCoefficient( float coefficient_ ){ coefficient = (GLfloat)coefficient_; }
    void setMethod( quint8 method_ ){ method = (GLuint)method_; }

protected:
    virtual void resizeGL(int w, int h) override;

    virtual void initShaders() override;
    virtual void initTextures() override;

private:
    QOpenGLTexture *m_popenGLCloudTextrue{nullptr};
    unsigned int m_i_frameWidth{2};
    unsigned int m_i_frameHeight{2};
    unsigned int m_i_cloudWidth{2};
    unsigned int m_i_cloudHeight{2};

    // QElapsedTimer timer;

    // 开启夜视
    bool isNight{false};

    // 增强系数
    //  float brightness{37};
    //  float contrast{70};
    //  float PI{3.1415926};

    // 夜视算法参数
    double PI{3.1415926};
    GLfloat brightness{0.45};
    GLfloat contrast{1.0};
    GLfloat saturation{-0.1};
    GLfloat highLight{0.14};
    GLfloat shadow{0.0};
    GLfloat midtone{1.0};
    GLfloat coefficient{1.4};

    GLfloat rgbMax{1.0};
    GLfloat rgbMin{0.0};

    GLuint method{3};

    QElapsedTimer  m_Etimer;

};

#endif // RHYTHMVIDEOGLES_H
