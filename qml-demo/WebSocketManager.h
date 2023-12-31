#ifndef WEBSOCKETMANAGER_H
#define WEBSOCKETMANAGER_H

#include <QObject>
#include "./ws_server.h"
#include "./src/dataType.h"

class WebSocketManager : public QObject
{
    Q_OBJECT
public:
    WebSocketManager();
    ~WebSocketManager();
    // Sending
    Q_INVOKABLE void sendTextMessage(const QString message);
    QHash<int, QVector<qint32>> m_maintainWave;
    int m_maintainWaveLength {48000};

private:
    WsServer * WebSocketServer;
    QThread* wsServerThread;
    enum DataTypes {
        Int32 = 0,
        UInt32,
        SingleFloat = 2,
        DoubleFloat = 3,
    };
    struct HeaderInfo {
        quint64 timestamp{};
        qint32 channelFlag{};
//        quint8 dataType{};
        //quint8 reserved[7];
//        quint32 totalLength{};
    };
    void setupStreamReader(QDataStream& reader);
    QJsonArray toJsonArray(const QVector<qint32>& myVec) {
        QJsonArray result;
        std::copy (myVec.begin(), myVec.end(), std::back_inserter(result));
        return result;
    }
    void messageReceived(const QString& textData);
    void handleDeviceID(const QString& deviceID);
    void dataReceived(const QJsonObject& dataObj);
    void handleDataFrame(const QJsonObject& data);
    void handleWaveData(const QByteArray& data);
signals:
    void clientAccepted(QString clientId);
    void incomingTextMessageReceived(const QString& message);
    void sendOutgoingTextMessage(const QString& message);
    void deviceIDReceived(const QString& deviceID);
    void leqDataReceived(const QJsonObject& data);
    void evDataReceived(const QJsonObject& data);
    void fftDataReceived(const QJsonObject& data);
    void prpdDataReceived(const QJsonObject& data);
    void waveDataReceived(const QJsonObject& data);

public slots:
    // Receiving
    void onClientAccepted(QString clientId);
    void onIncomingTextMessageReceived(const QString& message);
    void onIncomingBinaryMessageReceived(const QByteArray& message);

};


#endif // WEBSOCKETMANAGER_H
