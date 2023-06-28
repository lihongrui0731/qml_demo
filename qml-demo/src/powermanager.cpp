#include "powermanager.h"

PowerManager::PowerManager()
{
#ifdef Q_OS_WIN
    handle = HASP_INVALID_HANDLE_VALUE; //hasp_INVALID_HANDLE_VALUE;
#endif
}

PowerManager::~PowerManager()
{
     if( vendor_code !=nullptr ){
         delete vendor_code;
         vendor_code = nullptr;
     }
     if( data != nullptr ){
         delete data;
         data = nullptr;
     }
}

QString PowerManager::checkPower()
{
    QJsonObject obj;

#ifdef Q_OS_WIN
    //if( vendor_code == nullptr ){
        QVariant vc = fm_.openFile( ":/"+liences+".hvc" );
        vendor_code = (unsigned char *)vc.toString().toLatin1().data();
    //}

    if( data != nullptr ){
        delete data;
        data = nullptr;
    }

    unsigned int major_version_;
    unsigned int minor_version_;
    unsigned int build_server_;
    unsigned int build_number_;

    QString s;
    qint32 errorCode{-1};
    // 检查安全狗版本

    status = hasp_get_version (&major_version_,&minor_version_,&build_server_,&build_number_,static_cast<hasp_vendor_code_t>(vendor_code));

    if( status == HASP_HASP_NOT_FOUND ){
        obj.insert("errorCode",errorCode);
        obj.insert("msg","未识别到安全狗");
        return QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    }
    if( status != HASP_STATUS_OK ){
        obj.insert("errorCode",errorCode);
        obj.insert("msg","安全狗异常");
        return QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    }
    if( major_version != major_version_ || minor_version != minor_version_ ){
        obj.insert("errorCode",1);
        obj.insert("msg","安全狗版本已升级\n请联系厂商");
        return QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    }

    // 安全狗登录
    status = hasp_login(feature,  vendor_code , &handle);
    // delete vendor_code;
    if (status != HASP_STATUS_OK)
    {
        switch (status)
        {
            case HASP_FEATURE_NOT_FOUND:
                s = "安全狗异常";
                break;
            case HASP_HASP_NOT_FOUND:
                s = "未识别安全狗";
                break;
            case HASP_UNKNOWN_VCODE:
                s = "开发商代码错误";
                break;
            case HASP_INV_VCODE:
                s = "开发商代码错误";
                break;
            default:
                s = "安全狗异常";
                break;
        }
    }else{
        // 安全狗文件大小检查
        hasp_size_t memSize;
        status = hasp_get_size(handle, fileID, &memSize);
        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            s = "安全狗异常";
        }else{
            // 打开安全狗存储功能文件内容
            hasp_size_t offset{ 0 };
            data = new unsigned char[memSize];

            status = hasp_read(handle, fileID, offset, memSize, data);
            if (status != HASP_STATUS_OK)
            {
                switch (status)
                {
                    case HASP_MEM_RANGE:
                        s = "安全狗异常";
                        break;
                    default:
                        s = "安全狗异常";
                        break;
                }
            }

            errorCode = 0;
            s = QString( (char *)data );

            // delete[] data;
        }
    }

    obj.insert("errorCode",errorCode);
    obj.insert("msg",s);

    status = hasp_logout(handle);
    handle = HASP_INVALID_HANDLE_VALUE;

    // delete vendor_code;
#endif
    return QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
}
