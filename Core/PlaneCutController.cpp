#include "PlaneCutController.h"


#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCoordinate.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkTransform.h"
#include "vtkPlane.h"
#include "vtkClipPolyData.h"

#include <QDebug>

#define PLANE_RADIUS 40.0
#define CUT_DIRECTION_SCALE 20.0

PlaneCutController::PlaneCutController(QObject *parent)
    : QObject{parent}
    , m_currentModifyingActor(nullptr)
    , m_renderer(nullptr)
    , m_state(State::LineEdit)
    , m_shiftingObject(ShiftingObject::None)
    , m_lastMouseYOnVerticalPlane(0.0)
    , m_currentModifyingActorChanged(false)//For discarding mouse events when modifying object is changed
{
    qDebug() << "Constructor::PlaneCutController()";
    createMembers();
    initCutLine();
    initCutPlane();
}

void PlaneCutController::createMembers()
{
    m_lineSource = vtkSmartPointer<vtkLineSource>::New();
    m_planeSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
    m_sphereSourceCenter = vtkSmartPointer<vtkSphereSource>::New();
    m_sphereSourceLateral1 = vtkSmartPointer<vtkSphereSource>::New();
    m_sphereSourceLateral2 = vtkSmartPointer<vtkSphereSource>::New();
    m_cutDirectionSource = vtkSmartPointer<vtkArrowSource>::New();

    m_lineActor = vtkSmartPointer<vtkActor>::New();
    m_planeActor = vtkSmartPointer<vtkActor>::New();
    m_sphereActorCenter = vtkSmartPointer<vtkActor>::New();
    m_sphereActorLateral1 = vtkSmartPointer<vtkActor>::New();
    m_sphereActorLateral2 = vtkSmartPointer<vtkActor>::New();
    m_cutDirectionActor = vtkSmartPointer<vtkActor>::New();
}

PlaneCutController::~PlaneCutController()
{}

PlaneCutController *PlaneCutController::instance()
{
    static PlaneCutController *instance = nullptr;
    if (!instance) {
        instance = new PlaneCutController;
    }
    return instance;
}

void PlaneCutController::initRenderer(vtkRenderer* ren)
{
    m_renderer = ren;
}

void PlaneCutController::clearCutPlane()
{
    m_renderer->RemoveActor(m_planeActor);
    m_renderer->RemoveActor(m_sphereActorCenter);
    m_renderer->RemoveActor(m_sphereActorLateral1);
    m_renderer->RemoveActor(m_sphereActorLateral2);
    m_renderer->RemoveActor(m_cutDirectionActor);
    m_state = State::LineEdit;
    emit updateRenderer();
}

void PlaneCutController::mouseMoved()
{
    if(m_currentModifyingActorChanged)
        return;

    if(m_state == State::LineEdit)
    {
        updateCutLine(getMouseWorldPosition());
    }
    else if(m_state == State::PlaneEdit)
    {
        switch(m_shiftingObject)
        {
        case ShiftingObject::Central:
            movePlaneByCenterShiftingObject();
            break;
        case ShiftingObject::Lateral:
            movePlaneByLateralShiftingObject();
            break;
        case ShiftingObject::None:
            break;
        }
    }
}

void PlaneCutController::leftButtonPressed()
{
    if(m_currentModifyingActorChanged)
        return;

    if(m_state == State::LineEdit)
        addCutLine();
    else if(m_state == State::PlaneEdit)
        findPickedActor();
}

void PlaneCutController::leftButtonReleased()
{
    if(m_currentModifyingActorChanged){
        m_currentModifyingActorChanged = false;
        return;
    }
    if(m_state == State::LineEdit)
    {
        addCutPlane();
        m_state = State::PlaneEdit;
    }
    else if(m_state == State::PlaneEdit)
    {
        m_shiftingObject = ShiftingObject::None;
    }
}

void PlaneCutController::addCutPlane()
{
    m_renderer->RemoveActor(m_lineActor);
    auto distance = std::sqrt(vtkMath::Distance2BetweenPoints(m_lineSource->GetPoint1(), m_lineSource->GetPoint2()));
    if(distance < 0.05) return;

    m_renderer->AddActor(m_planeActor);
    m_renderer->AddActor(m_sphereActorCenter);
    m_renderer->AddActor(m_sphereActorLateral1);
    m_renderer->AddActor(m_sphereActorLateral2);
    m_renderer->AddActor(m_cutDirectionActor);

    m_rotationAxis = fromCameraToCutLineCenter();
    Eigen::Vector3d cutLine = cutLineVector();
    updateCutPlane(centerOfCutPlane(), cutLine);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    transform->PostMultiply();
    double axis[3] = {cutLine[0], cutLine[1], cutLine[2]};
    transform->RotateWXYZ(0.5, axis);
    m_planeActor->SetUserTransform(transform);
}

