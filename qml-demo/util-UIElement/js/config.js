var config_SandV = {
    "showLineLabel":true, //按钮 存在svg时 显示描边文字
    "hasSound":true,
    "hasVibration":true,
    "deviceCount":2,
    "default":"sound", // sound || vibration
    "station":"*",
    "title":"GIS振声可视化监测及缺陷定位诊断系统",
    //"title" : "可视化声纹诊断系统",
    "coverImg":"huanbao", // 封面图片  默认homeImage
    "colorType":"huanbao_",   // 颜色布局
    "homeType":"normal",  // 首页布局
    "showBtnIcon":true,
    "checkPower": false,
    "realtimeRawData":false,
    "useOpengl":true,
    "showMouse":true,
    "windowRotato":false // 仅对linux生效
};

var config_nightVersion = {  // 广州
    "showLineLabel":true, //按钮 存在svg时 显示描边文字
    "hasSound":true,
    "hasVibration":false,
    "deviceCount":1,
    "default":"sound", // sound || vibration
    "station":"*",
    "title":"GIS振声可视化系统",
    //"title":"声纹成像实时检测系统",
    "coverImg":"homeImage", // 封面图片  默认homeImage
    "colorType":"default_",   // 颜色布局
    "homeType":"normal",  // 首页布局
    "showBtnIcon":true,
    "checkPower": false,
    "realtimeRawData": false,
    "useOpengl":true,
    "showMouse":true,
    "windowRotato":true // 仅对linux生效
};

//config_biandianzhan
var config_biandianzhan = {
    "showLineLabel":true, //按钮 存在svg时 显示描边文字
    "hasSound":true,
    "hasVibration":false,
    "deviceCount":1,
    "default":"sound", // sound || vibration
    "station":"*",
    "title":"高压部件放电异响检测与在线诊断系统",
    "coverImg":"simple2", // 封面图片  默认homeImage
    "colorType":"default_",   // 颜色布局
    "homeType":"simple",  // 首页布局
    "showBtnIcon":false,
    "checkPower":false,
    "realtimeRawData": true,
    "useOpengl":false,
    "showMouse":true,
    "windowRotato":false
};
