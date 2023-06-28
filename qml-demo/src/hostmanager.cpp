#include "hostmanager.h"

HostManager::HostManager()
{
}

HostManager::~HostManager(){
}

QString HostManager::findHost(){
    // QString hostName = QHostInfo::localHostName();
    QList <QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, list)
    {
        if ( address.toString() != "127.0.0.1" && address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            qDebug() << "IP地址：" << address.toString();	//我们使用IPv4地址
            ip_ = address.toString();
            break;
        }
    }
    return ip_;
}

void HostManager::lookup( const QHostInfo &host ){

}
