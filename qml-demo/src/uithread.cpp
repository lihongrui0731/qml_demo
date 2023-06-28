#include "uithread.h"

#ifdef Q_OS_LINUX
#include <malloc.h>
#endif

UIThread::UIThread()
{
}

UIThread::~UIThread()
{
    clearData();
}

void UIThread::clearData(){
    m_dataObj.clear();

#ifdef Q_OS_LINUX
    //linux 内存紧缩 实时释放
    malloc_trim(0);
#endif
}

void UIThread::setType( QString type_ ){
    m_type = type_;
}

void UIThread::setData( QString data_ ){
    m_data = data_;
}

void UIThread::run(){
    sortData();
}

void UIThread::setEVdata( QString ev_ , QString baseTime , double dt , int frameCount ){

    m_dataObj.clear();
    QJsonParseError json_error;
    QJsonDocument data_(QJsonDocument::fromJson(ev_.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject data_obj = data_.object();

    QJsonObject::Iterator iter = data_obj.begin();

    QJsonObject data_obj_;
    while( iter != data_obj.end() ){
        data_obj_.insert( iter.key() , QJsonArray() );
        iter++;
    }

    // QMap<QString,QJsonValue> data_map;
    for( int i = 0; i < frameCount; i++ ){
        QJsonObject::Iterator iter = data_obj.begin();
        while( iter != data_obj.end() ){
            QJsonArray arr = iter.value().toArray();
            QJsonArray arr_{};
            arr_.append(arr[i]);
            data_obj_.insert(iter.key(),arr_);
            QJsonObject data_obj_values;
            data_obj_values.insert("values",data_obj_);
            m_dataObj.insert( static_cast<int>(dt*i*1000) , data_obj_values );
            iter++;
        }
    }

    m_lastTime = static_cast<int>(dt*(frameCount-1)*1000);

    QMap<quint32 , QJsonObject>::Iterator data_iter = m_dataObj.begin();
    QJsonDocument jdoc = QJsonDocument( data_iter.value() );
    QString d_ = jdoc.toJson();

    emit sendFinalData( d_ );
   // qDebug() << m_dataObj ;
}

void UIThread::sortData(){
    m_dataObj.clear();
    m_lastTime = 0;
    QJsonParseError json_error;
    QJsonDocument data_(QJsonDocument::fromJson(m_data.toLatin1(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }
    QJsonObject data_obj = data_.object();

    QJsonObject::Iterator iter = data_obj.begin();

    QJsonDocument jdoc = QJsonDocument( iter.value().toObject() );
    QString d_ = jdoc.toJson( QJsonDocument::Compact );

    while( iter != data_obj.end() ){
        m_dataObj.insert( iter.key().toInt() , iter.value().toObject() );
        m_lastTime = iter.key().toInt() > (int)m_lastTime?iter.key().toInt() :m_lastTime;
        iter++;
    }

    emit sendFinalData( d_ );
}

QString UIThread::changeTimeLine( int timeline ){
    if( m_dataObj.size() <= 0 ){
        return "";
    }

    if( timeline >= (int)m_lastTime ){
        timeline = m_lastTime;
    }

    QMap<quint32 , QJsonObject>::Iterator iter = m_dataObj.upperBound(timeline);
    if( iter != m_dataObj.begin() && (int)(iter.key() - 1 ) > timeline ){
        iter--;
    }

    if( m_i_currentTime != iter.key() ){
        m_i_currentTime = iter.key();
    }else{
        return "";
    }

    if( iter == m_dataObj.end() ){
        return "";
    }

    QJsonDocument jdoc = QJsonDocument( iter.value() );
    QString d = jdoc.toJson( QJsonDocument::Indented );
    return d;
}
