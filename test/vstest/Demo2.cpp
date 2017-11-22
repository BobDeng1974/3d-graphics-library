
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif
#include <sys/types.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Pipeline.h"
#include "GlutBackend.h"
#include "Texture.h"
#include "CallBack.h"
#include "App.h"
#include "Mesh.h"
#include "SkyBox.h"
#include "Engine.h"
#include "SkinnedMesh.h"
#include "SkinnedTechnique.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class Lab2 : public ICallbacks, public OgldevApp
{
public:

	Lab2()
	{
		m_pGameCamera = NULL;
		m_pEffect = NULL;
		m_pSkyBox = NULL;
		m_pMesh = NULL;
		m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.55f;
		m_directionalLight.DiffuseIntensity = 0.9f;
		m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;

		m_position = glm::vec3(6.0f, 0.0f, 6.0f);
		m_scale = m_position.z;
	}

	~Lab2()
	{
		SAFE_DELETE(m_pEffect);
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pSkyBox);
		SAFE_DELETE(m_pMesh);
	}

	bool Init()
	{
		glm::vec3 Pos(0.0f, 3.0f, -1.0f);
		glm::vec3 Target(6.0f, 0.0f, 6.0f);
		glm::vec3 Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pEffect = new SkinningTechnique();

		if (!m_pEffect->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_pEffect->SetDirectionalLight(m_directionalLight);
		m_pEffect->SetMatSpecularIntensity(0.0f);
		m_pEffect->SetMatSpecularPower(0);
		m_pMesh = new SkinnedMesh();
		if (!m_pMesh->LoadMesh("Content/model.dae")) {
			printf("Mesh load failed\n");
			return false;
		}

		m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);

		if (!m_pSkyBox->Init(".",
			"Content/orbital/orbital-element_ft.png",
			"Content/orbital/orbital-element_bk.png",
			"Content/orbital/orbital-element_up.png",
			"Content/orbital/orbital-element_dn.png",
			"Content/orbital/orbital-element_rt.png",
			"Content/orbital/orbital-element_lf.png")) {
			return false;
		}

#ifndef WIN32
		if (!m_fontRenderer.InitFontRenderer()) {
			return false;
		}
#endif        	
		return true;
	}

	void Run()
	{
		GLUTBackendRun(this);
	}


	virtual void RenderSceneCB()
	{
		CalcFPS();

		m_pGameCamera->OnRender();
		m_scale -= 0.001f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pEffect->Enable();

		vector<glm::mat4> Transforms;
		float RunningTime = GetRunningTime();

		m_pMesh->BoneTransform(RunningTime, Transforms);
		
		for (uint i = 0; i < Transforms.size(); i++) {
			m_pEffect->SetBoneTransform(i, Transforms[i]);
		}

		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

		Pipeline p;
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		p.Scale(0.1f, 0.1f, 0.1f);

		m_position.z = m_scale;
		glm::vec3 Pos(m_position);
		p.WorldPos(Pos);
		p.Rotate(270.0f, 0.0, 180.0f);
		m_pEffect->SetWVP(p.GetWVPTrans());
		m_pEffect->SetWorldMatrix(p.GetWorldTrans());

		m_pMesh->Render();
		m_pSkyBox->Render();

		//RenderFPS();

		glutSwapBuffers();
	}


	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		default:
			m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}


	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}


private:

	SkinningTechnique* m_pEffect;
	Camera* m_pGameCamera;
	DirectionalLight m_directionalLight;
	SkinnedMesh* m_pMesh;
	SkyBox* m_pSkyBox;
	glm::vec3 m_position;
	PersProjInfo m_persProjInfo;
	float m_scale;
};


int main(int argc, char** argv)
{
	GLUTBackendInit(argc, argv, true, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Lab - 2")) {
		return 1;
	}

	SRANDOM;

	Lab2* pApp = new Lab2();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}