#ifndef TDMSDATA_H
#define TDMSDATA_H

#include <QObject>
#include <QQuickItem>
#include <QMetaType>
#include <QVariant>
#include <QVariantMap>
#include <QCoreApplication>
#include <QThread>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

#include "tdms_to_map.h"
#include "rawdata_to_map.h"
#include "report_frame.h"
#include "sample_frame.h"
#include "plan.h"

#include <QElapsedTimer>

#include "dataType.h"

class TdmsData : public QThread
{
    Q_OBJECT
public:
    TdmsData();
    ~TdmsData();

//    QVariantMap openSoundData(QString file);
//    QVariantMap openVibrationData(QString file);

private:
  TdmsToMap tdms;

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

  void saveTdmsData();

  // QElapsedTimer timer;

public slots:
  void openSoundData( QString f);
  void openVibrationData( QString f );
  void saveSoundTdmsData();

signals:
  void sendData( QVariantMap d );
//  void sendDataToCal( qint32 * );

  void startReprocess( QSharedPointer<sovi::Plan> plan );
  void stopReprocess();
  void sendDataToCal( QSharedPointer<SampleFrame> );

  void finish();
};

#endif // TDMSDATA_H
