#include <glm/glm.hpp>

#include "PhysicsEngine.h"
#include "BoundingSphere.h"

#include <iostream>

void PhysicsEngine::AddObject(const PhysicsObject& object)
{
	m_objects.push_back(object);
}

void PhysicsEngine::Simulate(float delta)
{

	for (unsigned int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i].Integrate(delta);
	}
}

void PhysicsEngine::HandleCollisions(float time)
{
	/* In the general version, we don't consider mass. */
	for (unsigned int i = 0; i < m_objects.size(); i++)
	{
		for (unsigned int j = i + 1; j < m_objects.size(); j++)
		{
			IntersectData intersectData =
				m_objects[i].GetCollider().Intersect(
					m_objects[j].GetCollider());

			if (intersectData.GetDoesIntersect())
			{
				float reserveEnergy = 1.0f;
				if (time - m_objects[i].GetLast() > 0.85) {
					reserveEnergy = LOSS;
					m_objects[i].SetLast(time);
					
				}
				if (time - m_objects[j].GetLast() > 0.85) {
					reserveEnergy = LOSS;
					m_objects[j].SetLast(time);
				}

				glm::vec3 v_i = m_objects[i].GetVelocity() * reserveEnergy;
				glm::vec3 v_j = m_objects[j].GetVelocity() * reserveEnergy;


				if (m_objects[i].GetCollider().GetType() == Collider::TYPE_SPHERE &&
					m_objects[j].GetCollider().GetType() == Collider::TYPE_SPHERE) {

					/* Build Cartesian coordinate system, x is collision direction */
					glm::vec3 x_direction = glm::normalize(intersectData.GetDirection());

					/* Velocity decomposition */

					/* For v_i */
					glm::vec3 v_i_x = glm::dot(v_i, x_direction) * x_direction;
					glm::vec3 v_i_yz = v_i - v_i_x;

					/* For v_j */
					glm::vec3 v_j_x = glm::dot(v_j, x_direction) * x_direction;
					glm::vec3 v_j_yz = v_j - v_j_x;

					m_objects[i].SetVelocity(glm::vec3(v_i_yz + v_j_x));
					m_objects[j].SetVelocity(glm::vec3(v_j_yz + v_i_x));
				}
				else if (m_objects[j].GetCollider().GetType() == Collider::TYPE_GROUND) {
					glm::vec3 v;
					if (v_i.y > 0 && (v_i.y) < 0.5f && intersectData.GetDistance() < 0.1f) {
						// Set acceleration
						//std::cout << v_i.y << " stable " << std::endl;
						v = glm::vec3(v_i.x, 0.0f, v_i.z);
						m_objects[i].SetAcceleration(glm::vec3(0.0f));
					}
					else {
						glm::vec3 norm = glm::normalize(intersectData.GetDirection());
						v = glm::vec3(glm::reflect(v_i, norm));
						//std::cout << v_i.y << " unstable " << v.y << std::endl;
					}
					
					//std::cout << v.x << " ground" << std::endl;
					m_objects[i].SetVelocity(v);
				}
			}
		}

	}
}
