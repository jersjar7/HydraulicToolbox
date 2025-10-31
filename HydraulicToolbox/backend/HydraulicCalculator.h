#ifndef HYDRAULICCALCULATOR_H
#define HYDRAULICCALCULATOR_H

#include "Channel.h"
#include "Flow.h"
#include "Analyzer.h"
#include "ProjectDataStructures.h"
#include <memory>
#include <QString>

struct CalculationResults
{
    double normalDepth{0.0};
    double velocity{0.0};
    double froudeNumber{0.0};
    QString flowRegime;
    bool isValid{false};
    QString errorMessage;
};

class HydraulicCalculator
{
public:
    HydraulicCalculator();
    ~HydraulicCalculator();

    CalculationResults calculate(const ProjectData& projectData,
                                 const GeometryData& geometryData,
                                 const HydraulicData& hydraulicData);

private:
    std::unique_ptr<Channel> create_channel(const GeometryData& geometryData);
    Flow create_flow(const HydraulicData& hydraulicData);
    QString determine_flow_regime(FlowRegime regime) const;
    bool validate_inputs(const GeometryData& geometryData,
                         const HydraulicData& hydraulicData,
                         QString& errorMessage);
};

#endif // HYDRAULICCALCULATOR_H
