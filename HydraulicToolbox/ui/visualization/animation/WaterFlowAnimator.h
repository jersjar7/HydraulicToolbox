#ifndef WATERFLOWANIMATOR_H
#define WATERFLOWANIMATOR_H

#include "ParticleSystem.h"
#include "ProjectDataStructures.h"
#include "../backend/HydraulicCalculator.h"
#include "../renderers/ChannelRenderer.h"
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <memory>

class WaterFlowAnimator
{
public:
    WaterFlowAnimator(const GeometryData& geometry,
                      const CalculationResults& results,
                      ChannelRenderer* renderer);
    ~WaterFlowAnimator();

    void update(double deltaTime);
    vtkSmartPointer<vtkActor> get_particle_actor();

    void set_spawn_rate(double particlesPerSecond);
    void set_particle_size(double size);
    void reset();

private:
    void spawn_particles(double deltaTime);
    void update_particle_physics(double deltaTime);
    void apply_gravity_at_outlet();

    double calculate_spawn_spread() const;
    bool is_particle_at_outlet(const Particle& particle) const;

    std::unique_ptr<ParticleSystem> particleSystem_;

    Point3D inletCenter_;
    Point3D outletCenter_;
    Vector3D flowDirection_;

    double flowVelocity_{0.0};
    double normalDepth_{0.0};
    double spawnRate_{20.0};
    double spawnAccumulator_{0.0};
    double channelLength_{0.0};

    const double GRAVITY_{9.81};
};

#endif // WATERFLOWANIMATOR_H
