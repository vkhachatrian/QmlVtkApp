#include "SplineCutController.h"

#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCoordinate.h"
#include "vtkCamera.h"
#include "vtkTriangle.h"
#include "vtkProperty.h"
#include "vtkSphereSource.h"
#include "vtkCellPicker.h"
#include "vtkImplicitPolyDataDistance.h"
#include "vtkParametricFunctionSource.h"
#include "vtkParametricSpline.h"
#include "vtkAlgorithmOutput.h"
#include "vtkPolyLine.h"

#include <QDebug>

#define SPLINE_HEIGHT 75
#define DISTANCE_FROM_OBJECT 15
#define SMOOTH_SCALE 800

SplineCutController::SplineCutController(QObject *parent)
    : QObject{parent}
    , m_renderer(nullptr)
    , m_currentPickedActor(nullptr)
    , m_drawingDirection(0.0, 0.0, 1.0)
    , m_planeCoord(0.0, 0.0, 0.0)
    , m_currentModifyingActor(nullptr)
    , m_isInCumulativeMode(false)
{
    createMembers();
}

SplineCutController::~SplineCutController() {}

SplineCutController *SplineCutController::instance()
{
    static SplineCutController *instance = nullptr;
    if (!instance) {
        instance = new SplineCutController;
    }
    return instance;
}

void SplineCutController::initRenderer(vtkRenderer* ren)
{
    m_renderer = ren;
}

void SplineCutController::leftButtonPressed()
{
    m_currentPickedActor = pickActor();
    if(m_currentPickedActor && m_currentPickedActor->GetObjectName() == "SplinePoint")
        return;
    if(m_pointActors.size() < 1) {
        double* projVector = m_renderer->GetActiveCamera()->GetDirectionOfProjection();
        m_drawingDirection = Eigen::Vector3d(projVector[0], projVector[1], projVector[2]);
        m_drawingDirection.normalize();
    }
    Eigen::Vector3d clickStartPos;
    if(m_pointActors.size() >= 1)
        clickStartPos = getClickWorldPositionOnPlane(m_planeCoord, m_drawingDirection);
    else
        clickStartPos = getMouseWorldPosition(true);

    appendSplinePoint(clickStartPos);
    if(m_pointActors.size() > 1) {
        recreateSplineSurface();
    }
    if(m_pointActors.size() == 2){
        m_renderer->AddActor(m_splineActor);
        m_renderer->AddActor(m_splineSurfaceActor);
    }
}

void SplineCutController::rightButtonPressed()
{
    vtkSmartPointer<vtkActor> pickedActor = pickActor();
    if(pickedActor) {
        removePointActor(pickedActor);
        if(m_pointActors.size() > 1)
            recreateSplineSurface();
        else
        {
            m_renderer->RemoveActor(m_splineActor);
            m_renderer->RemoveActor(m_splineSurfaceActor);
        }
    }
}

void SplineCutController::mouseMoved()
{
    if(m_currentPickedActor && m_currentPickedActor->GetObjectName() == "SplinePoint")
    {
        Eigen::Vector3d shiftCoord = getClickWorldPositionOnPlane(m_planeCoord, m_drawingDirection);
        vtkSmartPointer<vtkSphereSource> sphere = vtkSphereSource::SafeDownCast(m_currentPickedActor->GetMapper()->GetInputConnection(0, 0)->GetProducer());
        sphere->SetCenter(shiftCoord[0], shiftCoord[1], shiftCoord[2]);

        updateSpline();
        recreateSplineSurface();
    }
}

void SplineCutController::setModifyingActor(vtkSmartPointer<vtkActor> actor)
{
    m_currentModifyingActor = actor;
}

void SplineCutController::modelsUnloaded()
{
    clearCutSpline();
}

