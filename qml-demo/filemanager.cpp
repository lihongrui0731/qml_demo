#include "filemanager.h"
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QVariant>

FileManager::FileManager(QObject *parent)
    : QThread{parent}
{

    };

QVariant FileManager::readJsonFile(QString fileUrl) {
    qDebug() << fileUrl;
    QFile file(fileUrl);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }
    QVariant data = file.readAll();
    return data;
};
