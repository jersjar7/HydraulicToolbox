#ifndef PROJECTDATASTRUCTURES_H
#define PROJECTDATASTRUCTURES_H

#include <QString>

struct ProjectData
{
    QString projectName;
    QString location;
    bool useUsCustomary{true};
};

struct GeometryData
{
    QString channelType;
    double bottomWidth{0.0};
    double sideSlope{0.0};
    double length{0.0};
    double bedSlope{0.0};
};

struct HydraulicData
{
    double discharge{0.0};
    double manningN{0.0};
};

#endif // PROJECTDATASTRUCTURES_H
