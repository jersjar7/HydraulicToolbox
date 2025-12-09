#include "ParticleSystem.h"
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <algorithm>

ParticleSystem::ParticleSystem()
    : particles_{}
    , particleSize_{0.05}
    , particleActor_{vtkSmartPointer<vtkActor>::New()}
{
    particleActor_->GetProperty()->SetColor(0.9, 0.95, 1.0);
    particleActor_->GetProperty()->SetOpacity(0.7);
    particleActor_->GetProperty()->SetSpecular(0.6);
    particleActor_->GetProperty()->SetSpecularPower(30);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::spawn_particle(double x, double y, double z,
                                    double vx, double vy, double vz)
{
    Particle particle;
    particle.x = x;
    particle.y = y;
    particle.z = z;
    particle.velocityX = vx;
    particle.velocityY = vy;
    particle.velocityZ = vz;
    particle.age = 0.0;
    particle.maxAge = 100.0;
    particle.isActive = true;

    particles_.push_back(particle);
}

void ParticleSystem::update(double deltaTime)
{
    update_particle_positions(deltaTime);
    remove_dead_particles();
    rebuild_vtk_geometry();
}

void ParticleSystem::update_particle_positions(double deltaTime)
{
    for(auto& particle : particles_)
    {
        if(!particle.isActive)
            continue;

        particle.x += particle.velocityX * deltaTime;
        particle.y += particle.velocityY * deltaTime;
        particle.z += particle.velocityZ * deltaTime;

        particle.age += deltaTime;

        if(particle.age >= particle.maxAge)
            particle.isActive = false;
    }
}

void ParticleSystem::remove_dead_particles()
{
    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(),
                       [](const Particle& p) { return !p.isActive; }),
        particles_.end()
        );
}

void ParticleSystem::rebuild_vtk_geometry()
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    for(const auto& particle : particles_)
    {
        if(particle.isActive)
            points->InsertNextPoint(particle.x, particle.y, particle.z);
    }

    vtkSmartPointer<vtkPolyData> pointsPolyData = vtkSmartPointer<vtkPolyData>::New();
    pointsPolyData->SetPoints(points);

    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(particleSize_);
    sphereSource->SetThetaResolution(8);
    sphereSource->SetPhiResolution(8);

    vtkSmartPointer<vtkGlyph3D> glyph3D = vtkSmartPointer<vtkGlyph3D>::New();
    glyph3D->SetSourceConnection(sphereSource->GetOutputPort());
    glyph3D->SetInputData(pointsPolyData);
    glyph3D->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyph3D->GetOutputPort());

    particleActor_->SetMapper(mapper);
}

void ParticleSystem::clear_all_particles()
{
    particles_.clear();
}

vtkSmartPointer<vtkActor> ParticleSystem::get_particle_actor()
{
    return particleActor_;
}

size_t ParticleSystem::get_active_particle_count() const
{
    return std::count_if(particles_.begin(), particles_.end(),
                         [](const Particle& p) { return p.isActive; });
}

void ParticleSystem::set_particle_size(double size)
{
    particleSize_ = size;
}
