#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "WorkflowController.h"
#include "VisualizationPanel.h"
#include "ParameterPanel.h"

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

    Ui::MainWindow* ui;

    QSplitter* mainSplitter_;
    VisualizationPanel* visualizationPanel_;
    ParameterPanel* parameterPanel_;
    QLabel* unitSystemIndicator_;

    QMenu* fileMenu_;
    QAction* newProjectAction_;
    QAction* openProjectAction_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    QAction* exitAction_;

    WorkflowController* workflowController_;
};

#endif // MAINWINDOW_H
