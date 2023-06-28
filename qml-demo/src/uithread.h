#ifndef UITHREAD_H
#define UITHREAD_H

#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class UIThread : public QThread
{
    Q_OBJECT
public:
    explicit UIThread();
    ~UIThread();

    void run() override;

    Q_INVOKABLE void setType( QString type_ );
    Q_INVOKABLE void setData( QString data_ );
    Q_INVOKABLE QString changeTimeLine( int timeline );
    Q_INVOKABLE void clearData();

    Q_INVOKABLE void setEVdata( QString ev_ , QString baseTime , double dt , int frameCount ); //打开文件获取每个时间段的值

signals:
    void sendFinalData( QString data );

private:
    QString m_type{"prpd"};
    QString m_data;
    quint64 m_lastTime{0};
    quint32 m_i_currentTime{0};
    QMap<quint32 , QJsonObject> m_dataObj;

    void sortData();
};

#endif // UITHREAD_H
