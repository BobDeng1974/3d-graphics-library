#ifndef SIMULATION_FLOCK_H
#define SIMULATION_FLOCK_H
#include <vector>

#include "SimulationBoid.h"

class Flock {
public:
	void AddBoid(const Boid& boid);
	const Boid& GetBoid(const unsigned int index);
	const unsigned int GetBoidNum() { return m_boids.size(); }
	void Run();

private:
	std::vector<Boid> m_boids;
};

#endif