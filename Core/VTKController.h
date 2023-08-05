#ifndef VTKCONTROLLER_H
#define VTKCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QFutureWatcher>

#include "VTKCameraController.h"
#include "vtkActor.h"
#include "vtkSmartPointer.h"

class QQuickVTKRenderItem;

class VTKController : public QObject
{
    Q_OBJECT
public:
    static VTKController *instance();
    ~VTKController();

    enum CutType
    {
        PlaneCut,
        SplineCut,
        Segmentation,
        None
    };

    Q_INVOKABLE void initModel(const QString& manibularObjPath, const QString& maxillaryObjPath);
    Q_INVOKABLE void unloadModel();
    Q_INVOKABLE void setMaxillaryOpacity(double val);
    Q_INVOKABLE void setMandibularOpacity(double val);
    Q_INVOKABLE void setMandibularVisibility(bool visibility);
    Q_INVOKABLE void setMaxillaryVisibility(bool visibility);
    Q_INVOKABLE void switchView(const ViewType& viewType);
    Q_INVOKABLE void switchCameraMode(const CameraMode& mode);
    Q_INVOKABLE void  planeCutActivated();
    Q_INVOKABLE void  splineCutActivated();

    void setEngine(QQmlApplicationEngine *engine);

    void leftButtonPressed();
    void leftButtonReleased();
    void rightButtonPressed();
    void mouseMoved();

    vtkSmartPointer<vtkActor> pickActor();
    CutType getCutType();

public slots:
    void onCameraUpdated();

private:
    explicit VTKController(QObject *parent = nullptr);
    void makeConnections();
    void loadModel();
    vtkSmartPointer<vtkActor> createActor(const QString& objPath);
    bool foundAlwaysOnTopObject();

    QQuickVTKRenderItem* m_vtkRenderItem;
    VTKCameraController* m_vtkCameraController;
    vtkSmartPointer<vtkActor> m_upperSideActor;
    vtkSmartPointer<vtkActor> m_downSideActor;
    vtkSmartPointer<vtkActor> m_lastPickedActor;

    QQmlApplicationEngine* m_engine;
    QString m_manibularObjectPath;
    QString m_maxillaryObjectPath;
    QFutureWatcher<void>* m_futureWatcherLoadingModel;

    CutType m_type;
signals:
    void loadingFinished();
    void choosedModifyingObject(const QString& name);
};

#endif // VTKCONTROLLER_H
