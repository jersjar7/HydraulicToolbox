#include "WorkflowController.h"

WorkflowController::WorkflowController(QObject* parent)
    : QObject(parent)
    , currentStage_{WorkflowStage::ProjectSetup}
    , stageComplete_{false, false, false, false, false}
    , projectData_{}
    , geometryData_{}
    , hydraulicData_{}
    , calculationResults_{}
    , calculator_{}
{
}

WorkflowStage WorkflowController::get_current_stage() const
{
    return currentStage_;
}

bool WorkflowController::is_stage_complete(WorkflowStage stage) const
{
    int index = static_cast<int>(stage);
    return stageComplete_[index];
}

bool WorkflowController::can_advance_to_stage(WorkflowStage stage) const
{
    int targetIndex = static_cast<int>(stage);

    if(targetIndex == 0)
        return true;

    for(int i = 0; i < targetIndex; ++i)
    {
        if(!stageComplete_[i])
            return false;
    }

    return true;
}

void WorkflowController::set_current_stage(WorkflowStage stage)
{
    if(currentStage_ != stage && can_advance_to_stage(stage))
    {
        currentStage_ = stage;
        emit current_stage_changed(stage);
    }
}

void WorkflowController::mark_stage_complete(WorkflowStage stage, bool complete)
{
    int index = static_cast<int>(stage);

    if(stageComplete_[index] != complete)
    {
        stageComplete_[index] = complete;
        emit stage_completion_changed(stage, complete);
    }
}

ProjectData& WorkflowController::get_project_data()
{
    return projectData_;
}

GeometryData& WorkflowController::get_geometry_data()
{
    return geometryData_;
}

HydraulicData& WorkflowController::get_hydraulic_data()
{
    return hydraulicData_;
}

void WorkflowController::perform_calculation()
{
    calculationResults_ = calculator_.calculate(projectData_, geometryData_, hydraulicData_);
    emit calculation_completed(calculationResults_);
}

CalculationResults WorkflowController::get_calculation_results() const
{
    return calculationResults_;
}

bool WorkflowController::has_any_data_entered() const
{
    // Check if geometry data has been entered
    if(!geometryData_.channelType.isEmpty() ||
        geometryData_.bottomWidth != 0.0 ||
        geometryData_.sideSlope != 0.0 ||
        geometryData_.bedSlope != 0.0)
    {
        return true;
    }

    // Check if hydraulic data has been entered
    if(hydraulicData_.discharge != 0.0 ||
        hydraulicData_.manningN != 0.0)
    {
        return true;
    }

    return false;
}

void WorkflowController::clear_all_data()
{
    // Clear geometry data
    geometryData_ = GeometryData{};

    // Clear hydraulic data
    hydraulicData_ = HydraulicData{};

    // Clear calculation results
    calculationResults_ = CalculationResults{};

    // Mark stages as incomplete (except project setup)
    stageComplete_[1] = false;  // Geometry
    stageComplete_[2] = false;  // Hydraulic
    stageComplete_[3] = false;  // Analysis
    stageComplete_[4] = false;  // Export

    // Emit signals to update UI
    for(int i = 1; i < 5; ++i)
    {
        emit stage_completion_changed(static_cast<WorkflowStage>(i), false);
    }

    // Return to geometry definition stage
    set_current_stage(WorkflowStage::GeometryDefinition);
}
