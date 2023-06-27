gcc:unix {
    equals(QT_ARCH, "arm") {
        LIBS += -L$$PWD/lib/armadillo/arm
    }
    equals(QT_ARCH, "arm64") {
        LIBS += -L$$PWD/lib/armadillo/aarch64
    }
    equals(QT_ARCH, "x86_64") {
        LIBS += -L$$PWD/lib/armadillo/x86_64
    }
}
mingw {
    LIBS += -L$$PWD/lib/armadillo/mingw64
}
win32:msvc:equals(QT_ARCH, "x86_64") {
    LIBS += -L$$PWD/lib/armadillo/msvc_x64
}

LIBS += -larmadillo
