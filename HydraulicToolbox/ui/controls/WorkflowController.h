#ifndef WORKFLOWCONTROLLER_H
#define WORKFLOWCONTROLLER_H

#include <QObject>
#include <QString>
#include <array>
#include "../backend/HydraulicCalculator.h"
#include "ProjectDataStructures.h"

enum class WorkflowStage
{
    ProjectSetup = 0,
    GeometryDefinition = 1,
    HydraulicParameters = 2,
    AnalysisResults = 3,
    Export = 4
};

class WorkflowController : public QObject
{
    Q_OBJECT

public:
    explicit WorkflowController(QObject* parent = nullptr);

    WorkflowStage get_current_stage() const;
    bool is_stage_complete(WorkflowStage stage) const;
    bool can_advance_to_stage(WorkflowStage stage) const;

    void set_current_stage(WorkflowStage stage);
    void mark_stage_complete(WorkflowStage stage, bool complete);

    ProjectData& get_project_data();
    GeometryData& get_geometry_data();
    HydraulicData& get_hydraulic_data();

    void perform_calculation();
    CalculationResults get_calculation_results() const;

    // New methods for unit system change handling
    bool has_any_data_entered() const;
    void clear_all_data();

signals:
    void current_stage_changed(WorkflowStage newStage);
    void stage_completion_changed(WorkflowStage stage, bool complete);
    void calculation_completed(const CalculationResults& results);

private:
    WorkflowStage currentStage_;
    std::array<bool, 5> stageComplete_;

    ProjectData projectData_;
    GeometryData geometryData_;
    HydraulicData hydraulicData_;

    CalculationResults calculationResults_;
    HydraulicCalculator calculator_;
};

#endif // WORKFLOWCONTROLLER_H
