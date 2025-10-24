#ifndef RECTANGULARCHANNEL_H
#define RECTANGULARCHANNEL_H

#include "Channel.h"

class RectangularChannel : public Channel
{
public:
    RectangularChannel(double width, double depth);

    double calculate_area() const override;
    double calculate_wetted_perimeter() const override;
    bool is_valid() const override;
    void set_depth(double depth) override;

private:
    double width_;
    double depth_;
};

#endif // RECTANGULARCHANNEL_H
