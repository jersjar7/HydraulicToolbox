#ifndef UNITSYSTEMCONSTANTS_H
#define UNITSYSTEMCONSTANTS_H

namespace UnitSystemConstants
{
// Gravity constants
constexpr double GRAVITY_SI = 9.81;              // m/s²
constexpr double GRAVITY_US_CUSTOMARY = 32.2;    // ft/s²

// Manning's equation coefficients
constexpr double MANNINGS_COEFFICIENT_SI = 1.0;
constexpr double MANNINGS_COEFFICIENT_US = 1.49;

// Unit labels for length
constexpr const char* LABEL_LENGTH_SI = "m";
constexpr const char* LABEL_LENGTH_US = "ft";

// Unit labels for velocity
constexpr const char* LABEL_VELOCITY_SI = "m/s";
constexpr const char* LABEL_VELOCITY_US = "ft/s";

// Unit labels for discharge
constexpr const char* LABEL_DISCHARGE_SI = "m³/s";
constexpr const char* LABEL_DISCHARGE_US = "cfs";

// Unit labels for area
constexpr const char* LABEL_AREA_SI = "m²";
constexpr const char* LABEL_AREA_US = "ft²";

// Display names for unit systems
constexpr const char* SYSTEM_NAME_US = "US Customary";
constexpr const char* SYSTEM_NAME_SI = "SI Metric";

// Helper function to get gravity based on unit system
inline double get_gravity(bool useUsCustomary)
{
    return useUsCustomary ? GRAVITY_US_CUSTOMARY : GRAVITY_SI;
}

// Helper function to get Manning's coefficient based on unit system
inline double get_mannings_coefficient(bool useUsCustomary)
{
    return useUsCustomary ? MANNINGS_COEFFICIENT_US : MANNINGS_COEFFICIENT_SI;
}
}

#endif // UNITSYSTEMCONSTANTS_H
