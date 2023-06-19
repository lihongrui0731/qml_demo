#include "RhythmSpectraGLES.h"
#include <cmath>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>

RhythmSpectraGLES::RhythmSpectraGLES() :
    OpenGLESBase(), m_f_minScale(1.0f), m_f_maxScale(20.0f)
{
    m_user_matrix4x4.setToIdentity();
}

RhythmSpectraGLES::~RhythmSpectraGLES(){
     qDebug() << " 析构 spec gles ";

}

const float& RhythmSpectraGLES::getScaleSize()const
{
    return m_user_matrix4x4(1, 1);
}

void RhythmSpectraGLES::wheelEvent(QWheelEvent*event)
{
    return;
    const float float_epsilon { std::numeric_limits<float>::epsilon() };

    float _f_scale = m_user_matrix4x4(1, 1);

    float _y1 = 1.0f - m_user_matrix4x4(1, 3); // local top y
    float _y2 = -1.0f - m_user_matrix4x4(1, 3); // local buttom y
    float _rotio = static_cast<float>(event->y()) / static_cast<float>(1000); //height()
    float _localy = _y1 * ( 1 - _rotio ) + _y2 * _rotio;

    if ( event->delta() > 0 ) {
        _f_scale *= 1.1f;
        if ( fabsf(_f_scale - m_f_maxScale) <= float_epsilon ) {
            return;
        } else if ( _f_scale > m_f_maxScale ) {
            // never read
            //_localy /= m_f_maxScale/_f_scale;
            //_f_scale = m_f_maxScale;
            return;
        } else {
            _localy *= 1.1f;
        }
    } else if ( event->delta() < 0 ) {
        _f_scale *= 0.9f;
        if (fabsf(_f_scale - m_f_minScale) <= float_epsilon) {
            return;
        } else if ( _f_scale < m_f_minScale ) {
            // never read
            //_localy /= m_f_minScale/_f_scale;
            //_f_scale = m_f_minScale;
            return;
        } else {
            _localy *= 0.9f;
        }
    } else {
        return;
    }
    float _worldy = (1-2.0f* static_cast<float>(event->pos().y())/ static_cast<float>(1000)); //height()
    m_user_matrix4x4(1, 1) = _f_scale;
    // deal with the the scale over the limit -1.0f ~ 1.0f
    float _localCoordinateMoved = _worldy - _localy;
    if ( _f_scale+_localCoordinateMoved < 1.0f ) {
        _localCoordinateMoved += 1.0f-_localCoordinateMoved-_f_scale;
    } else if ( _localCoordinateMoved - _f_scale > -1.0f ) {
        _localCoordinateMoved -= _localCoordinateMoved-_f_scale+1.0f;
    }
    m_user_matrix4x4(1, 3) =_localCoordinateMoved;

   // emit scaleAndMove(_f_scale, _localCoordinateMoved);

//    float _oldy = (1-2.0f*event->pos().y()/height())*m_f_scale;
//    qDebug()<<_oldy;
//    m_user_matrix4x4(1, 3) += _oldy;
//    qDebug()<<m_user_matrix4x4;
//    if ( event->delta() > 0 ) {
//        m_f_scale *= 1.1f;
//        m_f_scale = m_f_scale <= m_f_maxScale ? m_f_scale : m_f_maxScale;
//    } else if ( event->delta() < 0 ) {
//        m_f_scale *= 0.9f;
//        m_f_scale = m_f_scale >= m_f_minScale ? m_f_scale : m_f_minScale;
//    } else {
//        return;
//    }
//    float _newy = (1-2.0f*event->pos().y()/height())*m_f_scale;
//    qDebug()<<_newy;
//    m_user_matrix4x4(1, 1) = m_f_scale;
//    m_user_matrix4x4(1, 3) -= _newy;
//    qDebug()<<m_user_matrix4x4;
    //repaint();
    //zz todo
}

void RhythmSpectraGLES::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    // never read
    //float _y1 = 1.0f - m_user_matrix4x4(1, 3); // local top y
    //float _y2 = -1.0f - m_user_matrix4x4(1, 3); // local buttom y
    //float _rotio = static_cast<float>(event->y()) / static_cast<float>(height());
    //float _localy = _y1 * ( 1 - _rotio ) + _y2 * _rotio;
    //float _worldy = (1.0f-2.0f* static_cast<float>(event->pos().y()) / static_cast<float>(height()));

}

