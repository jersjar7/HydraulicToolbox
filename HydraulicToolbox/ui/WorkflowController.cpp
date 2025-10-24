#include "WorkflowController.h"

WorkflowController::WorkflowController(QObject* parent)
    : QObject(parent)
    , currentStage_{WorkflowStage::ProjectSetup}
    , stageComplete_{false, false, false, false, false}
    , projectData_{}
    , geometryData_{}
    , hydraulicData_{}
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
