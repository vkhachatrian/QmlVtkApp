#include "VTKController.h"
#include "VTKCameraController.h"
#include "PlaneCutController.h"
#include "SplineCutController.h"
#include "CustomMouseInteractorStyle.h"

#include <QQuickWindow>
#include <QQuickVTKRenderItem.h>
#include <QtConcurrent/QtConcurrent>

#include "vtkAbstractPolyDataReader.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSTLReader.h"
#include "vtkOBJReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkLight.h"
#include "vtkLightKit.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataNormals.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkOutlineFilter.h"
#include "vtkCellPicker.h"
#include "vtkRenderer.h"

VTKController::VTKController(QObject *parent)
    : QObject{parent}
    , m_vtkRenderItem(nullptr)
    , m_lastPickedActor(nullptr)
    , m_type(CutType::None)
{
    qDebug() << "Constructor::VTKController()";
    m_futureWatcherLoadingModel = new QFutureWatcher<void>;
    m_vtkCameraController = new VTKCameraController(this);
    makeConnections();
}

VTKController::~VTKController() {

    m_futureWatcherLoadingModel->deleteLater();
}

vtkSmartPointer<vtkActor> VTKController::createActor(const QString &objPath)
{
    QStringList list = objPath.split(".");
    QString ext = list.back();

    vtkSmartPointer<vtkAbstractPolyDataReader> reader;

    if (ext == "stl") {
        reader = vtkSmartPointer<vtkSTLReader>::New();
    }
    else {
        reader = vtkSmartPointer<vtkOBJReader>::New();
    }

    reader->SetFileName(objPath.toStdString().c_str());
    reader->Update();

    vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection(reader->GetOutputPort());
    //normals->ComputePointNormalsOn();
    //normals->SplittingOff();

    vtkSmartPointer<vtkPolyDataMapper> pointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pointsMapper->SetInputConnection(normals->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(pointsMapper.Get());

    return actor;
}

void VTKController::loadModel()
{
    qDebug() << "--------------------------> Initialization of model started <----------------------------" << m_engine->rootObjects().size();

    if(m_engine->rootObjects().size() == 0){
        qDebug() << "----------------------> Initialization Failed -> Root Objects Not Founded ";
        return;
    }
    m_vtkRenderItem = m_engine->rootObjects().value(0)->findChild<QQuickVTKRenderItem*>("RenderItem");
    if(m_vtkRenderItem == nullptr){
        qDebug() << "----------------------> Initialization Failed -> Render Item not founded";
        return;
    }
    m_vtkRenderItem->renderWindow()->renderWindow()->SetMultiSamples(8);

    vtkNew<vtkLightKit> lightKit;
    lightKit->AddLightsToRenderer(m_vtkRenderItem->renderer());

    vtkNew<vtkLight> lightScene;
    //vtkNew<vtkLight> lightNorthWest;

    lightScene->SetPosition(0, 0, 0);
    lightScene->SetFocalPoint(0, 0, 0);
    lightScene->SetColor(0.89, 0.89, 0.89);
    lightScene->SetIntensity(0.1);

    auto camera = m_vtkCameraController->getCamera();
    m_vtkRenderItem->renderer()->SetActiveCamera(camera);
    m_vtkRenderItem->renderer()->AddLight(lightScene.GetPointer());

    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    m_vtkRenderItem->renderer()->SetGradientBackground(true);
    m_vtkRenderItem->renderer()->SetBackground2(0.89, 0.89, 0.89);
    m_vtkRenderItem->renderer()->SetBackground(1, 1, 1);

    auto mouseInteractorStyle = CustomMouseInteractorStyle::Instance();
    mouseInteractorStyle->SetCurrentRenderer(m_vtkRenderItem->renderer());
    m_vtkRenderItem->renderer()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(mouseInteractorStyle);
    PlaneCutController::instance()->initRenderer(m_vtkRenderItem->renderer());
    SplineCutController::instance()->initRenderer(m_vtkRenderItem->renderer());

    m_upperSideActor = createActor(m_maxillaryObjectPath);
    m_downSideActor = createActor(m_manibularObjectPath);

    m_upperSideActor->SetObjectName("Maxillary");
    m_downSideActor->SetObjectName("Mandibular");
    m_vtkRenderItem->renderer()->AddActor(m_upperSideActor);
    m_vtkRenderItem->renderer()->AddActor(m_downSideActor);

    qDebug() << "--------------------------> Initialization of model finished <----------------------------" << m_engine->rootObjects().size();
}

void VTKController::makeConnections()
{
    connect(m_vtkCameraController, &VTKCameraController::cameraUpdated, this, &VTKController::onCameraUpdated);
    connect(m_futureWatcherLoadingModel, &QFutureWatcher<void>::finished, this, [this](){
        if(m_vtkRenderItem) {
            m_vtkRenderItem->renderer()->ResetCamera();
            m_vtkRenderItem->update();
        }
        emit loadingFinished();
    });
    connect(PlaneCutController::instance(), &PlaneCutController::updateRenderer, this, &VTKController::onCameraUpdated);
    connect(SplineCutController::instance(), &SplineCutController::updateRenderer, this, &VTKController::onCameraUpdated);
}

bool VTKController::foundAlwaysOnTopObject()
{
    return m_type == CutType::PlaneCut && PlaneCutController::instance()->shiftingObjectPicked();
}

VTKController *VTKController::instance()
{
    static VTKController *instance = nullptr;
    if (!instance) {
        instance = new VTKController;
    }
    return instance;
}

void VTKController::initModel(const QString& manibularObjPath, const QString& maxillaryObjPath)
{
    m_manibularObjectPath = manibularObjPath;
    m_maxillaryObjectPath = maxillaryObjPath;
    m_futureWatcherLoadingModel->setFuture(QtConcurrent::run([this](){ loadModel(); }));
}

void VTKController::unloadModel()
{
    m_vtkRenderItem->renderer()->RemoveActor(m_upperSideActor);
    m_vtkRenderItem->renderer()->RemoveActor(m_downSideActor);
    m_vtkRenderItem->renderer()->RemoveAllLights();
    PlaneCutController::instance()->modelsUnloaded();
    SplineCutController::instance()->modelsUnloaded();
    m_type = CutType::None;
    m_upperSideActor = nullptr;
    m_downSideActor = nullptr;
}

void VTKController::setMaxillaryOpacity(double val)
{
    m_upperSideActor->GetProperty()->SetOpacity(val);
}

void VTKController::setMandibularOpacity(double val)
{
    m_downSideActor->GetProperty()->SetOpacity(val);
}

void VTKController::setMandibularVisibility(bool visibility)
{
    m_downSideActor->SetVisibility(visibility);
}

void VTKController::setMaxillaryVisibility(bool visibility)
{
    m_upperSideActor->SetVisibility(visibility);
}

void VTKController::switchView(const ViewType& viewType)
{
    m_vtkCameraController->switchView(viewType);
}

void VTKController::switchCameraMode(const CameraMode& mode)
{
   m_vtkCameraController->switchCameraMode(mode);
}

void VTKController::setEngine(QQmlApplicationEngine *engine)
{
    m_engine = engine;
}

void VTKController::leftButtonPressed()
{
    vtkSmartPointer<vtkActor> currentPickedActor = pickActor();
    switch(m_type)
    {
    case CutType::None:
        break;
    case CutType::PlaneCut:
        PlaneCutController::instance()->updateModifyingActorIfNeeded(currentPickedActor, m_lastPickedActor);
        PlaneCutController::instance()->leftButtonPressed();
        break;
    case CutType::SplineCut:
        SplineCutController::instance()->leftButtonPressed();
        break;
    case CutType::Segmentation:
        break;
    }
    if(currentPickedActor && (currentPickedActor->GetObjectName() == "Maxillary" || currentPickedActor->GetObjectName() == "Mandibular"))
        m_lastPickedActor = currentPickedActor;

}

void VTKController::leftButtonReleased()
{
    switch(m_type)
    {
    case CutType::None:
        break;
    case CutType::PlaneCut:
        PlaneCutController::instance()->leftButtonReleased();
        break;
    case CutType::SplineCut:
        break;
    case CutType::Segmentation:
        break;
    }
}

void VTKController::rightButtonPressed()
{
    switch(m_type)
    {
    case CutType::None:
        break;
    case CutType::PlaneCut:
        break;
    case CutType::SplineCut:
        SplineCutController::instance()->rightButtonPressed();
        break;
    case CutType::Segmentation:
        break;
    }
}

void VTKController::mouseMoved()
{
    switch(m_type)
    {
    case CutType::None:
        break;
    case CutType::PlaneCut:
        PlaneCutController::instance()->mouseMoved();
        break;
    case CutType::SplineCut:
        SplineCutController::instance()->mouseMoved();
        break;
    case CutType::Segmentation:
        break;
    }
}

void VTKController::planeCutActivated()
{
    m_downSideActor->SetPickable(true);
    m_upperSideActor->SetPickable(true);
    m_type = CutType::PlaneCut;
}

void VTKController::splineCutActivated()
{
    m_downSideActor->SetPickable(false);
    m_upperSideActor->SetPickable(false);
    SplineCutController::instance()->setModifyingActor(m_lastPickedActor);
    m_type = CutType::SplineCut;
}

vtkSmartPointer<vtkActor> VTKController::pickActor()
{
    if(m_type == CutType::SplineCut)
        return nullptr;
    m_upperSideActor->SetPickable(false);
    m_downSideActor->SetPickable(false);
    if(foundAlwaysOnTopObject())
        return nullptr;
    m_upperSideActor->SetPickable(true);
    m_downSideActor->SetPickable(true);

    int* clickPos = m_vtkRenderItem->renderer()->GetRenderWindow()->GetInteractor()->GetEventPosition();
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.001);
    picker->Pick(clickPos[0], clickPos[1], 0, m_vtkRenderItem->renderer());
    if(picker->GetActor() && (picker->GetActor()->GetObjectName() == "Maxillary" || picker->GetActor()->GetObjectName() == "Mandibular")){
        emit choosedModifyingObject(QString::fromStdString(picker->GetActor()->GetObjectName()));
    }
    return picker->GetActor();
}

VTKController::CutType VTKController::getCutType()
{
    return m_type;
}

void VTKController::onCameraUpdated()
{
    m_vtkRenderItem->update();
}
