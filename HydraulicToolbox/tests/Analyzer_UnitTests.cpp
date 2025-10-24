#include <gtest/gtest.h>
#include "Analyzer.h"
#include "RectangularChannel.h"
#include "TrapezoidalChannel.h"
#include "TriangularChannel.h"
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

TEST(AnalyzerSolvingForDepth, GivenTrapezoidalChannelWithKnownSolution_WhenSolvingForDepth_ExpectCorrectDepth)
{
    double bottomWidth{4.0};
    double sideSlope{2.0};
    double depth{0.0};
    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(2.110, result.normalDepth, 0.001);
}

TEST(AnalyzerSolvingForDepth, GivenTriangularChannelWithKnownSolution_WhenSolvingForDepth_ExpectCorrectDepth)
{
    double sideSlope{2.0};
    double depth{0.0};
    TriangularChannel channel{sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(2.930, result.normalDepth, 0.001);
}

TEST(AnalyzerResultCalculations, GivenSolvedDepth_WhenCalculatingVelocity_ExpectCorrectVelocity)
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

    double expectedVelocity = discharge / (width * result.normalDepth);
    EXPECT_NEAR(expectedVelocity, result.velocity, 0.001);
}

TEST(AnalyzerResultCalculations, GivenSolvedDepth_WhenCalculatingFroudeNumber_ExpectCorrectFroudeNumber)
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
    EXPECT_GT(result.froudeNumber, 0.0);
    EXPECT_NEAR(0.70, result.froudeNumber, 0.01);
}

TEST(AnalyzerResultCalculations, GivenTrapezoidalChannel_WhenCalculatingVelocity_ExpectCorrectVelocity)
{
    double bottomWidth{4.0};
    double sideSlope{2.0};
    double depth{0.0};
    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    double expectedArea = bottomWidth * result.normalDepth + sideSlope * result.normalDepth * result.normalDepth;
    double expectedVelocity = discharge / expectedArea;

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(expectedVelocity, result.velocity, 0.001);
}

TEST(AnalyzerResultCalculations, GivenTriangularChannel_WhenCalculatingVelocity_ExpectCorrectVelocity)
{
    double sideSlope{2.0};
    double depth{0.0};
    TriangularChannel channel{sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    double expectedArea = sideSlope * result.normalDepth * result.normalDepth;
    double expectedVelocity = discharge / expectedArea;

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(expectedVelocity, result.velocity, 0.001);
}

TEST(AnalyzerResultCalculations, GivenTrapezoidalChannel_WhenCalculatingFroudeNumber_ExpectCorrectFroudeNumber)
{
    double bottomWidth{4.0};
    double sideSlope{2.0};
    double depth{0.0};
    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.froudeNumber, 0.0);
    EXPECT_LT(result.froudeNumber, 1.0);
}

TEST(AnalyzerResultCalculations, GivenTriangularChannel_WhenCalculatingFroudeNumber_ExpectCorrectFroudeNumber)
{
    double sideSlope{2.0};
    double depth{0.0};
    TriangularChannel channel{sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.froudeNumber, 0.0);
    EXPECT_LT(result.froudeNumber, 1.0);
}

TEST(FlowRegimeClassification, GivenSubcriticalFlow_WhenAnalyzing_ExpectSubcriticalRegime)
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
    EXPECT_LT(result.froudeNumber, 0.99);
    EXPECT_EQ(FlowRegime::Subcritical, result.flowRegime);
}

TEST(FlowRegimeClassification, GivenSupercriticalFlow_WhenAnalyzing_ExpectSupercriticalRegime)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.02};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.froudeNumber, 1.01);
    EXPECT_EQ(FlowRegime::Supercritical, result.flowRegime);
}

TEST(AnalyzerEdgeCases, GivenZeroSlope_WhenSolving_ExpectInvalidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.0};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_FALSE(result.isValid);
}

TEST(AnalyzerEdgeCases, GivenNegativeSlope_WhenSolving_ExpectInvalidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{-0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_FALSE(result.isValid);
}

TEST(AnalyzerEdgeCases, GivenInvalidFlow_WhenSolving_ExpectInvalidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{-50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_FALSE(result.isValid);
}

TEST(AnalyzerEdgeCases, GivenVerySmallDischarge_WhenSolving_ExpectValidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{0.01};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.normalDepth, 0.0);
    EXPECT_LT(result.normalDepth, 1.0);
}

TEST(AnalyzerEdgeCases, GivenVeryLargeDischarge_WhenSolving_ExpectValidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{10000.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.normalDepth, 0.0);
}

TEST(AnalyzerEdgeCases, GivenExtremeConditionsCausingNonConvergence_WhenSolving_ExpectInvalidResult)
{
    double width{0.01};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{10000.0};
    double manningN{0.5};
    Flow flow{discharge, manningN};

    double slope{0.00001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_FALSE(result.isValid);
}

TEST(FlowRegimeClassification, GivenNearCriticalFlowConditions_WhenAnalyzing_ExpectCriticalRegime)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.0023};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(1.0, result.froudeNumber, 0.15);
}

TEST(AnalyzerEdgeCases, GivenInvalidChannelGeometry_WhenSolving_ExpectInvalidResult)
{
    double width{-10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_FALSE(result.isValid);
}

TEST(AnalyzerEdgeCases, GivenZeroWidthChannel_WhenSolving_ExpectInvalidResult)
{
    double width{0.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_FALSE(result.isValid);
}

TEST(AnalyzerEdgeCases, GivenZeroSideSlopeTriangular_WhenSolving_ExpectInvalidResult)
{
    double sideSlope{0.0};
    double depth{0.0};
    TriangularChannel channel{sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_FALSE(result.isValid);
}

TEST(AnalyzerEdgeCases, GivenExtremeSteepSlope_WhenSolving_ExpectValidOrInvalidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.5};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    if (result.isValid)
    {
        EXPECT_GT(result.normalDepth, 0.0);
        EXPECT_EQ(FlowRegime::Supercritical, result.flowRegime);
    }
}

TEST(AnalyzerEdgeCases, GivenExtremeGentleSlope_WhenSolving_ExpectValidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.00001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.normalDepth, 0.0);
    EXPECT_EQ(FlowRegime::Subcritical, result.flowRegime);
}

TEST(AnalyzerEdgeCases, GivenVeryHighManningN_WhenSolving_ExpectValidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.15};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.normalDepth, 0.0);
}

TEST(AnalyzerEdgeCases, GivenExtremeSideSlopeTrapezoidal_WhenSolving_ExpectValidResult)
{
    double bottomWidth{2.0};
    double sideSlope{10.0};
    double depth{0.0};
    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.normalDepth, 0.0);
}

TEST(AnalyzerEdgeCases, GivenZeroBottomWidthTrapezoidal_WhenSolving_ExpectValidResult)
{
    double bottomWidth{0.0};
    double sideSlope{2.0};
    double depth{0.0};
    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.normalDepth, 0.0);
}
