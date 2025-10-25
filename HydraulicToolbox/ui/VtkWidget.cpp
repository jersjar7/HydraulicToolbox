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

VtkWidget::VtkWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
    , renderer_{vtkSmartPointer<vtkRenderer>::New()}
    , renderWindow_{vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()}
    , actor_{vtkSmartPointer<vtkActor>::New()}
    , interactor_{vtkSmartPointer<vtkRenderWindowInteractor>::New()}
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

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    points->InsertNextPoint(0.0, 0.0, 0.0);
    points->InsertNextPoint(length, 0.0, 0.0);
    points->InsertNextPoint(length, 0.0, width);
    points->InsertNextPoint(0.0, 0.0, width);

    points->InsertNextPoint(0.0, channelDepth, 0.0);
    points->InsertNextPoint(length, channelDepth, 0.0);
    points->InsertNextPoint(length, channelDepth, width);
    points->InsertNextPoint(0.0, channelDepth, width);

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

    vtkIdType bottom[4] = {0, 1, 2, 3};
    cells->InsertNextCell(4, bottom);

    vtkIdType leftWall[4] = {0, 1, 5, 4};
    cells->InsertNextCell(4, leftWall);

    vtkIdType rightWall[4] = {3, 2, 6, 7};
    cells->InsertNextCell(4, rightWall);

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetPolys(cells);

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);

    actor_->SetMapper(mapper);
    actor_->GetProperty()->SetColor(0.6, 0.6, 0.6);
    actor_->SetVisibility(1);

    renderer_->AddActor(actor_);

    renderWindow_->Render();

    renderer_->ResetCamera();

    // Adjust for nice 3D perspective
    vtkCamera* camera = renderer_->GetActiveCamera();
    camera->Azimuth(40);
    camera->Elevation(25);

    renderer_->ResetCameraClippingRange();

    // Final render with correct camera
    renderWindow_->Render();
}
