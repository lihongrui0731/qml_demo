#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QQuickItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QScopedPointer>
#include <QDateTime>

#include <QSettings>
#include <QCoreApplication>
#include <QStandardPaths>

#include <QDir>
#include "plan.h"

#ifdef Q_OS_WIN
    #include "adjustPrivilege.h"
#endif

class ConfigManager : public QQuickItem
{
    Q_OBJECT
public:
    ConfigManager();
    ~ConfigManager();

    Q_INVOKABLE void setPlanInfo( QString info );
    Q_INVOKABLE void setOrginPlanInfo();

    Q_INVOKABLE void setParamConfig( QString config , QString sourceType );
    Q_INVOKABLE void setSettingConfig( QString config );

    Q_INVOKABLE QVariant getParamConfig( QString key );

    // 保存实时参数到配置文件
    Q_INVOKABLE void saveParams( QString param );
    // 读取实时参数
    Q_INVOKABLE QString getParams();

    Q_INVOKABLE void closeSystem();

    Q_INVOKABLE QString getRootPath( QString dir = "" );

    Q_INVOKABLE bool setSystemTime( QStringList date );

    QSharedPointer<sovi::Plan> createPlan(QString info);

private:
    QSharedPointer<sovi::Plan> plan_;

#ifdef Q_OS_WIN
    AdjustPrivilege *priv{nullptr};
#endif

signals:
    void sendPlanInfo( QSharedPointer<sovi::Plan> plan );
    void command( QString config );
    void disconnectSound();
    void disconnectDaq();
    void closeByCom();
    void batteryInfoChanged( const QString& type, quint8 value );
};
#endif // CONFIGMANAGER_H
