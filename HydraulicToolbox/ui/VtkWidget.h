#ifndef VTKWIDGET_H
#define VTKWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkAnnotatedCubeActor.h>
#include "../ui/ProjectDataStructures.h"
#include "../backend/HydraulicCalculator.h"
#include <vtkRenderWindowInteractor.h>

class VtkWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
    explicit VtkWidget(QWidget* parent = nullptr);
    ~VtkWidget();

public slots:
    void show_content();
    void hide_content();
    void render_channel(const GeometryData& geometry, const CalculationResults& results);

    void set_view_top();
    void set_view_front();
    void set_view_right();
    void set_view_isometric();
    void reset_view();

private:
    void setup_vtk_pipeline();
    void setup_camera();
    void setup_orientation_marker();
    void set_camera_view(double posX, double posY, double posZ,
                         double upX, double upY, double upZ);

    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow_;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor_;
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkActor> waterActor_;
    vtkSmartPointer<vtkAnnotatedCubeActor> cubeActor_;
    vtkSmartPointer<vtkOrientationMarkerWidget> orientationWidget_;

    double focalPointX_;
    double focalPointY_;
    double focalPointZ_;
    double viewDistance_;
};

#endif // VTKWIDGET_H
