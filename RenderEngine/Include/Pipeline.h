/*
Copyright 2010 Etay Meiri

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

#ifndef PIPELINE_H
#define	PIPELINE_H

#include <glm/glm.hpp>

#include "Camera.h"



class Pipeline
{
public:
	Pipeline()
	{
		m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void Scale(float s)
	{
		Scale(s, s, s);
	}


	void Scale(const glm::vec3& scale)
	{
		Scale(scale.x, scale.y, scale.z);
	}

	void Scale(float ScaleX, float ScaleY, float ScaleZ)
	{
		m_scale.x = ScaleX;
		m_scale.y = ScaleY;
		m_scale.z = ScaleZ;
	}

	void WorldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void WorldPos(const glm::vec3& Pos)
	{
		m_worldPos = Pos;
	}

	void Rotate(float RotateX, float RotateY, float RotateZ)
	{
		m_rotateInfo.x = RotateX;
		m_rotateInfo.y = RotateY;
		m_rotateInfo.z = RotateZ;
	}

	void Rotate(const glm::vec3& r)
	{
		Rotate(r.x, r.y, r.z);
	}

	void SetPerspectiveProj(const PersProjInfo& p)
	{
		m_persProjInfo = p;
	}

	void SetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
	{
		m_camera.Pos = Pos;
		m_camera.Target = Target;
		m_camera.Up = Up;
	}

	void SetCamera(const Camera& camera)
	{
		SetCamera(camera.GetPos(), camera.GetTarget(), camera.GetUp());
	}





	const glm::mat4& GetWorldTrans();
	const glm::mat4& GetProjTrans();
	const glm::mat4& GetViewTrans();

	const glm::mat4& GetVPTrans();
	const glm::mat4& GetWVPTrans();
	const glm::mat4& GetWPTrans();

private:
	glm::vec3 m_scale;
	glm::vec3 m_worldPos;
	glm::vec3 m_rotateInfo;

	glm::mat4 m_Wtransformation;
	glm::mat4 m_Ptransformation;
	glm::mat4 m_Vtransformation;
	glm::mat4 m_WPtransformation;
	glm::mat4 m_VPtransformation;
	glm::mat4 m_WVPtransformation;


	PersProjInfo m_persProjInfo;

	struct {
		glm::vec3 Pos;
		glm::vec3 Target;
		glm::vec3 Up;
	} m_camera;
};


#endif	/* PIPELINE_H */