void SplineCutController::clearCutSpline()
{
    m_renderer->RemoveActor(m_splineSurfaceActor);
    m_renderer->RemoveActor(m_splineActor);
    while(m_pointActors.size() != 0)
    {
        m_renderer->RemoveActor(m_pointActors.at(0));
        m_pointActors.removeAt(0);
    }
    emit updateRenderer();
}

void SplineCutController::cumulativeModeActivated(bool activate)
{
    m_isInCumulativeMode = activate;
    updateSpline();
    recreateSplineSurface();
}

void SplineCutController::createMembers()
{
    m_splineSurfaceActor = vtkSmartPointer<vtkActor>::New();
    m_splineSurfaceActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    m_splineSurfaceActor->GetProperty()->SetOpacity(0.3);
    m_splineSurfaceActor->SetPickable(false);

    m_splineActor = vtkSmartPointer<vtkActor>::New();
    m_splineActor->GetProperty()->SetColor(0.0, 0.0, 1.0);

    m_implicitPolyDataDistance = vtkSmartPointer<vtkImplicitPolyDataDistance>::New();
}

vtkSmartPointer<vtkActor> SplineCutController::pickActor()
{
    int* clickPos = m_renderer->GetRenderWindow()->GetInteractor()->GetEventPosition();
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.001);
    picker->Pick(clickPos[0], clickPos[1], 0, m_renderer);
    vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();
    return picker->GetActor();
}

Eigen::Vector3d SplineCutController::getMouseWorldPosition(bool isFirstPoint) {
    int* pos = m_renderer->GetRenderWindow()->GetInteractor()->GetEventPosition();

    vtkNew<vtkCoordinate> coordinate;
    coordinate->SetCoordinateSystemToDisplay();
    coordinate->SetValue(pos[0], pos[1], 0.1);

    double* worldCoord = coordinate->GetComputedWorldValue(m_renderer);

    Eigen::Vector3d result;
    if(isFirstPoint)
    {
        //Closest Point from Modifying Actor
        double closestPoint[3];
        double* camPosition = m_renderer->GetActiveCamera()->GetPosition();
        vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
        polydata->DeepCopy(m_currentModifyingActor->GetMapper()->GetInput());
        m_implicitPolyDataDistance->SetInput(polydata);
        double signedDistance = std::abs(m_implicitPolyDataDistance->EvaluateFunctionAndGetClosestPoint(m_renderer->GetActiveCamera()->GetPosition() , closestPoint));
        signedDistance -= DISTANCE_FROM_OBJECT;

        //Angle
        double fromCamToClosestPoint[3] = {closestPoint[0] - camPosition[0], closestPoint[1] - camPosition[1], closestPoint[2] - camPosition[2]};
        double fromCamToSelectedPoint[3] = {worldCoord[0] - camPosition[0], worldCoord[1] - camPosition[1], worldCoord[2] - camPosition[2]};
        double angle = vtkMath::AngleBetweenVectors(fromCamToClosestPoint, fromCamToSelectedPoint);

        //Distance
        double neededDistance = signedDistance/cos(angle);
        Eigen::Vector3d fromCamToSelectPoint(fromCamToSelectedPoint[0], fromCamToSelectedPoint[1], fromCamToSelectedPoint[2]);
        fromCamToSelectPoint.normalize();

        //Intersection Point
        result = {camPosition[0] + fromCamToSelectPoint[0] * neededDistance,
                  camPosition[1] + fromCamToSelectPoint[1] * neededDistance,
                  camPosition[2] + fromCamToSelectPoint[2] * neededDistance};
        m_planeCoord = result;
    }
    else
        result = {worldCoord[0], worldCoord[1], worldCoord[2]};
    return result;
}

