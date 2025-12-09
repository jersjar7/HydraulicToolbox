#ifndef CHANNELRENDERER_H
#define CHANNELRENDERER_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include "ProjectDataStructures.h"
#include "../backend/HydraulicCalculator.h"

struct Point3D
{
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

struct Vector3D
{
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

class ChannelRenderer
{
public:
    virtual ~ChannelRenderer() = default;

    virtual void render(vtkSmartPointer<vtkRenderer> renderer,
                        vtkSmartPointer<vtkActor>& bottomActor,
                        vtkSmartPointer<vtkActor>& wallsActor,
                        vtkSmartPointer<vtkActor>& waterActor,
                        const GeometryData& geometry,
                        const CalculationResults& results) = 0;

    virtual Point3D get_inlet_center(const GeometryData& geometry,
                                     const CalculationResults& results) const = 0;
    virtual Point3D get_outlet_center(const GeometryData& geometry,
                                      const CalculationResults& results) const = 0;
    virtual Vector3D get_flow_direction() const = 0;

protected:
    void create_channel_bottom(vtkSmartPointer<vtkActor>& bottomActor,
                               double length,
                               double width,
                               vtkSmartPointer<vtkRenderer> renderer);

    void create_water_surface(vtkSmartPointer<vtkActor>& waterActor,
                              double waterLevel,
                              double length,
                              double width,
                              vtkSmartPointer<vtkRenderer> renderer);

    void apply_channel_material_properties(vtkSmartPointer<vtkActor>& actor);
    void apply_water_material_properties(vtkSmartPointer<vtkActor>& actor);
};

#endif // CHANNELRENDERER_H
