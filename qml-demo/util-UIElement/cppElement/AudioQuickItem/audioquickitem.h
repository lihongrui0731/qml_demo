#ifndef AUDIOQUICKITEM_H
#define AUDIOQUICKITEM_H

#include <math.h>

#include <QAudioOutput>
#include <QByteArray>
#include <QIODevice>
#include <QObject>
#include <QTimer>
#include <QScopedPointer>
#include "dataType.h"

class AudioQuickItem : public QIODevice
{
    Q_OBJECT
public:
    AudioQuickItem( QByteArray pcm );
    ~AudioQuickItem();

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len ) override;
    qint64 bytesAvailable() const override;

    void clearData();

private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);
   //  void set
private:
    QByteArray data_pcm; //存放pcm数据
    int        len_written; //记录已写入多少字节
    bool       isWrite{false};
    quint8     noDataTimes{0}; // 3次nodata重启
signals:
    void changeTimeLine();
    void finish();
};

#endif // AUDIOQUICKITEM_H
