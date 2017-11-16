/*
Copyright 2011 Etay Meiri

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIGHTS_COMMON_H
#define	LIGHTS_COMMON_H

#include <glm/glm.hpp>
#include <string>

class BaseLight
{
public:
	std::string Name;
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = glm::vec3(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}

	//virtual void AddToATB(TwBar *bar);
};


class DirectionalLight : public BaseLight
{
public:
	glm::vec3 Direction;

	DirectionalLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//virtual void AddToATB(TwBar *bar);
};


struct LightAttenuation
{
	float Constant;
	float Linear;
	float Exp;

	LightAttenuation()
	{
		Constant = 1.0f;
		Linear = 0.0f;
		Exp = 0.0f;
	}
};


class PointLight : public BaseLight
{
public:
	glm::vec3 Position;
	LightAttenuation Attenuation;

	PointLight()
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//virtual void AddToATB(TwBar *bar);
};


class SpotLight : public PointLight
{
public:
	glm::vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}

	//virtual void AddToATB(TwBar *bar);
};


#define COLOR_WHITE Vector3f(1.0f, 1.0f, 1.0f)
#define COLOR_RED Vector3f(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN Vector3f(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN Vector3f(0.0f, 1.0f, 1.0f)
#define COLOR_BLUE Vector3f(0.0f, 0.0f, 1.0f)

#endif
