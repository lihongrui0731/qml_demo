#include "RhythmVideoGLES.h"

#include <memory>
#include <math.h>

RhythmVideoGLES::RhythmVideoGLES()
    : OpenGLESBase()
{   
}

RhythmVideoGLES::~RhythmVideoGLES()
{
    qDebug() << " 析构 video GLES";

    if( m_popenGLCloudTextrue != nullptr ){
        m_popenGLCloudTextrue->destroy();
        delete m_popenGLCloudTextrue;
        m_popenGLCloudTextrue = nullptr;
    }
}

void RhythmVideoGLES::initShaders()
{
    // Compile vertex shader
    m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vtexture.glsl");
    /*
    if (!m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vtexture.glsl"))
    {
        return;
    }
    */
    m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ftexture.glsl");
    // Compile fragment shader
    /*
    if (!m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ftexture.glsl"))
    {
        return;
    }
    */
    // Link shader pipeline
    m_openGLShaderProgram.link();

    // Bind shader pipeline for use
    m_openGLShaderProgram.bind();
}

void RhythmVideoGLES::initTextures()
{
    unsigned char data[] = {0, 0, 0, 255, 0, 0, 0, 255,
                            0, 0, 0, 255, 0, 0, 0, 255};

    /** ***********************************************************************************
     ** this is constructed a texture immidiate from array
     **************************************************************************************/
    m_popenGLTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_popenGLTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_popenGLTexture->setSize(static_cast<int>(m_i_frameWidth), static_cast<int>(m_i_frameHeight) , 1);
    m_popenGLTexture->allocateStorage();
    // Set nearest filtering mode for texture minification
    m_popenGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    m_popenGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    // Wrap texture coordinates by repeating
    m_popenGLTexture->setWrapMode(QOpenGLTexture::Repeat);
    m_popenGLTexture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data);

    /** ***********************************************************************************
     ** this is constructed a texture immidiate from array
     **************************************************************************************/
    m_popenGLCloudTextrue = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_popenGLCloudTextrue->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_popenGLCloudTextrue->setSize( static_cast<int>(m_i_cloudWidth), static_cast<int>(m_i_cloudHeight), 1);
    m_popenGLCloudTextrue->allocateStorage();
    // Set nearest filtering mode for texture minification
    m_popenGLCloudTextrue->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    m_popenGLCloudTextrue->setMagnificationFilter(QOpenGLTexture::Linear);
    // Wrap texture coordinates by repeating
    m_popenGLCloudTextrue->setWrapMode(QOpenGLTexture::Repeat);
    m_popenGLCloudTextrue->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data);

    /** ***********************************************************************************
     ** this is constructed a texture by creating a Qimage
     **************************************************************************************/
//    QImage _image(data, 2, 2, 2*3, QImage::Format_RGB888);
//    m_popenGLTexture = new QOpenGLTexture(_image.mirrored());
//    m_popenGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);
//    m_popenGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
//    m_popenGLTexture->setWrapMode(QOpenGLTexture::Repeat);

//    m_popenGLCloudTextrue = new QOpenGLTexture(_image.mirrored());
//    m_popenGLCloudTextrue->setMinificationFilter(QOpenGLTexture::Nearest);
//    m_popenGLCloudTextrue->setMagnificationFilter(QOpenGLTexture::Linear);
//    m_popenGLCloudTextrue->setWrapMode(QOpenGLTexture::Repeat);
}

void RhythmVideoGLES::resizeGL(int, int)
{
    // Calculate aspect ratio
//    qreal aspect = qreal(w) / qreal(h ? h : 1);
    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
//    const qreal zNear = 3.0, zFar = 7.0, fov = 30.0;
    // Reset projection
    m_matrix4x4_projection.setToIdentity();
    // Set perspective projection
//    m_matrix4x4_projection.perspective(fov, 1, zNear, zFar);
    m_matrix4x4_projection.ortho(-1, 1, -1, 1, -1, 1);
}

