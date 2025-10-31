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
#include <QPushButton>
#include "ProjectSetupWidget.h"
#include "GeometryDefinitionWidget.h"
#include "HydraulicParametersWidget.h"
#include "AnalysisResultsWidget.h"
#include "WorkflowController.h"
#include "WorkflowTabBar.h"
#include "ExportWidget.h"
#include "VtkWidget.h"
#include <QResizeEvent>

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
    void resizeEvent(QResizeEvent* event) override;

private:
    void setup_ui();
    void setup_menu_bar();
    void setup_layout();
    void setup_view_controls();
    void apply_dark_theme();
    void update_unit_system_indicator();
    void adjust_parameter_panel_height(WorkflowStage stage);

    bool visualizationRendered_;

    Ui::MainWindow* ui;

    QSplitter* mainSplitter_;
    QWidget* visualizationArea_;
    VtkWidget* vtkWidget_;
    QWidget* parameterPanel_;
    QLabel* unitSystemIndicator_;
    QWidget* viewControlsContainer_;

    QMenu* fileMenu_;
    QAction* newProjectAction_;
    QAction* openProjectAction_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    QAction* exitAction_;

    WorkflowController* workflowController_;
    WorkflowTabBar* workflowTabBar_;
    QStackedWidget* stageStackedWidget_;

    ProjectSetupWidget* projectSetupWidget_;
    GeometryDefinitionWidget* geometryDefinitionWidget_;
    HydraulicParametersWidget* hydraulicParametersWidget_;
    AnalysisResultsWidget* analysisResultsWidget_;
    ExportWidget* exportWidget_;

    QPushButton* viewTopButton_;
    QPushButton* viewFrontButton_;
    QPushButton* viewRightButton_;
    QPushButton* viewIsoButton_;
    QPushButton* viewResetButton_;
};

#endif // MAINWINDOW_H
