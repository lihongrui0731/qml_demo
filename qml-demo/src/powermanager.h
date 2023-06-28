#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <QQuickItem>
#include <QObject>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include "filemanager.h"
// #include "dog/dog_api.h"
#ifdef Q_OS_WIN
#include "fdog/hasp_api.h"
#endif
class PowerManager:public QQuickItem
{
    Q_OBJECT
public:
    PowerManager();
    ~PowerManager();

    Q_INVOKABLE QString checkPower();

private:

    /*
    const QString liences{"GFHNC"};
    constexpr static int fileID{46};

    const dog_feature_t feature{1};
    dog_handle_t handle{DOG_INVALID_HANDLE_VALUE};
    dog_status_t status;
    */
#ifdef Q_OS_WIN
    const QString liences{"TZIKZ"};
    constexpr static int fileID{2};

    const hasp_size_t feature{2};
    hasp_handle_t handle{HASP_INVALID_HANDLE_VALUE};
    hasp_status_t status;
#endif
    unsigned char * data{nullptr};
    unsigned char * vendor_code{nullptr};

    unsigned int major_version{8};
    unsigned int minor_version{51};

    // 国内狗版本
    //unsigned int major_version{2};
    //unsigned int minor_version{52};

    fileManager fm_;
};

#endif // POWERMANAGER_H
