win32:msvc:equals(QT_ARCH, "x86_64") {
    LIBS += -L$$PWD/lib/dog/x64
}
mingw {
    LIBS += -L$$PWD/lib/dog/x64
}
LIBS += -ldog_windows_x64_3161382