void PlaneCutController::initCutPlane()
{
    m_planeSource->SetCenter(0.0, 0.0, 0.0);
    m_planeSource->SetNormal(0.0, 1.0, 0.0);
    m_planeSource->SetRadius(PLANE_RADIUS);
    m_planeSource->SetNumberOfSides(50);
    m_planeSource->SetGeneratePolyline(false);
    m_planeSource->Update();

    vtkPolyData* plane = m_planeSource->GetOutput();
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(plane);
    m_planeActor->SetMapper(mapper);
    m_planeActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    m_planeActor->GetProperty()->SetOpacity(0.3);
    m_planeActor->SetPickable(false);

    m_sphereActorCenter->SetObjectName("Central");
    m_sphereActorLateral1->SetObjectName("Lateral");
    m_sphereActorLateral2->SetObjectName("Lateral");

    initPlaneShiftingObject(m_sphereSourceCenter, m_sphereActorCenter, {0.0, 0.0, 0.0});
    initPlaneShiftingObject(m_sphereSourceLateral1, m_sphereActorLateral1, {0.0, 0.0, 0.0});
    initPlaneShiftingObject(m_sphereSourceLateral2, m_sphereActorLateral2, {0.0, 0.0, 0.0});
    m_sphereActorCenter->GetProperty()->SetColor(1.0, 0.0, 0.0);
    initCutDirectionObject();
}

void PlaneCutController::initCutLine()
{
    updateCutLine(Eigen::Vector3d(0.0, 0.0, 0.0), true);
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(m_lineSource->GetOutputPort());

    m_lineActor->SetMapper(mapper);
    m_lineActor->GetProperty()->SetColor(0.0 , 0.0 , 0.0);
}

void PlaneCutController::addCutLine()
{
    updateCutLine(getMouseWorldPosition(), true);
    m_renderer->AddActor(m_lineActor);
}


void PlaneCutController::initPlaneShiftingObject(vtkSmartPointer<vtkSphereSource> source, vtkSmartPointer<vtkActor> actor, const Eigen::Vector3d& centerCoord)
{
    source->SetRadius(1.5);
    source->SetCenter(centerCoord[0], centerCoord[1], centerCoord[2]);
    source->SetThetaResolution(200);
    source->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(source->GetOutput());
    mapper->SetResolveCoincidentTopologyToPolygonOffset();
    mapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(0, -35000);

    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
}

void PlaneCutController::initCutDirectionObject()
{
    m_cutDirectionSource->SetShaftResolution(100);
    m_cutDirectionSource->SetShaftRadius(0.01);
    m_cutDirectionSource->SetTipResolution(100);
    m_cutDirectionSource->SetTipLength(0.2);
    m_cutDirectionSource->SetTipRadius(0.05);

    vtkSmartPointer<vtkPolyDataMapper> mapper =vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(m_cutDirectionSource->GetOutputPort());
    mapper->SetResolveCoincidentTopologyToPolygonOffset();
    mapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(0, -35000);

    m_cutDirectionActor->SetMapper(mapper);
    m_cutDirectionActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
}

Eigen::Vector3d PlaneCutController::getMouseWorldPosition() {
    int* pos = m_renderer->GetRenderWindow()->GetInteractor()->GetEventPosition();

    vtkNew<vtkCoordinate> coordinate;
    coordinate->SetCoordinateSystemToDisplay();
    coordinate->SetValue(pos[0], pos[1], 0.1);

    double* worldCoord = coordinate->GetComputedWorldValue(m_renderer);
    return Eigen::Vector3d(worldCoord[0], worldCoord[1], worldCoord[2]);
}

void PlaneCutController::findPickedActor()
{
    int* clickPos = m_renderer->GetRenderWindow()->GetInteractor()->GetEventPosition();
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.001);
    picker->Pick(clickPos[0], clickPos[1], 0, m_renderer);
    if (picker->GetActor())
    {
        if(picker->GetActor()->GetObjectName() == "Central")
            m_shiftingObject = ShiftingObject::Central;
        else if(picker->GetActor()->GetObjectName() == "Lateral")
            m_shiftingObject = ShiftingObject::Lateral;
        else
            m_shiftingObject = ShiftingObject::None;

        double* projVector = m_renderer->GetActiveCamera()->GetDirectionOfProjection();
        Eigen::Vector3d projVectorEigen( projVector[0], projVector[1], projVector[2]);
        Eigen::Vector3d worldPosOnVertPlane = getClickWorldPositionOnVerticalPlane(projVectorEigen);
        m_lastMouseYOnVerticalPlane = worldPosOnVertPlane.y();
    }
}

