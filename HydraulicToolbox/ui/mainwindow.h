#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QWidget>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include "ProjectSetupWidget.h"
#include "GeometryDefinitionWidget.h"
#include "HydraulicParametersWidget.h"
#include "AnalysisResultsWidget.h"
#include "WorkflowController.h"
#include "WorkflowTabBar.h"
#include "ExportWidget.h"
#include "VtkWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tab_clicked(WorkflowStage stage);
    void on_project_setup_data_changed();
    void on_geometry_data_changed();
    void on_hydraulic_parameters_data_changed();
    void on_current_stage_changed(WorkflowStage newStage);
    void on_calculation_completed(const CalculationResults& results);

private:
    void setup_ui();
    void setup_menu_bar();
    void setup_layout();
    void apply_dark_theme();
    void update_unit_system_indicator();

    bool visualizationRendered_;

    Ui::MainWindow* ui;

    // Layout components
    QSplitter* mainSplitter_;
    QWidget* visualizationArea_;
    VtkWidget* vtkWidget_;
    QWidget* parameterPanel_;
    QLabel* unitSystemIndicator_;

    // Menu components
    QMenu* fileMenu_;
    QAction* newProjectAction_;
    QAction* openProjectAction_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    QAction* exitAction_;

    // Workflow components
    WorkflowController* workflowController_;
    WorkflowTabBar* workflowTabBar_;
    QStackedWidget* stageStackedWidget_;

    // Workflow widgets
    ProjectSetupWidget* projectSetupWidget_;
    GeometryDefinitionWidget* geometryDefinitionWidget_;
    HydraulicParametersWidget* hydraulicParametersWidget_;
    AnalysisResultsWidget* analysisResultsWidget_;
    ExportWidget* exportWidget_;

};

#endif // MAINWINDOW_H
