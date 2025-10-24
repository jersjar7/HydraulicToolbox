#ifndef TRAPEZOIDALCHANNEL_H
#define TRAPEZOIDALCHANNEL_H

#include "Channel.h"

class TrapezoidalChannel : public Channel
{
public:
    TrapezoidalChannel(double bottomWidth, double sideSlope, double depth);

    double calculate_area() const override;
    double calculate_wetted_perimeter() const override;
    bool is_valid() const override;
    void set_depth(double depth) override;

private:
    double bottomWidth_;
    double sideSlope_;
    double depth_;
};

#endif // TRAPEZOIDALCHANNEL_H
