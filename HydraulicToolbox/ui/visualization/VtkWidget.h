#ifndef VTKWIDGET_H
#define VTKWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <QTimer>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkRenderWindowInteractor.h>
#include <memory>
#include "ProjectDataStructures.h"
#include "../backend/HydraulicCalculator.h"
#include "renderers/ChannelRenderer.h"
#include "animation/WaterFlowAnimator.h"

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

    void start_water_animation();
    void stop_water_animation();

private slots:
    void update_animation();

private:
    void setup_vtk_pipeline();
    void setup_camera();
    void setup_orientation_marker();
    void setup_lighting();
    void set_camera_view(double posX, double posY, double posZ,
                         double upX, double upY, double upZ);

    std::unique_ptr<ChannelRenderer> create_renderer(const QString& channelType);
    void setup_camera_for_geometry(const GeometryData& geometry, const CalculationResults& results);

    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow_;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor_;
    vtkSmartPointer<vtkActor> channelBottomActor_;
    vtkSmartPointer<vtkActor> channelWallsActor_;
    vtkSmartPointer<vtkActor> waterActor_;
    vtkSmartPointer<vtkActor> particleActor_;
    vtkSmartPointer<vtkAnnotatedCubeActor> cubeActor_;
    vtkSmartPointer<vtkOrientationMarkerWidget> orientationWidget_;

    double focalPointX_;
    double focalPointY_;
    double focalPointZ_;
    double viewDistance_;

    QTimer* animationTimer_;
    std::unique_ptr<WaterFlowAnimator> waterFlowAnimator_;
    std::unique_ptr<ChannelRenderer> currentChannelRenderer_;
    GeometryData currentGeometry_;
    CalculationResults currentResults_;
};

#endif // VTKWIDGET_H
