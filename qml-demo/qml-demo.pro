#include($$PWD/util-UIElement/UIElement.pri)
QT += quick virtualkeyboard printsupport
QT += quickcontrols2 gui
 QT += widgets
 QT += multimedia
 QT += charts opengl # datavisualization
 QT += websockets

DEFINES += QCUSTOMPLOT_USE_OPENGL
include($$PWD/common.pri)
include($$PWD/util-UIElement/UIElement.pri)
include($$PWD/3rdparty/arma.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        WebSocketData.cpp \
        WebSocketManager.cpp \
#        configmanager.cpp \
#        datathread.cpp \
        filemanager.cpp \
        main.cpp \
#        tdmsdata.cpp \
        uithread.cpp \
        ws_server.cpp

RESOURCES += qml.qrc \
             $$PWD/util-UIElement/uiElement.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  WebSocketData.h \
  WebSocketManager.h \
#  configmanager.h \
#  datathread.h \
  easy.h \
  filemanager.h \
#  tdmsdata.h \
  src/dataType.h \
  uithread.h \
  ws_server.h