void PlaneCutController::movePlaneByCenterShiftingObject()
{
    double* projVector = m_renderer->GetActiveCamera()->GetDirectionOfProjection();
    Eigen::Vector3d projVectorEigen( projVector[0], projVector[1], projVector[2]);
    Eigen::Vector3d worldPosOnVertPlane = getClickWorldPositionOnVerticalPlane(projVectorEigen);

    double deltaY = worldPosOnVertPlane[1] - m_lastMouseYOnVerticalPlane;
    m_lastMouseYOnVerticalPlane = worldPosOnVertPlane[1];

    double* central = m_sphereSourceCenter->GetCenter();
    m_sphereSourceCenter->SetCenter(central[0], central[1] + deltaY, central[2]);
    m_sphereSourceCenter->Update();

    double* lateral1 = m_sphereSourceLateral1->GetCenter();
    m_sphereSourceLateral1->SetCenter(lateral1[0], lateral1[1] + deltaY, lateral1[2]);
    m_sphereSourceLateral1->Update();

    double* lateral2 = m_sphereSourceLateral2->GetCenter();
    m_sphereSourceLateral2->SetCenter(lateral2[0], lateral2[1] + deltaY, lateral2[2]);
    m_sphereSourceLateral2->Update();

    double* centerOfPlane = m_planeSource->GetCenter();
    m_planeSource->SetCenter(centerOfPlane[0], centerOfPlane[1] + deltaY, centerOfPlane[2]);
    m_planeSource->Update();
    updateCutDirection();
}

void PlaneCutController::movePlaneByLateralShiftingObject()
{
    Eigen::Vector3d clickPosOnVertPlane = getClickWorldPositionOnVerticalPlane(m_rotationAxis);
    double* center = m_planeSource->GetCenter();
    Eigen::Vector3d  centerCoord(center[0], center[1], center[2]);
    Eigen::Vector3d newPlaneParralelVector(clickPosOnVertPlane[0] - centerCoord[0], clickPosOnVertPlane[1] - centerCoord[1], clickPosOnVertPlane[2] - centerCoord[2]);

    updateCutPlane(centerCoord, newPlaneParralelVector);
}

void PlaneCutController::runPlaneCut()
{
    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetNormal(-m_planeSource->GetNormal()[0], -m_planeSource->GetNormal()[1], -m_planeSource->GetNormal()[2]);
    plane->SetOrigin(m_planeSource->GetCenter()[0], m_planeSource->GetCenter()[1], m_planeSource->GetCenter()[2]);

    vtkSmartPointer<vtkClipPolyData> cutter = vtkSmartPointer<vtkClipPolyData>::New();
    cutter->SetInputData(vtkPolyData::SafeDownCast(m_currentModifyingActor->GetMapper()->GetInput()));
    cutter->SetClipFunction(plane);
    cutter->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(cutter->GetOutput());

    m_currentModifyingActor->SetMapper(mapper);
    m_currentModifyingActor->GetMapper()->Modified();
    emit updateRenderer();
}

void PlaneCutController::changeSwapDirection()
{
    double* normal = m_planeSource->GetNormal();
    normal[0] = -normal[0];
    normal[1] = -normal[1];
    normal[2] = -normal[2];
    m_planeSource->SetNormal(normal);
    updateCutDirection();
}

Eigen::Vector3d PlaneCutController::getClickWorldPositionOnVerticalPlane(const Eigen::Vector3d& rotAxis)
{
    double rotationAxis[3] = {rotAxis[0], rotAxis[1], rotAxis[2]};
    double* cameraPosition = m_renderer->GetActiveCamera()->GetPosition();
    double* centerCoord = m_planeSource->GetCenter();

    double vecFromCamToPlaneCenter[3] = {centerCoord[0] - cameraPosition[0], centerCoord[1] - cameraPosition[1], centerCoord[2] - cameraPosition[2]};
    double lengthFromCamToPlaneCenter = std::sqrt(vtkMath::Distance2BetweenPoints(cameraPosition, centerCoord));

    double angle1 = vtkMath::AngleBetweenVectors(vecFromCamToPlaneCenter, rotationAxis);

    double closestDistanceFromCamToVertPlane = lengthFromCamToPlaneCenter * cos(angle1);
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

void PlaneCutController::updateModifyingActorIfNeeded(vtkSmartPointer<vtkActor> currentPickedActor, vtkSmartPointer<vtkActor> lastPickedActor)
{
    m_currentModifyingActorChanged = currentPickedActor && !currentPickedActor->GetObjectName().empty() && currentPickedActor->GetObjectName() != lastPickedActor->GetObjectName();
    m_currentModifyingActor = m_currentModifyingActorChanged ? currentPickedActor : lastPickedActor; //Ternary operator covers the issue of first object initialization

}

void PlaneCutController::modelsUnloaded()
{
    clearCutPlane();
    m_currentModifyingActor = nullptr;
    m_currentModifyingActorChanged = false;
    m_lastMouseYOnVerticalPlane = 0.0;
}

bool PlaneCutController::shiftingObjectPicked()
{
    int* clickPos = m_renderer->GetRenderWindow()->GetInteractor()->GetEventPosition();
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.001);
    picker->Pick(clickPos[0], clickPos[1], 0, m_renderer);
    return (picker->GetActor() && (picker->GetActor()->GetObjectName() == "Central" || picker->GetActor()->GetObjectName() == "Lateral"));
}

