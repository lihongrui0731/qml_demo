INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/OpenGLESBase.h \
    $$PWD/RhythmSpectraGLES.h \
    $$PWD/RhythmVideoGLES.h

SOURCES += \
    $$PWD/OpenGLESBase.cpp \
    $$PWD/RhythmSpectraGLES.cpp \
    $$PWD/RhythmVideoGLES.cpp

RESOURCES += \
     $$PWD/shaders.qrc \
     $$PWD/textures.qrc

DISTFILES += \
    $$PWD/ftexture.glsl \
    $$PWD/vtexture.glsl
