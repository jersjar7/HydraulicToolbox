#ifndef FLOW_H
#define FLOW_H

class Flow
{
public:
    Flow(double discharge, double manningN);

    double get_discharge() const;
    double get_manning_n() const;
    bool is_valid() const;

private:
    double discharge_;
    double manningN_;
};

#endif // FLOW_H
