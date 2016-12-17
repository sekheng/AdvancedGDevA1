#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "DetectMemoryLeak.h"
#include "WeaponInfo\RenderGun.h"
#include <vector>

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

    virtual bool onNotify(const std::string &zeEvent);

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[10];
	Light* lights[2];

	GenericEntity* theCube;
	GenericEntity* theShip;
	RenderGun* theGun;

    std::vector<GenericEntity*> m_activeList, m_inactiveList;
    std::vector<size_t> waitingListToBeRemoved;
    EntityBase *spatialPartition, *boundaryOfScene;

	static SceneText* sInstance; // The pointer to the object that gets registered
    int health_;
    float timeLeft_Second;


    enum GAME_STATES
    {
        PLAYING,
        GAME_OVER,
        TOTAL_STATES,
    };
    GAME_STATES currGameState;
    void resetGame();
    void CreateAsteroid(const Vector3 &zePos, const Vector3 &zeScale);
    unsigned short num_ofAsteroidsLeft;
};

#endif