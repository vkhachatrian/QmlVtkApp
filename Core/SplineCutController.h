#ifndef SPLINECUTCONTROLLER_H
#define SPLINECUTCONTROLLER_H

#include <QObject>
#include <Eigen/Dense>

#include "vtkSmartPointer.h"

class vtkLineSource;
class vtkPolyDataMapper;
class vtkActor;
class vtkParametricFunctionSource;
class vtkParametricSpline;
class vtkImplicitPolyDataDistance;
class vtkRenderer;
class vtkPoints;
class vtkPolyData;

class SplineCutController : public QObject
{
    Q_OBJECT
public:
    explicit SplineCutController(QObject *parent = nullptr);
    static SplineCutController *instance();
    ~SplineCutController();
    void initRenderer(vtkRenderer* ren);

    void leftButtonPressed();
    void rightButtonPressed();
    void mouseMoved();

    void setModifyingActor(vtkSmartPointer<vtkActor> actor);
    void modelsUnloaded();

    Q_INVOKABLE void clearCutSpline();
    Q_INVOKABLE void cumulativeModeActivated(bool activate);

private:
    void createMembers();

    Eigen::Vector3d getMouseWorldPosition(bool isFirstPoint = false);
    Eigen::Vector3d getClickWorldPositionOnPlane(const Eigen::Vector3d &planeCoord, const Eigen::Vector3d& rotAxis);
    vtkSmartPointer<vtkActor> pickActor();

    vtkSmartPointer<vtkPoints> getOrdinaryPoints();
    vtkSmartPointer<vtkPoints> getCumulativePoints();
    vtkSmartPointer<vtkPolyData> createPolydata();

    void recreateSplineSurface();
    void removePointActor(vtkSmartPointer<vtkActor> actor);
    void updateSpline();
    void appendSplinePoint(Eigen::Vector3d pointCoord);

    vtkRenderer* m_renderer;
    vtkSmartPointer<vtkActor> m_splineSurfaceActor;
    vtkSmartPointer<vtkActor> m_currentPickedActor;
    vtkSmartPointer<vtkActor> m_splineActor;
    QList<vtkSmartPointer<vtkActor>> m_pointActors;
    Eigen::Vector3d m_drawingDirection;
    Eigen::Vector3d m_planeCoord;
    vtkSmartPointer<vtkActor> m_currentModifyingActor;
    vtkSmartPointer<vtkImplicitPolyDataDistance> m_implicitPolyDataDistance;

    bool m_isInCumulativeMode;

signals:
    void updateRenderer();

};

#endif // SPLINECUTCONTROLLER_H
