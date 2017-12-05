#include <iostream>

#include "Collider.h"
#include "BoundingSphere.h"
#include "BoundingGround.h"
#include "Utils.h"

IntersectData Collider::Intersect(const Collider& other) const
{
	if (m_type == TYPE_GROUND) {
		/* If this object is ground do nothing. We don't want ground to move. */
		return IntersectData(false, glm::vec3(0));
	}
	else if (m_type == TYPE_SPHERE && other.GetType() == TYPE_SPHERE)
	{
		BoundingSphere* self = (BoundingSphere*)this;
		return self->IntersectBoundingSphere((BoundingSphere&)other);
	}
	else if (m_type == TYPE_SPHERE && other.GetType() == TYPE_GROUND) 
	{
		BoundingSphere* self = (BoundingSphere*)this;
		return self->IntersectBoundingGround((BoundingGround&)other);
	}


	OGLDEV_ERROR0("Error: Collisions not implemented between specified ");

	//Control should never reach this point
	return IntersectData(false, glm::vec3(0));
}

