#include "IconHelper.h"

#include <QColor>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QUrl>
#include <QCoreApplication>
#include <QStandardPaths>

IconHelper *IconHelper::instance()
{
    static IconHelper *instance = nullptr;
    if (!instance) {
        instance = new IconHelper;
    }
    return instance;
}

IconHelper::~IconHelper()
{
}

IconHelper::IconHelper(QObject *parent)
    : QObject{parent}
{
}

QString IconHelper::getExternalIconWithColor(const QString& iconUrl, const QColor& oldColor, const QColor& newColor)
{
    QString iconPath;
    QString iconName;
    if(iconUrl.startsWith("qrc")){
        iconPath = iconUrl.sliced(3);
        iconName = iconUrl.split("/").back();
    }
    else{
        iconPath = iconUrl.sliced(8);
        iconName = iconUrl.split("/").back();
        iconName = iconName.mid(iconName.indexOf("_") + 1, 7);
    }

    QString tempFolder = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir tempDir(tempFolder);
    if (!tempDir.exists())
        tempDir.mkpath(tempFolder);

    const QString tempFilePath = tempDir.absolutePath() + "/icons_" + iconName + "_" + newColor.name().right(6) + ".svg";
    QFile qTempFile(tempFilePath);

    if (qTempFile.exists())
        return "file:///" + tempFilePath;
    else
    {
        QFile file(iconPath);
        if (!file.exists() || !file.open(QIODevice::ReadOnly) || !qTempFile.open(QIODevice::ReadWrite)) {
            return iconUrl;
        }

        QByteArray baData = file.readAll();
        QString svgcontents = QString(baData);
        QString toFind = oldColor.name();
        QString toReplace = newColor.name();
        svgcontents = svgcontents.replace(toFind, toReplace, Qt::CaseInsensitive);

        QTextStream out(&qTempFile);

        out << svgcontents;
        qTempFile.close();
        file.close();

        return "file:///" + tempFilePath;
    }
}
