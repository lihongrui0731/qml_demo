#ifndef OPENRSD_H
#define OPENRSD_H

#include <QObject>
#include <QQuickItem>
#include <QMetaType>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QCoreApplication>
#include <QThread>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

#include "rhythm_composite_file.h"
#include "soundcam_slice_reader.h"
#include "metadata_reader.h"
#include "rsd_defs_metadata.h"

#include "dataType.h"

class OpenRSD : public QThread
{
    Q_OBJECT
public:
    OpenRSD();
    ~OpenRSD();

private:
    RhythmCompositeFile *rcf_{nullptr};
    SoundcamSliceReader *sreader{nullptr};
    MetadataReader *metaReader{nullptr};

    float m_frequencyMax;
    float m_frequencyMin;
    int m_pdata_time;

    QVariant spectra_data;
    QVariant spectrum_data;
    QVariant video_data;
    QVariant camera_data;
    QVariant audio_data;
    QVariant eigenvalue_data;
    QVariant spectra_json_data;
    QVariant raw_data;
    QVariant time_line;
    QVariant timedata_data;

    QVariantMap data;

    QList<QJsonObject> m_eigenvalue;

    std::map<int, float*> _map_spectrum;
    std::map<int, float*> _local_map_spectrum;
    std::map<int, unsigned char*> _picture_map;

    QMap< int , QByteArray > _camera_map;
    std::map<int, float*> _beam_map;
    std::map<int, qint32*> _raw_map;

    std::map<int, qint32*> _locS_dataNeg;
    std::map<int, qint32*> _locS_dataPos;
    std::map<int, qint32*> _locS_data;

    QMap<QString,QVector<float>> _eigenvalue;

    std::map<int, float*>::const_iterator _map_spectrum_iterator{_map_spectrum.begin()};
    std::map<int, float*>::const_iterator _local_map_spectrum_iterator{_local_map_spectrum.begin()};
    std::map<int, unsigned char*>::const_iterator _picture_map_iterator{_picture_map.begin()};
    std::map<int, float*>::const_iterator _beam_map_iterator{_beam_map.begin()};

    QMap<int,QByteArray>::const_iterator _camera_map_iterator{_camera_map.begin()};
    QMap<QString,QVector<float>>::const_iterator _eigenvalue_iterator{_eigenvalue.begin()};

    // void openSoundData( QString f);
    // void openVibrationData( QString f );

public slots:
    void openChannelData( QString f , QString ch = "/" , QList<rsd_defs_soundcam::DatasetTypes> dataTypes = {} );

signals:
    void sendData( QVariantMap d );
    void finish();

};

#endif // OPENRSD_H
