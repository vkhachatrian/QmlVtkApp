#include "CustomMouseInteractorStyle.h"
#include "VTKController.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"

#include <Eigen/Dense>

CustomMouseInteractorStyle::CustomMouseInteractorStyle()
    : m_rightButtonDown(false)
    , m_cameraMode(CameraMode::FreeRotation)
{}

CustomMouseInteractorStyle::~CustomMouseInteractorStyle()
{}

CustomMouseInteractorStyle *CustomMouseInteractorStyle::Instance()
{
    static CustomMouseInteractorStyle *instance = nullptr;
    if (!instance) {
        instance = new CustomMouseInteractorStyle;
    }
    return instance;
}

void CustomMouseInteractorStyle::SetCameraMode(const CameraMode &mode)
{
    switch(mode)
    {
    case CameraMode::FreeRotation:
        m_cameraMode = CameraMode::FreeRotation;
        break;
    case CameraMode::XLocked:
        m_cameraMode = CameraMode::XLocked;
        break;
    case CameraMode::YLocked:
        m_cameraMode = CameraMode::YLocked;
        break;
    case CameraMode::ZLocked:
        m_cameraMode = CameraMode::ZLocked;
        break;
    case CameraMode::PanView:
        m_cameraMode = CameraMode::PanView;
        break;
    case CameraMode::ZoomView:
        m_cameraMode = CameraMode::ZoomView;
        break;
    }
}

void CustomMouseInteractorStyle::OnMouseMove()
{

    if(!m_rightButtonDown) {
        VTKController::instance()->mouseMoved();
        if(VTKController::instance()->getCutType() == VTKController::CutType::None)
            vtkInteractorStyleTrackballCamera::OnMouseMove();
    }
    else {

        if (this->CurrentRenderer == nullptr) return;

        if(m_cameraMode == CameraMode::FreeRotation)
        {
            CustomMouseInteractorStyle::Rotate();
            return;
        }
        else if(m_cameraMode == CameraMode::ZoomView) {
            CustomMouseInteractorStyle::Dolly();
            return;
        }
        else if(m_cameraMode == CameraMode::PanView)
        {
            CustomMouseInteractorStyle::Pan();
            return;
        }

        vtkRenderWindowInteractor* renderWindowInteractor = this->Interactor;

        int dx = renderWindowInteractor->GetEventPosition()[0] - renderWindowInteractor->GetLastEventPosition()[0];
        int dy = renderWindowInteractor->GetEventPosition()[1] - renderWindowInteractor->GetLastEventPosition()[1];

        const int* size = this->CurrentRenderer->GetRenderWindow()->GetSize();

        double delta_elevation = -20.0 / size[1];
        double delta_azimuth = -20.0 / size[0];

        double rxf = dx * delta_azimuth * this->MotionFactor;
        double ryf = dy * delta_elevation * this->MotionFactor;

        vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();

        if(m_cameraMode == CameraMode::YLocked)camera->Azimuth(rxf);
        if(m_cameraMode == CameraMode::XLocked)camera->Elevation(ryf);

        if(m_cameraMode == CameraMode::ZLocked)
        {
            double* center = this->CurrentRenderer->GetCenter();
            double newAngle = vtkMath::DegreesFromRadians(atan2(renderWindowInteractor->GetEventPosition()[1] - center[1], renderWindowInteractor->GetEventPosition()[0] - center[0]));
            double oldAngle = vtkMath::DegreesFromRadians(atan2(renderWindowInteractor->GetLastEventPosition()[1] - center[1], renderWindowInteractor->GetLastEventPosition()[0] - center[0]));
            camera->Roll(newAngle - oldAngle);
        }

        camera->OrthogonalizeViewUp();

        if (this->AutoAdjustCameraClippingRange)
            this->CurrentRenderer->ResetCameraClippingRange();

        if (renderWindowInteractor->GetLightFollowCamera())
            this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();

        renderWindowInteractor->Render();
    }
}

void CustomMouseInteractorStyle::OnLeftButtonDown()
{
    VTKController::instance()->leftButtonPressed();
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void CustomMouseInteractorStyle::OnLeftButtonUp()
{
    VTKController::instance()->leftButtonReleased();
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void CustomMouseInteractorStyle::OnRightButtonDown()
{
    VTKController::instance()->rightButtonPressed();
    m_rightButtonDown = true;
    this->StartRotate();
}

void CustomMouseInteractorStyle::OnRightButtonUp()
{
    m_rightButtonDown = false;
    this->EndRotate();
}
