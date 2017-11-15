
#include "Pipeline.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


const glm::mat4& Pipeline::GetWorldTrans() {
	glm::mat4 ScaleTrans(1.0), RotateTrans(1.0), TranslationTrans(1.0);

	ScaleTrans = glm::scale(ScaleTrans, m_scale);

	RotateTrans = glm::rotate(RotateTrans, glm::radians(m_rotateInfo.x), glm::vec3(1, 0, 0));
	RotateTrans = glm::rotate(RotateTrans, glm::radians(m_rotateInfo.y), glm::vec3(0, 1, 0));
	RotateTrans = glm::rotate(RotateTrans, glm::radians(m_rotateInfo.z), glm::vec3(0, 0, 1));

	TranslationTrans = glm::translate(m_worldPos);

	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	return m_Wtransformation;
}

const glm::mat4& Pipeline::GetProjTrans()
{
	// why can't use it?
	//PersProjTrans = glm::perspectiveFov(glm::radians(m_persProjInfo.FOV), m_persProjInfo.Width, m_persProjInfo.Height, m_persProjInfo.zNear, m_persProjInfo.zFar);
	
	const float ar = m_persProjInfo.Width / m_persProjInfo.Height;
	const float zNear = m_persProjInfo.zNear;
	const float zFar = m_persProjInfo.zFar;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(glm::radians(m_persProjInfo.FOV / 2.0));

	glm::mat4 PersProjTrans;
	PersProjTrans[0][0] = 1.0f / (tanHalfFOV * ar);
	PersProjTrans[1][1] = 1.0f / tanHalfFOV;
	PersProjTrans[2][2] = (-zNear - zFar) / zRange;
	PersProjTrans[3][2] = 2.0f * zFar * zNear / zRange;
	PersProjTrans[2][3] = 1.0f;
	m_Ptransformation = PersProjTrans;

	return m_Ptransformation;
}

const glm::mat4& Pipeline::GetViewTrans()
{
	glm::mat4 CameraTranslationTrans, CameraRotateTrans;

	CameraTranslationTrans = glm::translate(glm::vec3(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z));

	glm::vec3 N = glm::normalize(m_camera.Target);
	glm::vec3 U = m_camera.Up;
	U = glm::normalize(glm::cross(U, (m_camera.Target)));
	glm::vec3 V = glm::cross(N, U);

	CameraRotateTrans[0][0] = U.x; CameraRotateTrans[1][0] = U.y; CameraRotateTrans[2][0] = U.z; 
	CameraRotateTrans[0][1] = V.x; CameraRotateTrans[1][1] = V.y; CameraRotateTrans[2][1] = V.z; 
	CameraRotateTrans[0][2] = N.x; CameraRotateTrans[1][2] = N.y; CameraRotateTrans[2][2] = N.z; 
	CameraRotateTrans[3][3] = 1.0f;

	m_Vtransformation = CameraRotateTrans * CameraTranslationTrans;

	return m_Vtransformation;
}


const glm::mat4& Pipeline::GetWPTrans()
{
	GetWorldTrans();
	GetProjTrans();

	m_WPtransformation = m_Ptransformation * m_Wtransformation;
	return m_WPtransformation;
}


const glm::mat4& Pipeline::GetVPTrans()
{
	GetViewTrans();
	GetProjTrans();

	m_VPtransformation = m_Ptransformation * m_Vtransformation;
	return m_VPtransformation;
}

const glm::mat4& Pipeline::GetWVPTrans() {

	GetWorldTrans();
	GetVPTrans();

	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	return m_WVPtransformation;
}