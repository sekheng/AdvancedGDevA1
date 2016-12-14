#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SpatialPartition\QuadTree.h"
#include "SceneGraph\UpdateTransformation.h"
#include "SceneGraph\UpdateRotation.h"
#include "SceneGraph/SceneGraph.h"
#include "WeaponInfo\Projectile.h"

#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	SceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player



	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));


	GraphicsManager::GetInstance()->AttachCamera(&camera);
    playerInfo = new CPlayerInfo();
    playerInfo->AttachCamera(&camera);

    boundaryOfScene = new EntityBase();
    boundaryOfScene->SetPosition(Vector3(0, 0, 0));
    boundaryOfScene->SetScale(Vector3(1000, 1000, 1000));
    boundaryOfScene->setName("Boundary");

	// Create entities into the scene
    m_activeList.push_back(Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f))); // Reference
    m_activeList.push_back(Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z))); // Lightball
	GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f), Vector3(20.f,20.f,20.f));
    aCube->setName("cube0");
	aCube->SetCollider(true);
	//aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
    m_activeList.push_back(aCube);

    aCube = Create::Entity("cube", Vector3(20.0f, 0.0f, -20.0f));
    aCube->setName("cube1");
    aCube->SetCollider(true);
    //aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
    m_activeList.push_back(aCube);
    aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, 20.0f));
    aCube->setName("cube2");
    aCube->SetCollider(true);
    //aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
    m_activeList.push_back(aCube);
    aCube = Create::Entity("cube", Vector3(20.0f, 0.0f, 20.0f));
    aCube->setName("cube3");
    aCube->SetCollider(true);
    //aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
    m_activeList.push_back(aCube);

	//debuging for scene graph, START
	GenericEntity* baseCube = Create::Entity("ASTEROID", Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f));
    baseCube->setName("cube4");
	SceneNode* baseNode = SceneGraph::GetInstance()->AddNode(baseCube);

	UpdateTransformation* baseMtx = new UpdateTransformation();
	UpdateRotation* baseMtx2 = new UpdateRotation();
	baseMtx->ApplyUpdate(1.f, 0.0f, 0.0f);
	baseMtx->SetSteps(-60, 60);
	baseMtx2->ApplyUpdate(1.f, 0.0f, 0.0f,1.f);
	baseMtx2->SetSteps(-60, 60);
	baseNode->SetUpdateTransformation(baseMtx);
	

	GenericEntity* childCube = Create::Entity("ASTEROID", Vector3(0.0f, 2.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f));
    childCube->setName("cube5");
	SceneNode* childNode = baseNode->AddChild(childCube);
	childNode->SetUpdateRotation(baseMtx2);

	GenericEntity* grandchildCube = Create::Entity("ASTEROID", Vector3(0.0f, -2.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f));
    grandchildCube->setName("cube6");
	SceneNode* grandchildNode = childNode->AddChild(grandchildCube);

	m_activeList.push_back(baseCube);
	m_activeList.push_back(childCube);
	m_activeList.push_back(grandchildCube);
	//debuging for scene graph, END

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	
	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));


	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 50.f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 2; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
    // For Score
    textObj[2] = Create::Text2DObject("text", Vector3(-halfWindowWidth, halfWindowHeight - halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.f, 1.f, 0.f));
    // For Num of Bullets and Clips
    textObj[3] = Create::Text2DObject("text", Vector3(-halfWindowWidth, halfWindowHeight - fontSize - halfFontSize, 0), "", Vector3(fontSize, fontSize, fontSize), Color(0, 1, 0));
    // For TimeLeft
    textObj[4] = Create::Text2DObject("text", Vector3(-200, halfWindowHeight - fontSize, 0), "", Vector3(fontSize + halfFontSize, fontSize + halfFontSize, fontSize), Color(0, 1, 0));

    spatialPartition = new QuadTree();
    spatialPartition->SetScale(Vector3(1000,1000,1000));
    spatialPartition->SetPosition(Vector3(0, 1, 0));
    QuadTree *zeQuadTree = dynamic_cast<QuadTree*>(spatialPartition);
    for (std::vector<GenericEntity*>::iterator it = m_activeList.begin(), end = m_activeList.end(); it != end; ++it)
    {
        //zeQuadTree->m_objectList.push_back(*it);
        spatialPartition->onNotify(**it);
    }
    //zeQuadTree->Update(0);
    spatialPartition->Update(0);

    for (size_t num = 0; num < 100; ++num)
    {
        m_inactiveList.push_back(new Projectile());
        m_inactiveList.back()->onNotify(*boundaryOfScene);
    }
    score_ = 0;
    timeLeft_Second = 60;
    currGameState = PLAYING;
}

