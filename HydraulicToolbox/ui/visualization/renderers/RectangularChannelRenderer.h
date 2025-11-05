#ifndef RECTANGULARCHANNELRENDERER_H
#define RECTANGULARCHANNELRENDERER_H

#include "ChannelRenderer.h"

class RectangularChannelRenderer : public ChannelRenderer
{
public:
    RectangularChannelRenderer() = default;
    ~RectangularChannelRenderer() override = default;

    void render(vtkSmartPointer<vtkRenderer> renderer,
                vtkSmartPointer<vtkActor>& bottomActor,
                vtkSmartPointer<vtkActor>& wallsActor,
                vtkSmartPointer<vtkActor>& waterActor,
                const GeometryData& geometry,
                const CalculationResults& results) override;

private:
    void create_rectangular_walls(vtkSmartPointer<vtkActor>& wallsActor,
                                  double length,
                                  double width,
                                  double channelDepth,
                                  vtkSmartPointer<vtkRenderer> renderer);
};

#endif // RECTANGULARCHANNELRENDERER_H
