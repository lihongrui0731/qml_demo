#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QQuickItem>
#include <QObject>
#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QStringList>
#include <QThread>
#include <QStandardPaths>
#include <QJsonObject>
#include <QObjectList>

#include <QMetaType>
#include <QVariant>
#include "tdms_to_map.h"
#include "tdmsdata.h"

class fileManager:public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile)
public:
    fileManager();
    ~fileManager();
    void run() override;

    QString file() const { return m_file; }
    void setFile(const QString &file) { m_file = file; }
    QVariant openFile(QString file);

    Q_INVOKABLE bool deleteFile1(QString f);
    Q_INVOKABLE bool exportFiles(QString files, QString toPath);
    Q_INVOKABLE void doExportFiles(QString files, QString toPath);

    Q_INVOKABLE QStringList eDevice();
    Q_INVOKABLE QStringList preinstall();

    Q_INVOKABLE void openDir(QString f);
    Q_INVOKABLE bool saveConfig( QString type , QString config );
    Q_INVOKABLE QString getConfig( QString file );

    Q_INVOKABLE QString getRootPath( QString dir = "" );

    Q_INVOKABLE QList<QString> getDataBaseInfo( QString dir );

    Q_INVOKABLE bool checkConnect();

private:
    QString m_export_files;
    QString export_path;
    QString act;

    QString m_file;
    QString m_dataType;

    TdmsToMap * tdms{nullptr};

    bool deleteDir(QString f);

    bool copyDir( QString source , QString destination , bool coverFileIfExist );
    bool copyFile( QString srcPath , QString dstPath , bool coverFileIfExist );

    bool writeFile( QString f , QString info );
    void pre_init( QString path );

    QJsonObject getJsonObject( QString f );

signals:
    void exportFinish( QString status ); // 导出结果信号 noPath目标路径不存在 error 导出失败 success 导出完成

};
#endif // FILEMANAGER_H
