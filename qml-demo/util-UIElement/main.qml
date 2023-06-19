import QtQuick 2.15
import QtQml 2.15
import QtQuick.Window 2.15
import QtCharts 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2
import QtQuick.VirtualKeyboard.Styles 2.2
import DataManager_ 1.0
import PowerManager_ 1.0
import ConfigManager 1.0
import FileManager 1.0

import './button'
import './chart'
import './element'
import './../js/color.js' as UI_color
import './../js/config.js' as UI_config
import './../js/params.js' as Params

Window {
    id:root

    property real enlarge: enlargeSetting
    property real dpi:Screen.devicePixelRatio?Screen.devicePixelRatio / root.enlarge : 1.0 / root.enlarge

    property string version: "1.1.5";
    property string usedConfig: appConfig;

    width: 1280
    height: Qt.platform.os=="linux"?800: 810
    minimumWidth: 800
    minimumHeight: 600
    flags: Qt.platform.os=="linux"?(Qt.Window | Qt.FramelessWindowHint):1

    onWindowStateChanged: {
        root.isHide = windowState == 1?true:false
    }

    onHeightChanged: {
      // loader.height = UI_config[root.usedConfig].windowRotato?root.width:root.height
    }

    onWidthChanged: {
      // loader.width = UI_config[root.usedConfig].windowRotato?root.height:root.width
    }

    property string sys: Qt.platform.os
    property string family: fontFamily?fontFamily:"simHei"

    property bool windowRotato: Qt.platform.os=="windows"?false:UI_config[root.usedConfig].windowRotato

    contentOrientation: root.windowRotato?Qt.InvertedLandscapeOrientation:Qt.PrimaryOrientation

    visible: true
    title: qsTr( UI_config[root.usedConfig].title )
    property string lastUrl: "home"
    property var channels: []
    property var channelName: {"sound":"sound","ch1":"channel1","ch2":"channel2","ch3":"channel3","ch4":"channel4"}
    property bool isSound: UI_config[root.usedConfig].default === "sound"?true:false
    property bool isVibration:  UI_config[root.usedConfig].default === "vibration"?true:false

    property bool showMouse: UI_config[root.usedConfig].showMouse
    property bool useOpengl: UI_config[root.usedConfig].useOpengl

    // 默认跟随权限 但是有权限的时候可以不启用
    property bool hasSound:  UI_config[root.usedConfig].hasSound
    property bool hasVibration: UI_config[root.usedConfig].hasVibration

    property bool hasProcess: !UI_config[root.usedConfig].checkPower;
    property bool hasReprocess: !UI_config[root.usedConfig].checkPower;

    property bool hasSoundPower: !UI_config[root.usedConfig].checkPower;
    property bool hasVibPower: !UI_config[root.usedConfig].checkPower;
    property var soundPowers: [];
    property var vibPowers: [];

    property int deviceCount: UI_config[root.usedConfig].deviceCount;

    property string act: ""
    property string params: ""
    property bool closeFile: false //是否关闭历史文件

    property bool isSoundReady: false
    property bool isVibrationReady: false
    property bool isSoundMeasuring: false
    property bool isVibrationMeasuring: false

    property var soundParamsConfig: ({})
    property var vibrationParamsConfig: ({})

    property var home: UI_config[root.usedConfig].homeType === "simple"?homeSimplePage:homePage

    property bool power: true
    property bool active: true
    property int sleepTime: 0
    property bool isHide: false  // 是否最小化

    // signal changePage

    function closeSystem(){
        cm.closeSystem();
    }

    function createPlan(){
        // 直接创建 任务 ---- 仅限单声学模式
        var channelInfo = [];
        var Channels = {};

        channelInfo.push(
            {
                "channelId": Params.default_.soundDeviceId,
                "sourceType":"sound",
                "device":{
                    "deviceType" : Params.default_.soundDeviceType,
                    "deviceId" : Params.default_.soundDeviceId
                }
            }
        );

        Channels["sc01"] = "sound";

        cm.setPlanInfo( JSON.stringify(channelInfo) );
        dm.setChannel( JSON.stringify( Channels ) );

    }

    function setPower( power_ ){
        if( power_.hasOwnProperty("online-capure") ){
            root.hasProcess = true;

            var deviceCount_ = 0
            var online_capure = power_["online-capure"];
            if( online_capure.hasOwnProperty("sound") ){
                root.hasSoundPower = true;
                root.soundPowers = online_capure["sound"];
                deviceCount_++;
            }else{
                root.hasSound = false;
            }
            if( online_capure.hasOwnProperty("vib") ){
                root.hasVibPower = true
                root.vibPowers = online_capure["vib"];
                deviceCount_++;
            }else{
                root.hasVibration = false;
            }

            root.deviceCount = root.deviceCount < 2?root.deviceCount: deviceCount_;

        }
        if( power_.hasOwnProperty("reprocess") ){
            root.hasReprocess = true;
        }
    }

    Timer{
        id: sleepTimer
        interval: 1000
        repeat:true
        running:  false
        onTriggered: {
            root.sleepTime++;
            // console.log( "sleep " + root.sleepTime )
        }
    }
    Connections{
        target: Qt.application

        function onActiveChanged(){
            root.active = Qt.application.active;
            if( root.active ){
                sleepTimer.stop()
            }else{
                root.sleepTime = 0;
                sleepTimer.start()
            }
        }
    }

    Pm{
        id: pm
        objectName: "powerManager"
    }

    Dm{
        id: dm
        objectName: "dataManager"
        onSendChannel:{
            root.channels.push(channelId)
        }
    }

    Cm{
        id: cm
        objectName: "configManager"
    }

    Fm{
        id: fm
    }

    Component.onCompleted: {
        root.channels = [];
        var vibrationConfig_ = fm.getConfig(cm.getRootPath() + '/../Config/vibration_config.json')
        vibrationConfig_ = JSON.parse(vibrationConfig_);
        root.channelName.sound = "声学通道"
        for( var key in vibrationConfig_.channelName ){
            root.channelName[key] = vibrationConfig_.channelName[key];
        }

        var obj = {"soundcam/realtimeData":UI_config[root.usedConfig].realtimeRawData}
        cm.setSettingConfig(JSON.stringify(obj))

        var params_ = cm.getParams();
        if( params_ != "" ){
            params_ = JSON.parse(params_);

            root.soundParamsConfig["distance"] = params_["distance"];
            root.soundParamsConfig["autoDynamic"] = params_["dynamicRange"]["dynamic"];
            root.soundParamsConfig["peak"] = params_["dynamicRange"]["peak"];
            root.soundParamsConfig["max"] = params_["dynamicRange"]["max"];
            root.soundParamsConfig["denoiseDynamic"] = params_["dynamicRange"]["denoise"];
            root.soundParamsConfig["thresholdDynamic"] = params_["dynamicRange"]["threshold"];
            root.soundParamsConfig["frequencyMin"] = params_["frequency"]["min"];
            root.soundParamsConfig["frequencyMax"] = params_["frequency"]["max"];
            root.soundParamsConfig["brightness"] = params_["image"]["brightness"];
            root.soundParamsConfig["contrast"] = params_["image"]["contrast"];
            root.soundParamsConfig["saturation"] = params_["image"]["saturation"];
            root.soundParamsConfig["highLight"] = params_["image"]["highLight"];
            root.soundParamsConfig["shadow"] = params_["image"]["shadow"];
            root.soundParamsConfig["midtone"] = params_["image"]["midtone"];
            root.soundParamsConfig["coefficient"] = params_["image"]["coefficient"];
        }

        loader.sourceComponent = root.home;
    }

    // 首页
    Component{
        id: homePage
        Home{
            id: home
            objectName: "homeItem"
            anchors.fill: parent
            width: parent.width
            height: parent.height
            onDirectChanged: {
                changeUrl( home.direct )
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }

    // 简易版首页
    Component{
        id: homeSimplePage
        HomeSimple{
            id: home
            objectName: "homeItem"
            anchors.fill: parent
            width: parent.width
            height: parent.height
            onDirectChanged: {
                changeUrl( home.direct )
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }


    // openGL 测试
    // Component{
       // id: test
        // TimeSpectraChart
        /*
        Rectangle{
            id: test2
            anchors.fill: parent
            width: parent.width
            height: parent.height

            TimeSpectraChart{
                id: timeSpectraChart
                anchors.centerIn: parent
                width: 400
                height: 400
            }
        }
        */
    // }

    // 引导页
    Component{
        id:guidePage
        Guide{
            id: guide
            objectName: "guideItem"
            anchors.fill: parent
            width: parent.width
            height: parent.height
            channels: root.channels
            channelName: root.channelName
            onRedirectClicked: {

                if( url == "dataContent" ){
                    root.act = "realTime"
                    root.params = ""

                    root.channels = guide.channels
                    root.channelName = guide.channelName
                    if( guide.channels.indexOf( "sound" ) !== -1 ){
                        root.isSound = true
                        root.isVibration = false
                        root.hasSound = true
                        root.hasVibration = guide.channels.length > 1?true:false
                    }else{
                        root.isSound = false
                        root.isVibration = true
                        root.hasSound = false
                        root.hasVibration = true
                    }
                }
                changeUrl( url )
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }

    // 成像界面
    Component{
        id:dataPage
        DataContent{
            id: dataContent
            objectName: "dataItem"
            anchors.fill: parent
            width: parent.width
            height: parent.height

            isSound: root.isSound
            isVibration: root.isVibration
            channels: root.channels
            channelName: root.channelName
            hasSound: root.hasSound
            hasVibration: root.hasVibration

            file:root.act=="openFile"?JSON.parse( root.params ).file:""

            onRedirectClicked: {
                root.act = "";
                changeUrl( url , back )
                if( type == "sound" ){
                    root.isSound = true;
                    root.isVibration = false;
                }else if( type == "vibration" ){
                    root.isSound = false
                    root.isVibration = true;
                }
                root.isSoundMeasuring = false
                root.isSoundReady =false
                root.isVibrationMeasuring = false
                root.isVibrationReady = false
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }

    Component{
        id: systemConfigPage
        SystemConfig{
            id: systemConfig
            objectName: "systemConfig"
            anchors.fill: parent
            width: parent.width
            height: parent.height

            onRedirectClicked: {
                root.changeUrl( url , back )
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }

    // 历史文件界面
    Component{
        id:dataListPage
        DataList{
            id: dataList
            objectName: "dataHistory"
            anchors.fill: parent
            width: parent.width
            height: parent.height

            onRedirectClicked: {
                root.changeUrl( url , back )
            }
            onMenuClicked: {
                menu.open()
            }
            onJump: {
                // 带操作跳转页面
                root.changeUrl( url , back , act , params )
            }
        }
    }

    // 声纹参数配置
    Component{
        id: soundConfigPage
        SoundConfig{
            id:soundConfig
            objectName: "soundConfigItem"
            anchors.fill: parent
            width: parent.width
            height: parent.height
            onRedirectClicked: {
                root.changeUrl( url , back )
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }

    // 振动参数配置
    Component{
        id: vibrationConfigPage
        VibrationConfig{
            id:vibrationConfig
            objectName: "vibrationConfigItem"
            anchors.fill: parent
            width: parent.width
            height: parent.height

            channels: root.channels
            channelName: root.channelName

            onRedirectClicked: {
                root.changeUrl( url , back )
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }

    // 任务列表
    /*
    Component{
        id: planPage
        Plan{
            id: plan
            anchors.fill: parent
            width: parent.width
            height: parent.height

            onRedirectClicked: {
                changeUrl( url , back )
            }
            onMenuClicked: {
                menu.open()
            }
        }
    }
    */

    function changeUrl( url , lastUrl = "home" , act = "" ,  params ="" ){
        if( lastUrl !== "home" && lastUrl !== "" ){
            root.lastUrl = lastUrl
        }
        // console.log("lastUrl:"+root.lastUrl)

        if( act != "" ){
           root.act = act
           root.params = params
        }

        switch( url ){
            case "home":
                root.lastUrl = "home"
                loader.sourceComponent = root.home
                break;
            case "guide":
                loader.sourceComponent = guidePage
                break;
            case "dataContent":
                loader.sourceComponent = dataPage
                break;
            case "dataList":
                loader.sourceComponent = dataListPage
                break;
            case "systemConfig":
                loader.sourceComponent = systemConfigPage
                break;
            case "soundConfig":
                loader.sourceComponent = soundConfigPage
                break;
            case "vibrationConfig":
                loader.sourceComponent = vibrationConfigPage
                break;
            case "back":
                changeUrl( root.lastUrl )
                break;
            default:
                break;
        }
//      dataContent.file = JSON.parse(params).file;
    }

    property int inputX: 0 // 键盘x坐标(动态)
    property int inputY: root.height // 键盘y坐标(动态)

    Page{
        id: page
        x: root.windowRotato?( root.width - root.height) / 2:0
        y: root.windowRotato?( root.height - root.width) / 2:0
        width: root.windowRotato?root.height:root.width
        height: root.windowRotato?root.width:root.height
        rotation: root.windowRotato? 270:0

        Loader{
            id:loader
            objectName: "loader"
            anchors.fill: parent

            onLoaded: {
                if( root.closeFile && loader.sourceComponent != dataPage ){
                    worker_ev.clearData();
                    dm.clearData();
                    root.closeFile = false;
                }

                if( root.sys == "linux" ){
                    dm.releaseMomery();
                }

                if( root.sys=="windows" && UI_config[root.usedConfig].checkPower ){
                    var pw =pm.checkPower();
                    pw = JSON.parse(pw);
                    if( pw.errorCode !== 0 ){
                        messageDialog.show( pw.msg );
                        root.power = false
                    }else{
                        root.power = true
                        var power_ = JSON.parse(pw.msg);
                        root.setPower(power_["functions"]);
                    }
                }
            }
        }

        // 总菜单
        Dialog{
            id: menu
            anchors.centerIn: parent
            implicitWidth: 480/root.dpi
            implicitHeight: 260/root.dpi

            z:999
            standardButtons: Dialog.CloseOnPressOutside | Dialog.CloseOnEscape
            modal: true
            Overlay.modal: Rectangle{
                color: "#33333333"
                rotation: page.rotation
                x:-page.x
                y:-page.y
            }

            Label{
                text:"功能总菜单"
                font.bold: true
                font.pixelSize: 15/root.dpi
            }

            Button{
                width: 30/root.dpi
                height: 30/root.dpi
                x: 420/root.dpi
                y: 0/root.dpi
                z: 9999
                background:Rectangle{
                    color: UI_color[UI_config[root.usedConfig].colorType].common_button_bg
                    radius: 5/root.dpi
                }
                Label{
                    text: "×"
                    font.pixelSize: 20/root.dpi
                    color: "#fff"
                    anchors.centerIn: parent
                }
                onClicked: {
                    menu.close()
                }

                MouseArea{
                    anchors.fill: parent;
                    hoverEnabled: true;
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    acceptedButtons: Qt.NoButton
                }
            }

            Grid{
                spacing: 20/root.dpi
                columns: 4
                x: 40/root.dpi
                y: 40/root.dpi
    /*
                MenuRectButton{
                    name: "站点\n管理"
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        menu.close()
                    }
                }

                MenuRectButton{
                    name: "任务\n管理"
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        menu.close()
                    }
                }
    */

                MenuRectButton{
                    name: "系统\n配置"
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        root.changeUrl("systemConfig")
                        menu.close()
                    }
                }

                MenuRectButton{
                    name: "参数\n配置"
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        root.changeUrl("soundConfig")
                        menu.close()
                    }
                }

                MenuRectButton{
                    name: "数据\n采集"
                    bgcolor: UI_color[UI_config[root.usedConfig].colorType].main_button_bg
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        if( !root.hasVibration ){
                            root.createPlan()
                            root.changeUrl("dataContent")
                        }else{
                            root.changeUrl("guide")
                        }
                        menu.close()
                    }
                }

                MenuRectButton{
                    name: "数据\n查看"
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        root.changeUrl("dataList")
                        menu.close()
                    }
                }
    /*
                MenuRectButton{
                    name: "报告\n生成"
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        menu.close()
                    }
                }

                MenuRectButton{
                    name: "用户\n登录"
                    w:80/root.dpi
                    h:80/root.dpi
                    onMenuButtonClicked: {
                        menu.close()
                    }
                }
    */
            }

        }

        InputPanel {
            id: inputPanel
            z: 99
            //更改x,y即可更改键盘位置
            //更改width即可更改键盘大小
            width: 800 / root.dpi
            x: ( parent.width - inputPanel.width ) / 2
            y: parent.height

            // width: root.width - 100/root.dpi
            visible: Qt.inputMethod.visible

            externalLanguageSwitchEnabled: true

            states: State {
                name: "visible"
                when: inputPanel.active
                PropertyChanges {
                    target: inputPanel
                    // 将键盘顶部放在屏幕底部会使其隐藏起来
                    x: ( parent.width - inputPanel.width ) / 2
                    y: inputY
                }
            }

            transitions: Transition {
               id: inputPanelTransition
               from: ""
               to: "visible"
               reversible: true
               enabled: !VirtualKeyboardSettings.fullScreenMode
               //设置键盘弹出效果
               ParallelAnimation {
                   NumberAnimation {
                       properties: "y"
                       //过渡时间
                       duration: 200
                       //弹出效果
                       easing.type: Easing.InOutQuad
                   }
               }
            }

            Binding {
                target: InputContext
                property: "animating"
                value: inputPanelTransition.running
            }

            Component.onCompleted: {
                //VirtualKeyboardSettings.locale = "eesti" // 复古键盘样式
                VirtualKeyboardSettings.wordCandidateList.alwaysVisible = true
            }

            // 这种集成方式下点击隐藏键盘的按钮是没有效果的，只会改变active，因此我们自己处理一下
            onActiveChanged: {
                if(!active) {
                    visible = false;
                    // loader.height = root.height;
                    inputPanel_outside.visible = false
                }
            }
        }

        MouseArea{
            id: inputPanel_outside
            anchors.fill: parent
            cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
            z:0
            visible: false
            onClicked: {
                forceActiveFocus()
            }
        }

        Popup {
            id: photoDialog
            x: (parent.width - photoDialog.width) / 2
            y: (parent.height - photoDialog.height) / 2
            width: parent.width * 0.8 / root.dpi
            height: parent.height * 0.8 / root.dpi

            implicitWidth: 500
            implicitHeight:400

            modal: true
            focus: true
            closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnPressOutsideParent

            Overlay.modal: Rectangle{
                color: "#33333333"
                rotation: page.rotation
                x:-page.x
                y:-page.y
            }

            Image{
                id: photoImage
                anchors.fill: parent
                anchors.margins: 10
                source: "";
                z:1
                asynchronous: true
            }

            Rectangle{
                y: 20
                x: photoDialog.width - 40 - 30
                width: 30
                height: 30
                border.width: 1
                border.color: UI_color[UI_config[root.usedConfig].colorType].border
                color: "transparent"
                Text{
                    text: "×"
                    anchors.centerIn: parent
                    font.pointSize: 20
                }
                MouseArea{
                    anchors.fill: parent
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    onClicked: {
                        photoDialog.close()
                    }
                }
                z:2
            }

            function show( file ){
                photoImage.source = file;
                photoDialog.open()
            }

        }

        Popup {
            id: messageDialog
            x: (parent.width - messageDialog.width) / 2
            y: (parent.height - messageDialog.height) / 2
            width: parent.width * 0.3 / root.dpi
            height: parent.height * 0.3 / root.dpi

            implicitWidth: 100
            implicitHeight: 80

            modal: true
            focus: true
            closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnPressOutside | Popup.CloseOnPressOutsideParent

            Overlay.modal: Rectangle{
                color: "#33333333"
                rotation: page.rotation
                x:-page.x
                y:-page.y
            }

            property bool autoClose: false;

            background: Rectangle{
                radius: 10
                color: UI_color[UI_config[root.usedConfig].colorType].main
                Rectangle{
                    anchors.fill: parent
                    anchors.margins: 5
                    border.color: "#333333"
                    radius: 10
                    color: "transparent"
                }
/*
                Rectangle{
                    x: -messageDialog.x + 80
                    y: -messageDialog.y - 110
                    width: page.width
                    height: page.height
                    color: "#333333"
                    opacity: 0.5
                }
*/
            }

            Text{
                id: popup_content
                text: "提示"
                anchors.centerIn: parent
                font.pixelSize: 30/root.dpi
            }

            MouseArea{
                x: -messageDialog.x
                y: -messageDialog.y
                cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                width: 2000
                height: 2000

                onClicked: {
                    messageDialog.close()
                    popup_timer.stop()
                }
            }

            function show( content ){
                popup_content.text = content
                messageDialog.open()
                if( messageDialog.autoClose ){
                    popup_timer.start()
                }
            }

            Timer{
                id:popup_timer
                interval: 2000
                repeat: false
                // triggeredOnStart: true
                onTriggered: {
                    messageDialog.close()
                    popup_timer.stop()
                }
            }
        }

        Timer{
            // 延时用
            id:sleep
            function setTimeout(cb, delayTime) {
                sleep.interval = delayTime;
                sleep.repeat = false;
                sleep.triggered.connect(cb);
                sleep.triggered.connect(function release () {
                    sleep.triggered.disconnect(cb); // This is important
                    sleep.triggered.disconnect(release); // This is important as well
                });
                sleep.start();
            }
        }

        MouseArea{
            id: keyboard_outside
            anchors.fill: parent
            cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
            z:0
            visible: false
            onClicked: {
                numberKeyboard_.active = false
                keyboard_.active = false
            }
        }

        // 自制数字键盘
        NumberKeyboard{
            id: numberKeyboard_
            z: 99999
            visible: false
            x: ( parent.width - numberKeyboard_.width ) / 2
            y: parent.height

            enter: Transition {
               reversible: true
               //设置键盘弹出效果
               NumberAnimation {
                   properties: "y"
                   from:  page.height
                   to: (page.height - numberKeyboard_.height) / 2
                   //过渡时间
                   duration: 200
                   //弹出效果
                   easing.type: Easing.InOutQuad
               }
            }

            onActiveChanged: {
                numberKeyboard_.visible = numberKeyboard_.active;
                keyboard_outside.visible = numberKeyboard_.active
            }
        }

        // 自制数字键盘
        Keyboard{
            id: keyboard_
            z:99999
            visible: false
            x: ( parent.width - keyboard_.width ) / 2
            y: parent.height

            enter: Transition {
               reversible: true
               NumberAnimation {
                   properties: "y"
                   from:  page.height
                   to: (page.height - keyboard_.height) / 2
                   //过渡时间
                   duration: 200
                   //弹出效果
                   easing.type: Easing.InOutQuad
               }
            }

            onActiveChanged: {
                keyboard_.visible = keyboard_.active;
                keyboard_outside.visible = keyboard_.active
            }
        }
    }
}
