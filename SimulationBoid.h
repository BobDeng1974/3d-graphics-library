#ifndef SIMULATION_BOID_H
#define SIMULATION_BOID_H

#include <glm/glm.hpp>
#include <vector>

#define PI (atan(1)*4)
#define frand(X) (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X)))
#define X_LENTH 30.0f
#define Y_LENTH 30.0f
#define Z_LENTH 30.0f
#define MIN_COLLISION_DIST 1.0f
#define BOUND_FORCE 1.0f


class Boid {
public:
	Boid(const float x, const float y, const float z);

	void Run(const std::vector<Boid>& boids);

	inline const glm::vec3& getPosition() const { return m_position; }

	inline const glm::vec3& getVelocity() const { return m_velocity; }

	inline const glm::vec3& getAcceleration() const { return m_acceleration; }

	const glm::vec3& getRotation();

private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	float r;
	float maxforce;  // Maximum steering force
	float maxspeed;  // Maximum speed

	void ApplyForce(const glm::vec3& force);

	// We accumulate a new acceleration each time based on three rules
	void Flock(const std::vector<Boid>& boids);

	// Method to update position
	void Update();

	// A method that calculates and applies a steering force towards a target
	// STEER = DESIRED MINUS VELOCITY
	glm::vec3& Seek(const glm::vec3& target);

	// Wraparound
	void Borders();

	// Separation
	// Method checks for nearby boids and steers away
	glm::vec3& Separate(const std::vector<Boid>& boids);

	// Alignment
	// For every nearby boid in the system, calculate the average velocity
	glm::vec3& Align(const std::vector<Boid>& boids);

	// Cohesion
	// For the average position (i.e. center) of all nearby boids, calculate
	// steering vector towards that position
	glm::vec3& Cohesion(const std::vector<Boid>& boids);
};
#endif