#ifndef FILESEARCHINGTHREAD_H
#define FILESEARCHINGTHREAD_H

#include <QThread>
#include <QObject>

class FileSearchingThread : public QThread
{
    Q_OBJECT
public:
    explicit FileSearchingThread(QObject *parent = nullptr);
    ~FileSearchingThread();

    void setSearchingPath(const QString& searchPath);

private:
    QString m_searchPath;
    QString m_mandibular;
    QString m_maxillary;

    void searchFileOrFolder(const QString& searchPath);

signals:
    void searchingFinished(const QString& manibularObjPath, const QString& maxillaryObjPath);

protected:
    virtual void run() override;
};

#endif // FILESEARCHINGTHREAD_H
