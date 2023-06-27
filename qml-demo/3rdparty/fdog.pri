win32:msvc:equals(QT_ARCH, "x86_64") {
    LIBS += -L$$PWD/lib/fdog/msvc_x64
    LIBS += -lhasp_windows_x64_30599
}
mingw {
    LIBS += -L$$PWD/lib/fdog/mingw64
    LIBS += -lhasp_windows_x64_30599
}
gcc:unix:equals(QT_ARCH, "x86_64") {
    LIBS += -L$$PWD/lib/fdog/x86_64
    LIBS += -lhasp_linux_x86_64_30599
}
