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

#include "WeaponInfo\Projectile.h"
#include "MyMath.h"
#include "MusicsStuff\MusicSystem.h"


#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());
static size_t ROCK_ID = 0;
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

    num_ofAsteroidsLeft = 0;

    boundaryOfScene = new EntityBase();
    boundaryOfScene->SetPosition(Vector3(0, 0, 0));
    boundaryOfScene->SetScale(Vector3(1000, 1000, 1000));
    boundaryOfScene->setName("Boundary");

	// Create entities into the scene
    //EntityManager::GetInstance()->AddEntity(Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f))); // Reference
    //EntityManager::GetInstance()->AddEntity(Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z))); // Lightball
	//GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f), Vector3(20.f,20.f,20.f));
 //   aCube->setName("cube0");
	//aCube->SetCollider(true);
	////aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
 //   m_activeList.push_back(aCube);

 //   aCube = Create::Entity("cube", Vector3(20.0f, 0.0f, -20.0f));
 //   aCube->setName("cube1");
 //   aCube->SetCollider(true);
 //   //aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
 //   m_activeList.push_back(aCube);
 //   aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, 20.0f));
 //   aCube->setName("cube2");
 //   aCube->SetCollider(true);
 //   //aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
 //   m_activeList.push_back(aCube);
 //   aCube = Create::Entity("cube", Vector3(20.0f, 0.0f, 20.0f));
 //   aCube->setName("cube3");
 //   aCube->SetCollider(true);
 //   //aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
 //   m_activeList.push_back(aCube);

	//debuging for scene graph, START

	CreateSatelite(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));

	// I am testing out scene graph animation
	CreatePlanet(Vector3(100, 0, 0), Vector3(25, 25, 25));

	spatialPartition = new QuadTree();

	
	//CreateAsteroid(Vector3(Math::RandIntMinMax(-50, 50), 0, Math::RandIntMinMax(-50, 50)), Vector3(1, 1, 1));
	
	for (int i = 0; i < 2; ++i)
	{
		CreateSatelite2(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 5, boundaryOfScene->GetScale().x / 5), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 5, boundaryOfScene->GetScale().z / 5)), Vector3(1, 1, 1));
	}
	
	//debuging for scene graph, END

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	theShip = Create::Entity("SHIP_1", Vector3(0, 0, 0));
	theShip->InitLOD("SHIP_1", "SHIP_2", "SHIP_3");
	theShip->onNotify(200.f, 400.f);
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 100.0f));

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
	//satelite info
	textObj[7] = Create::Text2DObject("text", Vector3(-halfWindowWidth, halfWindowHeight - (fontSize * 3) - halfFontSize, 0), "", Vector3(fontSize, fontSize, fontSize), Color(0, 1, 0));
    // For TimeLeft
    textObj[4] = Create::Text2DObject("text", Vector3(-400, halfWindowHeight - fontSize - 50, 0), "", Vector3(fontSize + halfFontSize, fontSize + halfFontSize, fontSize), Color(0, 1, 0));
    // For GameOver Screen
    textObj[5] = Create::Text2DObject("text", Vector3(-halfWindowWidth, 0, 0), "", Vector3(fontSize , fontSize , fontSize), Color(0, 1, 0));
    textObj[6] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -fontSize * 2, 0), "", Vector3(fontSize * 2, fontSize * 2, fontSize), Color(0, 1, 0));
    // For GameOver Screen

    
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

	CreateAsteroid(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
	CreateAsteroid(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
	CreateAsteroid(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));

    for (size_t num = 0; num < 100; ++num)
    {
        Projectile *zeBullet = new Projectile();
        zeBullet->onNotify(100.f, 300.f);
        zeBullet->InitLOD("BULLET1", "BULLET2", "BULLET3");
        m_inactiveList.push_back(zeBullet);
        m_inactiveList.back()->onNotify(*boundaryOfScene);
        zeBullet->onNotify(m_activeList);
    }
    health_ = 100;
    timeLeft_Second = 120;
	timeTillSateliteRDY = 5;
    currGameState = PLAYING;
	startAnimation = false;
    playerInfo->setBoundary(boundaryOfScene->GetScale());

	theGun = Create::Gun("Gun", Vector3(playerInfo->GetCurrCamera().GetCameraPos().x, playerInfo->GetCurrCamera().GetCameraPos().y, playerInfo->GetCurrCamera().GetCameraPos().z));
    DisplaySpatialPartition = false;
}

