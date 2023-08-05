#ifndef VTKCAMERACONTROLLER_H
#define VTKCAMERACONTROLLER_H

#include <QObject>
#include <Eigen/Dense>

#include "vtkCamera.h"
#include "vtkSmartPointer.h"
#include "Enums.h"

class QPropertyAnimation;

class VTKCameraController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(double currentPercent READ currentPercent WRITE setCurrentPercent)

public:
    explicit VTKCameraController(QObject* parent = nullptr);
    ~VTKCameraController();

    void switchView(const ViewType& type);
    void switchCameraMode(const CameraMode& mode);

    void setCurrentPercent(double percent);
    double currentPercent() const;

    vtkCamera *getCamera() const;

private:
    void updateCamera();

    vtkSmartPointer<vtkCamera> m_camera;
    Eigen::Vector3d m_oldPosition;
    Eigen::Vector3d m_targetPosition;
    Eigen::Vector3d m_targetUpPosition;
    Eigen::Vector3d m_focalPoint;
    Eigen::Vector3d m_oldViewUpPosition;
    QPropertyAnimation *m_rotateAnimation;

    double m_currentPercent;

signals:
    void cameraUpdated();
};

#endif //VTKCAMERACONTROLLER_H
