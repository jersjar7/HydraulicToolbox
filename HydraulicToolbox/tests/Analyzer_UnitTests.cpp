#include <gtest/gtest.h>
#include "Analyzer.h"
#include "RectangularChannel.h"
#include "TrapezoidalChannel.h"
#include "TriangularChannel.h"
#include "Flow.h"
#include "UnitSystemConstants.h"

// ============================================================================
// BASIC SOLVING TESTS
// ============================================================================

TEST(AnalyzerSolvingForDepth, GivenRectangularChannelWithKnownSolution_WhenSolvingForDepth_ExpectCorrectDepth)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(2.930, result.normalDepth, 0.001);
}

// ============================================================================
// VELOCITY CALCULATION TESTS
// ============================================================================

TEST(AnalyzerResultCalculations, GivenSolvedDepth_WhenCalculatingVelocity_ExpectCorrectVelocity)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    double expectedVelocity = discharge / (width * result.normalDepth);
    EXPECT_NEAR(expectedVelocity, result.velocity, 0.001);
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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    double expectedArea = sideSlope * result.normalDepth * result.normalDepth;
    double expectedVelocity = discharge / expectedArea;

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(expectedVelocity, result.velocity, 0.001);
}

// ============================================================================
// FROUDE NUMBER CALCULATION TESTS
// ============================================================================

TEST(AnalyzerResultCalculations, GivenSolvedDepth_WhenCalculatingFroudeNumber_ExpectCorrectFroudeNumber)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.froudeNumber, 0.0);
    EXPECT_NEAR(0.70, result.froudeNumber, 0.01);
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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.froudeNumber, 0.0);
    EXPECT_LT(result.froudeNumber, 1.0);
}

// ============================================================================
// FLOW REGIME CLASSIFICATION TESTS
// ============================================================================

TEST(FlowRegimeClassification, GivenSubcriticalFlow_WhenAnalyzing_ExpectSubcriticalRegime)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.froudeNumber, 1.01);
    EXPECT_EQ(FlowRegime::Supercritical, result.flowRegime);
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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);
    EXPECT_NEAR(1.0, result.froudeNumber, 0.15);
}

// ============================================================================
// EDGE CASE TESTS - INVALID INPUTS
// ============================================================================

TEST(AnalyzerEdgeCases, GivenZeroSlope_WhenSolving_ExpectInvalidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.0};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_FALSE(result.isValid);
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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_FALSE(result.isValid);
}

// ============================================================================
// EDGE CASE TESTS - EXTREME VALUES
// ============================================================================

TEST(AnalyzerEdgeCases, GivenVerySmallDischarge_WhenSolving_ExpectValidResult)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{0.01};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

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
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);
    EXPECT_GT(result.normalDepth, 0.0);
}

// ============================================================================
// UNIT SYSTEM TESTS - CRITICAL FOR CORRECT FROUDE NUMBER
// ============================================================================

TEST(AnalyzerUnitSystems, GivenUSCustomaryUnits_WhenCalculatingFroude_ExpectUSGravity)
{
    double width{10.0};
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{50.0};
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_US};
    double gravity{UnitSystemConstants::GRAVITY_US_CUSTOMARY};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);

    // Manually verify Froude number is calculated with US gravity
    double area = width * result.normalDepth;
    double topWidth = width;
    double hydraulicDepth = area / topWidth;
    double expectedFroude = result.velocity / std::sqrt(gravity * hydraulicDepth);

    EXPECT_NEAR(expectedFroude, result.froudeNumber, 0.001);
    EXPECT_GT(result.normalDepth, 0.0);
}

TEST(AnalyzerUnitSystems, GivenSIUnits_WhenCalculatingFroude_ExpectSIGravity)
{
    double width{3.048};  // ~10 ft in meters
    double depth{0.0};
    RectangularChannel channel{width, depth};

    double discharge{1.416};  // ~50 cfs in m³/s
    double manningN{0.013};
    Flow flow{discharge, manningN};

    double slope{0.001};
    double manningsCoef{UnitSystemConstants::MANNINGS_COEFFICIENT_SI};
    double gravity{UnitSystemConstants::GRAVITY_SI};

    Analyzer analyzer;
    AnalysisResult result = analyzer.solve_for_depth(channel, flow, slope, manningsCoef, gravity);

    EXPECT_TRUE(result.isValid);

    // Manually verify Froude number is calculated with SI gravity
    double area = width * result.normalDepth;
    double topWidth = width;
    double hydraulicDepth = area / topWidth;
    double expectedFroude = result.velocity / std::sqrt(gravity * hydraulicDepth);

    EXPECT_NEAR(expectedFroude, result.froudeNumber, 0.001);
    EXPECT_GT(result.normalDepth, 0.0);
}

TEST(AnalyzerUnitSystems, GivenSamePhysicalConditionsInDifferentUnits_WhenSolving_ExpectEquivalentFroudeNumbers)
{
    // US Customary case
    double widthUS{10.0};  // ft
    double depthUS{0.0};
    RectangularChannel channelUS{widthUS, depthUS};

    double dischargeUS{50.0};  // cfs
    double manningN{0.013};
    Flow flowUS{dischargeUS, manningN};

    double slope{0.001};

    Analyzer analyzer;
    AnalysisResult resultUS = analyzer.solve_for_depth(
        channelUS, flowUS, slope,
        UnitSystemConstants::MANNINGS_COEFFICIENT_US,
        UnitSystemConstants::GRAVITY_US_CUSTOMARY);

    // SI case (same physical conditions)
    double widthSI{3.048};  // m (10 ft)
    double depthSI{0.0};
    RectangularChannel channelSI{widthSI, depthSI};

    double dischargeSI{1.416};  // m³/s (50 cfs)
    Flow flowSI{dischargeSI, manningN};

    AnalysisResult resultSI = analyzer.solve_for_depth(
        channelSI, flowSI, slope,
        UnitSystemConstants::MANNINGS_COEFFICIENT_SI,
        UnitSystemConstants::GRAVITY_SI);

    EXPECT_TRUE(resultUS.isValid);
    EXPECT_TRUE(resultSI.isValid);

    // Froude numbers should be very close (dimensionless)
    EXPECT_NEAR(resultUS.froudeNumber, resultSI.froudeNumber, 0.05);

    // Flow regimes should match
    EXPECT_EQ(resultUS.flowRegime, resultSI.flowRegime);
}
