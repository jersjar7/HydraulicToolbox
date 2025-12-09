#include "WaterFlowAnimator.h"
#include <cmath>
#include <random>

WaterFlowAnimator::WaterFlowAnimator(const GeometryData& geometry,
                                     const CalculationResults& results,
                                     ChannelRenderer* renderer)
    : particleSystem_{std::make_unique<ParticleSystem>()}
    , inletCenter_{renderer->get_inlet_center(geometry, results)}
    , outletCenter_{renderer->get_outlet_center(geometry, results)}
    , flowDirection_{renderer->get_flow_direction()}
    , flowVelocity_{results.velocity}
    , normalDepth_{results.normalDepth}
    , spawnRate_{50.0}
    , spawnAccumulator_{0.0}
    , channelLength_{outletCenter_.x - inletCenter_.x}
    , GRAVITY_{9.81}
{
    double particleSize = normalDepth_ * 0.02;
    particleSystem_->set_particle_size(particleSize);
}

WaterFlowAnimator::~WaterFlowAnimator()
{
}

void WaterFlowAnimator::update(double deltaTime)
{
    spawn_particles(deltaTime);
    update_particle_physics(deltaTime);
    apply_gravity_at_outlet();
    particleSystem_->update(deltaTime);
}

void WaterFlowAnimator::spawn_particles(double deltaTime)
{
    spawnAccumulator_ += spawnRate_ * deltaTime;

    int particlesToSpawn = static_cast<int>(spawnAccumulator_);
    spawnAccumulator_ -= particlesToSpawn;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Spawn across full water depth and width
    std::uniform_real_distribution<> depthDist(0.1 * normalDepth_, 0.9 * normalDepth_);
    std::uniform_real_distribution<> widthDist(-0.4, 0.4);  // Relative to center

    for(int i = 0; i < particlesToSpawn; ++i)
    {
        double x = inletCenter_.x;
        double y = depthDist(gen);  // Full depth range

        // Calculate actual width at inlet (depends on channel type)
        double z = inletCenter_.z + widthDist(gen) * (inletCenter_.z > 0.1 ? inletCenter_.z : normalDepth_);

        double vx = flowVelocity_ * flowDirection_.x;
        double vy = 0.0;
        double vz = 0.0;

        particleSystem_->spawn_particle(x, y, z, vx, vy, vz);
    }
}

void WaterFlowAnimator::update_particle_physics(double deltaTime)
{
    // Physics updates happen in ParticleSystem::update()
}

void WaterFlowAnimator::apply_gravity_at_outlet()
{
    // Access particles and apply gravity when they reach outlet
    // This is a simplified approach - particles already have velocity
    // In a more sophisticated version, we'd modify ParticleSystem to expose particles
    // For now, particles will flow straight through - gravity effect can be enhanced later
}

bool WaterFlowAnimator::is_particle_at_outlet(const Particle& particle) const
{
    return particle.x >= outletCenter_.x;
}

double WaterFlowAnimator::calculate_spawn_spread() const
{
    return normalDepth_ * 0.3;
}

vtkSmartPointer<vtkActor> WaterFlowAnimator::get_particle_actor()
{
    return particleSystem_->get_particle_actor();
}

void WaterFlowAnimator::set_spawn_rate(double particlesPerSecond)
{
    spawnRate_ = particlesPerSecond;
}

void WaterFlowAnimator::set_particle_size(double size)
{
    particleSystem_->set_particle_size(size);
}

void WaterFlowAnimator::reset()
{
    particleSystem_->clear_all_particles();
    spawnAccumulator_ = 0.0;
}
