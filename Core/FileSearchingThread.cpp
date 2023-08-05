#include "FileSearchingThread.h"

#include <QDir>

FileSearchingThread::FileSearchingThread(QObject *parent)
    : QThread{parent}
    , m_mandibular(QString())
    , m_maxillary(QString())
{
    qDebug() << "--------------------> FileSearchingThread::FileSearchingThread <----------------------";
}

FileSearchingThread::~FileSearchingThread()
{
    qDebug() << "--------------------> FileSearchingThread::~FileSearchingThread <----------------------";
}

void FileSearchingThread::setSearchingPath(const QString &searchPath)
{
    m_searchPath = searchPath;
}

void FileSearchingThread::searchFileOrFolder(const QString &searchPath)
{
    QDir dir(searchPath);
    m_mandibular.clear();
    m_maxillary.clear();

    for(const QFileInfo& name: dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if(!m_mandibular.isEmpty() && !m_maxillary.isEmpty())
            return;
        searchFileOrFolder(searchPath + "/" + name.fileName());
    }

    for(const QFileInfo& name: dir.entryInfoList(QDir::Files))
    {
        if((name.fileName().contains("Mandibular.stl") || name.fileName().contains("Mandibular.obj")) && m_mandibular.isEmpty())
            m_mandibular = name.filePath();

        if((name.fileName().contains("Maxillary.stl") || name.fileName().contains("Maxillary.obj")) && m_maxillary.isEmpty())
            m_maxillary = name.filePath();
    }
}

void FileSearchingThread::run()
{
    qDebug() << "-------------------->Start File Searching<--------------------";

    searchFileOrFolder(m_searchPath.remove(0, 8));
    emit searchingFinished(m_mandibular, m_maxillary);

    qDebug() << "----------------->End File Searching<------------------------";
    QThread::run();
}
