#ifndef BATTERY_H
#define BATTERY_H

#include <QQuickItem>
#include <iostream>
#include <QVector>
#include <QThread>
#include <QSettings>
#include <QCoreApplication>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

using namespace std;

class battery:public QQuickItem
{
    Q_OBJECT
public:
    battery();
    virtual ~battery();

    // 电量来源于串口
    bool isCom{false};
    int percent{100};
    int status{0};
    Q_INVOKABLE void saveBattery( int value_ ,int status_ );

    Q_INVOKABLE QVector<int> getBattery();

    QString openFile( QString f );
};

#endif // BATTERY_H
