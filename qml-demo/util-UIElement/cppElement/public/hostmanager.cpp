#include "hostmanager.h"

HostManager::HostManager()
{
}

HostManager::~HostManager(){
}

QString HostManager::findHost(){
    // QString hostName = QHostInfo::localHostName();
    ip_ = "";
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
      QList<QNetworkAddressEntry> entry;
      foreach(QNetworkInterface inter, interfaces)
      {
          // 过滤掉虚拟机
          if(-1 != inter.humanReadableName().indexOf("VMware"))
              continue;
          if (inter.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning))
          {
               entry = inter.addressEntries();
              // entry.at(0) 是IPv6信息
               for( int i =0; i < entry.size(); i++) {
                  if (entry.at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
                  {
                      if (-1 != inter.name().indexOf("wireless")){
                          qDebug() << inter.humanReadableName() << inter.name() << " 无线网IP: " << entry.at(i).ip().toString();
                          ip_ = entry.at(i).ip().toString();
                          break;
                      }else if (-1 != inter.name().indexOf("ethernet")){
                          qDebug() << inter.humanReadableName() << inter.name() << " 以太网IP: " << entry.at(i).ip().toString();
                          ip_ = entry.at(i).ip().toString();
                          break;
                      }else if(-1 != inter.name().indexOf("wlan0")){
                          qDebug() << inter.humanReadableName() << inter.name() << " 无线网IP: " << entry.at(i).ip().toString();
                          ip_ = entry.at(i).ip().toString();
                          break;
                      }else if(-1 != inter.name().indexOf("eth0")){
                          qDebug() << inter.humanReadableName() << inter.name() << " 以太网IP: " << entry.at(i).ip().toString();
                          ip_ = entry.at(i).ip().toString();
                          break;
                      }
                  }
              }
              entry.clear();
          }
      }

    return ip_;
}

void HostManager::lookup( const QHostInfo &host ){

}
