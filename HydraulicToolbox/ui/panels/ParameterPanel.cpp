#include "ParameterPanel.h"
#include <QtWidgets/qwidget.h>

ParameterPanel::ParameterPanel(WorkflowController* controller, QWidget* parent)
    : QWidget(parent)
    , workflowController_{controller}
    , stageStackedWidget_{nullptr}
    , workflowTabBar_{nullptr}
    , projectSetupWidget_{nullptr}
    , geometryDefinitionWidget_{nullptr}
    , hydraulicParametersWidget_{nullptr}
    , analysisResultsWidget_{nullptr}
    , exportWidget_{nullptr}
{
    setup_ui();
    apply_styling();
}

ParameterPanel::~ParameterPanel()
{
}

void ParameterPanel::setup_ui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    stageStackedWidget_ = new QStackedWidget(this);

    projectSetupWidget_ = new ProjectSetupWidget();
    geometryDefinitionWidget_ = new GeometryDefinitionWidget();
    hydraulicParametersWidget_ = new HydraulicParametersWidget();
    analysisResultsWidget_ = new AnalysisResultsWidget();
    exportWidget_ = new ExportWidget();

    stageStackedWidget_->addWidget(projectSetupWidget_);
    stageStackedWidget_->addWidget(geometryDefinitionWidget_);
    stageStackedWidget_->addWidget(hydraulicParametersWidget_);
    stageStackedWidget_->addWidget(analysisResultsWidget_);
    stageStackedWidget_->addWidget(exportWidget_);

    mainLayout->addWidget(stageStackedWidget_);

    workflowTabBar_ = new WorkflowTabBar(workflowController_, this);
    mainLayout->addWidget(workflowTabBar_);

    connect(workflowTabBar_, &WorkflowTabBar::tab_clicked,
            this, &ParameterPanel::tab_clicked);
}

void ParameterPanel::apply_styling()
{
    setStyleSheet("QWidget { background-color: #3c3c3c; }");
}

void ParameterPanel::set_current_stage(WorkflowStage stage)
{
    int index = static_cast<int>(stage);

    if(index < stageStackedWidget_->count())
    {
        stageStackedWidget_->setCurrentIndex(index);
    }

    adjust_height_for_stage(stage);
}

void ParameterPanel::adjust_height_for_stage(WorkflowStage stage)
{
    int preferredHeight = 200;

    switch(stage)
    {
    case WorkflowStage::ProjectSetup:
        preferredHeight = 270;
        break;
    case WorkflowStage::GeometryDefinition:
        preferredHeight = 320;
        break;
    case WorkflowStage::HydraulicParameters:
        preferredHeight = 380;
        break;
    case WorkflowStage::AnalysisResults:
        preferredHeight = 250;
        break;
    case WorkflowStage::Export:
        preferredHeight = 400;
        break;
    }

    setMinimumHeight(preferredHeight);

    setMaximumHeight(500);
}

ProjectSetupWidget* ParameterPanel::get_project_setup_widget()
{
    return projectSetupWidget_;
}

GeometryDefinitionWidget* ParameterPanel::get_geometry_definition_widget()
{
    return geometryDefinitionWidget_;
}

HydraulicParametersWidget* ParameterPanel::get_hydraulic_parameters_widget()
{
    return hydraulicParametersWidget_;
}

AnalysisResultsWidget* ParameterPanel::get_analysis_results_widget()
{
    return analysisResultsWidget_;
}

ExportWidget* ParameterPanel::get_export_widget()
{
    return exportWidget_;
}
