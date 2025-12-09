#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vtkSmartPointer.h>
#include <vtkActor.h>

struct Particle
{
    double x{0.0};
    double y{0.0};
    double z{0.0};
    double velocityX{0.0};
    double velocityY{0.0};
    double velocityZ{0.0};
    double age{0.0};
    double maxAge{5.0};
    bool isActive{true};
};

class ParticleSystem
{
public:
    ParticleSystem();
    ~ParticleSystem();

    void spawn_particle(double x, double y, double z,
                        double vx, double vy, double vz);
    void update(double deltaTime);
    void clear_all_particles();

    vtkSmartPointer<vtkActor> get_particle_actor();

    size_t get_active_particle_count() const;
    void set_particle_size(double size);

private:
    void update_particle_positions(double deltaTime);
    void remove_dead_particles();
    void rebuild_vtk_geometry();

    std::vector<Particle> particles_;
    double particleSize_{0.05};

    vtkSmartPointer<vtkActor> particleActor_;
};

#endif // PARTICLESYSTEM_H
