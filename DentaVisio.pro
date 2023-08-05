QT += quick gui quickcontrols2 svg

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/Core
INCLUDEPATH += $$PWD/Dependencies/VTK-9.2.6/include/vtk-9.2
INCLUDEPATH += $$PWD/Dependencies/Eigen3.4.0

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/Dependencies/VTK-9.2.6/lib/Debug
    LIBS += -lvtkCommonCore-9.2d -lvtkCommonDataModel-9.2d -lvtkCommonExecutionModel-9.2d -lvtkCommonTransforms-9.2d -lvtkCommonColor-9.2d
    LIBS += -lvtkRenderingCore-9.2d -lvtkRenderingOpenGL2-9.2d -lvtkGUISupportQt-9.2d -lvtkGUISupportQtQuick-9.2d
    LIBS += -lvtkIOGeometry-9.2d -lvtkFiltersSources-9.2d
    LIBS += -lvtkInteractionStyle-9.2d -lvtkInteractionWidgets-9.2d -lvtksys-9.2d -lvtkImagingCore-9.2d
    LIBS += -lvtkImagingGeneral-9.2d -lvtkCommonMisc-9.2d -lvtkRenderingVolume-9.2d -lvtkFiltersCore-9.2d -lvtkFiltersGeneral-9.2d -lvtkIOImage-9.2d -lvtkCommonComputationalGeometry-9.2d
} else {
    LIBS += -L$$PWD/Dependencies/VTK-9.2.6/lib/Release
    LIBS += -lvtkCommonCore-9.2 -lvtkCommonDataModel-9.2 -lvtkCommonExecutionModel-9.2 -lvtkCommonTransforms-9.2  -lvtkCommonColor-9.2
    LIBS += -lvtkRenderingCore-9.2 -lvtkRenderingOpenGL2-9.2 -lvtkGUISupportQt-9.2 -lvtkGUISupportQtQuick-9.2
    LIBS += -lvtkIOGeometry-9.2 -lvtkFiltersSources-9.2
    LIBS += -lvtkInteractionStyle-9.2 -lvtkInteractionWidgets-9.2 -lvtksys-9.2 -lvtkImagingCore-9.2
    LIBS += -lvtkImagingGeneral-9.2  -lvtkCommonMisc-9.2 -lvtkRenderingVolume-9.2 -lvtkFiltersCore-9.2 -lvtkFiltersGeneral-9.2 -lvtkIOImage-9.2 -lvtkCommonComputationalGeometry-9.2
}

SOURCES += \
        $$PWD/Core/Parameters.cpp \
        $$PWD/Core/FileSearchingThread.cpp \
        $$PWD/Core/VTKController.cpp \
        $$PWD/Core/CameraParametersModel.cpp \
        $$PWD/Core/AppController.cpp \
        $$PWD/Core/VTKCameraController.cpp \
        $$PWD/Core/CustomMouseInteractorStyle.cpp \
        $$PWD/Core/PlaneCutController.cpp \
        $$PWD/Core/TeethParameters.cpp \
        $$PWD/Core/TeethModel.cpp \
        $$PWD/Core/IconHelper.cpp \
        $$PWD/Core/SplineCutController.cpp \
        main.cpp

RESOURCES += \
    resource.qrc

RC_ICONS += appLogo.ico

HEADERS += \
    $$PWD/Core/Parameters.h \
    $$PWD/Core/FileSearchingThread.h \
    $$PWD/Core/VTKController.h \
    $$PWD/Core/CameraParametersModel.h \
    $$PWD/Core/AppController.h \
    $$PWD/Core/Utils3D.h \
    $$PWD/Core/VTKCameraController.h \
    $$PWD/Core/CustomMouseInteractorStyle.h \
    $$PWD/Core/TeethParameters.h \
    $$PWD/Core/TeethModel.h \
    $$PWD/Core/PlaneCutController.h \
    $$PWD/Core/Enums.h \
    $$PWD/Core/IconHelper.h \
    $$PWD/Core/SplineCutController.h

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/qml
QML_IMPORT_PATH += $$PWD/Dependencies/VTK-9.2.6/lib/Release/qml/Release
QML_IMPORT_PATH += $$PWD/Dependencies/VTK-9.2.6/lib/Debug/qml/Debug

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
