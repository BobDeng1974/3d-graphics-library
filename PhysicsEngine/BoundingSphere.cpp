
#include "boundingSphere.h"
#include <iostream>

IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other) const
{
	//The radius is the distance from any point on the sphere to the center.
	//
	//Therefore, by adding the radius of two spheres together, the result is
	//the distance between the centers of the spheres when they are touching.
	float radiusDistance = m_radius + other.GetRadius();
	glm::vec3 direction = (other.GetCenter() - m_center);
	float centerDistance = glm::length(direction);
	direction /= centerDistance;

	//Since the radiusDistance is the distance between the centers of the 
	//spheres are when they're touching, you can subtract that from the
	//distance between the centers of the spheres to get the actual distance
	//between the two spheres.
	float distance = centerDistance - radiusDistance;

	//Spheres can only be intersecting if the distance between them is less
	//than 0.
	return IntersectData(distance < 0, glm::normalize(direction) * distance);
}


IntersectData BoundingSphere::IntersectBoundingGround(const BoundingGround& other) const
{
	/* The collision point on ground always is (m_center.x, m_center.y, ground.m_center.z) */
	glm::vec3 groundCenter = glm::vec3(m_center.x, other.GetCenter().y, m_center.z);

	glm::vec3 direction = groundCenter - m_center;
	float centerDistance = glm::length(direction);


	float distance = centerDistance - m_radius;
	bool isIntersected = distance < 0
		&& fabs(m_center.x - other.GetCenter().x) < other.GetLenght() / 2.0f
		&& fabs(m_center.y - other.GetCenter().y) < other.GetWidth() / 2.0f;
	return IntersectData(isIntersected, glm::normalize(direction) * distance);
}
