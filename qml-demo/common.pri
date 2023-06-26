# 各子项目 include 本文件，设定公用的配置

## defines

### 面向不同项目启用代码模块的编译选项

CONFIG += c++14
unix {
    CONFIG += link_pkgconfig
}

### 需要带优化调试时启用
#CONFIG += force_debug_info

### DEBUG 构建时加入优化级别
#QMAKE_CXXFLAGS_DEBUG += -O1

#msvc 编译器 中文解析问题
msvc {
    QMAKE_CFLAGS += -utf-8
    QMAKE_CXXFLAGS += -utf-8
}

# build 时通过环境变量指定当前编译的应用
_SOVI_BUILD_APP = $$(SOVI_BUILD_APP)
_SOVI_FEATURES = $$(SOVI_FEATURES)
_SOVI_DRIVER_DAQ = $$(SOVI_DRIVER_DAQ)

_APP_VENDOR = RhythmTech
_APP_ID = sovi

# 视频编码模块目前只在RK平台可用
equals(_SOVI_BUILD_APP, gateway):contains(_SOVI_FEATURES, video):equals(QT_ARCH, "arm64") {
    CONFIG += sovi_proc_video
}

# 选择启用何种 DAQ 驱动
contains(_SOVI_DRIVER_DAQ, .*aims.*) {
    CONFIG += sovi_drv_aims
}
contains(_SOVI_DRIVER_DAQ, .*rdaq.*) {
    CONFIG += sovi_drv_rdaq
}
contains(_SOVI_DRIVER_DAQ, .*adlink.*) {
    CONFIG += sovi_drv_adlink
}

contains(_SOVI_FEATURES, prst_tdms) {
    CONFIG += sovi_persist_tdms
    DEFINES += ENABLE_PERSIST_TDMS
}
#默认启用
    CONFIG += sovi_persist_rsd


windows {
    CONFIG += sovi_camera_qtmm
    DEFINES += OPT_CAMERA=1
} else:linux {
    contains(_SOVI_FEATURES, qtcam) {
        CONFIG += sovi_camera_qtmm
        DEFINES += OPT_CAMERA=1
    } else {
        CONFIG += sovi_camera_v4l2
        DEFINES += OPT_CAMERA=2
    }
}

#message(DEFINES $${DEFINES})

equals(QT_ARCH, arm64)|equals(QT_ARCH, arm) {
  CONFIG += sovi_drv_board
  DEFINES += ENABLE_DRIVER_BOARD
}

sovi_drv_aims {
  DEFINES += ENABLE_DRIVER_AIMS
}

sovi_drv_rdaq {
  DEFINES += ENABLE_DRIVER_RDAQ
}

sovi_drv_adlink {
  DEFINES += ENABLE_DRIVER_ADLINK
}

sovi_proc_video {
  DEFINES += ENABLE_VIDEO_ENCODING
}

### 部分
#DEFINES += LOG_TRACE

### 代码中使用计划废弃特性时给出警告
DEFINES += QT_DEPRECATED_WARNINGS

### 代码中使用 Qt 旧版已废弃特性时阻止编译 (050C00: Qt 5.12)
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050C00

#build_pass {
#message(CONFIG: $${CONFIG})
#message(DEFINES: $${DEFINES})
#}

# 各子项目公用的设置

## 各库模块编译后默认输出到公用的 lib 目录
DESTDIR = $$OUT_PWD/../lib

### PWD 指向本文件所在的源代码目录路径
### OUT_PWD 指向 include 本文件的 pro 文件对应的编译输出目录路径

## 添加第三方库的 header 文件

INCLUDEPATH = $$PWD/3rdparty/include

## 默认加入公用的 lib 目录

LIBS = -L$$OUT_PWD/../lib
