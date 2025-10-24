#include "TriangularChannel.h"
#include <gtest/gtest.h>

TEST(TriangularChannelGeometry, GivenSideSlopeAndDepth_WhenCalculatingArea_ExpectCorrectValue)
{
    double sideSlope{2.0};
    double depth{3.0};

    TriangularChannel channel{sideSlope, depth};

    double expectedArea{18.0};
    EXPECT_DOUBLE_EQ(expectedArea, channel.calculate_area());
}

TEST(TriangularChannelGeometry, GivenSideSlopeAndDepth_WhenCalculatingWettedPerimeter_ExpectCorrectValue)
{
    double sideSlope{2.0};
    double depth{3.0};

    TriangularChannel channel{sideSlope, depth};

    double expectedWettedPerimeter{13.416407864998739};
    EXPECT_DOUBLE_EQ(expectedWettedPerimeter, channel.calculate_wetted_perimeter());
}

TEST(TriangularChannelGeometry, GivenSideSlopeAndDepth_WhenCalculatingHydraulicRadius_ExpectCorrectValue)
{
    double sideSlope{2.0};
    double depth{3.0};

    TriangularChannel channel{sideSlope, depth};

    double expectedHydraulicRadius{1.3416407864998738};
    EXPECT_DOUBLE_EQ(expectedHydraulicRadius, channel.calculate_hydraulic_radius());
}

TEST(TriangularChannelGeometry, GivenNegativeDepth_WhenCreatingChannel_ExpectInvalidConfiguration)
{
    double sideSlope{2.0};
    double depth{-1.0};

    TriangularChannel channel{sideSlope, depth};

    EXPECT_FALSE(channel.is_valid());
}

TEST(TriangularChannelGeometry, GivenZeroSideSlope_WhenCreatingChannel_ExpectInvalidConfiguration)
{
    double sideSlope{0.0};
    double depth{3.0};

    TriangularChannel channel{sideSlope, depth};

    EXPECT_FALSE(channel.is_valid());
}