Eigen::Vector3d PlaneCutController::normalOfCutLine()
{
    return cutLineVector().cross(fromCameraToCutLineCenter());
}

Eigen::Vector3d PlaneCutController::closestPointFromCamToCentralPlane()
{
    //Get Camera Right Direction Vector
    double* camRightDirectionAsDoubleArray = m_renderer->GetActiveCamera()->GetDirectionOfProjection();
    Eigen::Vector3d camRightDirection(camRightDirectionAsDoubleArray[0], camRightDirectionAsDoubleArray[1], camRightDirectionAsDoubleArray[2]);
    camRightDirection.normalize();

    //Get Vector of Camera Position and Renderer Center
    double* cameraPosition = m_renderer->GetActiveCamera()->GetPosition();
    double centerCoord[3] = {0.0, 0.0, 0.0};
    double fromCamToRenCenter[3] = {centerCoord[0] - cameraPosition[0] , centerCoord[1] - cameraPosition[1] , centerCoord[2] - cameraPosition[2]};

    //Get Corrected Angle
    double angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(camRightDirectionAsDoubleArray , fromCamToRenCenter));
    angle = angle > 90 ? 180 - angle : angle;
    angle = vtkMath::RadiansFromDegrees(angle);

    double distanceFromCamToRenCenter = std::sqrt(vtkMath::Distance2BetweenPoints(cameraPosition, centerCoord));
    double distanceFromCamToCentralPlane = distanceFromCamToRenCenter * cos(angle);

    //Get Intersection Point Camera Right Direction Vector and Central Plane
    return Eigen::Vector3d(camRightDirection[0] * distanceFromCamToCentralPlane,
                           camRightDirection[1] * distanceFromCamToCentralPlane,
                           camRightDirection[2] * distanceFromCamToCentralPlane);

}

Eigen::Vector3d PlaneCutController::cutLineVector()
{
    double* point1 = m_lineSource->GetPoint1();
    double* point2 = m_lineSource->GetPoint2();
    return Eigen::Vector3d(point2[0] - point1[0] , point2[1] - point1[1] , point2[2] - point1[2]);
}

Eigen::Vector3d PlaneCutController::centerOfCutPlane()
{
    Eigen::Vector3d closestPointFromCameraToCentralPlane = closestPointFromCamToCentralPlane();
    Eigen::Vector3d fromCamToCutLineCenter = fromCameraToCutLineCenter();

    double closestPointAsDoubleArray[3] = {closestPointFromCameraToCentralPlane[0], closestPointFromCameraToCentralPlane[1], closestPointFromCameraToCentralPlane[2]};
    double fromCamToCutLineCenterAsDoubleArray[3] = {fromCamToCutLineCenter[0], fromCamToCutLineCenter[1], fromCamToCutLineCenter[2]};

    double angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(fromCamToCutLineCenterAsDoubleArray, closestPointAsDoubleArray));
    angle = angle > 90 ? 180 - angle : angle;
    angle = vtkMath::RadiansFromDegrees(angle);

    double distanceFromCameraToCentralPlane = std::sqrt(closestPointAsDoubleArray[0] * closestPointAsDoubleArray[0] +
            closestPointAsDoubleArray[1] * closestPointAsDoubleArray[1] +
            closestPointAsDoubleArray[2] * closestPointAsDoubleArray[2]);

    double intersectDistance = distanceFromCameraToCentralPlane / cos(angle);
    fromCamToCutLineCenter.normalize();
    fromCamToCutLineCenter[0] *= intersectDistance;
    fromCamToCutLineCenter[1] *= intersectDistance;
    fromCamToCutLineCenter[2] *= intersectDistance;

    double* cameraPosition = m_renderer->GetActiveCamera()->GetPosition();
    return Eigen::Vector3d(fromCamToCutLineCenter[0] + cameraPosition[0],
                           fromCamToCutLineCenter[1] + cameraPosition[1],
                           fromCamToCutLineCenter[2] + cameraPosition[2]);
}

