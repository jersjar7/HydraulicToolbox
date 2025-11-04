#include "VtkWidget.h"
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkProperty.h>
#include <vtkLight.h>
#include <cmath>

VtkWidget::VtkWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
    , renderer_{vtkSmartPointer<vtkRenderer>::New()}
    , renderWindow_{vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()}
    , interactor_{vtkSmartPointer<vtkRenderWindowInteractor>::New()}
    , channelBottomActor_{vtkSmartPointer<vtkActor>::New()}
    , channelWallsActor_{vtkSmartPointer<vtkActor>::New()}
    , waterActor_{vtkSmartPointer<vtkActor>::New()}
    , cubeActor_{vtkSmartPointer<vtkAnnotatedCubeActor>::New()}
    , orientationWidget_{vtkSmartPointer<vtkOrientationMarkerWidget>::New()}
    , focalPointX_{0.0}
    , focalPointY_{0.0}
    , focalPointZ_{0.0}
    , viewDistance_{10.0}
{
    setMouseTracking(false);
    setAttribute(Qt::WA_AcceptTouchEvents, false);
    setEnableTouchEventProcessing(false);
    setup_vtk_pipeline();
}

VtkWidget::~VtkWidget()
{
}

void VtkWidget::setup_vtk_pipeline()
{
    renderWindow_->AddRenderer(renderer_);

    setRenderWindow(renderWindow_);

    renderer_->SetBackground(0.8, 0.8, 0.8);

    interactor_ = renderWindow_->GetInteractor();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    interactor_->SetInteractorStyle(style);
    interactor_->Initialize();

    channelBottomActor_->SetVisibility(0);
    channelWallsActor_->SetVisibility(0);
    waterActor_->SetVisibility(0);

    setup_lighting();
    setup_orientation_marker();
    setup_camera();
}

void VtkWidget::setup_camera()
{
    vtkCamera* camera = renderer_->GetActiveCamera();
    camera->ParallelProjectionOn();
}

void VtkWidget::setup_orientation_marker()
{
    cubeActor_->SetXPlusFaceText("RIGHT");
    cubeActor_->SetXMinusFaceText("LEFT");
    cubeActor_->SetYPlusFaceText("TOP");
    cubeActor_->SetYMinusFaceText("BOTTOM");
    cubeActor_->SetZPlusFaceText("FRONT");
    cubeActor_->SetZMinusFaceText("BACK");

    cubeActor_->SetFaceTextScale(0.1);

    cubeActor_->GetXPlusFaceProperty()->SetColor(0.95, 0.16, 0.36);
    cubeActor_->GetXMinusFaceProperty()->SetColor(0.35, 0.0, 0.0);
    cubeActor_->GetYPlusFaceProperty()->SetColor(0.0, 0.91, 0.63);
    cubeActor_->GetYMinusFaceProperty()->SetColor(0.0, 0.35, 0.0);
    cubeActor_->GetZPlusFaceProperty()->SetColor(0.0, 0.44, 0.83);
    cubeActor_->GetZMinusFaceProperty()->SetColor(0.0, 0.0, 0.35);

    cubeActor_->GetTextEdgesProperty()->SetOpacity(0.0);
    cubeActor_->GetCubeProperty()->SetColor(0.5, 0.5, 0.5);

    orientationWidget_->SetOrientationMarker(cubeActor_);
    orientationWidget_->SetInteractor(interactor_);

    orientationWidget_->SetViewport(0.82, 0.60, 0.98, 0.80);
    orientationWidget_->SetEnabled(1);
    orientationWidget_->InteractiveOff();
}

void VtkWidget::setup_lighting()
{
    renderer_->RemoveAllLights();
    renderer_->AutomaticLightCreationOff();

    vtkSmartPointer<vtkLight> keyLight = vtkSmartPointer<vtkLight>::New();
    keyLight->SetPosition(1.0, 1.0, 1.0);
    keyLight->SetFocalPoint(0.0, 0.0, 0.0);
    keyLight->SetColor(1.0, 1.0, 1.0);
    keyLight->SetIntensity(0.8);
    renderer_->AddLight(keyLight);

    vtkSmartPointer<vtkLight> fillLight = vtkSmartPointer<vtkLight>::New();
    fillLight->SetPosition(-1.0, 0.5, 0.5);
    fillLight->SetFocalPoint(0.0, 0.0, 0.0);
    fillLight->SetColor(1.0, 1.0, 1.0);
    fillLight->SetIntensity(0.4);
    renderer_->AddLight(fillLight);

    vtkSmartPointer<vtkLight> rimLight = vtkSmartPointer<vtkLight>::New();
    rimLight->SetPosition(0.0, 1.0, -1.0);
    rimLight->SetFocalPoint(0.0, 0.0, 0.0);
    rimLight->SetColor(1.0, 1.0, 1.0);
    rimLight->SetIntensity(0.3);
    renderer_->AddLight(rimLight);
}

