gcc:unix {
    equals(QT_ARCH, "arm64") {
        LIBS += -L$$PWD/lib/ffmpeg/aarch64
    }
}

LIBS += -lavformat -lavcodec -lavutil -lswresample
#    -lpthread
