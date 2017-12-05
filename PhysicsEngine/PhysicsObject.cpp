#include "PhysicsObject.h"
#include "BoundingSphere.h"
#include <iostream>
#include <cassert>
#include <cstring>

void PhysicsObject::Integrate(float delta)
{
	if (m_collider->GetType() != Collider::TYPE_GROUND) {
		glm::vec3 a = m_acceleration;
		m_position += m_velocity * delta + a * delta * delta * 0.5f;
		m_velocity += delta * a;

		m_collider->SetCenter(m_position);
		//std::cout << " " << a.y << " " << std::endl;
		//std::cout << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;
	}
}
