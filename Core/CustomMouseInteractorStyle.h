#ifndef CUSTOMMOUSEINTERACTORSTYLE_H
#define CUSTOMMOUSEINTERACTORSTYLE_H

#include <QObject>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLineSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkVector.h>

#include "Enums.h"

class CustomMouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:

    static CustomMouseInteractorStyle *Instance();

    void SetCameraMode(const CameraMode& mode);

    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnMouseMove() override;

    bool m_rightButtonDown;

private:
    explicit CustomMouseInteractorStyle();
    ~CustomMouseInteractorStyle();

    CameraMode m_cameraMode;
};


#endif // CUSTOMMOUSEINTERACTORSTYLE_H
