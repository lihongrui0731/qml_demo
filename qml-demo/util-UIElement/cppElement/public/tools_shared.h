#ifndef TOOLS_SHARED_H
#define TOOLS_SHARED_H

#include <QMargins>

namespace RhythmUI
{

struct picture{
    float max{0};
    float min{0};
    float ave{0};
    float* picturedata{nullptr};
};

/// UI 模式，决定程序总体的功能和可用的工具
enum SoundcamUIModes {
    /// 设备信息、程序信息、选项配置等
    Info = 0,
    /// 回放最近一次采集的，或从文件加载的数据
    Playback = 1,
    /// 执行采集
    Measuring = 2,
};

enum class AcousticImageModes {
    /// 自动模式
    ImageModeAuto,
    /// 固定阈值模式
    ImageModeFixedThreshold,
    /// 去噪模式 (绘图上限取 avg+crestMargin 与 xMax 两者中数值较大者)
    ImageModeAverageThreshold,
    /// 瞬态模式
    ImageModeTransient
};

/// 侧边工具条按钮的弹出面板，相对于工具条的方位
enum ToolPanelPositions {
    /// 向右弹出，适用于左侧工具条
    Right,
    /// 向左弹出，适用于右侧工具条
    Left
};

/// 工具按钮尺寸
const int ToolButtonSize { 60 };

/// 常规控件间距
const int MarginWidthNormal { 4 };

/// 稍大的控件间距
const int MarginWidthLarger { 8 };

/// 四边常规边距
const QMargins MarginsAround { MarginWidthNormal, MarginWidthNormal, MarginWidthNormal, MarginWidthNormal };

/// 左右常规边距，顶底无边距
const QMargins MarginsLeftRight { MarginWidthNormal, 0, MarginWidthNormal, 0 };

/// 左右无边距，顶底常规边距
const QMargins MarginsTopBottom { 0, MarginWidthNormal, 0, MarginWidthNormal};

}

#endif // TOOLS_SHARED_H
