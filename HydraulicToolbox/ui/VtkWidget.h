#ifndef VTKWIDGET_H
#define VTKWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkActor.h>

class VtkWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
    explicit VtkWidget(QWidget* parent = nullptr);
    ~VtkWidget();

public slots:
    void show_content();
    void hide_content();

private:
    void setup_vtk_pipeline();

    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow_;
    vtkSmartPointer<vtkActor> actor_;
};

#endif // VTKWIDGET_H
