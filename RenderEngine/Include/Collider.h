#ifndef COLLIDER_INCLUDED_H
#define COLLIDER_INCLUDED_H

#include "IntersectData.h"
#include <glm/glm.hpp>

/**
* The Collider class is the base class for colliders that can be used in the
* physics engine. More specific colliders should inherit from this class
*/
class Collider {
public:
	/**
	* This enumeration stores all the types of colliders that can be used.
	*/
	enum
	{
		TYPE_SPHERE,
		TYPE_AABB,
		TYPE_GROUND
	};

	/**
	* Creates a Collider in a usable state.
	*
	* @param type The type of collider this represents.
	*/
	Collider(int type) {
		m_type = type;
	}
	/**
	* Calculates information about if this collider is intersecting with
	* another collider.
	*
	* @param other The collider that is being checked for intersection.
	*/
	IntersectData Intersect(const Collider& other) const;

	/**
	* Moves the entire collider by translation distance. Should be overriden
	* by subclasses.
	*
	* @param translation Distance to move the collider
	*/
	virtual void SetCenter(const glm::vec3& center) {}
	/**
	* Returns the center position of the collider. Should be overriden by
	* subclasses.
	*/
	virtual  glm::vec3 GetCenter() const { return glm::vec3(0, 0, 0); }

	/** Basic getter */
	inline int GetType() const { return m_type; }
private:
	/**
	* What type of collider this is. Should be set to some value from the
	* TYPE enum. This value determines what sort of intersect checks are used
	* and other details about the collider; should only be set by an appropriate
	* subclass or strange behaviour may result!
	*/
	int m_type;
};

#endif