#include "configmanager.h"
#include <QProcess>
#include <time.h>

ConfigManager::ConfigManager(){
}

ConfigManager::~ConfigManager(){
}

QVariant ConfigManager::getParamConfig( QString key ){
    QSettings news { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() + "-var" };

    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() };

    QVariant result;
    if( news.contains(key) ){
        result = news.value( key );
    }else{
        result = settings.value( key , "" );
    }

    return result;
}

// 设置系统时间
bool ConfigManager::setSystemTime( QStringList date ){

#ifdef Q_OS_WIN
    priv = new AdjustPrivilege();

    qDebug() << priv->m_bEnble << "-------------------";

    SYSTEMTIME st;
    GetSystemTime(&st);

    st.wYear=date[0].toInt();
    st.wMonth=date[1].toInt();
    st.wDay=date[2].toInt();
    st.wHour=date[3].toInt();
    st.wMinute=date[4].toInt();

    //st.wSecond=sec;
    //st.wDayOfWeek= 0;
    //st.wMilliseconds= 0;

    // qDebug() << SetSystemTime(&st) << "ccccc" << st.wDayOfWeek << st.wHour;

    int res = SetSystemTime(&st);
    qDebug() << res << GetLastError();

    delete priv;
    if( res == 0 ){
        return false;
    }else{
        return true;
    }

#endif

#ifdef Q_OS_LINUX
    QString setDateCmd = QString("sudo date -s %1/%2/%3").arg(
        QString::number(date[1].toInt()),
        QString::number(date[2].toInt()),
        QString::number(date[0].toInt()));
    system(setDateCmd.toLocal8Bit());
    QString setTimeCmd = QString("sudo date -s %1:%2:%3").arg(
        QString::number(date[3].toInt()),
        QString::number(date[4].toInt()),
        QString::number(0));
    system(setTimeCmd.toLocal8Bit());
    system("sudo hcclock --systohc");
    system("sudo clock -w");
    return true;
#endif

};

void ConfigManager::setSettingConfig( QString config ){
    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() + "-var" };

    // 通道信息
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(config.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject rootArr = jsonDoc.object();
    QList<QString> key = rootArr.keys();
    for( const QString &k : qAsConst(key)){
         if( rootArr[k].isBool() ){
             settings.setValue(k,rootArr[k].toBool());
         }else{
             settings.setValue(k,rootArr[k].toString());
         }
    }
}

void ConfigManager::setParamConfig( QString config , QString sourceType ){
    // QSharedPointer<sovi::Plan> plan { new sovi::Plan() };
    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() + "-var" };

    // 通道信息
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(config.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject rootArr = jsonDoc.object();
    if( sourceType == "sound" ){
        QString dataDuration = QString::number(rootArr.value("s_save_time").toInt());
        QString collectMode = rootArr.value("collect_mode").toString();
        int acousFps =  rootArr.value("acous_fps").toInt();
        settings.setValue("gui/dataDuration",dataDuration);
        settings.setValue("soundcam.storage/workingState",collectMode);
        settings.setValue("soundcam/acousFps",acousFps);
    }else{
        QString vibDataDuration =  QString::number(rootArr.value("v_save_time").toInt());
        settings.setValue("gui/vibDataDuration",vibDataDuration);
    }

}

QSharedPointer<sovi::Plan> ConfigManager::createPlan( QString info ){

    QSettings news { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() + "-var" };

    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName() };

    int dataDuration = 0;
    if( news.contains("gui/dataDuration") ){
        dataDuration = news.value("gui/dataDuration", 10).toInt();
    }else{
        dataDuration = settings.value("gui/dataDuration", 10).toInt();
    }

    int vibDataDuration = 0;
    if( news.contains("gui/vibDataDuration") ){
        vibDataDuration = news.value("gui/vibDataDuration", 10).toInt();
    }else{
        vibDataDuration = settings.value("gui/vibDataDuration", 10).toInt();
    }

    QSharedPointer<sovi::Plan> plan { new sovi::Plan() };

    QDateTime current_time = QDateTime::currentDateTime();
    QString currentTime = current_time.toString("yyyyMMddhhmmss.ddd");
    QString id_ = current_time.toString("yyyyMMddhhmmssddd");

    plan->setId("11cba80da801000");
    //plan->setId( id_ );

    plan->setName( currentTime + "-test");

    plan->setActivationMode(sovi::PlanActivationModes::ActivateOnDelay);
    plan->setActivationDelay(5);

    plan->setSessionMode(sovi::StartStopByCommand);

    QSharedPointer<sovi::Plan::CaptureConfig> capCfg { new sovi::Plan::CaptureConfig() };
    // session session duration limit - 连续测量期间内存保留数据时长上限
    capCfg->setSoundLengthMs(dataDuration * 1000);
    // TODO 区分 ADLINK 振动通道间歇式的每次样本时长，和连续测量期间内存保留数据时长上限
    capCfg->setVibraLengthMs(vibDataDuration * 1000);
    plan->setCaptureConfig(capCfg);

    QSharedPointer<sovi::Plan::ReportConfig> rptCfg { new sovi::Plan::ReportConfig() };
    // 短周期上报的时长（对应 ProcessBatchModes::ShortAggregate 模式）
    rptCfg->setReportCycleMs(200);
    plan->setReportConfig(rptCfg);

    // 通道信息
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(info.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return plan;
    }
    QJsonArray rootArr = jsonDoc.array();
    foreach(auto obj , rootArr ){
        QSharedPointer<sovi::Plan::ChannelConfig> ch {
            new sovi::Plan::ChannelConfig(
                    obj["channelId"].toString(),
                    obj["sourceType"].toString() ) };

        QJsonObject device_ = obj["device"].toObject();
        ch->setDeviceInfo( device_["deviceType"].toString() , device_["deviceId"].toString() , device_["deviceChannel"].toString() );

        QJsonObject config_ = obj["config"].toObject();
        ch->setConfigInfo( config_["fs"].toInt() , config_["deltaf"].toDouble() , config_["sensitivity"].toDouble() , config_["iepeEnable"].toBool() );

        plan->addChannelConfig(ch);
    }
    return plan;
}

void ConfigManager::setPlanInfo( QString info ){
    plan_ = createPlan( info );
    emit sendPlanInfo( plan_ );
}

void ConfigManager::setOrginPlanInfo(){
    emit sendPlanInfo( plan_ );
}

void ConfigManager::closeSystem(){
      // 串口
      emit closeByCom();
      QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

      QProcess *parent = nullptr;
      QString program = "sudo";
      QStringList arguments;
      arguments << "poweroff";
      QProcess *myProcess = new QProcess(parent);
      myProcess->start(program,arguments);
}

void ConfigManager::saveParams( QString param ){
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
//    qDebug()<< appData;
    QDir dataDir{appData};
    QString root_ = appData + "/Config";

    QFile file(root_ + "/param.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "File open error";
    }else {
        file.write(param.toLatin1());
    }
    file.close();
}

QString ConfigManager::getParams(){
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
//    qDebug()<< appData;
    QDir dataDir{appData};
    QString root_ = appData + "/Config";

    QFile file(root_ + "/param.json");

    if (!file.exists()) //文件不存在
        return "";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QByteArray array = file.readAll();
    file.close();
    QString json_ = QString(array);
    return json_;
}

QString ConfigManager::getRootPath( QString dir ){
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QDir dataDir{appData};
    QString root_ = appData + "/Config";

    if( dir != "" ){
        dataDir.setPath( appData );
        if(!dataDir.exists(dir)){
           dataDir.mkpath(dir);
        }
        root_ += "/" +dir;
    }

    return root_;
}
