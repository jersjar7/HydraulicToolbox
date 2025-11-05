#include "TrapezoidalChannelRenderer.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void TrapezoidalChannelRenderer::render(vtkSmartPointer<vtkRenderer> renderer,
                                        vtkSmartPointer<vtkActor>& bottomActor,
                                        vtkSmartPointer<vtkActor>& wallsActor,
                                        vtkSmartPointer<vtkActor>& waterActor,
                                        const GeometryData& geometry,
                                        const CalculationResults& results)
{
    double bottomWidth = geometry.bottomWidth;
    double sideSlope = geometry.sideSlope;
    double normalDepth = results.normalDepth;
    double channelDepth = normalDepth * 1.2;
    double length = bottomWidth * 10.0;

    create_channel_bottom(bottomActor, length, bottomWidth, renderer);

    create_trapezoidal_walls(wallsActor, length, bottomWidth, sideSlope, channelDepth, renderer);

    create_trapezoidal_water_surface(waterActor, normalDepth, length, bottomWidth, sideSlope, renderer);
}

void TrapezoidalChannelRenderer::create_trapezoidal_walls(vtkSmartPointer<vtkActor>& wallsActor,
                                                          double length,
                                                          double bottomWidth,
                                                          double sideSlope,
                                                          double channelDepth,
                                                          vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkPoints> wallPoints = vtkSmartPointer<vtkPoints>::New();

    // Bottom corners (4 points at Y=0)
    wallPoints->InsertNextPoint(0.0, 0.0, 0.0);                      // 0: front-left bottom
    wallPoints->InsertNextPoint(length, 0.0, 0.0);                   // 1: back-left bottom
    wallPoints->InsertNextPoint(length, 0.0, bottomWidth);           // 2: back-right bottom
    wallPoints->InsertNextPoint(0.0, 0.0, bottomWidth);              // 3: front-right bottom

    // Top corners (4 points at Y=channelDepth, expanded outward by sideSlope * depth)
    double horizontalExpansion = sideSlope * channelDepth;
    wallPoints->InsertNextPoint(0.0, channelDepth, -horizontalExpansion);                    // 4: front-left top
    wallPoints->InsertNextPoint(length, channelDepth, -horizontalExpansion);                 // 5: back-left top
    wallPoints->InsertNextPoint(length, channelDepth, bottomWidth + horizontalExpansion);    // 6: back-right top
    wallPoints->InsertNextPoint(0.0, channelDepth, bottomWidth + horizontalExpansion);       // 7: front-right top

    vtkSmartPointer<vtkCellArray> wallCells = vtkSmartPointer<vtkCellArray>::New();

    // Left sloped wall (connects bottom-left edge to top-left edge)
    vtkIdType leftWall[4] = {0, 1, 5, 4};
    wallCells->InsertNextCell(4, leftWall);

    // Right sloped wall (connects bottom-right edge to top-right edge)
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

void TrapezoidalChannelRenderer::create_trapezoidal_water_surface(vtkSmartPointer<vtkActor>& waterActor,
                                                                  double waterLevel,
                                                                  double length,
                                                                  double bottomWidth,
                                                                  double sideSlope,
                                                                  vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkPoints> waterPoints = vtkSmartPointer<vtkPoints>::New();

    // Water surface is trapezoidal - wider than bottom due to side slopes
    double waterSurfaceExpansion = sideSlope * waterLevel;

    waterPoints->InsertNextPoint(0.0, waterLevel, -waterSurfaceExpansion);
    waterPoints->InsertNextPoint(length, waterLevel, -waterSurfaceExpansion);
    waterPoints->InsertNextPoint(length, waterLevel, bottomWidth + waterSurfaceExpansion);
    waterPoints->InsertNextPoint(0.0, waterLevel, bottomWidth + waterSurfaceExpansion);

    vtkSmartPointer<vtkCellArray> waterCells = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType waterFace[4] = {0, 1, 2, 3};
    waterCells->InsertNextCell(4, waterFace);

    vtkSmartPointer<vtkPolyData> waterPolyData = vtkSmartPointer<vtkPolyData>::New();
    waterPolyData->SetPoints(waterPoints);
    waterPolyData->SetPolys(waterCells);

    vtkSmartPointer<vtkPolyDataMapper> waterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    waterMapper->SetInputData(waterPolyData);

    waterActor->SetMapper(waterMapper);
    apply_water_material_properties(waterActor);
    waterActor->SetVisibility(1);

    renderer->AddActor(waterActor);
}
