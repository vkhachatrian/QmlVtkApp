#include "VTKCameraController.h"
#include "CustomMouseInteractorStyle.h"
#include "Utils3D.h"

#include <QQmlEngine>
#include <QPropertyAnimation>

VTKCameraController::VTKCameraController(QObject *parent)
    : QObject(parent)
    , m_currentPercent(1.0)
{
    m_camera = vtkSmartPointer<vtkCamera>::New();
    m_rotateAnimation = new QPropertyAnimation(this, "currentPercent", this);
}

VTKCameraController::~VTKCameraController()
{}

void VTKCameraController::switchView(const ViewType& type)
{
    auto oldPositionArray = m_camera->GetPosition();
    m_oldPosition = Eigen::Vector3d(oldPositionArray[0], oldPositionArray[1], oldPositionArray[2]);

    auto focalPointArray = m_camera->GetFocalPoint();
    m_focalPoint = Eigen::Vector3d(focalPointArray[0], focalPointArray[1], focalPointArray[2]);

    auto oldUpPositionArray = m_camera->GetViewUp();
    m_oldViewUpPosition = Eigen::Vector3d(oldUpPositionArray[0], oldUpPositionArray[1], oldUpPositionArray[2]);

    Eigen::Vector3d origin(0.0, 0.0, 0.0);
    Eigen::Vector3d lookVector(0.0, 0.0, 1.0);
    Eigen::Vector3d rightVector(1.0, 0.0, 0.0);
    Eigen::Vector3d upVector(0.0, 1.0, 0.0);

    auto distance = m_camera->GetDistance();
    m_rotateAnimation->stop();

    switch(type)
    {
    case ViewType::Front:
    {
        m_targetPosition = Eigen::Vector3d(origin + (lookVector * (m_focalPoint.z() + distance)));
        m_targetPosition.y() = m_focalPoint.y();
        m_targetUpPosition = Eigen::Vector3d(0.0, 1.0, 0.0);
        break;
    }
    case ViewType::Back:
    {
        m_targetPosition = Eigen::Vector3d(origin + (lookVector * (m_focalPoint.z() - distance)));
        m_targetPosition.y() = m_focalPoint.y();
        m_targetUpPosition = Eigen::Vector3d(0.0, 1.0, 0.0);
        break;
    }
    case ViewType::Right:
    {
        m_targetPosition = Eigen::Vector3d(origin + (rightVector * (m_focalPoint.x() - distance)));
        m_targetPosition.y() = m_focalPoint.y();
        m_targetUpPosition = Eigen::Vector3d(0.0, 1.0, 0.0);
        break;
    }
    case ViewType::Left:
        m_targetPosition = Eigen::Vector3d(origin + (rightVector * (m_focalPoint.x() + distance)));
        m_targetPosition.y() = m_focalPoint.y();
        m_targetUpPosition = Eigen::Vector3d(0.0, 1.0, 0.0);
        break;
    case ViewType::Top:
        m_targetPosition = Eigen::Vector3d(origin + (upVector * (m_focalPoint.y() + distance)));
        m_targetPosition.x() = m_focalPoint.x();
        m_targetUpPosition = Eigen::Vector3d(0.0, 0.0, -1.0);
        break;
    case ViewType::Bottom:
        m_targetPosition = Eigen::Vector3d(origin + (upVector * (m_focalPoint.y() - distance)));
        m_targetPosition.x() = m_focalPoint.x();
        m_targetUpPosition = Eigen::Vector3d(0.0, 0.0, 1.0);
        break;
    }

    m_rotateAnimation->setStartValue(0.0);
    m_rotateAnimation->setEndValue(1.0);
    m_rotateAnimation->setDuration(800);
    m_rotateAnimation->start();
}

void VTKCameraController::switchCameraMode(const CameraMode &mode)
{
    auto mouseInteractorStlye = CustomMouseInteractorStyle::Instance();
    mouseInteractorStlye->SetCameraMode(mode);
}

void VTKCameraController::setCurrentPercent(double percent)
{
    m_currentPercent = percent;
    updateCamera();
}

double VTKCameraController::currentPercent() const
{
    return m_currentPercent;
}

vtkCamera *VTKCameraController::getCamera() const
{
    return m_camera.GetPointer();
}

void VTKCameraController::updateCamera()
{
    if (m_currentPercent == 1.0) {
        m_camera->SetViewUp(m_targetUpPosition.x(), m_targetUpPosition.y(), m_targetUpPosition.z());
        m_camera->SetPosition(m_targetPosition.x(), m_targetPosition.y(), m_targetPosition.z());
    }
    else {
        Eigen::Vector3d oldPositionNormalized = (m_oldPosition - m_focalPoint).normalized();
        Eigen::Vector3d newPositionNormalized = (m_targetPosition - m_focalPoint).normalized();
        Eigen::Quaterniond qIdentity = Eigen::Quaterniond::Identity();
        Eigen::Quaterniond qTarget = Eigen::Quaterniond::FromTwoVectors(oldPositionNormalized, newPositionNormalized);
        Eigen::Quaterniond qCurrent = qIdentity.slerp(m_currentPercent, qTarget);
        Eigen::Vector3d curentPositionNormalized = qCurrent * oldPositionNormalized;

        double oldLength = (m_oldPosition - m_focalPoint).norm();
        double newLength = (m_targetPosition - m_focalPoint).norm();
        double currentLength = Utils3D::lerp(oldLength, newLength, m_currentPercent);

        Eigen::Vector3d currentPosition = (curentPositionNormalized * currentLength + m_focalPoint);

        auto oldUpPositionArray = m_camera->GetViewUp();
        Eigen::Vector3d oldUpPosition(oldUpPositionArray[0], oldUpPositionArray[1], oldUpPositionArray[2]);
        Eigen::Vector3d newUpPosition = m_targetUpPosition;
        Eigen::Vector3d currentUpPosition = Utils3D::lerp(oldUpPosition, newUpPosition, m_currentPercent);

        m_camera->SetViewUp(currentUpPosition.x(), currentUpPosition.y(), currentUpPosition.z());
        m_camera->SetPosition(currentPosition.x(), currentPosition.y(), currentPosition.z());
    }

    //auto oldPositionArray = m_camera->GetPosition();
    //m_oldPosition = Eigen::Vector3d(oldPositionArray[0], oldPositionArray[1], oldPositionArray[2]);

    emit cameraUpdated();
}