void RhythmSpectraGLES::initQuadrangle()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        {QVector3D(-1.0f, -1.0f,  0.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  0.0f), QVector2D(0.0f, 1.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  0.0f), QVector2D(1.0f, 0.0f)},  // v2
        {QVector3D( 1.0f,  1.0f,  0.0f), QVector2D(1.0f, 1.0f)}, // v3
    };

    // Indices for drawing quadrangle faces using triangle strips.
    GLushort indices[] = {
        0,  1,  2,  3     // triangle strip ( v0,  v1,  v2,  v3)
    };

    // Transfer vertex data to VBO 0
    m_glbuff_arrayBuf.bind();
    m_glbuff_arrayBuf.allocate(vertices, 4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    m_glbuff_indexBuf.bind();
    m_glbuff_indexBuf.allocate(indices, 4 * sizeof(GLushort));
}

void RhythmSpectraGLES::initShaders()
{
    // Compile vertex shader
    m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl");
    /*
    if (!m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    {
            return;
    }
    */
    // Compile fragment shader
    m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl");
    /*
    if (!m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
    {
        return;
    }
    */
    // Link shader pipeline
    m_openGLShaderProgram.link();

    // Bind shader pipeline for use
    m_openGLShaderProgram.bind();
}

void RhythmSpectraGLES::initTextures()
{
    m_i_width = 2;
    m_i_height = 2;
    constexpr unsigned char data[] = {0, 0, 0, 255, 0, 0, 0, 255,
                                      0, 0, 0, 255, 0, 0, 0, 255};

    /* **********************************************************************************
     * this is constructe from unsigned char array
     * **********************************************************************************/
    m_popenGLTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_popenGLTexture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);

    //    m_popenGLTexture->create();
    // given some `width`, `height` and `data_ptr`
    m_popenGLTexture->setSize(m_i_width, m_i_height, 1);
    m_popenGLTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_popenGLTexture->allocateStorage();
    m_popenGLTexture->setData( QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data );
    m_popenGLTexture->setWrapMode(QOpenGLTexture::ClampToEdge);

    /* **********************************************************************************
     * this is constructe a texture from a QImage
     * **********************************************************************************/
    //    QImage _image(data, m_i_width, m_i_height, m_i_width*3, QImage::Format_RGB888);
    //    m_popenGLTexture = new QOpenGLTexture(_image.mirrored());
    //    // Set nearest filtering mode for texture minification
    //    m_popenGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    //    // Set bilinear filtering mode for texture magnification
    //    m_popenGLTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
    //    // Wrap texture coordinates by repeating
    //    m_popenGLTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
}

void RhythmSpectraGLES::resizeGL(int, int)
{
    // Calculate aspect ratio
    //    qreal aspect = qreal(w) / qreal(h ? h : 1);
    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    //    const qreal zNear = 3.0, zFar = 7.0, fov = 30.0;
    // Reset projection
    m_matrix4x4_projection.setToIdentity();
    // Set perspective projection
    //    m_matrix4x4_projection.perspective(fov, 1, zNear, zFar);

    m_matrix4x4_projection.ortho(-1, 1, -1, 1, -1000, 1000);
}

void RhythmSpectraGLES::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_popenGLTexture->bind(0);
    m_openGLShaderProgram.bind();
    // Set modelview-projection matrix
    m_openGLShaderProgram.setUniformValue("mvp_matrix", m_matrix4x4_projection*m_user_matrix4x4);

    // Use texture unit 0 which contains cube.png
    m_openGLShaderProgram.setUniformValue("texture", 0);

    // Draw cube geometry
    drawQuadrangle();
    m_openGLShaderProgram.release();
    m_popenGLTexture->release();
}

void RhythmSpectraGLES::setFrame(const unsigned int &width, const unsigned int &height, const unsigned char *frame)
{
    /* *************************************************************************************************************************
     * this is used unchar construct a texture
     * *************************************************************************************************************************/

    if ( m_i_width != static_cast<int>(width) || m_i_height != static_cast<int>(height) ) {
        m_popenGLTexture->destroy();
        m_popenGLTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);

        m_i_width = static_cast<int>(width);
        m_i_height = static_cast<int>(height);

        m_popenGLTexture->setFormat(QOpenGLTexture::RGB8_UNorm);
        m_popenGLTexture->setSize(m_i_width, m_i_height);
        m_popenGLTexture->allocateStorage();
        //        // Set nearest filtering mode for texture minification
        m_popenGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
        //        // Set bilinear filtering mode for texture magnification
        m_popenGLTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
        //        // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
        m_popenGLTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
       //  qDebug() << "depth ---- " << m_popenGLTexture->depth();

    }
    m_popenGLTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, frame);

    /* *************************************************************************************************************************
     * this is used a QImage build a texture
     * *************************************************************************************************************************/
    //    QImage _image(frame, static_cast<int>(width), static_cast<int>(height), static_cast<int>(width)*3, QImage::Format_RGB888);
    //    m_popenGLTexture->destroy();
    //    m_popenGLTexture->setData(_image.mirrored());
}
