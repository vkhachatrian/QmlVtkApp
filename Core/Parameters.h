#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>

enum CameraModeParamsType {
    eFreeRotation,
    eXAxis,
    eYAxis,
    eZAxis,
    ePanView,
    eZoomView
};

enum ViewButtonsParamsType {
    eFront,
    eBack,
    eRight,
    eLeft,
    eTop,
    eBottom
};

class Parameters: public QObject
{
    Q_OBJECT;

public:
    static Parameters *instance();
    ~Parameters();

    QVariantMap getCameraModeParamsType(const CameraModeParamsType& type);
    QVariantMap getViewButtonsParamsType(const ViewButtonsParamsType& type);
private:
    explicit Parameters(QObject* parent = nullptr) {};
};

#endif //PARAMETERS_H
