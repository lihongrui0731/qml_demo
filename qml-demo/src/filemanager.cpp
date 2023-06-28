#include <QQuickItem>
#include <QObject>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDir>
#include <QHostInfo>
#include <QStorageInfo>
#include <QStringList>
#include <QDesktopServices>
#include <QFileInfo>
#include "filemanager.h"
#include <QThread>
#include <QStandardPaths>
#include <QVariant>
#include <QVariantMap>
#include <QJsonObject>
#include <QObjectList>
#include <QDateTime>

#include <QQmlApplicationEngine>

// #include "tdms_to_map.h"
// #include "tdmsdata.h"

#ifdef Q_OS_WIN
  #include <Windows.h>
#else
#endif

fileManager::fileManager()
{
}
fileManager::~fileManager(){
    if( tdms !=nullptr ){
        delete tdms;
        tdms = nullptr;        
    }
}

/*
    删除指定文件/文件夹
*/
bool fileManager::deleteFile1(QString f){

    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(f.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
    {
        qDebug()<<tr("解析json错误！");
        return false;
    }
    QJsonArray jsonArr = document.array();

    bool res = true;

    for( int i = 0; i < jsonArr.count(); i++ ){
        qDebug() << jsonArr[i].toString();
        QFileInfo fileinfo(jsonArr[i].toString());
        if( fileinfo.isFile() ){
            QFile file( jsonArr[i].toString() );
            res = file.remove();
        }else{
            res = deleteDir( jsonArr[i].toString() );
        }
        if( !res ){
            break;
        }
    }
    return res;
}

/*
   递归删除文件夹
*/
bool fileManager::deleteDir( QString f ){
    QDir dir(f);
    if(!dir.exists())
    {
        return false ;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList)
    { //遍历文件信息
        if (file.isFile())
        { // 是文件，删除
            file.dir().remove(file.fileName());
        }else
        { // 递归调用函数，删除子文件夹
            deleteDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 这时候文件夹已经空了，再删除文件夹本身
}

/*
 \brief fileManager::exportFiles
 \param files 为jsonArray字符串
 \param toPath 带导出目录
*/
void fileManager::run(){
    if( act == "export" ){
        doExportFiles( m_export_files , export_path );
    }
}

bool fileManager::exportFiles(QString files , QString toPath){
    m_export_files = files;
    export_path = toPath;
    act = "export";
    start(QThread::HighPriority);
    return true;
}

void fileManager::doExportFiles( QString files , QString toPath ){
    #ifdef Q_OS_LINUX
        QString hostName = qgetenv ("USER");
        toPath = "///media/" + hostName + "/" + toPath;
    #endif
    QDir dir(toPath);

    if (!dir.exists()) {  //目标路径不存在
        qDebug()<<"路径没找到";
        //return false;
        emit exportFinish("noPath");
    }

    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(files.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
    {
        qDebug()<<tr("解析json错误！");
        emit exportFinish("error");
        //return false;
    }
    QJsonArray jsonArr = document.array();

    bool res = true;
    for( int i = 0; i < jsonArr.count(); i++ ){
        qDebug() << jsonArr[i].toString();
        QFileInfo fileInfo( jsonArr[i].toString() );
        if( fileInfo.isFile() ){
            res = copyFile( jsonArr[i].toString() , toPath + '/' + fileInfo.fileName() , true );
        }else{
            QDir dirc( jsonArr[i].toString() );
            res = copyDir( jsonArr[i].toString() , toPath + '/' + dirc.dirName() , true );
        }
        if( !res ){
            break;
        }
    }
    if(res){
        emit exportFinish("success");
    }else{
        emit exportFinish("error");
    }
}


/*
   获取外接存储列表
*/
QStringList fileManager::eDevice(){
#if defined (WIN32)
    QString hostName = QHostInfo::localHostName();
    QFileInfoList drvList = QDir::drives();
    QStringList validUSBPathList;
    validUSBPathList.clear();
    foreach (QFileInfo info, drvList)
    {
#ifdef UNICODE
        if ( DRIVE_REMOVABLE == GetDriveTypeW((WCHAR *)info.filePath().utf16()) )
#else
        if ( DRIVE_REMOVABLE == GetDriveTypeA(info.filePath().toStdString().c_str()) )
#endif
        {
            validUSBPathList.append(info.absolutePath());
        }
    }
    return validUSBPathList ;
#else
    QString hostName = qgetenv ("USER");
    QString mount_ = "sudo systemd-mount /dev/sda /media/"+hostName+"/ESD-USB";
    system(mount_.toLocal8Bit());

    QCoreApplication::processEvents(QEventLoop::AllEvents, 500);

    QString folder = "/media/"+hostName;
    QDir dir(folder);
    folder = dir.fromNativeSeparators(folder);//  "\\"转为"/"
    QStringList allFolder = QStringList("");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);
    allFolder = dir.entryList();
    return allFolder;
#endif
}

/*
    打开指定目录
*/
void fileManager::openDir( QString dir ){
    QDesktopServices::openUrl(QUrl("file:"+dir , QUrl::TolerantMode));
}

/*
    复制目录
*/
bool fileManager::copyDir( QString source , QString destination , bool override = true ){
    QDir directory(source);
    if (!directory.exists())
    {
        qDebug()<<"不存在";
        return false;
    }

    QDir dest(destination);
    if( !dest.exists() ){
        dest.mkpath( destination );
    }

    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();

    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }
    return !error;
}

/*
 *  复制文件
*/
bool fileManager::copyFile( QString srcPath , QString dstPath , bool coverFileIfExist = true ){

    #ifdef Q_OS_LINUX
       QString commond = "sudo cp "+srcPath+" "+dstPath;
       system( commond.toLocal8Bit() );
       system("sudo sync");
       return true;
   #endif

    srcPath.replace("\\", "/");
    dstPath.replace("\\", "/");

    if (srcPath == dstPath) {
        return true;
    }

    if (!QFile::exists(srcPath)) {  //源文件不存在
        return false;
    }

    if (QFile::exists(dstPath)) {
        if (coverFileIfExist) {
            QFile::remove(dstPath);
        }
    }

    QFile f{srcPath};
    f.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
    if (!f.copy(dstPath)){
        return false;
    }else{
        QCoreApplication::processEvents(QEventLoop::AllEvents, 500);
        #ifdef Q_OS_LINUX
            system("sudo sync");
        #endif
    }
    return true;
}

/*
 *
 * 写配置文件
*/
bool fileManager::saveConfig( QString type , QString config ){
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QDir dataDir{appData};
    if(!dataDir.exists("Config"))
        dataDir.mkpath("Config");
    QString config_path = appData+"/Config/";
    QString file = config_path + type + "_config.json";

    return writeFile( file , config );
}

/*
 *
 * 读取配置文件信息
*/
QString fileManager::getConfig( QString f ){
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QDir dataDir{appData};

    if(!dataDir.exists("Config")){
       dataDir.mkpath("Config");

       copyFile( ":/js/sound_config.json" , appData + "/Config/sound_config.json" , true );
       copyFile( ":/js/vibration_config.json" , appData + "/Config/vibration_config.json" , true );
       copyFile( ":/js/update.json" , appData + "/Config/update.json" , true );
    }else{
        QFile file{ appData + "/Config/update.json" };
        if( !file.exists() ){

            copyFile( ":/js/sound_config.json" , appData + "/Config/sound_config.json" , true );
            copyFile( ":/js/vibration_config.json" , appData + "/Config/vibration_config.json" , true );
            copyFile( ":/js/update.json" , appData + "/Config/update.json" , true );
        }else{
            QString s_target = openFile( appData + "/Config/update.json" ).toString();
            QString s_source = openFile( ":/js/update.json" ).toString();

            QJsonParseError error;
            QJsonDocument document_target = QJsonDocument::fromJson(s_target.toUtf8(),&error);
            QJsonObject jsonObj_target = document_target.object();

            QJsonDocument document_source = QJsonDocument::fromJson(s_source.toUtf8(),&error);
            QJsonObject jsonObj_source = document_source.object();

            if( jsonObj_target["version"] != jsonObj_source["version"] ){
                copyFile( ":/js/sound_config.json" , appData + "/Config/sound_config.json" , true );
                copyFile( ":/js/vibration_config.json" , appData + "/Config/vibration_config.json" , true );
                copyFile( ":/js/update.json" , appData + "/Config/update.json" , true );
            }
        }

    }

    return openFile(f).toString();
}

bool fileManager::writeFile( QString f , QString info ){
    QFile file(f);
    if( !file.exists() ){
       if( !file.open(QIODevice::Append) ){
            return false;
       }
       file.close();
    }

    bool isok = file.open(QIODevice::WriteOnly);
    if(isok == true)
    {
        //写文件操作的时候需要把QString类型转换成QByteArray
        file.write(info.toUtf8());
        file.close();
    }else return false;

    return true;
}

/*
 *
 *  获取预设配置文件
 */
QStringList fileManager::preinstall(){
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QDir dataDir{appData};
    if(!dataDir.exists("Preinstall")){
        dataDir.mkpath("Preinstall");
        pre_init( appData + "/Preinstall" );
    }

    QString config_path = appData+"/Preinstall/";
    QDir directory(config_path);
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Files );
    QStringList datas;
    QString fileName;
    QString file;
    QString json;
    QJsonObject jsonObj;
    QJsonObject obj;
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        fileName = fileNames.at(i);
        file = config_path + fileName;

        json = openFile( file ).toString();
        jsonObj = getJsonObject( json );

        obj["key"] = file;
        obj["value"] = jsonObj["dataName"];
        QJsonDocument jsonDoc(obj);
        QByteArray ba = jsonDoc.toJson();

        datas.append(ba);
    }
    return datas;
}

