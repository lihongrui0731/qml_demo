#ifndef RHYTHMSPECTRAGLES_H
#define RHYTHMSPECTRAGLES_H

#include <limits>
#include "OpenGLESBase.h"

class RhythmSpectraGLES : public OpenGLESBase{
public:
    RhythmSpectraGLES();
    virtual ~RhythmSpectraGLES();

    void setFrame(const unsigned int &width, const unsigned int &height, const unsigned char* frame);
    const float& getScaleSize()const;

    void paintGL();
    void resizeGL(int w, int h) override;

protected:

    virtual void initShaders() override;
    virtual void initTextures() override;
    virtual void initQuadrangle() override;
    virtual void wheelEvent(QWheelEvent*event) ;
    virtual void mousePressEvent(QMouseEvent* event) ;

private:
    const float m_f_minScale;
    const float m_f_maxScale;
    QMatrix4x4 m_user_matrix4x4;
    int m_i_width{2};
    int m_i_height{2};

signals:
    void scaleAndMove(const float& move, const float& scale);
};

#endif // RHYTHMSPECTRAGLES_H