void SceneText::Update(double dt)
{
	// Update our entities
    switch (currGameState)
    {
    case PLAYING:
        timeLeft_Second -= (float)(dt);
        if (timeLeft_Second <= Math::EPSILON)
        {
            timeLeft_Second = 0;
            currGameState = GAME_OVER;
        }
        break;
    case GAME_OVER:
        break;
    default:
        break;
    }
	EntityManager::GetInstance()->Update(dt);
    for (std::vector<GenericEntity*>::iterator it = m_activeList.begin(), end = m_activeList.end(); it != end; ++it)
    {
        (*it)->Update(dt);
        if ((*it)->IsDone())
            waitingListToBeRemoved.push_back(it - m_activeList.begin());
    }
    if (!waitingListToBeRemoved.empty())
    {
        for (std::vector<size_t>::reverse_iterator rit = waitingListToBeRemoved.rbegin(), rend = waitingListToBeRemoved.rend(); rit != rend; ++rit)
        {
            m_inactiveList.push_back(m_activeList[*rit]);
            m_activeList.erase(m_activeList.begin() + (*rit));
        }
        waitingListToBeRemoved.clear();
    }
	SceneGraph::GetInstance()->Update();
    spatialPartition->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	//{
	//	cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	//}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	//{
	//	cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	//}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs


	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)
    playerInfo->Update(dt);

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[0]->SetText(ss.str());

	// Update the player position into textObj[2]
	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetCurrCamera().GetCameraPos();
	textObj[1]->SetText(ss1.str());

    ss1.str("");
    ss1 << "Total Score:" << score_;
    textObj[2]->SetText(ss1.str());

    ss1.str("");
    ss1 << playerInfo->getCurrNumBullet() << "|" << playerInfo->getCurrNumClips();
    textObj[3]->SetText(ss1.str());

    ss1.str("");
    ss1 << "TimeLeft:" << timeLeft_Second;
    textObj[4]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();
	SceneGraph::GetInstance()->Render();
    spatialPartition->Render();
    for (std::vector<GenericEntity*>::iterator it = m_activeList.begin(), end = m_activeList.end(); it != end; ++it)
    {
		if (SceneGraph::GetInstance()->GetNode(*it) == NULL)
			(*it)->Render();
    }


	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();



    for (std::vector<GenericEntity*>::iterator it = m_activeList.begin(), end = m_activeList.end(); it != end; ++it)
    {
        delete (*it);
    }
    m_activeList.clear();
    for (std::vector<GenericEntity*>::iterator it = m_inactiveList.begin(), end = m_inactiveList.end(); it != end; ++it)
        delete (*it);
    m_inactiveList.clear();
    delete spatialPartition;
    delete boundaryOfScene;

	// Delete the lights
	delete lights[0];
	delete lights[1];
}

bool SceneText::onNotify(const std::string &zeEvent)
{
    if (zeEvent.find("FIRE_BULLET") != std::string::npos)
    {
        EntityBase *zeBullet = nullptr;
        for (std::vector<GenericEntity*>::iterator it = m_inactiveList.begin(), end = m_inactiveList.end(); it != end; ++it)
        {
            if ((*it)->getName().find("Projectile") != std::string::npos)
            {
                zeBullet = *it;
                m_activeList.push_back(*it);
                m_inactiveList.erase(it);
                spatialPartition->onNotify(*zeBullet);
                break;
            }
        }
        return zeBullet->onNotify(playerInfo->GetCurrCamera().GetCameraPos(), playerInfo->GetCurrCamera().GetCameraTarget());
    }
    else if (zeEvent.find("SCORE") != std::string::npos)
    {
        size_t posOfColon = zeEvent.find(":");
        score_ += stoi(zeEvent.substr(posOfColon + 1));
        return true;
    }
    return false;
}