QJsonObject fileManager::getJsonObject( QString json ){
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8(),&error);
    QJsonObject jsonObj = document.object();
    return jsonObj;
}

bool fileManager::checkConnect(){
    //检查ip配置文件是否生成

    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    QDir dataDir{appData+"/../"};
    if(!dataDir.exists("pad-var.ini")){
        return false;
    }
    return true;
}

/*
   打开json文件
*/
QVariant fileManager::openFile(QString f){
    QFile file(f);
    // file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }
    QString value = file.readAll();
    return value;
}

/*
*   初始话预设配置文件
*   todo
*/
void fileManager::pre_init( QString path ){
    copyFile( ":/js/data1.json" , path + "/data1.json" , true );
}

/**
*   获取初始路径地址
**/
QString fileManager::getRootPath( QString dir ){
#ifdef Q_OS_LINUX
    QString appData { QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
#endif

#ifdef Q_OS_WIN
    QString appData{ QCoreApplication::applicationDirPath() };
#endif

//    qDebug()<< appData;
    QDir dataDir{appData};
    QString root_ = "";
    if(!dataDir.exists("RhythmData")){
       dataDir.mkpath("RhythmData");
    }

    if(!dataDir.exists("RhythmData/screenshot")){
       dataDir.mkpath("RhythmData/screenshot");
    }
    root_ = appData + "/RhythmData";
    if( dir != "" ){
        dataDir.setPath( appData + "/RhythmData" );
        if(!dataDir.exists(dir)){
           dataDir.mkpath(dir);
        }
        root_ += "/" +dir;
    }

    return root_;
}

/**
 *  打开历史文件的基础信息
 */
QList<QString> fileManager::getDataBaseInfo( QString dir ){
    tdms = new TdmsToMap();
    quint64 res = tdms->getChannels(dir);

    QMultiMap<QString, QString> channelFrameTypes = tdms->channelFrameTypes();
    QList<QString> channels = channelFrameTypes.uniqueKeys();
    // QList<QString> channels;
    QList<QString> channels_;

    if( res != 404 ){
        // 404 表示无数据
        if( channels.isEmpty() ){
            //兼容 老版本tdms
            channels_.append("sound");
            return channels_;
        }

        if( channels.indexOf("sc01")!= -1 ){
            channels_.append("sound");
        }else{
            for(const QString& channel : qAsConst(channels) ){
                QString channelName = "ch" + QString::number(channel.rightRef(1).toInt()) ;
                channels_.append(channelName);
            }
        }
    }

    tdms->releaseTdmsMemory();

    delete tdms;
    tdms = nullptr;

    return channels_;
}
