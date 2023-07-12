#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QThread>
#include <QVariant>

class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = nullptr);

    Q_INVOKABLE QVariant readJsonFile(QString fileUrl);
private:
    QString* fileHandle_h5;

signals:

};

#endif // FILEMANAGER_H