void VtkWidget::show_content()
{
    if (channelBottomActor_)
    {
        channelBottomActor_->SetVisibility(1);
    }
    if (channelWallsActor_)
    {
        channelWallsActor_->SetVisibility(1);
    }
    if (waterActor_)
    {
        waterActor_->SetVisibility(1);
    }
    renderWindow_->Render();
}

void VtkWidget::hide_content()
{
    if (channelBottomActor_)
    {
        channelBottomActor_->SetVisibility(0);
    }
    if (channelWallsActor_)
    {
        channelWallsActor_->SetVisibility(0);
    }
    if (waterActor_)
    {
        waterActor_->SetVisibility(0);
    }
    renderWindow_->Render();
}

void VtkWidget::render_channel(const GeometryData& geometry, const CalculationResults& results)
{
    double width = geometry.bottomWidth;
    double normalDepth = results.normalDepth;
    double channelDepth = normalDepth * 1.2;
    double length = width * 10.0;

    // ===== CREATE CHANNEL BOTTOM =====
    vtkSmartPointer<vtkPoints> bottomPoints = vtkSmartPointer<vtkPoints>::New();
    bottomPoints->InsertNextPoint(0.0, 0.0, 0.0);
    bottomPoints->InsertNextPoint(length, 0.0, 0.0);
    bottomPoints->InsertNextPoint(length, 0.0, width);
    bottomPoints->InsertNextPoint(0.0, 0.0, width);

    vtkSmartPointer<vtkCellArray> bottomCells = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType bottom[4] = {0, 1, 2, 3};
    bottomCells->InsertNextCell(4, bottom);

    vtkSmartPointer<vtkPolyData> bottomPolyData = vtkSmartPointer<vtkPolyData>::New();
    bottomPolyData->SetPoints(bottomPoints);
    bottomPolyData->SetPolys(bottomCells);

    vtkSmartPointer<vtkPolyDataMapper> bottomMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    bottomMapper->SetInputData(bottomPolyData);

    channelBottomActor_->SetMapper(bottomMapper);
    channelBottomActor_->GetProperty()->SetColor(0.35, 0.30, 0.25);
    channelBottomActor_->GetProperty()->SetAmbient(0.3);
    channelBottomActor_->GetProperty()->SetDiffuse(0.7);
    channelBottomActor_->SetVisibility(1);

    renderer_->AddActor(channelBottomActor_);

    // ===== CREATE CHANNEL WALLS =====
    vtkSmartPointer<vtkPoints> wallPoints = vtkSmartPointer<vtkPoints>::New();

    wallPoints->InsertNextPoint(0.0, 0.0, 0.0);
    wallPoints->InsertNextPoint(length, 0.0, 0.0);
    wallPoints->InsertNextPoint(length, 0.0, width);
    wallPoints->InsertNextPoint(0.0, 0.0, width);

    wallPoints->InsertNextPoint(0.0, channelDepth, 0.0);
    wallPoints->InsertNextPoint(length, channelDepth, 0.0);
    wallPoints->InsertNextPoint(length, channelDepth, width);
    wallPoints->InsertNextPoint(0.0, channelDepth, width);

    vtkSmartPointer<vtkCellArray> wallCells = vtkSmartPointer<vtkCellArray>::New();

    vtkIdType leftWall[4] = {0, 1, 5, 4};
    wallCells->InsertNextCell(4, leftWall);

    vtkIdType rightWall[4] = {3, 2, 6, 7};
    wallCells->InsertNextCell(4, rightWall);

    vtkSmartPointer<vtkPolyData> wallPolyData = vtkSmartPointer<vtkPolyData>::New();
    wallPolyData->SetPoints(wallPoints);
    wallPolyData->SetPolys(wallCells);

    vtkSmartPointer<vtkPolyDataMapper> wallMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    wallMapper->SetInputData(wallPolyData);

    channelWallsActor_->SetMapper(wallMapper);
    channelWallsActor_->GetProperty()->SetColor(0.50, 0.50, 0.50);
    channelWallsActor_->GetProperty()->SetAmbient(0.3);
    channelWallsActor_->GetProperty()->SetDiffuse(0.7);
    channelWallsActor_->SetVisibility(1);

    renderer_->AddActor(channelWallsActor_);

    // ===== CREATE WATER SURFACE =====
    vtkSmartPointer<vtkPoints> waterPoints = vtkSmartPointer<vtkPoints>::New();

    double waterLevel = normalDepth;
    waterPoints->InsertNextPoint(0.0, waterLevel, 0.0);
    waterPoints->InsertNextPoint(length, waterLevel, 0.0);
    waterPoints->InsertNextPoint(length, waterLevel, width);
    waterPoints->InsertNextPoint(0.0, waterLevel, width);

    vtkSmartPointer<vtkCellArray> waterCells = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType waterFace[4] = {0, 1, 2, 3};
    waterCells->InsertNextCell(4, waterFace);

    vtkSmartPointer<vtkPolyData> waterPolyData = vtkSmartPointer<vtkPolyData>::New();
    waterPolyData->SetPoints(waterPoints);
    waterPolyData->SetPolys(waterCells);

    vtkSmartPointer<vtkPolyDataMapper> waterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    waterMapper->SetInputData(waterPolyData);

    waterActor_->SetMapper(waterMapper);
    waterActor_->GetProperty()->SetColor(0.1, 0.5, 0.9);
    waterActor_->GetProperty()->SetOpacity(0.65);
    waterActor_->GetProperty()->SetAmbient(0.2);
    waterActor_->GetProperty()->SetDiffuse(0.6);
    waterActor_->GetProperty()->SetSpecular(0.4);
    waterActor_->GetProperty()->SetSpecularPower(20);
    waterActor_->SetVisibility(1);

    renderer_->AddActor(waterActor_);

    // ===== SETUP CAMERA =====
    renderer_->ResetCamera();

    vtkCamera* camera = renderer_->GetActiveCamera();

    focalPointX_ = length / 2.0;
    focalPointY_ = channelDepth / 2.0;
    focalPointZ_ = width / 2.0;

    camera->SetFocalPoint(focalPointX_, focalPointY_, focalPointZ_);

    double maxDimension = std::max({length, channelDepth, width});
    viewDistance_ = maxDimension * 2.5;

    double azimuth = 40.0;
    double elevation = 25.0;
    double azimuthRad = azimuth * 3.14159265359 / 180.0;
    double elevationRad = elevation * 3.14159265359 / 180.0;

    double posX = focalPointX_ + viewDistance_ * cos(elevationRad) * cos(azimuthRad);
    double posY = focalPointY_ + viewDistance_ * sin(elevationRad);
    double posZ = focalPointZ_ + viewDistance_ * cos(elevationRad) * sin(azimuthRad);

    camera->SetPosition(posX, posY, posZ);
    camera->SetViewUp(0.0, 1.0, 0.0);

    camera->SetParallelScale(maxDimension * 0.6);

    renderer_->ResetCameraClippingRange();

    renderWindow_->Render();
}

