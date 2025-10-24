#include <gtest/gtest.h>
#include "RectangularChannel.h"

TEST(RectangularChannelGeometry, GivenWidthAndDepth_WhenCalculatingArea_ExpectCorrectValue)
{
    double width{5.0};
    double depth{2.0};

    RectangularChannel channel{width, depth};

    double expectedArea{10.0};
    EXPECT_DOUBLE_EQ(expectedArea, channel.calculate_area());
}

TEST(RectangularChannelGeometry, GivenWidthAndDepth_WhenCalculatingWettedPerimeter_ExpectCorrectValue)
{
    double width{5.0};
    double depth{2.0};

    RectangularChannel channel{width, depth};

    double expectedWettedPerimeter{9.0};
    EXPECT_DOUBLE_EQ(expectedWettedPerimeter, channel.calculate_wetted_perimeter());
}

TEST(RectangularChannelGeometry, GivenWidthAndDepth_WhenCalculatingHydraulicRadius_ExpectCorrectValue)
{
    double width{5.0};
    double depth{2.0};

    RectangularChannel channel{width, depth};

    double expectedHydraulicRadius{10.0 / 9.0};
    EXPECT_DOUBLE_EQ(expectedHydraulicRadius, channel.calculate_hydraulic_radius());
}

TEST(RectangularChannelGeometry, GivenZeroWidth_WhenCreatingChannel_ExpectInvalidConfiguration)
{
    double width{0.0};
    double depth{2.0};

    RectangularChannel channel{width, depth};

    EXPECT_FALSE(channel.is_valid());
}

TEST(RectangularChannelGeometry, GivenNegativeDepth_WhenCreatingChannel_ExpectInvalidConfiguration)
{
    double width{5.0};
    double depth{-1.0};

    RectangularChannel channel{width, depth};

    EXPECT_FALSE(channel.is_valid());
}