void SceneText::Update(double dt)
{
    MusicSystem::accessing().Update(dt);
	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	//vector<EntityBase*> listToDelete;
    for (std::vector<GenericEntity*>::iterator it = m_activeList.begin(), end = m_activeList.end(); it != end; ++it)
    {
		if ((*it)->getName().find("MAIN") != std::string::npos)
		{
			spatialPartition->onNotify(**it);
			
		}
		(*it)->Update(dt);
		if ((*it)->getName().find("Projectile") == std::string::npos)
			(*it)->onNotify(playerInfo->GetCurrCamera().GetCameraPos(), playerInfo->GetCurrCamera().GetCameraTarget());
		if ((*it)->GetLODStatus() == true)
		{
			(*it)->onNotify(playerInfo->GetCurrCamera().GetCameraPos());
		}
		if ((*it)->IsDone())
		{
			(*it)->removeItselfFromQuad();
			waitingListToBeRemoved.push_back(it - m_activeList.begin());
			if (!(*it)->IsShot())
			{
				health_ -= 10;
			}
			if ((*it)->getName().find("rock") != std::string::npos && num_ofAsteroidsLeft > 0)
				--num_ofAsteroidsLeft;

			if ((*it)->getName().find("SPECIAL") != std::string::npos)
			{
				SceneNode* temp = SceneGraph::GetInstance()->GetNode(*it)->GetParent();
				
				temp->deleteUpdateTransform();
				UpdateTransformation* baseMtx = new UpdateTransformation();

				Vector3 dist = Vector3(0, 0, 0) - temp->getRealPosition();
				//baseMtx->ApplyUpdate(Math::RandFloatMinMax(0.01f, 0.05f), 0.0f, Math::RandFloatMinMax(0.01f, 0.05f));	  
				baseMtx->ApplyUpdate(dist.Normalized().x / 2, dist.Normalized().y / 2, dist.Normalized().z / 2);
				baseMtx->SetSteps(0, 400);
				baseMtx->setContinue(true);

				temp->SetUpdateTransformation(baseMtx);
				
			}
			
			/*for (std::vector<SceneNode*>::iterator it2 = SceneGraph::GetInstance()->GetRoot()->getTheChildren().begin(), end = SceneGraph::GetInstance()->GetRoot()->getTheChildren().end(); it2 != end; ++it2)
			{
				if ((*it) == (*it2)->GetEntity())
				{
					listToDelete.push_back((*it2)->GetEntity());
					
				}
			}*/
			
		}
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

	for (std::vector<GenericEntity*>::iterator it = listOfOBJForLOD.begin(), end = listOfOBJForLOD.end(); it != end; ++it)
	{
		if ((*it)->GetLODStatus() == true)
		{
			(*it)->onNotify(playerInfo->GetCurrCamera().GetCameraPos());
		}
		if ((*it)->getName().find("Planet") == std::string::npos)
		{
			(*it)->onNotify(playerInfo->GetCurrCamera().GetCameraPos(), playerInfo->GetCurrCamera().GetCameraTarget());
		}
	}
	/*for (std::vector<EntityBase*>::iterator it3 = listToDelete.begin(), end = listToDelete.end(); it3 != end; ++it3)
	{
		SceneGraph::GetInstance()->GetNode((*it3))->DeleteAllChildren();
	}*/
	theShip->onNotify(playerInfo->GetCurrCamera().GetCameraPos());
	theShip->onNotify(playerInfo->GetCurrCamera().GetCameraPos(), playerInfo->GetCurrCamera().GetCameraTarget());
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
    if (KeyboardController::GetInstance()->IsKeyDown('Y'))
        DisplaySpatialPartition = !DisplaySpatialPartition;

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
	theGun->onNotify(playerInfo->GetCurrCamera().GetCameraPos(), playerInfo->GetCurrCamera().GetCameraRotation());
	theGun->onNotify(playerInfo->GetCurrCamera().GetCameraRight());
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

    switch (currGameState)
    {
    case PLAYING:
        timeLeft_Second -= (float)(dt);
        if (timeLeft_Second <= Math::EPSILON || health_ <= Math::EPSILON)
        {
			float timeSurvived = 120 - timeLeft_Second;
			if (timeSurvived <= Math::EPSILON)
				timeSurvived = 0;
            timeLeft_Second = 0;
            currGameState = GAME_OVER;
            textObj[2]->SetText("");

            textObj[3]->SetText("");

            textObj[4]->SetText("");
            ss1.str("");
			ss1 << "Total Time survived:" << timeSurvived;
            textObj[5]->SetText(ss1.str());
            textObj[6]->SetText("Press R to retry");
        }
        else
        {
            ss1.str("");
            ss1 << "Ship's health:" << health_;
            textObj[2]->SetText(ss1.str());

            ss1.str("");
            ss1 << playerInfo->getCurrNumBullet() << "|" << playerInfo->getCurrNumClips();
            textObj[3]->SetText(ss1.str());

            ss1.str("");
            ss1 << "TimeLeft:" << timeLeft_Second;
            textObj[4]->SetText(ss1.str());

			ss1.str("");
			ss1 << "Time Till Satelite is ready:" << timeTillSateliteRDY;
			textObj[7]->SetText(ss1.str());
			//int difficulty = 16 - (int)(timeLeft_Second / 10);
			
			if (num_ofAsteroidsLeft <= 2)
			{
				CreateAsteroid(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
			}
			int numOfChild = SceneGraph::GetInstance()->GetRoot()->getTheChildren().size();
			for (int i = 0; i < numOfChild; ++i)
			{
				SceneNode* temp = SceneGraph::GetInstance()->GetRoot()->getTheChildren()[i];
				/*if (temp->getRealPosition().x > boundaryOfScene->GetScale().x /10
					|| temp->getRealPosition().x < -boundaryOfScene->GetScale().x / 10
					|| temp->getRealPosition().z > boundaryOfScene->GetScale().z / 10
					|| temp->getRealPosition().z < -boundaryOfScene->GetScale().z / 10)*/
				if ((Vector3(0, 0, 0) - temp->getRealPosition()).LengthSquared() <= 5)//despawn when hit origin
				{
					//temp->GetEntity()->SetIsDone(true);
                    temp->GetEntity()->onNotify("DESTROY_SHIP");
					/*nodesToDelete.push_back(temp);
					cout << temp->GetEntity()->getName() << endl;
					cout << temp->GetEntity()->GetPosition() << endl;*/
					//int dmg = temp->getTheChildren().size() + 1;
					//health_ -= dmg;
					/*int totalHealthToMinus = temp->GetNumOfChild() + 1;
					health_ -= 2 * totalHealthToMinus;*/
				}
				
			}
			//for (vector<SceneNode*>::iterator it = nodesToDelete.begin(); it != nodesToDelete.end(); ++it)
			//{
			//	//SceneGraph::GetInstance()->GetRoot()->DeleteChild((*it)->GetEntity());
			//	//SceneGraph::GetInstance()->GetNode((*it)->GetEntity())->DeleteAllChildren();
			//	//SceneGraph::GetInstance()->DeleteNode((*it)->GetEntity());
			//}
			Vector3 distToSatellite = baseNode_s->getRealPosition() - playerInfo->GetCurrCamera().GetCameraPos();
			if (distToSatellite.LengthSquared() < 700)
			{
				if (KeyboardController::GetInstance()->IsKeyPressed('Q'))
				{
					if (startAnimation == false)
					{
						sceneGraphAnimation();
						startAnimation = true;
					}
					if (startAnimation == true && timeTillSateliteRDY <= 0)
					{
						//refill ammo
						playerInfo->refillAmmo();
						timeTillSateliteRDY = 5;
					}

				}
				
			}
			if (startAnimation == true && timeTillSateliteRDY > 0)
			{
				timeTillSateliteRDY -= dt;
			}
			if (timeTillSateliteRDY < 0)
			{
				timeTillSateliteRDY = 0;
			}
        }
        break;
	case GAME_OVER:
	{
					  if (KeyboardController::GetInstance()->IsKeyPressed('R'))
					  {
						  resetGame();
					  }
					  int numOfChild2 = SceneGraph::GetInstance()->GetRoot()->getTheChildren().size();
					  for (int i = 0; i < numOfChild2; ++i)
					  {
						  SceneNode* temp = SceneGraph::GetInstance()->GetRoot()->getTheChildren()[i];
						  if (temp->getRealPosition().x > boundaryOfScene->GetScale().x / 10//despawn
							  || temp->getRealPosition().x < -boundaryOfScene->GetScale().x / 10
							  || temp->getRealPosition().z > boundaryOfScene->GetScale().z / 10
							  || temp->getRealPosition().z < -boundaryOfScene->GetScale().z / 10)
						  {
							  temp->GetEntity()->SetIsDone(true);
						  }
					  }
	}
        break;
    default:
        break;
    }

	//int count = SceneGraph::GetInstance()->GetRoot()->getTheChildren().size();
	//vector<EntityBase*> listToDelete;
	//for (int i = 0; i < count; ++i)
	//{
	//	EntityBase* temp = SceneGraph::GetInstance()->GetRoot()->getTheChildren()[i]->GetEntity();
	//	if (temp->IsDone() == true)
	//		//SceneGraph::GetInstance()->DeleteNode(temp);
	//		listToDelete.push_back(temp);
	//}
	//for (std::vector<EntityBase*>::iterator it = listToDelete.begin(), end = listToDelete.end(); it != end; ++it)
	//{
	//	SceneGraph::GetInstance()->GetNode(*it)->DeleteAllChildren();
	//}
	
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
	theGun->Render();
	theShip->Render();
    if (DisplaySpatialPartition)
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
        zeBullet->onNotify(playerInfo->GetCurrCamera().GetCameraRotation());
        return zeBullet->onNotify(playerInfo->GetCurrCamera().GetCameraPos(), playerInfo->GetCurrCamera().GetCameraTarget());
    }
    else if (zeEvent.find("SCORE") != std::string::npos)
    {
        size_t posOfColon = zeEvent.find(":");
        //health_ -= stoi(zeEvent.substr(posOfColon + 1));
        MusicSystem::accessing().playMusic("explode");
        //--num_ofAsteroidsLeft;
        return true;
    }
    else if (zeEvent.find("ASTEROID_DIED") != std::string::npos)
    {
        MusicSystem::accessing().playMusic("explode");
        //--num_ofAsteroidsLeft;
        return true;
    }
    return false;
}

void SceneText::resetGame()
{
    camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
    playerInfo->Reset();
	startAnimation = false;
	timeTillSateliteRDY = 5;
    health_ = 100;
    timeLeft_Second = 120;
    currGameState = PLAYING;
    textObj[5]->SetText("");
    textObj[6]->SetText("");
    ROCK_ID = 0;
    num_ofAsteroidsLeft = 0;
    delete spatialPartition;
    spatialPartition = new QuadTree();
    for (std::vector<GenericEntity*>::iterator it = m_activeList.begin(), end = m_activeList.end(); it != end; ++it)
    {
        if ((*it)->getName().find("Projectile") != std::string::npos)
            m_inactiveList.push_back(*it);
        else
            delete *it;
    }
    m_activeList.clear();
    waitingListToBeRemoved.clear();
    for (std::vector<GenericEntity*>::iterator it = m_inactiveList.begin(), end = m_inactiveList.end(); it != end; ++it)
    {
        if ((*it)->getName().find("Projectile") == std::string::npos)
        {
            waitingListToBeRemoved.push_back(it - m_inactiveList.begin());
            delete *it;
        }
    }
    for (std::vector<size_t>::reverse_iterator rit = waitingListToBeRemoved.rbegin(), rend = waitingListToBeRemoved.rend(); rit != rend; ++rit)
    {
        m_inactiveList.erase(m_inactiveList.begin() + *rit);
    }
    waitingListToBeRemoved.clear();
	SceneGraph::GetInstance()->GetRoot()->DeleteAllChildren();
	CreateAsteroid(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
	CreateAsteroid(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
	CreateAsteroid(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
	
	CreateSatelite(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
	CreatePlanet(Vector3(100, 0, 0), Vector3(25, 25, 25));
	for (int i = 0; i < 2; ++i)
	{
		CreateSatelite2(Vector3(Math::RandFloatMinMax(-boundaryOfScene->GetScale().x / 10, boundaryOfScene->GetScale().x / 10), Math::RandFloatMinMax(2, 3), Math::RandFloatMinMax(-boundaryOfScene->GetScale().z / 10, boundaryOfScene->GetScale().z / 10)), Vector3(1, 1, 1));
	}
	ROCK_ID = 0;
    spatialPartition->onNotify("RESET");
    spatialPartition->SetScale(Vector3(1000, 1000, 1000));
    spatialPartition->SetPosition(Vector3(0, 1, 0));
    for (std::vector<GenericEntity*>::iterator it = m_activeList.begin(), end = m_activeList.end(); it != end; ++it)
    {
        spatialPartition->onNotify(**it);
    }
    spatialPartition->Update(0);
}

void SceneText::CreatePlanet(const Vector3 &zePos, const Vector3 &zeScale)
{
	GenericEntity* dummy = Create::Entity("quad", Vector3(0.0f, 0.0f, 0.0f), Vector3(0.1f, 0.1f, 0.1f));
	dummy->setName("dummy");
	SceneNode* dummyNode = SceneGraph::GetInstance()->AddNode(dummy);
	dummyNode->ApplyTranslate(5, 0, 0);

    GenericEntity* base = Create::Entity("PLANET", Vector3(0.0f, 0.0f, 0.0f), zeScale);
    base->setName("PlanetBase");
	base->InitLOD("PLANET", "PLANET1", "PLANET2");
	base->onNotify(400.f, 800.f);
	SceneNode* baseNode = dummyNode->AddChild(base);
	baseNode->ApplyTranslate(zePos.x + 50, zePos.y + ((zeScale.y * 5)), zePos.z);

    GenericEntity* stand1 = Create::Entity("PLANET_RING", Vector3(0.0f, 0.0f, 0.0f), zeScale * 2.0f);
    stand1->setName("PlanetRing1");
	stand1->InitLOD("PLANET_RING", "PLANET_RING1", "PLANET_RING2");
	stand1->onNotify(400.f, 800.f);
	SceneNode* SecondbaseNode = baseNode->AddChild(stand1);

	GenericEntity* stand2 = Create::Entity("PLANET_RING", Vector3(0.0f, 0.0f, 0.0f), zeScale * 2.0f);
	stand2->setName("PlanetRing1");
	stand2->InitLOD("PLANET_RING", "PLANET_RING1", "PLANET_RING2");
	stand2->onNotify(400.f, 800.f);
	SceneNode* ThirdbaseNode = baseNode->AddChild(stand2);

	listOfOBJForLOD.push_back(base);
	listOfOBJForLOD.push_back(stand1);
	listOfOBJForLOD.push_back(stand2);

	UpdateRotation* rotateWholeSystem = new UpdateRotation();
	UpdateRotation* rotateRing = new UpdateRotation();
	UpdateRotation* rotateSickly = new UpdateRotation();
	UpdateRotation* rotateSickly2 = new UpdateRotation();
	rotateWholeSystem->ApplyUpdate(1, 0, 1, 0);
	rotateWholeSystem->SetSteps(0, 60);
	rotateWholeSystem->setContinue(true);
	dummyNode->SetUpdateRotation(rotateWholeSystem);

	rotateRing->ApplyUpdate(1, 0, 1, 0);
	rotateRing->SetSteps(0, 60);
	rotateRing->setContinue(true);
	baseNode->SetUpdateRotation(rotateRing);

	rotateSickly->ApplyUpdate(-1,0, 0,1);
	rotateSickly->SetSteps(0, 60);
	rotateSickly->setContinue(true);
	SecondbaseNode->SetUpdateRotation(rotateSickly);
	rotateSickly2->ApplyUpdate(-1, 1, 0, 0);
	rotateSickly2->SetSteps(0, 60);
	rotateSickly2->setContinue(true);
	ThirdbaseNode->SetUpdateRotation(rotateSickly2);
}

void SceneText::CreateSatelite(const Vector3 &zePos, const Vector3 &zeScale)
{
	GenericEntity* base = Create::Entity("BASE", Vector3(0.0f, 0.0f, 0.0f), zeScale * 10);
	base->setName("base");
	base->InitLOD("BASE", "BASE1", "BASE2");
	base->onNotify(200.f, 400.f);
	baseNode_s = SceneGraph::GetInstance()->AddNode(base);
	baseNode_s->ApplyTranslate(zePos.x, zePos.y - ((zeScale.y * 10)), zePos.z);

	GenericEntity* stand1 = Create::Entity("STAND1", Vector3(0.0f, 0.0f, 0.0f), zeScale*8);
	stand1->setName("stand1");
	secondNode_s = baseNode_s->AddChild(stand1);
	
	GenericEntity* stand2 = Create::Entity("STAND2", Vector3(0.0f, 0.0f, 0.0f), zeScale*8);
	stand2->setName("stand2");
	thirdNode_s = secondNode_s->AddChild(stand2);
	thirdNode_s->ApplyTranslate(0, 5, 0);

	GenericEntity* dish = Create::Entity("DISH", Vector3(0.0f, 0.0f, 0.0f), zeScale * 5);
	dish->setName("dish");
	dish->InitLOD("DISH", "DISH1", "DISH2");
	dish->onNotify(200.f, 400.f);
	forthNode_s = thirdNode_s->AddChild(dish);
	//forthNode->ApplyRotate(45, 0, 0, 1);
	forthNode_s->ApplyTranslate(0, 15, 0);

	listOfOBJForLOD.push_back(base);
	listOfOBJForLOD.push_back(dish);
}
void SceneText::sceneGraphAnimation()
{
	UpdateTransformation* stand1T = new UpdateTransformation();
	UpdateRotation* stand1R = new UpdateRotation();
	stand1T->ApplyUpdate(0, 0.1f, 0);
	stand1T->SetSteps(0, 150);
	secondNode_s->SetUpdateTransformation(stand1T);
	stand1R->ApplyUpdate(0.5f, 0, 1, 0);
	stand1R->SetSteps(0, 200);
	stand1R->setContinue(true);
	secondNode_s->SetUpdateRotation(stand1R);

	UpdateRotation* stand2R = new UpdateRotation();
	stand2R->ApplyUpdate(1.f, 0, 0, 1);
	stand2R->SetSteps(-100, 100);
	stand2R->setContinue(true);
	thirdNode_s->SetUpdateRotation(stand2R);

	UpdateRotation* dishR = new UpdateRotation();
	dishR->ApplyUpdate(1.f, 0, 1, 0);
	dishR->SetSteps(0, 10);
	stand2R->setContinue(false);
	forthNode_s->SetUpdateRotation(dishR);
}

void SceneText::CreateSatelite2(const Vector3 &zePos, const Vector3 &zeScale)
{
	GenericEntity* base = Create::Entity("BASE", Vector3(0.0f, 0.0f, 0.0f), zeScale * 10);
	base->setName("base" + to_string(ROCK_ID++));
	base->InitLOD("BASE", "BASE1", "BASE2");
	base->onNotify(200.f, 400.f);
	SceneNode* baseNode_s2 = SceneGraph::GetInstance()->AddNode(base);
	baseNode_s2->ApplyTranslate(zePos.x, zePos.y - ((zeScale.y * 10)), zePos.z);

	GenericEntity* stand1 = Create::Entity("STAND1", Vector3(0.0f, 0.0f, 0.0f), zeScale * 8);
	stand1->setName("stand1" + to_string(ROCK_ID));
	SceneNode* secondNode_s2 = baseNode_s2->AddChild(stand1);

	GenericEntity* stand2 = Create::Entity("STAND2", Vector3(0.0f, 0.0f, 0.0f), zeScale * 8);
	stand2->setName("stand2" + to_string(ROCK_ID));
	SceneNode* thirdNode_s2 = secondNode_s2->AddChild(stand2);
	thirdNode_s2->ApplyTranslate(0, 5, 0);

	listOfOBJForLOD.push_back(base);

	UpdateTransformation* stand1T = new UpdateTransformation();
	UpdateRotation* stand1R = new UpdateRotation();
	stand1T->ApplyUpdate(0, 0.1f, 0);
	stand1T->SetSteps(0, 150);
	secondNode_s2->SetUpdateTransformation(stand1T);
	stand1R->ApplyUpdate(0.5f, 0, 1, 0);
	stand1R->SetSteps(0, 200);
	stand1R->setContinue(true);
	secondNode_s2->SetUpdateRotation(stand1R);

	UpdateRotation* stand2R = new UpdateRotation();
	stand2R->ApplyUpdate(1.f, 0, 0, 1);
	stand2R->SetSteps(-100, 100);
	stand2R->setContinue(true);
	thirdNode_s2->SetUpdateRotation(stand2R);

}
void SceneText::CreateAsteroid(const Vector3 &zePos, const Vector3 &zeScale)
{
	int random = Math::RandIntMinMax(1, 4);
	Vector3 dist;
	
	GenericEntity* baseRock = Create::Entity("ASTEROID", Vector3(0.0f, 0.0f, 0.0f), zeScale);
	baseRock->setName("rock" + to_string(ROCK_ID++) +"MAIN");
	SceneNode* baseNode = SceneGraph::GetInstance()->AddNode(baseRock);
	baseNode->ApplyTranslate(zePos.x, zePos.y, zePos.z);
	baseRock->InitLOD("ASTEROID", "ASTEROID1", "ASTEROID2");
	baseRock->onNotify(200.f, 400.f);
	UpdateTransformation* baseMtx = new UpdateTransformation();
	UpdateRotation* baseMtx1 = new UpdateRotation();
	dist = Vector3(0, 0, 0) - zePos;
	//baseMtx->ApplyUpdate(Math::RandFloatMinMax(0.01f, 0.05f), 0.0f, Math::RandFloatMinMax(0.01f, 0.05f));	  
	baseMtx->ApplyUpdate(dist.Normalized().x / 5, dist.Normalized().y / 5, dist.Normalized().z / 5);
	baseMtx->SetSteps(0, Math::RandFloatMinMax(120, 200));
	baseMtx->setContinue(true);
	baseMtx1->ApplyUpdate(1, 0, 0, 1);
	baseMtx1->SetSteps(0, 60);
	baseMtx1->setContinue(true);
	baseNode->SetUpdateTransformation(baseMtx);
	//baseNode->SetUpdateRotation(baseMtx1);

	GenericEntity* childRock = Create::Entity("ASTEROID", Vector3(0.0f, 2.0f * zeScale.y, 0.0f), zeScale);
	childRock->setName("rock" + to_string(ROCK_ID++)+ "SPECIAL");
	childRock->InitLOD("ROCK1_1", "ROCK1_2", "ROCK1_3");
	childRock->onNotify(200.f, 400.f);
	SceneNode* childNode = baseNode->AddChild(childRock);
	UpdateRotation* childMtx = new UpdateRotation();
	//UpdateRotation* baseMtx1 = new UpdateRotation();
	
	childMtx->ApplyUpdate(1, 0, 1, 0);
	
	childMtx->SetSteps(-100, 100);
	childMtx->setContinue(true);
	childNode->SetUpdateRotation(childMtx);
	//float test = childNode->GetRotate(childNode->temp);

	GenericEntity* grandchildRock = Create::Entity("ASTEROID", Vector3(0.0f, -2.0f* zeScale.y, 0.0f), zeScale);
	grandchildRock->setName("rock" + to_string(ROCK_ID++));
	grandchildRock->InitLOD("ROCK2_1", "ROCK2_2", "ROCK2_3");
	grandchildRock->onNotify(200.f, 400.f);
	SceneNode* grandchildNode = baseNode->AddChild(grandchildRock);

	GenericEntity* anotherchildRock = Create::Entity("ASTEROID", Vector3(-2.0f, 0.f* zeScale.y, 0.0f* zeScale.z), zeScale);
	anotherchildRock->setName("rock" + to_string(ROCK_ID++));
	anotherchildRock->InitLOD("ROCK3_1", "ROCK3_2", "ROCK3_3");
	anotherchildRock->onNotify(100.f, 200.f);
	SceneNode* anotherchildnode = childNode->AddChild(anotherchildRock);

	GenericEntity* anotherchildRock2 = Create::Entity("ASTEROID", Vector3(4.0f, 0.f* zeScale.y, 0.0f* zeScale.z), zeScale);
	anotherchildRock2->setName("rock" + to_string(ROCK_ID++));
	anotherchildRock2->InitLOD("ROCK3_1", "ROCK3_2", "ROCK3_3");
	anotherchildRock2->onNotify(200.f, 400.f);
	SceneNode* anotherchildnode2 = childNode->AddChild(anotherchildRock2);

	/*GenericEntity* anotherchildRock3 = Create::Entity("ASTEROID", Vector3(0.0f, 0.f* zeScale.y, 4.0f), zeScale);
	anotherchildRock3->setName("rock" + to_string(ROCK_ID++));
	anotherchildRock3->InitLOD("ROCK3_1", "ROCK3_2", "ROCK3_3");
	anotherchildRock3->onNotify(100.f, 200.f);
	SceneNode* anotherchildnode3 = childNode->AddChild(anotherchildRock3);

	GenericEntity* anotherchildRock4 = Create::Entity("ASTEROID", Vector3(0.0f, 0.f* zeScale.y, -4.0f), zeScale);
	anotherchildRock4->setName("rock" + to_string(ROCK_ID++));
	anotherchildRock4->InitLOD("ROCK3_1", "ROCK3_2", "ROCK3_3");
	anotherchildRock4->onNotify(200.f, 400.f);
	SceneNode* anotherchildnode4 = childNode->AddChild(anotherchildRock4);*/
	//UpdateRotation* childMtx1 = new UpdateRotation();
	////UpdateRotation* baseMtx1 = new UpdateRotation();
	//if (random == 1)
	//{
	//	childMtx1->ApplyUpdate(1.2, 1, 0, 0);
	//}
	//else
	//{
	//	childMtx1->ApplyUpdate(1.2, 0, 0, 1);
	//}
	//childMtx1->SetSteps(0, 1);
	//childMtx1->setContinue(true);
	//anotherchildnode->SetUpdateRotation(childMtx1);

	m_activeList.push_back(baseRock);
	m_activeList.push_back(childRock);
	m_activeList.push_back(grandchildRock);
	m_activeList.push_back(anotherchildRock);
	m_activeList.push_back(anotherchildRock2);
	/*m_activeList.push_back(anotherchildRock3);
	m_activeList.push_back(anotherchildRock4);*/
	num_ofAsteroidsLeft += 4;
	
	/*spatialPartition->onNotify(*baseRock);
	spatialPartition->onNotify(*childRock);
	spatialPartition->onNotify(*grandchildRock);
	spatialPartition->onNotify(*anotherchildRock);
	spatialPartition->onNotify(*anotherchildRock2);*/
	
}