#include <gtest/gtest.h>
#include "Analyzer.h"
#include "RectangularChannel.h"
#include "Flow.h"

TEST(AnalyzerSolvingForDepth, GivenRectangularChannelWithKnownSolution_WhenSolvingForDepth_ExpectCorrectDepth)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(1.736, result.normalDepth, 0.001);
}