void RhythmVideoGLES::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 光斑平滑算法
    #if defined (WIN32)
       // glEnable(GL_POINT_SMOOTH);
       // glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    #endif

    m_popenGLTexture->bind(0);
    m_popenGLCloudTextrue->bind(1);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, 0.0);
    matrix.rotate(m_quaternion_rotation);

    m_openGLShaderProgram.bind();
    // Set modelview-projection matrix
    m_openGLShaderProgram.setUniformValue("mvp_matrix", m_matrix4x4_projection * matrix);

    // Use texture unit 0 which contains cube.png

    m_openGLShaderProgram.setUniformValue("isNight", isNight);
    m_openGLShaderProgram.setUniformValue("brightness", brightness);
    m_openGLShaderProgram.setUniformValue("contrast", contrast);
    m_openGLShaderProgram.setUniformValue("saturation", saturation);
    m_openGLShaderProgram.setUniformValue("highLight", highLight);
    m_openGLShaderProgram.setUniformValue("shadow", shadow);
    m_openGLShaderProgram.setUniformValue("midtone", midtone);
    m_openGLShaderProgram.setUniformValue("method", method);

    m_openGLShaderProgram.setUniformValue("globalRgbMax", rgbMax);
    m_openGLShaderProgram.setUniformValue("globalRgbMin", rgbMin);

    // 自算法 系数
    m_openGLShaderProgram.setUniformValue("coefficient", coefficient);

    m_openGLShaderProgram.setUniformValue("texture", 0);
    m_openGLShaderProgram.setUniformValue("texture1", 1);

    // Draw cube geometry
    // setFrame();

    drawQuadrangle();

    m_openGLShaderProgram.release();
    m_popenGLTexture->release(0);
    m_popenGLCloudTextrue->release(1);
}

/**************
 *  渲染 rgb
 **/
void RhythmVideoGLES::setFrame(const unsigned int &width, const unsigned int &height, const unsigned char *frame)
{

    if ( m_i_frameWidth != width || m_i_frameHeight != height ) {
        m_popenGLTexture->destroy();
        m_popenGLTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);

        m_i_frameWidth = width;
        m_i_frameHeight = height;
        m_popenGLTexture->setFormat(QOpenGLTexture::RGB8_UNorm);
        m_popenGLTexture->setSize(static_cast<int>(m_i_frameWidth), static_cast<int>(m_i_frameHeight),1);
        m_popenGLTexture->allocateStorage();
        // Set nearest filtering mode for texture minification
        m_popenGLTexture->setMinificationFilter(QOpenGLTexture::Linear);
        // Set bilinear filtering mode for texture magnification
        m_popenGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        // Wrap texture coordinates by repeating
        m_popenGLTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    }

    m_popenGLTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, frame);

    //    QImage _image(_data, static_cast<int>(width) , static_cast<int>(height), static_cast<int>(width)*3, QImage::Format_RGB888);
//    m_popenGLTexture->setData(_image.mirrored());
}

/**************
 *  直接渲染 image
 **/
void RhythmVideoGLES::setFrame(const unsigned int &width, const unsigned int &height, const QImage &frame)
{
    if ( m_i_frameWidth != width || m_i_frameHeight != height ) {
        m_popenGLTexture->destroy();
        m_popenGLTexture = new QOpenGLTexture(frame , QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
        m_i_frameWidth = width;
        m_i_frameHeight = height;
        m_popenGLTexture->setSize(static_cast<int>(m_i_frameWidth), static_cast<int>(m_i_frameHeight),1);
        m_popenGLTexture->setFormat(QOpenGLTexture::RGB8_UNorm);
        m_popenGLTexture->allocateStorage();
        // Set nearest filtering mode for texture minification
        m_popenGLTexture->setMinificationFilter(QOpenGLTexture::Linear);
        // Set bilinear filtering mode for texture magnification
        m_popenGLTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        // Wrap texture coordinates by repeating
        m_popenGLTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    }else{
        m_popenGLTexture->setData(frame , QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    }
}

void RhythmVideoGLES::setCloudFrame(const unsigned int &width, const unsigned int &height, const unsigned char *frame)
{
    if ( m_i_cloudWidth != width || m_i_cloudHeight != height ) {
        m_popenGLCloudTextrue->destroy();
        m_popenGLCloudTextrue = new QOpenGLTexture(QOpenGLTexture::Target2D);

        m_i_cloudWidth = width;
        m_i_cloudHeight = height;

        m_popenGLCloudTextrue->setSize( static_cast<int>(m_i_cloudWidth), static_cast<int>(m_i_cloudHeight));
        m_popenGLCloudTextrue->setFormat(QOpenGLTexture::RGB8_UNorm);
        m_popenGLCloudTextrue->allocateStorage();
        // Set nearest filtering mode for texture minification
        m_popenGLCloudTextrue->setMinificationFilter(QOpenGLTexture::Linear);
        // Set bilinear filtering mode for texture magnification
        m_popenGLCloudTextrue->setMagnificationFilter(QOpenGLTexture::Linear);
        // Wrap texture coordinates by repeating
        m_popenGLCloudTextrue->setWrapMode(QOpenGLTexture::ClampToEdge);
    }
    m_popenGLCloudTextrue->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, frame);
//    QImage _image(frame, static_cast<int>(width), static_cast<int>(height), static_cast<int>(width)*3, QImage::Format_RGB888);
//    m_popenGLCloudTextrue->setData(_image.mirrored());
}
