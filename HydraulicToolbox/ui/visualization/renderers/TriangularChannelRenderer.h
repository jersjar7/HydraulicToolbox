#ifndef TRIANGULARCHANNELRENDERER_H
#define TRIANGULARCHANNELRENDERER_H

#include "ChannelRenderer.h"

class TriangularChannelRenderer : public ChannelRenderer
{
public:
    TriangularChannelRenderer() = default;
    ~TriangularChannelRenderer() override = default;

    void render(vtkSmartPointer<vtkRenderer> renderer,
                vtkSmartPointer<vtkActor>& bottomActor,
                vtkSmartPointer<vtkActor>& wallsActor,
                vtkSmartPointer<vtkActor>& waterActor,
                const GeometryData& geometry,
                const CalculationResults& results) override;

    Point3D get_inlet_center(const GeometryData& geometry,
                             const CalculationResults& results) const override;
    Point3D get_outlet_center(const GeometryData& geometry,
                              const CalculationResults& results) const override;
    Vector3D get_flow_direction() const override;

private:
    void create_triangular_walls(vtkSmartPointer<vtkActor>& wallsActor,
                                 double length,
                                 double sideSlope,
                                 double channelDepth,
                                 vtkSmartPointer<vtkRenderer> renderer);

    void create_triangular_water_surface(vtkSmartPointer<vtkActor>& waterActor,
                                         double waterLevel,
                                         double length,
                                         double sideSlope,
                                         vtkSmartPointer<vtkRenderer> renderer);
};

#endif // TRIANGULARCHANNELRENDERER_H
