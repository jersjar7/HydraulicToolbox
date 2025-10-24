#include "TriangularChannel.h"
#include <cmath>

TriangularChannel::TriangularChannel(double sideSlope, double depth)
    : sideSlope_{sideSlope}
    , depth_{depth}
{
}

double TriangularChannel::calculate_area() const
{
    return sideSlope_ * depth_ * depth_;
}

double TriangularChannel::calculate_wetted_perimeter() const
{
    return 2.0 * depth_ * std::sqrt(sideSlope_ * sideSlope_ + 1.0);
}

bool TriangularChannel::is_valid() const
{
    return sideSlope_ > 0.0 && depth_ > 0.0;
}

void TriangularChannel::set_depth(double depth)
{
    depth_ = depth;
}
