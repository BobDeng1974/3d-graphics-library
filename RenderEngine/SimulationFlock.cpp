#include "SimulationFlock.h"


void Flock::AddBoid(const Boid& boid) {
	m_boids.push_back(boid);
}

const Boid& Flock::GetBoid(const unsigned int index) {
	return m_boids[index];
}

void Flock::Run() {
	for (unsigned int i = 0; i < m_boids.size(); ++i) {
		m_boids[i].Run(m_boids);
	}
}