Eigen::Vector3d PlaneCutController::fromCameraToCutLineCenter()
{
    double* cameraPosition = m_renderer->GetActiveCamera()->GetPosition();
    double* point1 = m_lineSource->GetPoint1();
    double* point2 = m_lineSource->GetPoint2();
    double lineMidCoord[3] = {(point2[0] + point1[0]) / 2, (point2[1] + point1[1]) / 2, (point2[2] + point1[2]) / 2};

    return Eigen::Vector3d(lineMidCoord[0] - cameraPosition[0], lineMidCoord[1] - cameraPosition[1], lineMidCoord[2] - cameraPosition[2]);
}

void PlaneCutController::updateCutPlane(const Eigen::Vector3d& centerCoord, const Eigen::Vector3d& direction)
{
    Eigen::Vector3d normalizedDirVect = direction;
    normalizedDirVect.normalize();
    Eigen::Vector3d sphereCoordLateral1(centerCoord[0] + PLANE_RADIUS * normalizedDirVect[0], centerCoord[1] + PLANE_RADIUS * normalizedDirVect[1], centerCoord[2] + PLANE_RADIUS * normalizedDirVect[2]);
    Eigen::Vector3d sphereCoordLateral2(centerCoord[0] - PLANE_RADIUS * normalizedDirVect[0], centerCoord[1] - PLANE_RADIUS * normalizedDirVect[1], centerCoord[2] - PLANE_RADIUS * normalizedDirVect[2]);
    Eigen::Vector3d newNormalVectorOfCutPlane = normalizedDirVect.cross(m_rotationAxis);

    double* currentNormal = m_planeSource->GetNormal();
    double newNormal[3] = {newNormalVectorOfCutPlane[0], newNormalVectorOfCutPlane[1], newNormalVectorOfCutPlane[2]};
    double angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(currentNormal, newNormal));
    if(angle > 90.0) //This is for not allowing to change cut direction plane
        newNormalVectorOfCutPlane = -newNormalVectorOfCutPlane;

    m_planeSource->SetCenter(centerCoord[0], centerCoord[1], centerCoord[2]);
    m_planeSource->SetNormal(newNormalVectorOfCutPlane[0], newNormalVectorOfCutPlane[1], newNormalVectorOfCutPlane[2]);
    m_sphereSourceCenter->SetCenter(centerCoord[0] , centerCoord[1] , centerCoord[2]);
    m_sphereSourceLateral1->SetCenter(sphereCoordLateral1[0] , sphereCoordLateral1[1] , sphereCoordLateral1[2]);
    m_sphereSourceLateral2->SetCenter(sphereCoordLateral2[0] , sphereCoordLateral2[1] , sphereCoordLateral2[2]);

    m_planeSource->Update();
    m_sphereSourceCenter->Update();
    m_sphereSourceLateral1->Update();
    m_sphereSourceLateral2->Update();
    updateCutDirection();
}

void PlaneCutController::updateCutLine(const Eigen::Vector3d &coord, bool bothEndpoints)
{
    if(bothEndpoints)
        m_lineSource->SetPoint1(coord[0] , coord[1], coord[2]);
    m_lineSource->SetPoint2(coord[0] , coord[1], coord[2]);
    m_lineSource->Update();
}

void PlaneCutController::updateCutDirection()
{
    vtkSmartPointer<vtkTransform> arrowTransform = vtkSmartPointer<vtkTransform>::New();
    arrowTransform->Identity();
    arrowTransform->PostMultiply();
    arrowTransform->Scale(CUT_DIRECTION_SCALE, CUT_DIRECTION_SCALE, CUT_DIRECTION_SCALE);

    double arrowDefaultDir[3] = {1.0, 0.0, 0.0};
    double* neededNormal = m_planeSource->GetNormal();
    double arrowRotationAxis[3];
    vtkMath::Cross(arrowDefaultDir, neededNormal, arrowRotationAxis);
    double angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(arrowDefaultDir, neededNormal));
    arrowTransform->RotateWXYZ(angle, arrowRotationAxis);
    arrowTransform->Translate(m_planeActor->GetCenter());
    m_cutDirectionActor->SetUserTransform(arrowTransform);
    emit updateRenderer();
}
