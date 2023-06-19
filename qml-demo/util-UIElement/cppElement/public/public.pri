INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qcustomplot.h \
    $$PWD/tools_shared.h \
    $$PWD/dataType.h \
    $$PWD/RhythmGradientBar.h \
    $$PWD/battery.h \
    $$PWD/fontoutlinelabel.h \
    $$PWD/tdmsdata.h \
    $$PWD/openrsd.h \
    $$PWD/hostmanager.h
SOURCES += \
    $$PWD/qcustomplot.cpp \
    $$PWD/RhythmGradientBar.cpp \
    $$PWD/battery.cpp \
    $$PWD/tdmsdata.cpp \
    $$PWD/openrsd.cpp \
    $$PWD/fontoutlinelabel.cpp \
    $$PWD/hostmanager.cpp

win32{
    HEADERS += $$PWD/mdump.h
    SOURCES += $$PWD/mdump.cpp
    LIBS += -lopengl32 -lglu32
}
