#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>


enum class ViewType
{
    Front,
    Back,
    Right,
    Left,
    Top,
    Bottom
};

enum class CameraMode
{
    FreeRotation,
    XLocked,
    YLocked,
    ZLocked,
    PanView,
    ZoomView
};

enum class TeethState {
    Normal = -1,
    Remove,
    Missing,
    Deciduous
};

enum class TeethNumberingSystem {
    X = -1,
    UniversalNumberingSystem,
    PalmerNotation,
    FDI
};

#endif // ENUMS_H
