#ifndef WORKFLOWCONTROLLER_H
#define WORKFLOWCONTROLLER_H

#include <QObject>
#include <QString>
#include <array>

enum class WorkflowStage
{
    ProjectSetup = 0,
    GeometryDefinition = 1,
    HydraulicParameters = 2,
    AnalysisResults = 3,
    Export = 4
};

struct ProjectData
{
    QString projectName;
    QString location;
    bool useUsCustomary{true};
};

struct GeometryData
{
    QString channelType;
    double bottomWidth{0.0};
    double depth{0.0};
    double sideSlope{0.0};
    double length{0.0};
    double bedSlope{0.0};
};

struct HydraulicData
{
    double discharge{0.0};
    double manningN{0.0};
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

signals:
    void current_stage_changed(WorkflowStage newStage);
    void stage_completion_changed(WorkflowStage stage, bool complete);

private:
    WorkflowStage currentStage_;
    std::array<bool, 5> stageComplete_;

    ProjectData projectData_;
    GeometryData geometryData_;
    HydraulicData hydraulicData_;
};

#endif // WORKFLOWCONTROLLER_H
