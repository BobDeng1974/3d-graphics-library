
#ifndef BOUNDING_SPHERE_INCLUDED_H
#define BOUNDING_SPHERE_INCLUDED_H

#include <glm/glm.hpp>

#include "IntersectData.h"
#include "Collider.h"
#include "BoundingGround.h"

/**
* The BoundingSphere class represents an sphere that can be used as a
* collider in a physics engine.
*/
class BoundingSphere : public Collider
{
public:
	/**
	* Creates a BoundingSphere in a usable state.
	*
	* @param center The center point of the sphere.
	* @param radius The distance from any point on the sphere to the center.
	*/
	BoundingSphere(const glm::vec3& center, float radius) :
		Collider(Collider::TYPE_SPHERE),
		m_center(center),
		m_radius(radius) {}

	/**
	* Computes information about if this sphere intersects another aphere.
	*
	* @param other The sphere that's being tested for intersection with this
	*                sphere.
	*/
	IntersectData IntersectBoundingSphere(const BoundingSphere& other) const;

	/**
	* Computes information about if this sphere intersects another ground.
	*
	* @param other The sphere that's being tested for intersection with this
	*                ground.
	*/
	IntersectData IntersectBoundingGround(const BoundingGround& other) const;

	virtual void SetCenter(const glm::vec3& center) { m_center = center; }
	virtual glm::vec3 GetCenter() const { return m_center; }

	/** Basic getter for the radius */
	inline float GetRadius()           const { return m_radius; }

private:
	/** The center point of the sphere */
	glm::vec3 m_center;
	/** The distance from any point on the sphere to the center */
	float    m_radius;
};

#endif
