#include <QVariantMap>

#include "Parameters.h"

Parameters *Parameters::instance()
{
    static Parameters *instance = nullptr;
    if (!instance) {
        instance = new Parameters;
    }
    return instance;
}

Parameters::~Parameters()
{}

QVariantMap Parameters::getCameraModeParamsType(const CameraModeParamsType &type)
{
    QVariantMap map;

    switch (type) {
    case CameraModeParamsType::eFreeRotation:
        map["icon"] = "qrc:/Icons/icon_rotate";
        map["name"] = "Free Rotation";
        map["toolTipText"] = "A";
        break;
    case CameraModeParamsType::eXAxis:
        map["icon"] = "qrc:/Icons/CameraModelIcons/icon_XLocked";
        map["name"] = "X-axis locked";
        map["toolTipText"] = "S";
        break;
    case CameraModeParamsType::eYAxis:
        map["icon"] = "qrc:/Icons/CameraModelIcons/icon_YLocked";
        map["name"] = "Y-axis locked";
        map["toolTipText"] = "D";
        break;
    case CameraModeParamsType::eZAxis:
        map["icon"] = "qrc:/Icons/CameraModelIcons/icon_ZLocked";
        map["name"] = "Z-axis locked";
        map["toolTipText"] = "F";
        break;
    case CameraModeParamsType::ePanView:
        map["icon"] = "qrc:/Icons/CameraModelIcons/icon_panView";
        map["name"] = "Pan View";
        map["toolTipText"] = "G";
        break;
    case CameraModeParamsType::eZoomView:
        map["icon"] = "qrc:/Icons/CameraModelIcons/icon_zoom";
        map["name"] = "Zoom View";
        map["toolTipText"] = "H";
        break;
    }

    return map;
}

QVariantMap Parameters::getViewButtonsParamsType(const ViewButtonsParamsType &type)
{
    QVariantMap map;

    switch (type) {
    case ViewButtonsParamsType::eFront:
        map["icon"] = "qrc:/Icons/icon_frontView";
        map["iconActive"] = "qrc:/Icons/icon_frontViewActive";
        map["promptText"] = "Front";
        map["toolTipText"] = "Q";
        break;
    case ViewButtonsParamsType::eBack:
        map["icon"] = "qrc:/Icons/icon_backView";
        map["iconActive"] = "qrc:/Icons/icon_backViewActive";
        map["promptText"] = "Back";
        map["toolTipText"] = "W";
        break;
    case ViewButtonsParamsType::eRight:
        map["icon"] = "qrc:/Icons/icon_rightView";
        map["iconActive"] = "qrc:/Icons/icon_rightViewActive";
        map["promptText"] = "Right";
        map["toolTipText"] = "E";
        break;
    case ViewButtonsParamsType::eLeft:
        map["icon"] = "qrc:/Icons/icon_leftView";
        map["iconActive"] = "qrc:/Icons/icon_leftViewActive";
        map["promptText"] = "Left";
        map["toolTipText"] = "R";
        break;
    case ViewButtonsParamsType::eTop:
        map["icon"] = "qrc:/Icons/icon_topView";
        map["iconActive"] = "qrc:/Icons/icon_topViewActive";
        map["promptText"] = "Top";
        map["toolTipText"] = "T";
        break;
    case ViewButtonsParamsType::eBottom:
        map["icon"] = "qrc:/Icons/icon_bottomView";
        map["iconActive"] = "qrc:/Icons/icon_bottomViewActive";
        map["promptText"] = "Bottom";
        map["toolTipText"] = "Y";
        break;
    }

    return map;
}
