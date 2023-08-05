#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <QObject>

class IconHelper : public QObject
{
    Q_OBJECT

public:
    static IconHelper *instance();
    ~IconHelper();
    static QString getExternalIconWithColor(const QString& iconUrl, const QColor& oldColor, const QColor& newColor);

private:
    explicit IconHelper(QObject *parent = nullptr);
};

#endif // ICONHELPER_H
