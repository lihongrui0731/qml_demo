#ifndef BATTERY_H
#define BATTERY_H

#include "filemanager.h"
#include <iostream>
#include <QVector>
#include <QThread>

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

    Q_INVOKABLE QVector<int> getBattery();

};

#endif // BATTERY_H
