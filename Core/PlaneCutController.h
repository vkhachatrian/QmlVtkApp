#ifndef PLAINCUTCONTROLLER_H
#define PLAINCUTCONTROLLER_H

#include <QObject>

#include "vtkLineSource.h"
#include "vtkRegularPolygonSource.h"
#include "vtkVector.h"
#include "vtkActor.h"
#include "vtkSphereSource.h"
#include "vtkArrowSource.h"

#include <Eigen/Dense>

class PlaneCutController : public QObject
{
    Q_OBJECT
public:
    enum State
    {
       LineEdit,
       PlaneEdit
    };

    enum ShiftingObject
    {
        None,
        Central,
        Lateral
    };
    static PlaneCutController *instance();
    void initRenderer(vtkRenderer* ren);

    Q_INVOKABLE void clearCutPlane();
    Q_INVOKABLE void runPlaneCut();
    Q_INVOKABLE void changeSwapDirection();

    void mouseMoved();
    void leftButtonPressed();
    void leftButtonReleased();
    void updateModifyingActorIfNeeded(vtkSmartPointer<vtkActor> currentPickedActor, vtkSmartPointer<vtkActor> lastPickedActor);
    void modelsUnloaded();
    bool shiftingObjectPicked();

private:
    explicit PlaneCutController(QObject *parent = nullptr);
    ~PlaneCutController();

    void createMembers();
    void makeConnections();

    void initCutPlane();
    void initCutLine();

    void addCutPlane();
    void addCutLine();

    void updateCutPlane(const Eigen::Vector3d &centerCoord, const Eigen::Vector3d &direction);
    void updateCutLine(const Eigen::Vector3d& coord, bool bothEndpoints = false);
    void updateCutDirection();

    void initPlaneShiftingObject(vtkSmartPointer<vtkSphereSource> source , vtkSmartPointer<vtkActor> actor, const Eigen::Vector3d &centerCoord);
    void initCutDirectionObject();

    void findPickedActor();
    void movePlaneByCenterShiftingObject();
    void movePlaneByLateralShiftingObject();

    Eigen::Vector3d normalOfCutLine();
    Eigen::Vector3d centerOfCutPlane();

    Eigen::Vector3d closestPointFromCamToCentralPlane();
    Eigen::Vector3d cutLineVector();
    Eigen::Vector3d fromCameraToCutLineCenter();

    Eigen::Vector3d getMouseWorldPosition();
    Eigen::Vector3d getClickWorldPositionOnVerticalPlane(const Eigen::Vector3d& rotAxis);

    vtkSmartPointer<vtkLineSource> m_lineSource;
    vtkSmartPointer<vtkRegularPolygonSource> m_planeSource;
    vtkSmartPointer<vtkSphereSource> m_sphereSourceCenter;
    vtkSmartPointer<vtkSphereSource> m_sphereSourceLateral1;
    vtkSmartPointer<vtkSphereSource> m_sphereSourceLateral2;
    vtkSmartPointer<vtkArrowSource> m_cutDirectionSource;

    vtkSmartPointer<vtkActor> m_lineActor;
    vtkSmartPointer<vtkActor> m_planeActor;
    vtkSmartPointer<vtkActor> m_sphereActorCenter;
    vtkSmartPointer<vtkActor> m_sphereActorLateral1;
    vtkSmartPointer<vtkActor> m_sphereActorLateral2;
    vtkSmartPointer<vtkActor> m_cutDirectionActor;

    vtkSmartPointer<vtkActor> m_currentModifyingActor;

    vtkRenderer* m_renderer;
    Eigen::Vector3d m_rotationAxis;

    State m_state;
    ShiftingObject m_shiftingObject;
    double m_lastMouseYOnVerticalPlane;
    bool m_currentModifyingActorChanged;
signals:
    void updateRenderer();
};

#endif // PLAINCUTCONTROLLER_H
