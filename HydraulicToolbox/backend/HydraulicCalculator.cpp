#include "HydraulicCalculator.h"
#include "RectangularChannel.h"
#include "TrapezoidalChannel.h"
#include "TriangularChannel.h"
#include "UnitSystemConstants.h"

HydraulicCalculator::HydraulicCalculator()
{
}

HydraulicCalculator::~HydraulicCalculator()
{
}

CalculationResults HydraulicCalculator::calculate(const ProjectData& projectData,
                                                  const GeometryData& geometryData,
                                                  const HydraulicData& hydraulicData)
{
    CalculationResults results;

    if(!validate_inputs(geometryData, hydraulicData, results.errorMessage))
    {
        results.isValid = false;
        return results;
    }

    try
    {
        std::unique_ptr<Channel> channel = create_channel(geometryData);

        if(!channel)
        {
            results.isValid = false;
            results.errorMessage = "Invalid channel type selected.";
            return results;
        }

        Flow flow = create_flow(hydraulicData);

        // Get correct constants based on unit system
        double manningsCoefficient = UnitSystemConstants::get_mannings_coefficient(projectData.useUsCustomary);
        double gravity = UnitSystemConstants::get_gravity(projectData.useUsCustomary);

        Analyzer analyzer;
        AnalysisResult backendResult = analyzer.solve_for_depth(*channel, flow, geometryData.bedSlope, manningsCoefficient, gravity);

        results.normalDepth = backendResult.normalDepth;
        results.velocity = backendResult.velocity;
        results.froudeNumber = backendResult.froudeNumber;
        results.flowRegime = determine_flow_regime(backendResult.flowRegime);
        results.isValid = backendResult.isValid;

        if(!results.isValid)
        {
            results.errorMessage = "Calculation failed to converge. Try adjusting input parameters.";
        }
    }
    catch(const std::exception& e)
    {
        results.isValid = false;
        results.errorMessage = QString("Calculation error: %1").arg(e.what());
    }

    return results;
}

std::unique_ptr<Channel> HydraulicCalculator::create_channel(const GeometryData& geometryData)
{
    double initialDepth = 1.0;

    if(geometryData.channelType == "Rectangular")
    {
        return std::make_unique<RectangularChannel>(geometryData.bottomWidth, initialDepth);
    }
    else if(geometryData.channelType == "Trapezoidal")
    {
        return std::make_unique<TrapezoidalChannel>(geometryData.bottomWidth, geometryData.sideSlope, initialDepth);
    }
    else if(geometryData.channelType == "Triangular")
    {
        return std::make_unique<TriangularChannel>(geometryData.sideSlope, initialDepth);
    }

    return nullptr;
}

Flow HydraulicCalculator::create_flow(const HydraulicData& hydraulicData)
{
    return Flow(hydraulicData.discharge, hydraulicData.manningN);
}

QString HydraulicCalculator::determine_flow_regime(FlowRegime regime) const
{
    switch(regime)
    {
    case FlowRegime::Subcritical:
        return "Subcritical";
    case FlowRegime::Critical:
        return "Critical";
    case FlowRegime::Supercritical:
        return "Supercritical";
    default:
        return "Unknown";
    }
}

bool HydraulicCalculator::validate_inputs(const GeometryData& geometryData,
                                          const HydraulicData& hydraulicData,
                                          QString& errorMessage)
{
    if(geometryData.bedSlope <= 0.0)
    {
        errorMessage = "Bed slope must be greater than zero.";
        return false;
    }

    if(geometryData.bedSlope > 1.0)
    {
        errorMessage = "Bed slope must be less than 1.0 (invalid slope).";
        return false;
    }

    if(hydraulicData.discharge <= 0.0)
    {
        errorMessage = "Discharge must be greater than zero.";
        return false;
    }

    if(hydraulicData.manningN <= 0.0 || hydraulicData.manningN > 0.2)
    {
        errorMessage = "Manning's n must be between 0 and 0.2.";
        return false;
    }

    if(geometryData.channelType == "Rectangular")
    {
        if(geometryData.bottomWidth <= 0.0)
        {
            errorMessage = "Bottom width must be greater than zero.";
            return false;
        }
    }
    else if(geometryData.channelType == "Trapezoidal")
    {
        if(geometryData.bottomWidth <= 0.0)
        {
            errorMessage = "Bottom width must be greater than zero.";
            return false;
        }
        if(geometryData.sideSlope <= 0.0)
        {
            errorMessage = "Side slope must be greater than zero.";
            return false;
        }
    }
    else if(geometryData.channelType == "Triangular")
    {
        if(geometryData.sideSlope <= 0.0)
        {
            errorMessage = "Side slope must be greater than zero.";
            return false;
        }
    }

    return true;
}