vtkSmartPointer<vtkPolyData> SplineCutController::createPolydata()
{
    vtkSmartPointer<vtkPoints> points = m_isInCumulativeMode ? getCumulativePoints() : getOrdinaryPoints();
    vtkSmartPointer<vtkPoints> surfacePoints = vtkSmartPointer<vtkPoints>::New();
    for(int i = 0; i < points->GetNumberOfPoints(); ++i)
    {
        double splinePointFrontCoord[3];
        points->GetPoint(i, splinePointFrontCoord);
        double splinePointBackCoord[3]= {splinePointFrontCoord[0] + m_drawingDirection[0] * SPLINE_HEIGHT,
                                         splinePointFrontCoord[1] + m_drawingDirection[1] * SPLINE_HEIGHT,
                                         splinePointFrontCoord[2] + m_drawingDirection[2] * SPLINE_HEIGHT};
        surfacePoints->InsertNextPoint(splinePointFrontCoord[0], splinePointFrontCoord[1], splinePointFrontCoord[2]);
        surfacePoints->InsertNextPoint(splinePointBackCoord[0], splinePointBackCoord[1], splinePointBackCoord[2]);
    }
    vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
    for(int i = 0; i < surfacePoints->GetNumberOfPoints(); i += 2)
    {
        int j = (i == surfacePoints->GetNumberOfPoints() - 2) ? 0 : i + 2;
        vtkSmartPointer<vtkTriangle> triangle1 = vtkSmartPointer<vtkTriangle>::New();
        triangle1->GetPointIds()->SetId(0,i);
        triangle1->GetPointIds()->SetId(1,i + 1);
        triangle1->GetPointIds()->SetId(2,j);

        vtkSmartPointer<vtkTriangle> triangle2 = vtkSmartPointer<vtkTriangle>::New();
        triangle2->GetPointIds()->SetId(0,j);
        triangle2->GetPointIds()->SetId(1,i + 1);
        triangle2->GetPointIds()->SetId(2,j + 1);

        triangles->InsertNextCell(triangle1);
        triangles->InsertNextCell(triangle2);
    }
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(surfacePoints);
    polydata->SetPolys(triangles);
    return  polydata;
}

vtkSmartPointer<vtkPoints> SplineCutController::getOrdinaryPoints()
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for(const vtkSmartPointer<vtkActor> &pointActor: m_pointActors)
    {
        double* splinePointFrontCoord = pointActor->GetCenter();
        points->InsertNextPoint(splinePointFrontCoord[0], splinePointFrontCoord[1], splinePointFrontCoord[2]);
    }
    return points;
}

vtkSmartPointer<vtkPoints> SplineCutController::getCumulativePoints()
{
    vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
    spline->SetPoints(getOrdinaryPoints());
    vtkSmartPointer<vtkParametricFunctionSource> surfaceSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    surfaceSource->SetParametricFunction(spline);
    surfaceSource->SetUResolution(SMOOTH_SCALE);
    surfaceSource->SetVResolution(SMOOTH_SCALE);
    surfaceSource->Update();

    return surfaceSource->GetOutput()->GetPoints();
}

void SplineCutController::recreateSplineSurface()
{
    if(m_pointActors.size() <= 1)
        return;
    updateSpline();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(createPolydata());

    m_splineSurfaceActor->SetMapper(mapper);
    m_splineSurfaceActor->GetMapper()->Modified();
}

void SplineCutController::removePointActor(vtkSmartPointer<vtkActor> actor)
{
    for(int i = 0; i < m_pointActors.size(); i++)
    {
        if(actor == m_pointActors.at(i)) {
            m_renderer->RemoveActor(m_pointActors.at(i));
            m_pointActors.removeAt(i);
            break;
        }
    }
    updateSpline();
}

