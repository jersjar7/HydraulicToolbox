#include "TrapezoidalChannel.h"
#include <cmath>

TrapezoidalChannel::TrapezoidalChannel(double bottomWidth, double sideSlope, double depth)
    : bottomWidth_{bottomWidth}
    , sideSlope_{sideSlope}
    , depth_{depth}
{
}

double TrapezoidalChannel::calculate_area() const
{
    return (bottomWidth_ + sideSlope_ * depth_) * depth_;
}

double TrapezoidalChannel::calculate_wetted_perimeter() const
{
    return bottomWidth_ + 2.0 * depth_ * std::sqrt(sideSlope_ * sideSlope_ + 1.0);
}

bool TrapezoidalChannel::is_valid() const
{
    return bottomWidth_ > 0.0 && sideSlope_ > 0.0 && depth_ > 0.0;
}

void TrapezoidalChannel::set_depth(double depth)
{
    depth_ = depth;
}
