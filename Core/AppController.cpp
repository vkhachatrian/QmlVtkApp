#include "AppController.h"
#include "FileSearchingThread.h"
#include "VTKController.h"

#include <QDir>

AppController::AppController(QObject *parent)
    : QObject{parent}
    , m_fileSearchingThread(nullptr)
{}

AppController::~AppController(){

    if(m_fileSearchingThread)
    {
        m_fileSearchingThread->deleteLater();
        m_fileSearchingThread = nullptr;
    }
}

void AppController::onSearchingFinished(const QString &mandibularObjPath, const QString &maxillaryObjPath)
{
    m_fileSearchingThread->quit();
    m_fileSearchingThread->wait();

    if(m_fileSearchingThread) {
        m_fileSearchingThread->deleteLater();
        m_fileSearchingThread = nullptr;
    }

    if(maxillaryObjPath.isEmpty())
    {
        emit searchingFileIsMissing(true);
        return;
    }
    else if(mandibularObjPath.isEmpty())
    {
        emit searchingFileIsMissing(false);
        return;
    }

    VTKController::instance()->initModel(mandibularObjPath, maxillaryObjPath);
}

void AppController::startObjectSearching(const QString &dirPath)
{
    if(!m_fileSearchingThread)
    {
        m_fileSearchingThread = new FileSearchingThread;
        connect(m_fileSearchingThread, &FileSearchingThread::searchingFinished, this, &AppController::onSearchingFinished, Qt::QueuedConnection);
    }

    m_fileSearchingThread->setSearchingPath(dirPath);
    m_fileSearchingThread->start();
}

AppController *AppController::instance()
{
    static AppController *instance = nullptr;
    if (!instance) {
        instance = new AppController;
    }
    return instance;
}
