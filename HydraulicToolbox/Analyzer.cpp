#include "Analyzer.h"
#include "Channel.h"
#include "Flow.h"
#include <cmath>

AnalysisResult Analyzer::solve_for_depth(Channel& channel, const Flow& flow, double slope) const
{
    AnalysisResult result;

    if (!flow.is_valid() || slope <= 0.0)
    {
        return result;
    }

    double targetDischarge{flow.get_discharge()};
    double manningN{flow.get_manning_n()};

    double minDepth{0.001};
    double maxDepth{100.0};
    double tolerance{0.001};
    int maxIterations{100};

    for (int i = 0; i < maxIterations; ++i)
    {
        double midDepth = (minDepth + maxDepth) / 2.0;
        channel.set_depth(midDepth);

        double area{channel.calculate_area()};
        double hydraulicRadius{channel.calculate_hydraulic_radius()};
        double calculatedDischarge = (1.0 / manningN) * area * std::pow(hydraulicRadius, 2.0/3.0) * std::sqrt(slope);

        if (std::abs(calculatedDischarge - targetDischarge) < tolerance)
        {
            result.normalDepth = midDepth;
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