void SplineCutController::updateSpline()
{
    if(m_pointActors.size() <= 1)
        return;
    vtkNew<vtkPolyDataMapper> mapper;
    if(m_isInCumulativeMode)
    {
        vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
        spline->SetPoints(getCumulativePoints());
        vtkSmartPointer<vtkParametricFunctionSource> surfaceSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
        surfaceSource->SetParametricFunction(spline);
        surfaceSource->SetUResolution(SMOOTH_SCALE);
        surfaceSource->SetVResolution(SMOOTH_SCALE);
        surfaceSource->Update();
        mapper->SetInputData(surfaceSource->GetOutput());
    }
    else
    {
        vtkSmartPointer<vtkPoints> points = getOrdinaryPoints();
        vtkSmartPointer<vtkPolyLine> polyline = vtkSmartPointer<vtkPolyLine>::New();
        polyline->GetPointIds()->SetNumberOfIds(int(points->GetNumberOfPoints()));
        for(int i = 0; i < points->GetNumberOfPoints(); ++i)
            polyline->GetPointIds()->SetId(i, i);
        vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
        lines->InsertNextCell(polyline);

        vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
        polydata->SetPoints(points);
        polydata->SetLines(lines);
        mapper->SetInputData(polydata);
    }

    m_splineActor->SetMapper(mapper);
    m_splineActor->GetMapper()->Modified();
}

void SplineCutController::appendSplinePoint(Eigen::Vector3d pointCoord)
{
    vtkSmartPointer<vtkSphereSource> sphereSourceCenter = vtkSmartPointer<vtkSphereSource>::New();
    sphereSourceCenter->SetRadius(0.3);
    sphereSourceCenter->SetCenter(pointCoord[0], pointCoord[1], pointCoord[2]);
    sphereSourceCenter->SetThetaResolution(200);
    sphereSourceCenter->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphereSourceCenter->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    actor->SetObjectName("SplinePoint");
    m_renderer->AddActor(actor);
    m_pointActors.append(actor);
}

Eigen::Vector3d SplineCutController::getClickWorldPositionOnPlane(const Eigen::Vector3d &planeCoord, const Eigen::Vector3d &rotAxis)
{
    double rotationAxis[3] = {rotAxis[0], rotAxis[1], rotAxis[2]};
    double* cameraPosition = m_renderer->GetActiveCamera()->GetPosition();
    double coordFromPlane[3] = {planeCoord[0], planeCoord[1], planeCoord[2]};

    double vecFromCamToPlaneCoord[3] = {coordFromPlane[0] - cameraPosition[0], coordFromPlane[1] - cameraPosition[1], coordFromPlane[2] - cameraPosition[2]};
    double lengthFromCamToPlaneCoord = std::sqrt(vtkMath::Distance2BetweenPoints(cameraPosition, coordFromPlane));

    double angle1 = vtkMath::AngleBetweenVectors(vecFromCamToPlaneCoord, rotationAxis);

    double closestDistanceFromCamToVertPlane = lengthFromCamToPlaneCoord * cos(angle1);
    if(closestDistanceFromCamToVertPlane < 0 )
    {
        closestDistanceFromCamToVertPlane = std::abs(closestDistanceFromCamToVertPlane);
        rotationAxis[0] = -rotationAxis[0];
        rotationAxis[1] = -rotationAxis[1];
        rotationAxis[2] = -rotationAxis[2];
    }
    Eigen::Vector3d mouseClickWorldPos = getMouseWorldPosition();

    double vecfromCamToClickPos[3] = {mouseClickWorldPos[0] - cameraPosition[0], mouseClickWorldPos[1] - cameraPosition[1], mouseClickWorldPos[2] - cameraPosition[2]};
    double angle2 = vtkMath::AngleBetweenVectors(vecfromCamToClickPos, rotationAxis);
    double intersectLengthByCamAndMouseClick = closestDistanceFromCamToVertPlane / cos(angle2);

    Eigen::Vector3d fromCamToClickPos(vecfromCamToClickPos[0] , vecfromCamToClickPos[1] , vecfromCamToClickPos[2]);
    fromCamToClickPos.normalize();

    return Eigen::Vector3d(cameraPosition[0] + fromCamToClickPos[0] * intersectLengthByCamAndMouseClick,
            cameraPosition[1] + fromCamToClickPos[1] * intersectLengthByCamAndMouseClick,
            cameraPosition[2] + fromCamToClickPos[2] * intersectLengthByCamAndMouseClick);
}
