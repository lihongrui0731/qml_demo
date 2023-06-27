gcc:linux {
    equals(QT_ARCH, arm64) {
        INCLUDEPATH += $$[QT_SYSROOT]/usr/include/hdf5/serial
        LIBS += -L$$[QT_SYSROOT]/usr/lib/aarch64-linux-gnu/hdf5/serial
    } else:equals(QT_ARCH, x86_64) {
        INCLUDEPATH += /usr/include/hdf5/serial
        LIBS += -L/usr/lib/x86_64-linux-gnu/hdf5/serial
    }
}
windows {
    INCLUDEPATH += $$PWD/include/hdf5-1.10
    LIBS += -L$$PWD/lib/hdf5/mingw64
}
LIBS += -lhdf5 -lhdf5_cpp -lhdf5_hl