void VtkWidget::set_camera_view(double posX, double posY, double posZ,
                                double upX, double upY, double upZ)
{
    vtkCamera* camera = renderer_->GetActiveCamera();

    camera->SetPosition(focalPointX_ + posX * viewDistance_,
                        focalPointY_ + posY * viewDistance_,
                        focalPointZ_ + posZ * viewDistance_);
    camera->SetFocalPoint(focalPointX_, focalPointY_, focalPointZ_);
    camera->SetViewUp(upX, upY, upZ);

    renderer_->ResetCameraClippingRange();
    renderWindow_->Render();
}

void VtkWidget::set_view_top()
{
    set_camera_view(0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

void VtkWidget::set_view_front()
{
    set_camera_view(0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
}

void VtkWidget::set_view_right()
{
    set_camera_view(1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void VtkWidget::set_view_isometric()
{
    double dist = 1.0 / std::sqrt(3.0);
    set_camera_view(dist, dist, dist, 0.0, 1.0, 0.0);
}

void VtkWidget::reset_view()
{
    double azimuth = 40.0;
    double elevation = 25.0;
    double azimuthRad = azimuth * 3.14159265359 / 180.0;
    double elevationRad = elevation * 3.14159265359 / 180.0;

    double dx = cos(elevationRad) * cos(azimuthRad);
    double dy = sin(elevationRad);
    double dz = cos(elevationRad) * sin(azimuthRad);

    set_camera_view(dx, dy, dz, 0.0, 1.0, 0.0);
}
