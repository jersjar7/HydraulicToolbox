#ifndef TRAPEZOIDALCHANNELRENDERER_H
#define TRAPEZOIDALCHANNELRENDERER_H

#include "ChannelRenderer.h"

class TrapezoidalChannelRenderer : public ChannelRenderer
{
public:
    TrapezoidalChannelRenderer() = default;
    ~TrapezoidalChannelRenderer() override = default;

    void render(vtkSmartPointer<vtkRenderer> renderer,
                vtkSmartPointer<vtkActor>& bottomActor,
                vtkSmartPointer<vtkActor>& wallsActor,
                vtkSmartPointer<vtkActor>& waterActor,
                const GeometryData& geometry,
                const CalculationResults& results) override;

private:
    void create_trapezoidal_walls(vtkSmartPointer<vtkActor>& wallsActor,
                                  double length,
                                  double bottomWidth,
                                  double sideSlope,
                                  double channelDepth,
                                  vtkSmartPointer<vtkRenderer> renderer);

    void create_trapezoidal_water_surface(vtkSmartPointer<vtkActor>& waterActor,
                                          double waterLevel,
                                          double length,
                                          double bottomWidth,
                                          double sideSlope,
                                          vtkSmartPointer<vtkRenderer> renderer);
};

#endif // TRAPEZOIDALCHANNELRENDERER_H
