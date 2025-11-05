#ifndef CHANNELRENDERER_H
#define CHANNELRENDERER_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include "ProjectDataStructures.h"
#include "../backend/HydraulicCalculator.h"

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
