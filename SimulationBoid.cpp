#include "SimulationBoid.h"
#include <cmath>

Boid::Boid(const float x, const float y, const float z) {
	m_acceleration = glm::vec3(0.0f);

	float longitude = frand(2.0f * PI), latitude = frand(2.0f * PI);
	m_velocity = glm::vec3(cosf(longitude), sinf(longitude), sinf(latitude));

	m_position = glm::vec3(x, y, z);
	r = 2.0;
	maxspeed = 0.25f;
	maxforce = 0.03f;
}

void Boid::Run(const std::vector<Boid>& boids) {
	Flock(boids);
	Update();
	Borders();
}

void Boid::ApplyForce(const glm::vec3& force) {
	// We could add mass here if we want A = F / M
	m_acceleration += force;
}

void Boid::Flock(const std::vector<Boid>& boids) {
	glm::vec3 sep = Separate(boids);  // Separation
	glm::vec3 ali = Align(boids);     // Alignment
	glm::vec3 coh = Cohesion(boids);  // Cohesion
									  // Arbitrarily weight these forces
	sep *= 1.5;
	ali *= 1.0;
	coh *= 1.0;
	// Add the force vectors to acceleration
	ApplyForce(sep);
	ApplyForce(ali);
	ApplyForce(coh);

	// Apply boundery force
	float x = m_position.x, y = m_position.y, z = m_position.z;
	if (fabs(x + X_LENTH) < MIN_COLLISION_DIST && fabs(x + X_LENTH) > 1e-5) ApplyForce(glm::vec3((BOUND_FORCE) / (x + X_LENTH), 0.0f, 0.0f));
	if (fabs(x - X_LENTH) < MIN_COLLISION_DIST && fabs(x - X_LENTH) > 1e-5) ApplyForce(glm::vec3((BOUND_FORCE) / (x - X_LENTH), 0.0f, 0.0f));
	if (fabs(y + Y_LENTH) < MIN_COLLISION_DIST && fabs(x + Y_LENTH) > 1e-5) ApplyForce(glm::vec3(0.0f, (BOUND_FORCE) / (y + Y_LENTH), 0.0f));
	if (fabs(y - Y_LENTH) < MIN_COLLISION_DIST && fabs(x - Y_LENTH) > 1e-5) ApplyForce(glm::vec3(0.0f, (BOUND_FORCE) / (y - Y_LENTH), 0.0f));
	if (fabs(z + Z_LENTH) < MIN_COLLISION_DIST && fabs(x + Z_LENTH) > 1e-5) ApplyForce(glm::vec3(0.0f, 0.0f, (BOUND_FORCE) / (z + Z_LENTH)));
	if (fabs(z - Z_LENTH) < MIN_COLLISION_DIST && fabs(x - Z_LENTH) > 1e-5) ApplyForce(glm::vec3(0.0f, 0.0f, (BOUND_FORCE) / (z - Z_LENTH)));
}

void Boid::Update() {

	// Update velocity
	m_velocity += m_acceleration;
	// Limit speed
	m_velocity = glm::normalize(m_velocity) * maxspeed;
	m_position += m_velocity;
	// Reset accelertion to 0 each cycle
	m_acceleration *= 0.0f;
}

glm::vec3& Boid::Seek(const glm::vec3& target) {
	glm::vec3 desired = target - m_position;
	// A vector pointing from the position to the target
	// Scale to maximum speed
	desired = glm::normalize(desired) * maxspeed;

	// Above two lines of code below could be condensed with new PVector
	// setMag() method Not using this method until Processing.js catches up
	// desired.setMag(maxspeed);

	// Steering = Desired minus Velocity
	glm::vec3 steer = desired - m_velocity;
	// Limit to maximum steering force
	steer = glm::normalize(steer) * maxforce;
	return steer;
}

void Boid::Borders() {}

glm::vec3& Boid::Separate(const std::vector<Boid>& boids) {
	float desiredseparation = 10.0f;
	glm::vec3 steer = glm::vec3(0.0f);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (unsigned int i = 0; i < boids.size(); ++i) {
		Boid other = boids[i];
		float d = glm::length(m_position - other.m_position);
		// If the distance is greater than 0 and less than an arbitrary
		// amount (0 when you are yourself)
		if ((d > 0) && (d < desiredseparation)) {
			// Calculate vector pointing away from neighbor
			glm::vec3 diff = m_position - other.m_position;
			diff = glm::normalize(diff);
			diff /= d;  // Weight by distance
			steer += diff;
			count++;  // Keep track of how many
		}
	}
	// Average -- divide by how many
	if (count > 0) {
		steer /= (float)count;
	}

	// As long as the vector is greater than 0
	if (glm::length(steer) > 0) {
		// First two lines of code below could be condensed with new PVector
		// setMag() method Not using this method until Processing.js catches
		// up steer.setMag(maxspeed);

		// Implement Reynolds: Steering = Desired - Velocity
		steer = glm::normalize(steer);
		steer *= maxspeed;
		steer -= m_velocity;
		steer = glm::normalize(steer);
		steer *= maxforce;
	}
	return steer;
}

glm::vec3& Boid::Align(const std::vector<Boid>& boids) {
	float neighbordist = 50;
	glm::vec3 sum = glm::vec3(0.0f);
	int count = 0;
	for (unsigned int i = 0; i < boids.size(); ++i) {
		Boid other = boids[i];
		float d = glm::length(m_position - other.m_position);
		if ((d > 0) && (d < neighbordist)) {
			sum += other.m_velocity;
			count++;
		}
	}
	if (count > 0) {
		sum /= (float)count;
		// First two lines of code below could be condensed with new PVector
		// setMag() method Not using this method until Processing.js catches
		// up sum.setMag(maxspeed);

		// Implement Reynolds: Steering = Desired - Velocity
		sum = glm::normalize(sum) * maxspeed;
		glm::vec3 steer = sum - m_velocity;
		steer = glm::normalize(steer) * maxforce;
		return steer;
	}
	else {
		return glm::vec3(0.0f);
	}
}

glm::vec3& Boid::Cohesion(const std::vector<Boid>& boids) {
	float neighbordist = 50;
	glm::vec3 sum = glm::vec3(0.0f);
	// Start with empty vector to accumulate all positions
	int count = 0;
	for (unsigned int i = 0; i < boids.size(); ++i) {
		Boid other = boids[i];
		float d = glm::length(m_position - other.m_position);
		if ((d > 0) && (d < neighbordist)) {
			sum += other.m_position;  // Add position
			count++;
		}
	}
	if (count > 0) {
		sum /= (float)count;
		return Seek(sum);  // Steer towards the position
	}
	else {
		return glm::vec3(0.0f);
	}
}

const glm::vec3& Boid::getRotation() {
	glm::vec3 direction = glm::normalize(m_velocity);
	float angle_y = -asinf(direction.z);
	float angle_z = atan2f(direction.y, direction.x);
	return glm::vec3(0.0, angle_y, angle_z);
}