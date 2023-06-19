#ifndef AUDIORENDER_H
#define AUDIORENDER_H

#include <QTimer>
#include <QByteArray>
#include <QObject>
#include <QQuickItem>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QVariant>

#include "audioquickitem.h"
#include "dataType.h"

#include <armadillo>
#include <sigpack/sigpack.h>

class AudioRender : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY( int volumeValue READ getVolume NOTIFY volumeValueChanged )
    Q_PROPERTY( bool isRun READ runState NOTIFY runStateChanaged  )
public:
    AudioRender();
    ~AudioRender();

    // local data
    Q_INVOKABLE void setData( QVariant data );
    Q_INVOKABLE void setRawData( QVariant data );
    Q_INVOKABLE void setFinalData( QVariant data );
    // raw data
    Q_INVOKABLE void addData( qint32 * data , quint32 len );
    Q_INVOKABLE void addData( QVariant data );

    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void start();
    Q_INVOKABLE void changeTimeLine( quint32 timeline );
    Q_INVOKABLE void reset();
    Q_INVOKABLE void volumeChanged(int);

    Q_INVOKABLE void volumeClose(bool);
    int getVolume();
    bool runState(){
        return run;
    };

private:
    // void initializeWindow();
    QAudioFormat initializeAudio(const QAudioDeviceInfo &deviceInfo);
    bool generateData( quint32 timeLine );

    qint32* downFrame( arma::Col<float> &tmp , float *data_ , int len );

private:
    QTimer *m_pushTimer = nullptr;
    QAudioFormat m_format;

    AudioQuickItem * m_audioItem{nullptr}; // m_generator;
    QAudioOutput * m_audioOutput{nullptr};

    quint32 m_sampleRate{50000};
    float m_f_durationSeconds;
    QMap<quint64, QVector<float>> raw_;
    std::map<int, qint32*> m_i_data;
    QByteArray m_b_data;
    quint64 m_i_len;
    quint64 m_i_count;
    bool run{false};
    bool m_b_noVolume{false};
    quint8 m_i_volumeValue{20};
    quint32 pos{0};
    quint32 m_i_timeline{0};

    quint8 m_i_enlarge{32};

    int ratio{4}; // 降幀係數

    bool m_outputCSV{false};

    QAudio::State audioState{ QAudio::StoppedState };

    static constexpr double MicSesitivity { 3.372e-6 };

    double sesitivity = {MicSesitivity};

    void csvOutput( QString p , qint32 *data , int len );

signals:
    void finish();
    void initalVolume( int value );
    void volumeValueChanged();
    void runStateChanaged();
private slots:
    void deviceChanged(int index);

};

#endif // AUDIORENDER_H
