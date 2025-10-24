#include <gtest/gtest.h>
#include "Flow.h"

TEST(FlowParameters, GivenDischargeAndManningN_WhenCreatingFlow_ExpectCorrectValues)
{
    double discharge{100.0};
    double manningN{0.015};

    Flow flow{discharge, manningN};

    EXPECT_DOUBLE_EQ(discharge, flow.get_discharge());
    EXPECT_DOUBLE_EQ(manningN, flow.get_manning_n());
}

TEST(FlowParameters, GivenZeroDischarge_WhenCreatingFlow_ExpectInvalidConfiguration)
{
    double discharge{0.0};
    double manningN{0.015};

    Flow flow{discharge, manningN};

    EXPECT_FALSE(flow.is_valid());
}

TEST(FlowParameters, GivenNegativeDischarge_WhenCreatingFlow_ExpectInvalidConfiguration)
{
    double discharge{-100.0};
    double manningN{0.015};

    Flow flow{discharge, manningN};

    EXPECT_FALSE(flow.is_valid());
}

TEST(FlowParameters, GivenZeroManningN_WhenCreatingFlow_ExpectInvalidConfiguration)
{
    double discharge{100.0};
    double manningN{0.0};

    Flow flow{discharge, manningN};

    EXPECT_FALSE(flow.is_valid());
}

TEST(FlowParameters, GivenNegativeManningN_WhenCreatingFlow_ExpectInvalidConfiguration)
{
    double discharge{100.0};
    double manningN{-0.015};

    Flow flow{discharge, manningN};

    EXPECT_FALSE(flow.is_valid());
}
