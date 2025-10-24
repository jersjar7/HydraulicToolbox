#include "Flow.h"

Flow::Flow(double discharge, double manningN)
    : discharge_{discharge}
    , manningN_{manningN}
{
}

double Flow::get_discharge() const
{
    return discharge_;
}

double Flow::get_manning_n() const
{
    return manningN_;
}

bool Flow::is_valid() const
{
    return discharge_ > 0.0 && manningN_ > 0.0;
}
