#ifndef PARAMETERPANEL_H
#define PARAMETERPANEL_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "ProjectSetupWidget.h"
#include "GeometryDefinitionWidget.h"
#include "HydraulicParametersWidget.h"
#include "AnalysisResultsWidget.h"
#include "ExportWidget.h"
#include "WorkflowTabBar.h"
#include "WorkflowController.h"

class ParameterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterPanel(WorkflowController* controller, QWidget* parent = nullptr);
    ~ParameterPanel();

    void set_current_stage(WorkflowStage stage);
    void adjust_height_for_stage(WorkflowStage stage);

    ProjectSetupWidget* get_project_setup_widget();
    GeometryDefinitionWidget* get_geometry_definition_widget();
    HydraulicParametersWidget* get_hydraulic_parameters_widget();
    AnalysisResultsWidget* get_analysis_results_widget();
    ExportWidget* get_export_widget();

signals:
    void tab_clicked(WorkflowStage stage);

private:
    void setup_ui();
    void apply_styling();

    WorkflowController* workflowController_;
    QStackedWidget* stageStackedWidget_;
    WorkflowTabBar* workflowTabBar_;

    ProjectSetupWidget* projectSetupWidget_;
    GeometryDefinitionWidget* geometryDefinitionWidget_;
    HydraulicParametersWidget* hydraulicParametersWidget_;
    AnalysisResultsWidget* analysisResultsWidget_;
    ExportWidget* exportWidget_;
};

#endif // PARAMETERPANEL_H
