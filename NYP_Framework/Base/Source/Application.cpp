#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

#include "MeshBuilder.h"
#include "LoadTGA.h"

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneText.h"
#include "EntityManager.h"
#include "MusicsStuff\MusicSystem.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());	//Obtain Width and Height values from the monitor;
    m_window_height = mode->height;
    m_window_width = mode->width;
    
    m_window = glfwCreateWindow(m_window_width, m_window_height, "NYP Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	// Init systems
	GraphicsManager::GetInstance()->Init();
    // Load all the meshes
    MeshBuilder::GetInstance()->GenerateAxes("reference");
    //MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
    MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
    MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
    MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
    MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
    //MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
    //MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
    MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
    MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
    MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
    MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
    MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
    MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
    MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
    MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
    //MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
    MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//moonGround1.tga");
    MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
    //MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");
    MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//moonGround2.tga");

    MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//NebulaFront.tga");
    MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//NebulaBack_Kai.tga");
    MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//NebulaLeft.tga");
    MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//NebulaRight.tga");
    MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//NebulaTop_KaiNew.tga");
    MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//NebulaBottom_Kai.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ASTEROID", "OBJ//asteroid.obj")->textureID = LoadTGA("Image//lasergun.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ASTEROID1", "OBJ//asteroid_mid.obj")->textureID = LoadTGA("Image//lasergun.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ASTEROID2", "OBJ//asteroid_low.obj")->textureID = LoadTGA("Image//lasergun.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK1_1", "OBJ//rock_HIGH.obj")->textureID = LoadTGA("Image//rock.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK1_2", "OBJ//rock_MEDIUM.obj")->textureID = LoadTGA("Image//rock.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK1_3", "OBJ//rock_LOW.obj")->textureID = LoadTGA("Image//rock.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK2_1", "OBJ//rock2_HIGH.obj")->textureID = LoadTGA("Image//rock2.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK2_2", "OBJ//rock2_MEDIUM.obj")->textureID = LoadTGA("Image//rock2.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK2_3", "OBJ//rock2_LOW.obj")->textureID = LoadTGA("Image//rock2.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK3_1", "OBJ//rock3_HIGH.obj")->textureID = LoadTGA("Image//rock3.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK3_2", "OBJ//rock3_MEDIUM.obj")->textureID = LoadTGA("Image//rock3.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROCK3_3", "OBJ//rock3_LOW.obj")->textureID = LoadTGA("Image//rock3.tga");
    MeshBuilder::GetInstance()->GenerateOBJ("BULLET1", "OBJ//Bullet1.obj")->textureID = LoadTGA("Image//bulletSkin.tga");
    MeshBuilder::GetInstance()->GenerateOBJ("BULLET2", "OBJ//Bullet2.obj")->textureID = LoadTGA("Image//bulletSkin.tga");
    MeshBuilder::GetInstance()->GenerateOBJ("BULLET3", "OBJ//Bullet3.obj")->textureID = LoadTGA("Image//bulletSkin.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("SHIP_1", "OBJ//ship_HIGH.obj")->textureID = LoadTGA("Image//ship.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("SHIP_2", "OBJ//ship_MEDIUM.obj")->textureID = LoadTGA("Image//ship.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("SHIP_3", "OBJ//ship_LOW.obj")->textureID = LoadTGA("Image//ship.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("SHIP_3", "OBJ//ship_LOW.obj")->textureID = LoadTGA("Image//ship.tga");

    MeshBuilder::GetInstance()->GenerateQuad("crosshair", Color(1, 1, 1), 1.f)->textureID = LoadTGA("Image//scope.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Gun", "OBJ//lasergun.obj")->textureID = LoadTGA("Image//asteroid.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("BASE", "OBJ//satelite_base.obj")->textureID = LoadTGA("Image//asteroid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BASE1", "OBJ//satelite_base_MEDIUM.obj")->textureID = LoadTGA("Image//asteroid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BASE2", "OBJ//satelite_base_LOW.obj")->textureID = LoadTGA("Image//asteroid.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("STAND1", "OBJ//satelite_stand1.obj")->textureID = LoadTGA("Image//rock2.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("STAND2", "OBJ//satelite_stand2.obj")->textureID = LoadTGA("Image//rock3.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("DISH", "OBJ//satelite_dish.obj")->textureID = LoadTGA("Image//rock.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("DISH1", "OBJ//satelite_dish_MEDIUM.obj")->textureID = LoadTGA("Image//rock.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("DISH2", "OBJ//satelite_dish_LOW.obj")->textureID = LoadTGA("Image//rock.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("PLANET_RING", "OBJ//PlanetRing.obj")->textureID = LoadTGA("Image//PlanetRing.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("PLANET_RING1", "OBJ//PlanetRing_MEDIUM.obj")->textureID = LoadTGA("Image//PlanetRing.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("PLANET_RING2", "OBJ//PlanetRing_LOW.obj")->textureID = LoadTGA("Image//PlanetRing.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("PLANET", "OBJ//Planet.obj")->textureID = LoadTGA("Image//Planet.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("PLANET1", "OBJ//Planet_MEDIUM.obj")->textureID = LoadTGA("Image//Planet.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("PLANET2", "OBJ//Planet_LOW.obj")->textureID = LoadTGA("Image//Planet.tga");

    Mesh *zeMesh = MeshBuilder::GetInstance()->GenerateQuadXZ("GRIDMESH", Color(1, 1, 1), 1.f);
    zeMesh->mode = Mesh::DRAW_LINES;

    MusicSystem::accessing().Init();
    MusicSystem::accessing().playBackgroundMusic("BGM");
}

void Application::Run()
{
	SceneManager::GetInstance()->SetActiveScene("Start");
    // Active Window Detection
    HWND hwnd = GetActiveWindow();
    UpdateInput();
    PostInputUpdate();
    m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		glfwPollEvents();
        if (hwnd == GetActiveWindow())
        {
            UpdateInput();

            SceneManager::GetInstance()->Update(m_timer.getElapsedTime());
            SceneManager::GetInstance()->Render();
		    PostInputUpdate();
        }

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...

        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
		
	}
	SceneManager::GetInstance()->Exit();
    SceneManager::GetInstance()->Destroy();
    EntityManager::GetInstance()->Destroy();
    GraphicsManager::GetInstance()->Destroy();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
    _CrtDumpMemoryLeaks();
}

void Application::UpdateInput()
{
	// Update Mouse Position
#ifdef _DEBUG
    POINT mousePosition;
    GetCursorPos(&mousePosition);

    MouseController::GetInstance()->UpdateMousePosition(-mousePosition.x, -mousePosition.y);
#else
    double mouse_currX, mouse_currY;
    glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
    MouseController::GetInstance()->UpdateMousePosition(-mouse_currX, -mouse_currY);
#endif
	//double mouse_currX, mouse_currY;
	//glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
#ifdef _DEBUG
        POINT mousePosition;
        GetCursorPos(&mousePosition);

        MouseController::GetInstance()->UpdateMousePosition(-mousePosition.x, -mousePosition.y);
        SetCursorPos(m_window_width / 2, m_window_height / 2);
#else
        double mouse_currX, mouse_currY;
        mouse_currX = m_window_width >> 1;
        mouse_currY = m_window_height >> 1;
        glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
#endif
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}