#include "battery.h"

battery::battery()
{
}

battery::~battery()
{
}


QString battery::openFile( QString f ){
    QFile file(f);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }
    QString value = file.readAll();
    return value;
}


void battery::saveBattery( int value ,int status_ ){
    isCom = true;
    percent = value;
    status = status_;
}

QVector<int> battery::getBattery(){
    QVector<int> bat;

    if( isCom ){
        bat.push_back( percent );
        bat.push_back( status );
        return bat;
    }

    #if defined (WIN32)
        SYSTEM_POWER_STATUS sysPowerStatus;
        GetSystemPowerStatus(&sysPowerStatus);
        bat.push_back( (int)sysPowerStatus.BatteryLifePercent );
        bat.push_back( (int)sysPowerStatus.BatteryFlag );
    #else

        QSettings settings {
            QSettings::IniFormat, QSettings::UserScope,
            QCoreApplication::organizationName(),
            QCoreApplication::applicationName()
        };
        QString mainboard = settings.value("mainboard/version", "rk3568").toString();

        // bat.push_back( openFile( "/sys/class/power_supply/rk-bat/capacity" ).toInt() );
        // bat.push_back( openFile( "/sys/class/power_supply/rk-bat/status" ).toInt() );

        if( mainboard == "rk3399" ){
            float raw = openFile( "/sys/bus/iio/devices/iio:device0/in_voltage0_raw" ).toDouble();
            float vol = 1.8 * ( raw / 1024.0 ); // 1.272 1.146       adc 652 / 724

            float capacity = (( vol - 1.146 ) / 0.126);
            int capacity_ = static_cast<int>( capacity * 100 ) ;

            if( capacity_ > 100 )  {
                bat.push_back( capacity_ );
                bat.push_back( 128 );
            }else{
                bat.push_back( capacity_ );
                bat.push_back( 1 );
            }
        }else if( mainboard == "rk3568" ){
            /* 3568 电量文件
              // 电量
              /sys/class/power_supply/battery/capacity
              // 电压
              /sys/class/power_supply/battery/voltage_now
              // 充电状态
              /sys/class/power_supply/battery/status
            */

            int capacity_ = openFile( "/sys/class/power_supply/battery/capacity" ).toInt();
            int status = openFile( "/sys/class/power_supply/battery/current_now" ).toInt();

           // qDebug() << "battery ------- : " << capacity_ << status;

            if( status > 0 )  {
                bat.push_back( capacity_ );
                bat.push_back( 128 );
            }else{
                bat.push_back( capacity_ );
                bat.push_back( 1 );
            }
        }

    #endif
    return bat;
}
