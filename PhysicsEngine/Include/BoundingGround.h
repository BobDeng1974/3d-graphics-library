#ifndef BOUNDING_GROUND_H
#define BOUNDING_GROUND_H
#include <glm/glm.hpp>

#include "Collider.h"

class BoundingGround : public Collider
{
public:
	BoundingGround(const glm::vec3& center, const float length, const float width) :
		Collider(Collider::TYPE_GROUND),
		m_center(center),
		m_length(length),
		m_width(width) {
	}

	virtual void SetCenter(const glm::vec3& center) {}
	virtual glm::vec3 GetCenter() const { return m_center; }
	float GetLenght() const { return m_length; };
	float GetWidth() const { return m_width; }
private:
	glm::vec3 m_center;
	float m_length;
	float m_width;
};

#endif // !BOUNDING_GROUND_H
