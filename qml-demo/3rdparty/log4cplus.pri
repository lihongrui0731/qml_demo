gcc:unix:equals(QT_ARCH, "arm") {
    LIBS += -L$$PWD/lib/log4cplus/arm
}
gcc:unix:equals(QT_ARCH, "arm64") {
    LIBS += -L$$PWD/lib/log4cplus/aarch64
}
mingw {
    LIBS += -L$$PWD/lib/log4cplus/mingw_64
}

LIBS += -llog4cplus
