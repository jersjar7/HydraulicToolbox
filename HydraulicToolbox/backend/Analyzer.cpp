#include "Analyzer.h"
#include "Channel.h"
#include "Flow.h"
#include <cmath>

AnalysisResult Analyzer::solve_for_depth(Channel& channel, const Flow& flow, double slope, double unitsCoefficient) const
{
    AnalysisResult result;

    if (!flow.is_valid() || slope <= 0.0)
    {
        return result;
    }

    double targetDischarge{flow.get_discharge()};
    double manningN{flow.get_manning_n()};

    double minDepth{0.001};
    double maxDepth{1000.0};
    double tolerance{0.001};
    int maxIterations{100};

    for (int i = 0; i < maxIterations; ++i)
    {
        double midDepth = (minDepth + maxDepth) / 2.0;
        channel.set_depth(midDepth);

        double area{channel.calculate_area()};
        double hydraulicRadius{channel.calculate_hydraulic_radius()};
        double calculatedDischarge = (unitsCoefficient / manningN) * area * std::pow(hydraulicRadius, 2.0/3.0) * std::sqrt(slope);

        if (std::abs(calculatedDischarge - targetDischarge) < tolerance)
        {
            result.normalDepth = midDepth;
            result.velocity = targetDischarge / area;

            double topWidth = channel.calculate_top_width();
            double hydraulicDepth = area / topWidth;
            double gravity = 9.81;
            result.froudeNumber = result.velocity / std::sqrt(gravity * hydraulicDepth);

            if (result.froudeNumber < 0.99)
                result.flowRegime = FlowRegime::Subcritical;
            else if (result.froudeNumber > 1.01)
                result.flowRegime = FlowRegime::Supercritical;
            else
                result.flowRegime = FlowRegime::Critical;

            result.isValid = true;
            return result;
        }

        if (calculatedDischarge < targetDischarge)
            minDepth = midDepth;
        else
            maxDepth = midDepth;
    }

    return result;
}
