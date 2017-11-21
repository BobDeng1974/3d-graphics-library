#ifndef PHYSICS_OBJECT_H
#include <glm/glm.hpp>

#include "Collider.h"
class PhysicsObject
{
public:
	/**
	* Creates a PhysicsObject in a usable state.
	*
	* @param collider A collider representing the shape and position of the
	*                   object. Should be in allocated memory.
	* @param velocity How fast this object is moving and in what direction.
	*/
	PhysicsObject(Collider* collider, const glm::vec3& velocity, const glm::vec3& acceleration) :
		m_position(collider->GetCenter()),
		m_velocity(velocity),
		m_acceleration(acceleration),
		m_collider(collider),
		m_pLast(0.0f) {}


	/**
	* Calculate this object's new location and properties after delta seconds
	*
	* @param delta How much time to simulate.
	*/
	void Integrate(float delta);

	/** Basic getter */
	inline const glm::vec3& GetPosition() const { return m_position; }
	/** Basic getter */
	inline const glm::vec3& GetVelocity() const { return m_velocity; }

	/**
	* Returns a collider in the position of this object, updating the
	* collider's position if necessary.
	*/
	inline const Collider& GetCollider() { return *m_collider; }

	/** Basic setter */
	inline void SetVelocity(const glm::vec3& velocity) { m_velocity = velocity; }

	inline void SetLast(const float p) { m_pLast = p;}
	
	inline float GetLast() { return m_pLast; }

	inline void SetAcceleration(const glm::vec3& acceleration) { m_acceleration = acceleration; }

	inline const glm::vec3& GetAcceleration() { return m_acceleration; }
private:
	/** Where this object is in 3D space. */
	glm::vec3 m_position;
	/** How fast this object is moving and in what direction */
	glm::vec3 m_velocity;
	/** Acceleration of the object */
	glm::vec3 m_acceleration;
	/** The collider representing the shape and position of this object. */
	Collider* m_collider;
	/* Last collision object, If no collision happended set to NULL */
	float m_pLast;
};

#endif // !PHYSICS_OBJECT_H

