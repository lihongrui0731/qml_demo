#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QThread>
#include <QVariant>

class FileManager : public QThread
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = nullptr);

    Q_INVOKABLE QVariant readJsonFile(QString fileUrl);

signals:

};

#endif // FILEMANAGER_H
