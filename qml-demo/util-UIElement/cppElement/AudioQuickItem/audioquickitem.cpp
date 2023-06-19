#include "audioquickitem.h"

#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QDebug>
#include <qmath.h>
#include <qendian.h>

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif

AudioQuickItem::AudioQuickItem(QByteArray pcm) : data_pcm(pcm){
    this->open(QIODevice::ReadWrite); // 为了解决QIODevice::read (QIODevice): device not open
    len_written = 0;
}

AudioQuickItem::~AudioQuickItem(){
    //data_pcm.clear();
    clearData();
    this->close();
}

qint64 AudioQuickItem::readData(char *data, qint64 maxlen)
{
    if (len_written >= data_pcm.size() ){
        return 0;
    }
    int len;

    //计算未播放的数据的长度
    len = (len_written + maxlen) > data_pcm.size() ? (data_pcm.size() - len_written) : maxlen;

    memcpy(data, data_pcm.data()+len_written, len); //把要播放的pcm数据存入声卡缓冲区里
    len_written += len; //更新已播放的数据长度
   // emit changeTimeLine();
    return len;
}

qint64 AudioQuickItem::writeData(const char *data, qint64 len)
{
    isWrite = true;
    data_pcm.append(data,len);
    // len_written = 0;
    isWrite = false;
    return len;
}

qint64 AudioQuickItem::bytesAvailable() const
{
    return data_pcm.size() + QIODevice::bytesAvailable();
}

void AudioQuickItem::clearData(){
    if( data_pcm.size() > 0 ){
        data_pcm.clear();
    }else{
        return;
    }
    len_written = 0;

#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif
}
