#include "scatterthread.h"

ScatterThread::ScatterThread()
{
    qRegisterMetaType<QCPColorMap*>("QCPColorMap*");
}

ScatterThread::~ScatterThread()
{}

void ScatterThread::setParams( int lineCount ){

    m_line_count = lineCount;
}

void ScatterThread::addData( QCPColorMap* heatMap , QJsonObject jsonObj ){
    for (int x = 0; x < 200; ++x) {
        for (int y = 0; y < 120; ++y) {
            heatMap->data()->setAlpha(x, y, 0);  // z为0，设置为透明
        }
    }

    /*
    QJsonParseError json_error;
    QJsonDocument jdoc(QJsonDocument::fromJson( data.toLatin1(), &json_error ));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    */

    // QJsonObject jsonObj = jdoc.object();
    QJsonArray jsonVals = jsonObj["values"].toArray();
    QJsonObject yc;
    QList<QString> yc_keys;
    for( quint8 i = 0; i < jsonVals.count(); i++ ){
         quint8 x = static_cast<quint8>(jsonVals[i].toObject()["x"].toDouble() * 10);
         yc = jsonVals[i].toObject()["yc"].toObject();
         yc_keys = yc.keys();
         for( const QString &key : qAsConst(yc_keys) ){
            quint8 color_ = yc[key].toInt() > m_line_count?m_line_count:yc[key].toInt();
            quint8 y = key.toUInt();
            // m_data.at(color_)->xAxis->append( jsonVals[i].toObject()["x"].toDouble() );
            //  m_data.at(color_)->yAxis->append( key.toDouble() );
            heatMap->data()->setAlpha( x , y , 255 );
            heatMap->data()->setCell( x , y , (double)color_ );
        }
    }

    emit sendData( heatMap );
}

ScatterThread::color ScatterThread::generateColor( float max ,float min , float value ){
    const float float_epsilon { std::numeric_limits<float>::epsilon() };
    Q_UNUSED(float_epsilon);
    if ( std::isinf( value ))
        return color(0, 0, 0);

    if (value < min) value = min;
    else if (value > max) value = max;

    float dv = max - min;

    float v = (value - min) / dv;
    float _8th = 0.125;
    quint8 r = 0;
    quint8 g = 0;
    quint8 b = 0;

    if (v < _8th) {
        b = 128 * (1 + v / _8th);
    } else if (v < 3 * _8th) {
        b = 255;
        g = 128 * (v - _8th) / _8th;
    } else if (v < 5 * _8th) {
        r = 128 * (v - 3 * _8th) / _8th;
        g = 255;
        b = 128 * (5 * _8th - v) / _8th;
    } else if (v < 7 * _8th) {
        r = 255;
        g = 128 * (7 * _8th - v) / _8th;
    } else {
        r = 128 * (9 * _8th - v) / _8th;
    }

    return color(r, g, b);
}
