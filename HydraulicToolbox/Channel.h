#ifndef CHANNEL_H
#define CHANNEL_H

class Channel
{
public:
    virtual ~Channel() = default;

    virtual double calculate_area() const = 0;
    virtual double calculate_wetted_perimeter() const = 0;
    virtual bool is_valid() const = 0;
    virtual void set_depth(double depth) = 0;

    double calculate_hydraulic_radius() const;
};

#endif // CHANNEL_H
