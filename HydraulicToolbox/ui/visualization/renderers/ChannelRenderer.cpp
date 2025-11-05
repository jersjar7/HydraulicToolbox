#include "ChannelRenderer.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void ChannelRenderer::create_channel_bottom(vtkSmartPointer<vtkActor>& bottomActor,
                                            double length,
                                            double width,
                                            vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkPoints> bottomPoints = vtkSmartPointer<vtkPoints>::New();
    bottomPoints->InsertNextPoint(0.0, 0.0, 0.0);
    bottomPoints->InsertNextPoint(length, 0.0, 0.0);
    bottomPoints->InsertNextPoint(length, 0.0, width);
    bottomPoints->InsertNextPoint(0.0, 0.0, width);

    vtkSmartPointer<vtkCellArray> bottomCells = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType bottom[4] = {0, 1, 2, 3};
    bottomCells->InsertNextCell(4, bottom);

    vtkSmartPointer<vtkPolyData> bottomPolyData = vtkSmartPointer<vtkPolyData>::New();
    bottomPolyData->SetPoints(bottomPoints);
    bottomPolyData->SetPolys(bottomCells);

    vtkSmartPointer<vtkPolyDataMapper> bottomMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    bottomMapper->SetInputData(bottomPolyData);

    bottomActor->SetMapper(bottomMapper);
    apply_channel_material_properties(bottomActor);
    bottomActor->SetVisibility(1);

    renderer->AddActor(bottomActor);
}

void ChannelRenderer::create_water_surface(vtkSmartPointer<vtkActor>& waterActor,
                                           double waterLevel,
                                           double length,
                                           double width,
                                           vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkPoints> waterPoints = vtkSmartPointer<vtkPoints>::New();
    waterPoints->InsertNextPoint(0.0, waterLevel, 0.0);
    waterPoints->InsertNextPoint(length, waterLevel, 0.0);
    waterPoints->InsertNextPoint(length, waterLevel, width);
    waterPoints->InsertNextPoint(0.0, waterLevel, width);

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

void ChannelRenderer::apply_channel_material_properties(vtkSmartPointer<vtkActor>& actor)
{
    actor->GetProperty()->SetColor(0.35, 0.30, 0.25);
    actor->GetProperty()->SetAmbient(0.3);
    actor->GetProperty()->SetDiffuse(0.7);
}

void ChannelRenderer::apply_water_material_properties(vtkSmartPointer<vtkActor>& actor)
{
    actor->GetProperty()->SetColor(0.1, 0.5, 0.9);
    actor->GetProperty()->SetOpacity(0.65);
    actor->GetProperty()->SetAmbient(0.2);
    actor->GetProperty()->SetDiffuse(0.6);
    actor->GetProperty()->SetSpecular(0.4);
    actor->GetProperty()->SetSpecularPower(20);
}
