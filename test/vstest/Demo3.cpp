#include <stdio.h>
#include <string>

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Pipeline.h"
#include "GlutBackend.h"
#include "Texture.h"
#include "CallBack.h"
#include "App.h"
#include "BasicLightingTechnique.h"
#include "Mesh.h"
#include "SkyBox.h"
#include "PhysicsEngine.h"
#include "BoundingSphere.h"


#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200


class Lab3 : public ICallbacks, public OgldevApp
{
public:

	Lab3()
	{
		m_pPhysicsEngine = NULL;

		m_pLightingTechnique = NULL;
		m_pGameCamera = NULL;

		m_pBallMesh = NULL;
		m_pGroundMesh = NULL;

		m_scale = 0.0f;
		m_pSkyBox = NULL;

		m_dirLight.AmbientIntensity = 0.2f;
		m_dirLight.DiffuseIntensity = 0.8f;
		m_dirLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_dirLight.Direction = glm::vec3(1.0f, -1.0f, 0.0f);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;

		prevTime = 0.0f;
		start = false;
	}


	virtual ~Lab3()
	{
		SAFE_DELETE(m_pPhysicsEngine);
		SAFE_DELETE(m_pLightingTechnique);
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pBallMesh);
		SAFE_DELETE(m_pSkyBox);
		SAFE_DELETE(m_pGroundMesh);
	}


	bool Init()
	{
		glm::vec3 Pos(0.0f, 10.0f, -40.0f);
		glm::vec3 Target(0.0f, 0.0f, 1.0f);
		glm::vec3 Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pLightingTechnique = new BasicLightingTechnique();

		if (!m_pLightingTechnique->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pLightingTechnique->Enable();
		m_pLightingTechnique->SetDirectionalLight(m_dirLight);
		m_pLightingTechnique->SetColorTextureUnit(0);

		m_pBallMesh = new Mesh();
		if (!m_pBallMesh->LoadMesh("Content/sphere.obj")) {
			return false;
		}

		m_pGroundMesh = new Mesh();
		if (!m_pGroundMesh->LoadMesh("Content/quad2.obj")) {
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

		/* Init physic world */
		m_pPhysicsEngine = new PhysicsEngine();

		/* Ball 1 */
		glm::vec3 pos1(15.0f, 10.0f, 0.0f);
		glm::vec3 vol1(-2.0f, 0.0f, 0.0f);
		Collider* pCollider1 = new BoundingSphere(pos1, 1.0f);
		PhysicsObject obj1(pCollider1, vol1, m_pPhysicsEngine->G);

		/* Ball 2 */
		glm::vec3 pos2(0.0f, 10.0f, 0.0f);
		glm::vec3 vol2(2.5f, 0.0f, 0.0f);
		Collider* pCollider2 = new BoundingSphere(pos2, 1.0f);
		PhysicsObject obj2(pCollider2, vol2, m_pPhysicsEngine->G);

		/* Ball 3 */
		glm::vec3 pos3(5.0f, 21.0f, 2.0f);
		glm::vec3 vol3(2.5f, 1.0f, 0.0f);
		Collider* pCollider3 = new BoundingSphere(pos3, 1.0f);
		PhysicsObject obj3(pCollider3, vol3, m_pPhysicsEngine->G);


		/* Ground */
		glm::vec3 pos_g(0.0f, -15.0f, 0.0f);
		glm::vec3 vol_g(0.0f, 0.0f, 0.0f);
		Collider* pCollider_g = new BoundingGround(pos_g, 58.0f, 58.0f);
		PhysicsObject objg(pCollider_g, vol_g, glm::vec3(0.0f));


		m_pPhysicsEngine->AddObject(obj1);
		m_pPhysicsEngine->AddObject(obj2);
		m_pPhysicsEngine->AddObject(obj3);
		m_pPhysicsEngine->AddObject(objg);


		return true;
	}


	void Run()
	{
		GLUTBackendRun(this);
	}


	virtual void RenderSceneCB()
	{

		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pLightingTechnique->Enable();

		Pipeline p;
		p.SetPerspectiveProj(m_persProjInfo);

		float RunningTime = GetRunningTime();

		/* In case of fast frame. */
		if (start) {
			float delta = RunningTime - prevTime;
			m_pPhysicsEngine->HandleCollisions(RunningTime);
			m_pPhysicsEngine->Simulate(delta);
		}
		prevTime = RunningTime;

		for (unsigned int i = 0; i < m_pPhysicsEngine->GetNumObjects(); i++) {
			PhysicsObject obj = m_pPhysicsEngine->GetObject(i);
			p.Scale(1.0f, 1.0f, 1.0f);
			p.Rotate(0.0f, 0.0f, 0.0f);
			p.WorldPos(obj.GetPosition());
			p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
			m_pLightingTechnique->SetWVP(p.GetWVPTrans());
			m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
			if (obj.GetCollider().GetType() == Collider::TYPE_SPHERE) {
				m_pBallMesh->Render();
			}
			else if (obj.GetCollider().GetType() == Collider::TYPE_GROUND) {
				m_pGroundMesh->Render();
			}
		}




		m_pSkyBox->Render();

		glutSwapBuffers();
	}


	void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		case OGLDEV_KEY_f:
			start = true;
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

	PhysicsEngine* m_pPhysicsEngine;
	BasicLightingTechnique* m_pLightingTechnique;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_dirLight;
	Mesh* m_pBallMesh;
	Mesh* m_pGroundMesh;
	SkyBox* m_pSkyBox;
	PersProjInfo m_persProjInfo;
	float prevTime;
	bool start;
};


int main(int argc, char** argv)
{
	GLUTBackendInit(argc, argv, true, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Lab - 3")) {
		return 1;
	}

	Lab3* pApp = new Lab3();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}