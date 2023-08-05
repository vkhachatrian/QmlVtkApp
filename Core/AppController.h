#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

class FileSearchingThread;

class AppController : public QObject
{
    Q_OBJECT
public:
    static AppController *instance();
    ~AppController();

    Q_INVOKABLE void startObjectSearching(const QString& dirPath);

private:
    explicit AppController(QObject *parent = nullptr);

    FileSearchingThread* m_fileSearchingThread;

signals:
    void searchingFileIsMissing(bool isMaxillary);

private slots:
    void onSearchingFinished(const QString& mandibularObjPath, const QString& maxillaryObjPath);
};

#endif // APPCONTROLLER_H
