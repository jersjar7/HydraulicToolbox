#include <gtest/gtest.h>
#include "TrapezoidalChannel.h"

TEST(TrapezoidalChannelGeometry, GivenBottomWidthSideSlopeAndDepth_WhenCalculatingArea_ExpectCorrectValue)
{
    double bottomWidth{4.0};
    double sideSlope{2.0};
    double depth{3.0};

    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double expectedArea{30.0};
    EXPECT_DOUBLE_EQ(expectedArea, channel.calculate_area());
}

TEST(TrapezoidalChannelGeometry, GivenBottomWidthSideSlopeAndDepth_WhenCalculatingWettedPerimeter_ExpectCorrectValue)
{
    double bottomWidth{4.0};
    double sideSlope{2.0};
    double depth{3.0};

    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double expectedWettedPerimeter{17.416407864998739};
    EXPECT_DOUBLE_EQ(expectedWettedPerimeter, channel.calculate_wetted_perimeter());
}

TEST(TrapezoidalChannelGeometry, GivenBottomWidthSideSlopeAndDepth_WhenCalculatingHydraulicRadius_ExpectCorrectValue)
{
    double bottomWidth{4.0};
    double sideSlope{2.0};
    double depth{3.0};

    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    double expectedHydraulicRadius{1.7225136338412326};
    EXPECT_DOUBLE_EQ(expectedHydraulicRadius, channel.calculate_hydraulic_radius());
}

TEST(TrapezoidalChannelGeometry, GivenZeroBottomWidth_WhenCreatingChannel_ExpectInvalidConfiguration)
{
    double bottomWidth{0.0};
    double sideSlope{2.0};
    double depth{3.0};

    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    EXPECT_FALSE(channel.is_valid());
}

TEST(TrapezoidalChannelGeometry, GivenZeroSideSlope_WhenCreatingChannel_ExpectInvalidConfiguration)
{
    double bottomWidth{4.0};
    double sideSlope{0.0};
    double depth{3.0};

    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    EXPECT_FALSE(channel.is_valid());
}

TEST(TrapezoidalChannelGeometry, GivenNegativeDepth_WhenCreatingChannel_ExpectInvalidConfiguration)
{
    double bottomWidth{4.0};
    double sideSlope{2.0};
    double depth{-1.0};

    TrapezoidalChannel channel{bottomWidth, sideSlope, depth};

    EXPECT_FALSE(channel.is_valid());
}
