#include "TriangularChannelRenderer.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void TriangularChannelRenderer::render(vtkSmartPointer<vtkRenderer> renderer,
                                       vtkSmartPointer<vtkActor>& bottomActor,
                                       vtkSmartPointer<vtkActor>& wallsActor,
                                       vtkSmartPointer<vtkActor>& waterActor,
                                       const GeometryData& geometry,
                                       const CalculationResults& results)
{
    double sideSlope = geometry.sideSlope;
    double normalDepth = results.normalDepth;
    double channelDepth = normalDepth * 1.2;

    double length = channelDepth * 10.0;

    create_triangular_walls(wallsActor, length, sideSlope, channelDepth, renderer);

    create_triangular_water_surface(waterActor, normalDepth, length, sideSlope, renderer);
}

void TriangularChannelRenderer::create_triangular_walls(vtkSmartPointer<vtkActor>& wallsActor,
                                                        double length,
                                                        double sideSlope,
                                                        double channelDepth,
                                                        vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkPoints> wallPoints = vtkSmartPointer<vtkPoints>::New();

    // Bottom center line (the apex/point where walls meet)
    wallPoints->InsertNextPoint(0.0, 0.0, 0.0);          // 0: front bottom center
    wallPoints->InsertNextPoint(length, 0.0, 0.0);       // 1: back bottom center

    // Top corners (walls slope outward from center)
    double topWidth = sideSlope * channelDepth;
    wallPoints->InsertNextPoint(0.0, channelDepth, -topWidth);       // 2: front-left top
    wallPoints->InsertNextPoint(length, channelDepth, -topWidth);    // 3: back-left top
    wallPoints->InsertNextPoint(length, channelDepth, topWidth);     // 4: back-right top
    wallPoints->InsertNextPoint(0.0, channelDepth, topWidth);        // 5: front-right top

    vtkSmartPointer<vtkCellArray> wallCells = vtkSmartPointer<vtkCellArray>::New();

    // Left sloped wall (triangle from bottom center to top-left edge)
    vtkIdType leftWall[3] = {0, 1, 3};
    wallCells->InsertNextCell(3, leftWall);

    vtkIdType leftWall2[3] = {0, 3, 2};
    wallCells->InsertNextCell(3, leftWall2);

    // Right sloped wall (triangle from bottom center to top-right edge)
    vtkIdType rightWall[3] = {0, 4, 1};
    wallCells->InsertNextCell(3, rightWall);

    vtkIdType rightWall2[3] = {0, 5, 4};
    wallCells->InsertNextCell(3, rightWall2);

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

void TriangularChannelRenderer::create_triangular_water_surface(vtkSmartPointer<vtkActor>& waterActor,
                                                                double waterLevel,
                                                                double length,
                                                                double sideSlope,
                                                                vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkPoints> waterPoints = vtkSmartPointer<vtkPoints>::New();

    // Water surface is triangular - width depends on water depth
    double waterSurfaceWidth = sideSlope * waterLevel;

    // Create triangular water surface (V-shaped in cross-section)
    waterPoints->InsertNextPoint(0.0, waterLevel, -waterSurfaceWidth);      // front-left
    waterPoints->InsertNextPoint(length, waterLevel, -waterSurfaceWidth);   // back-left
    waterPoints->InsertNextPoint(length, waterLevel, waterSurfaceWidth);    // back-right
    waterPoints->InsertNextPoint(0.0, waterLevel, waterSurfaceWidth);       // front-right

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

Point3D TriangularChannelRenderer::get_inlet_center(const GeometryData& geometry,
                                                    const CalculationResults& results) const
{
    double normalDepth = results.normalDepth;

    Point3D inlet;
    inlet.x = 0.0;
    inlet.y = normalDepth * 0.5;
    inlet.z = 0.0;

    return inlet;
}

Point3D TriangularChannelRenderer::get_outlet_center(const GeometryData& geometry,
                                                     const CalculationResults& results) const
{
    double normalDepth = results.normalDepth;
    double channelDepth = normalDepth * 1.2;
    double length = channelDepth * 10.0;

    Point3D outlet;
    outlet.x = length;
    outlet.y = normalDepth * 0.5;
    outlet.z = 0.0;

    return outlet;
}

Vector3D TriangularChannelRenderer::get_flow_direction() const
{
    Vector3D direction;
    direction.x = 1.0;
    direction.y = 0.0;
    direction.z = 0.0;

    return direction;
}
