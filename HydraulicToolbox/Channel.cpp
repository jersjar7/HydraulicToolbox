#include "Channel.h"

double Channel::calculate_hydraulic_radius() const
{
    return calculate_area() / calculate_wetted_perimeter();
}
