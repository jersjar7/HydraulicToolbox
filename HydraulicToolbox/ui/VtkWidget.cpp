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
#include <cmath>

VtkWidget::VtkWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
    , renderer_{vtkSmartPointer<vtkRenderer>::New()}
    , renderWindow_{vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()}
    , actor_{vtkSmartPointer<vtkActor>::New()}
    , interactor_{vtkSmartPointer<vtkRenderWindowInteractor>::New()}
    , cubeActor_{vtkSmartPointer<vtkAnnotatedCubeActor>::New()}
    , orientationWidget_{vtkSmartPointer<vtkOrientationMarkerWidget>::New()}
    , waterActor_{vtkSmartPointer<vtkActor>::New()}
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

    actor_->SetVisibility(0);

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

void VtkWidget::show_content()
{
    if (actor_)
    {
        actor_->SetVisibility(1);
    }
    renderWindow_->Render();
}

void VtkWidget::hide_content()
{
    if (actor_)
    {
        actor_->SetVisibility(0);
    }
    renderWindow_->Render();
}

void VtkWidget::render_channel(const GeometryData& geometry, const CalculationResults& results)
{
    double width = geometry.bottomWidth;
    double normalDepth = results.normalDepth;
    double channelDepth = normalDepth * 1.2;
    double length = width * 10.0;

    // ===== CREATE CHANNEL GEOMETRY =====
    vtkSmartPointer<vtkPoints> channelPoints = vtkSmartPointer<vtkPoints>::New();

    // Bottom corners (Y = 0)
    channelPoints->InsertNextPoint(0.0, 0.0, 0.0);        // 0
    channelPoints->InsertNextPoint(length, 0.0, 0.0);     // 1
    channelPoints->InsertNextPoint(length, 0.0, width);   // 2
    channelPoints->InsertNextPoint(0.0, 0.0, width);      // 3

    // Top corners (Y = channelDepth)
    channelPoints->InsertNextPoint(0.0, channelDepth, 0.0);        // 4
    channelPoints->InsertNextPoint(length, channelDepth, 0.0);     // 5
    channelPoints->InsertNextPoint(length, channelDepth, width);   // 6
    channelPoints->InsertNextPoint(0.0, channelDepth, width);      // 7

    vtkSmartPointer<vtkCellArray> channelCells = vtkSmartPointer<vtkCellArray>::New();

    // Bottom face (darker - sediment/concrete)
    vtkIdType bottom[4] = {0, 1, 2, 3};
    channelCells->InsertNextCell(4, bottom);

    // Left wall
    vtkIdType leftWall[4] = {0, 1, 5, 4};
    channelCells->InsertNextCell(4, leftWall);

    // Right wall
    vtkIdType rightWall[4] = {3, 2, 6, 7};
    channelCells->InsertNextCell(4, rightWall);

    vtkSmartPointer<vtkPolyData> channelPolyData = vtkSmartPointer<vtkPolyData>::New();
    channelPolyData->SetPoints(channelPoints);
    channelPolyData->SetPolys(channelCells);

    vtkSmartPointer<vtkPolyDataMapper> channelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    channelMapper->SetInputData(channelPolyData);

    actor_->SetMapper(channelMapper);
    actor_->GetProperty()->SetColor(0.5, 0.5, 0.5);  // Gray walls
    actor_->SetVisibility(1);

    renderer_->AddActor(actor_);

    // ===== CREATE WATER SURFACE =====
    vtkSmartPointer<vtkPoints> waterPoints = vtkSmartPointer<vtkPoints>::New();

    // Water surface at normal depth
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
    waterActor_->GetProperty()->SetColor(0.0, 0.4, 0.8);  // Blue water
    waterActor_->GetProperty()->SetOpacity(0.6);           // 60% opaque (40% transparent)
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
