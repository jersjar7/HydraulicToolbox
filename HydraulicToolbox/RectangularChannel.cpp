#include "RectangularChannel.h"

RectangularChannel::RectangularChannel(double width, double depth)
    : width_{width}
    , depth_{depth}
{
}

double RectangularChannel::calculate_area() const
{
    return width_ * depth_;
}

double RectangularChannel::calculate_wetted_perimeter() const
{
    return width_ + 2.0 * depth_;
}

bool RectangularChannel::is_valid() const
{
    return width_ > 0.0 && depth_ > 0.0;
}

void RectangularChannel::set_depth(double depth)
{
    depth_ = depth;
}
