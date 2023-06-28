#ifndef HOSTMANAGER_H
#define HOSTMANAGER_H
#include <QObject>
#include <QQuickItem>

#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QHostInfo>

QT_BEGIN_NAMESPACE
class QHostInfo;
QT_END_NAMESPACE

class HostManager:public QQuickItem
{
    Q_OBJECT
public:
    HostManager();
    ~HostManager();

    Q_INVOKABLE QString findHost();
private:
    void lookup(const QHostInfo &host);
    QString ip_{""};
};

#endif // HOSTMANAGER_H
