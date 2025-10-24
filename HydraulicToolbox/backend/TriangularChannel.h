#ifndef TRIANGULARCHANNEL_H
#define TRIANGULARCHANNEL_H

#include "Channel.h"

class TriangularChannel : public Channel
{
public:
    TriangularChannel(double sideSlope, double depth);

    double calculate_area() const override;
    double calculate_wetted_perimeter() const override;
    bool is_valid() const override;
    void set_depth(double depth) override;
    double calculate_top_width() const override;

private:
    double sideSlope_;
    double depth_;
};

#endif // TRIANGULARCHANNEL_H
