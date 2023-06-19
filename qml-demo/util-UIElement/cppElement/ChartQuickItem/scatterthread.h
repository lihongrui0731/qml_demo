#ifndef SCATTERTHREAD_H
#define SCATTERTHREAD_H

#include <QObject>
#include <QQuickItem>
#include <QMetaType>
#include <QVariant>
#include <QCoreApplication>
#include <QThread>
#include <QList>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "qcustomplot.h"

class ScatterThread : public QThread
{
    Q_OBJECT
    struct color{
        explicit color(int red_, int green_, int blue_):
            red(red_), green(green_), blue(blue_) {}
        int red{255};
        int green{255};
        int blue{255};
    };

public:
    ScatterThread();
    ~ScatterThread();

    ScatterThread::color generateColor(float max, float min, float value );
    void setParams( int line_count );

private:
    int m_line_count{16};

signals:
    void sendData( QCPColorMap* );

public slots:
    void addData( QCPColorMap* heatMap , QJsonObject data );
};
#endif // SCATTERTHREAD_H
