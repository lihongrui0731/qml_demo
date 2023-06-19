#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QObject>
#include <QQuickItem>
#include <QMetaType>
#include <QVariant>
#include <QThread>
#include <QCoreApplication>
#include <QImage>
#include "dataType.h"
#include "tools_shared.h"

class VideoThread : public QThread
{
    Q_OBJECT

    struct color{
        explicit color(int red_, int green_, int blue_):
            red(red_), green(green_), blue(blue_)
        {}
        int red{255};
        int green{255};
        int blue{255};
    };

public:
    VideoThread();
    ~VideoThread();

protected:
    virtual void run() override;

public:
    void setFinalVideoData(
        const unsigned int &width_pic, const unsigned int &height_pic, QMap<quint64, QVector<quint8>> pictures_,
        const unsigned int &width_beam, const unsigned int &height_beam, QMap<quint64, QVector<float>> beam_
    );

    void setFinalCameraData(
        const unsigned int &width_pic, const unsigned int &height_pic ,const unsigned int &bytesPerLine , QMap<quint64, QByteArray> camera_,
        const unsigned int &width_beam, const unsigned int &height_beam, QMap<quint64, QVector<float>> beam_
    );

    void setVideoData(
        const unsigned int &width_pic, const unsigned int &height_pic, std::map<int, unsigned char *> pictures_,
        const unsigned int &width_beam, const unsigned int &height_beam, std::map<int, float*> beam_
    );

    void setCameraData(
        const unsigned int &width_pic, const unsigned int &height_pic,const unsigned int &bytesPerLine , QMap<int, QByteArray> camera_,
        const unsigned int &width_beam, const unsigned int &height_beam, std::map<int, float*> beam_
    );

    void setTimeAxis( bool isTimeAxis_ ){
        isTimeAxis = isTimeAxis_;
    }

private:
    quint8 m_type{1};

    std::map<int, unsigned char *> m_pictures_;
    std::map<int, float*> m_beam_;
    QMap<int, QByteArray> m_camera_;
    QMap<int, QByteArray>::iterator _cameraItor{m_camera_.begin()};
    std::map<int, unsigned char *>::iterator _constItor{m_pictures_.begin()};
    std::map<int, float *>::iterator _beamItor{m_beam_.begin()};

    QMap<quint64, QVector<quint8>> m_pictures__;
    QMap<quint64, QByteArray> m_camera__;

    QMap<quint64, QVector<float>> m_beam__;
    QMap<quint64, QVector<quint8>>::iterator _constItor_{m_pictures__.begin()};
    QMap<quint64, QByteArray>::iterator _cameraItor_{m_camera__.begin()};
    QMap<quint64, QVector<float>>::iterator _beamItor_{m_beam__.begin()};

    std::map< int, unsigned char*>  m_map_pictures;
    std::map< int, RhythmUI::picture>  m_map_cloudPicture;
    QMap<int , QImage> m_map_camera;

    QVector<unsigned char> picture_temp;
   // QVector<unsigned char> beam_temp;
    QVector<unsigned char> pictureData;
    QVector<unsigned char> beamData;

    unsigned int m_picture_witdh;
    unsigned int m_picture_height;
    unsigned int m_beam_witdh;
    unsigned int m_beam_height;
    unsigned int bytesPerLine{0};

    //bool isSetVideo{false};
    //bool isSetBeam{false};

    bool isSetVideo{false};
    bool isSetBeam{false};
    bool isTimeAxis{false};

    QList<float> beamMaxList;

    RhythmUI::AcousticImageModes m_e_filterModelType{RhythmUI::AcousticImageModes::ImageModeAuto};

    float m_f_beamMax{13.9f};
    float m_f_beamMin{10.9f};
    float m_f_beamLimitMax{13.9f};
    float m_f_peakFactor{1.0f};
    float m_f_dynamicRange{1.0f};
    quint32 m_i_distance{1000};

    void sortData();
    void sortFinalData();

    void generateCloud(const unsigned int& width_, const unsigned int& height_, const float* data_, const float& min_, const float& max_);
    color generateColor( const float &value );

public slots:
    void setVideo( const unsigned int &width_, const unsigned int &height_, QVector<quint8> &picture_ );
    void setBeam( const unsigned int &width_, const unsigned int &height_, QVector<float> &beam_ );

    void setParams( RhythmUI::AcousticImageModes filterModelType_ ,const float &beamMax_ ,const float &beamMin_ ,const float &beamLimitMax_ ,const float &peakFactor_ ,const float &dynamicRange_ ,const quint32 &distance_ ){
        m_e_filterModelType = filterModelType_;
        m_f_beamMax = beamMax_;
        m_f_beamMin = beamMin_;
        m_f_beamLimitMax = beamLimitMax_;
        m_f_peakFactor = peakFactor_;
        m_f_dynamicRange = dynamicRange_;
        m_i_distance = distance_;
    }

signals:
    void sendData( QVariant p_data );
    void sendTimeAxis( QList<float> timeAxisList );
    void sendVideoData( std::map< int, unsigned char*> picture_, std::map<int , RhythmUI::picture> beam_ );

    void sendCameraData( QMap<int , QImage> , std::map<int , RhythmUI::picture> beam_ );

    // void sendVideo( unsigned char *picture_ );
    // void sendBeam( unsigned char *cloudPicture_ , float beamMax_ , float beamMin_ );

    void sendVideo( QVector<unsigned char> picture_ );
    void sendBeam( QVector<unsigned char> cloudPicture_ , float &beamMax_ , float &beamMin_ );

};

#endif // VIDEOTHREAD_H
