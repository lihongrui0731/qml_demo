/** *******************************************************************************************
** @brief thie is used OpenGL ES 2.0
**
**
**
**
**
**
** @author guweicao                                                            @date 2021/5/17
**********************************************************************************************/

#ifndef OPENGLESBASE_H
#define OPENGLESBASE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

// QOpenGLFunctions
class OpenGLESBase : protected QOpenGLFunctions
{
public:

    struct VertexData
    {
        QVector3D position;
        QVector2D texCoord;
    };

    OpenGLESBase();
    virtual ~OpenGLESBase();

    void initializeGL();
    void paintGL();
protected:

    virtual void resizeGL(int w, int h) ;

    virtual void initShaders();
    virtual void initTextures();
    virtual void initQuadrangle();
    void drawQuadrangle();


    QOpenGLShaderProgram  m_openGLShaderProgram;
    QOpenGLTexture       *m_popenGLTexture{nullptr};
    QMatrix4x4            m_matrix4x4_projection;
    QVector2D             m_vec2d_pmousePressPosition;
    QVector3D             m_vec3d_rotationAxis;
    qreal                 m_qreal_angularSpeed = 0;
    QQuaternion           m_quaternion_rotation;
    QOpenGLBuffer         m_glbuff_arrayBuf;
    QOpenGLBuffer         m_glbuff_indexBuf;

};

#endif // OPENGLESBASE_H
