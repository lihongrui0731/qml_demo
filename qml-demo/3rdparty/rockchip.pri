gcc:unix {
    equals(QT_ARCH, "arm64") {
        LIBS += -L$$PWD/lib/rockchip-aarch64
    }
}

LIBS += -lrockchip_mpp
