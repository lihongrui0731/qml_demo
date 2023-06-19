#include "OpenGLESBase.h"
#include <cmath>

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif

OpenGLESBase::OpenGLESBase() :
    m_glbuff_indexBuf(QOpenGLBuffer::IndexBuffer)
{
}

void OpenGLESBase::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initShaders();
    initTextures();
    // Enable depth buffer

    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
   //  glEnable(GL_TEXTURE_2D);
    m_glbuff_arrayBuf.create();
    m_glbuff_indexBuf.create();
    initQuadrangle();
}

void OpenGLESBase::initShaders()
{
    // Compile vertex shader
    m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl");
    /*
    if (!m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
       { qDebug() << " add vertex shader file failed." << m_openGLShaderProgram.log();
        return ;
        }
    */
    // Compile fragment shader
    m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl");
    /*
    if (!m_openGLShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
      { qDebug() << " add fragment shader file failed." << m_openGLShaderProgram.log();
        return ;
    }
    */
    // Link shader pipeline
    m_openGLShaderProgram.link();
    // Bind shader pipeline for use
    m_openGLShaderProgram.bind();
}

void OpenGLESBase::initTextures()
{
    QImage _pictureImage(":/cube.png");

    // Load cube.png image
    m_popenGLTexture = new QOpenGLTexture(_pictureImage.mirrored());

    // Set nearest filtering mode for texture minification
    m_popenGLTexture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    m_popenGLTexture->setMagnificationFilter(QOpenGLTexture::Nearest);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    m_popenGLTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
}

void OpenGLESBase::initQuadrangle()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  0.0f), QVector2D(0.0f, 1.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  0.0f), QVector2D(1.0f, 1.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  0.0f), QVector2D(0.0f, 0.0f)},  // v2
        {QVector3D( 1.0f,  1.0f,  0.0f), QVector2D(1.0f, 0.0f)}, // v3
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

void OpenGLESBase::drawQuadrangle()
{
    // Tell OpenGL which VBOs to use
    m_glbuff_arrayBuf.bind();
    m_glbuff_indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_openGLShaderProgram.attributeLocation("a_position");
    m_openGLShaderProgram.enableAttributeArray(vertexLocation);
    m_openGLShaderProgram.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = m_openGLShaderProgram.attributeLocation("a_texcoord");
    m_openGLShaderProgram.enableAttributeArray(texcoordLocation);
    m_openGLShaderProgram.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1

    // qDebug() << " draw opengl texture ------ ";

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, nullptr);

}

void OpenGLESBase::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 30.0;
    // Reset projection
    m_matrix4x4_projection.setToIdentity();
    // Set perspective projection
    m_matrix4x4_projection.perspective(fov, aspect, zNear, zFar);
}

void OpenGLESBase::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_popenGLTexture->bind(0);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(m_quaternion_rotation);

    // Set modelview-projection matrix
    m_openGLShaderProgram.setUniformValue("mvp_matrix", m_matrix4x4_projection * matrix);

    // Use texture unit 0 which contains cube.png
    m_openGLShaderProgram.setUniformValue("texture", 0);

    // Draw cube geometry
    drawQuadrangle();
}

OpenGLESBase::~OpenGLESBase()
{
    if( m_popenGLTexture != nullptr ){
        delete m_popenGLTexture;
        m_popenGLTexture = nullptr;
    }
    m_glbuff_arrayBuf.destroy();
    m_glbuff_indexBuf.destroy();
    qDebug() << " 析构 父类 ";

#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif
}
