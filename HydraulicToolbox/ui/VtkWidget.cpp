#include "VtkWidget.h"
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

VtkWidget::VtkWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
    , renderer_{vtkSmartPointer<vtkRenderer>::New()}
    , renderWindow_{vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()}
    , actor_{vtkSmartPointer<vtkActor>::New()}
{
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

    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetHeight(3.0);
    coneSource->SetRadius(1.0);
    coneSource->SetResolution(50);

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(coneSource->GetOutputPort());

    actor_->SetMapper(mapper);
    actor_->GetProperty()->SetColor(0.2, 0.6, 1.0);

    renderer_->AddActor(actor_);
    renderer_->ResetCamera();

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
