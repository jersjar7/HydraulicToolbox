#include "RectangularChannelRenderer.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void RectangularChannelRenderer::render(vtkSmartPointer<vtkRenderer> renderer,
                                        vtkSmartPointer<vtkActor>& bottomActor,
                                        vtkSmartPointer<vtkActor>& wallsActor,
                                        vtkSmartPointer<vtkActor>& waterActor,
                                        const GeometryData& geometry,
                                        const CalculationResults& results)
{
    double width = geometry.bottomWidth;
    double normalDepth = results.normalDepth;
    double channelDepth = normalDepth * 1.2;
    double length = width * 10.0;

    create_channel_bottom(bottomActor, length, width, renderer);

    create_rectangular_walls(wallsActor, length, width, channelDepth, renderer);

    create_water_surface(waterActor, normalDepth, length, width, renderer);
}

void RectangularChannelRenderer::create_rectangular_walls(vtkSmartPointer<vtkActor>& wallsActor,
                                                          double length,
                                                          double width,
                                                          double channelDepth,
                                                          vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkPoints> wallPoints = vtkSmartPointer<vtkPoints>::New();

    // Bottom corners
    wallPoints->InsertNextPoint(0.0, 0.0, 0.0);
    wallPoints->InsertNextPoint(length, 0.0, 0.0);
    wallPoints->InsertNextPoint(length, 0.0, width);
    wallPoints->InsertNextPoint(0.0, 0.0, width);

    // Top corners
    wallPoints->InsertNextPoint(0.0, channelDepth, 0.0);
    wallPoints->InsertNextPoint(length, channelDepth, 0.0);
    wallPoints->InsertNextPoint(length, channelDepth, width);
    wallPoints->InsertNextPoint(0.0, channelDepth, width);

    vtkSmartPointer<vtkCellArray> wallCells = vtkSmartPointer<vtkCellArray>::New();

    // Left wall (along Z=0)
    vtkIdType leftWall[4] = {0, 1, 5, 4};
    wallCells->InsertNextCell(4, leftWall);

    // Right wall (along Z=width)
    vtkIdType rightWall[4] = {3, 2, 6, 7};
    wallCells->InsertNextCell(4, rightWall);

    vtkSmartPointer<vtkPolyData> wallPolyData = vtkSmartPointer<vtkPolyData>::New();
    wallPolyData->SetPoints(wallPoints);
    wallPolyData->SetPolys(wallCells);

    vtkSmartPointer<vtkPolyDataMapper> wallMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    wallMapper->SetInputData(wallPolyData);

    wallsActor->SetMapper(wallMapper);
    wallsActor->GetProperty()->SetColor(0.50, 0.50, 0.50);
    wallsActor->GetProperty()->SetAmbient(0.3);
    wallsActor->GetProperty()->SetDiffuse(0.7);
    wallsActor->SetVisibility(1);

    renderer->AddActor(wallsActor);
}
