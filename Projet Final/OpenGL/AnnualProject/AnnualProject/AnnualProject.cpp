// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework 2014
// Guillaume Noisette
// Fabien Sacriste
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------
#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "../../EsgiGL/EsgiGL.h"
#include "../../EsgiGL/Common/vector.h"
#include "../../EsgiGL/Common/matrix.h"
#include "../../EsgiGL/EsgiShader.h"

#include "GameObject.h"
#include "GameObjectManager.h"
#include "DestructorManager.h"

// --- Globales --------------------------------------------------------------

static int sizeWindowX = 500;
static int sizeWindowY = 500;

static const int numberSudivisionX = 30;
static const int numberSudivisionZ = 30;
static float scale = 15;

float time = 0.f;

EsgiShader shader;

struct EsgiCamera
{
	vec3 position;
	vec3 target;
	vec3 orientation;
};
EsgiCamera camera;

GameObjectManager gom;
GameObject * ground;
unsigned int currentShrapnel = 0;

// --- Fonctions -------------------------------------------------------------

// ---

void Update(float elapsedTime)
{
	time += elapsedTime;
}

void Draw()
{
	// efface le color buffer et le depth buffer
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

#ifndef ESGI_GLES_20
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif

	mat4 projectionMatrix = esgiPerspective(45, (float)sizeWindowX/sizeWindowY, 0.1f, 1000.f);
	mat4 cameraMatrix = esgiLookAt(camera.position, camera.target, vec3(0, 1, 0));
	mat4 viewMatrix = esgiMultiplyMatrix(cameraMatrix, esgiRotateY(camera.orientation.y));
	mat4 worldMatrix;
	worldMatrix.Identity();
	worldMatrix.T.set(0.f, 0.f, 0.f, 1.f);
	
	gom.RenderObjects(projectionMatrix, viewMatrix, worldMatrix);
}

//
// Initialise les shader & mesh
//

void GenerationGround()
{
	ground = new GameObject();
	vec3 minimalSize(-scale, 0.f, -scale);
	vec3 maximalSize(scale, 0.f, scale);

	float sizeElementX = (maximalSize.x - minimalSize.x) / numberSudivisionX;
	float sizeElementZ = (maximalSize.z - minimalSize.z) / numberSudivisionZ;
	
	for (int i = 0; i < 1 + numberSudivisionX; ++i)
	{
		for (int j = 0; j < 1 + numberSudivisionZ; ++j)
		{
			ground->listVertices->push_back(vec3(minimalSize.x + sizeElementX * i, 0.f, minimalSize.z + sizeElementZ * j));
		}
	}
	
	Edge * edge1, * edge2, * edge3;
	for (int i = 0; i < numberSudivisionX; ++i)
	{
		edge1 = new Edge(i * (1 + numberSudivisionZ), (i + 1) * (1 + numberSudivisionZ));
		ground->listEdges->push_back(edge1);
		for (int j = 0; j < numberSudivisionZ; ++j)
		{
			edge2 = new Edge((i + 1) * (1 + numberSudivisionZ) + j, (i + 1) * (1 + numberSudivisionZ) + j + 1);
			ground->listEdges->push_back(edge2);
			edge3 = new Edge((i + 1) * (1 + numberSudivisionZ) + j + 1, i * (1 + numberSudivisionZ) + j);
			ground->listEdges->push_back(edge3);
			ground->listFaces->push_back(new Face(edge1, edge2, edge3));
			edge1 = new Edge((i + 1) * (1 + numberSudivisionZ) + j + 1, i * (1 + numberSudivisionZ) + j + 1);
			ground->listEdges->push_back(edge1);
			switch(i)
			{
			case 0:
				edge2 = new Edge(i * (1 + numberSudivisionZ) + j + 1, i * (1 + numberSudivisionZ) + j);
				ground->listEdges->push_back(edge2);
				break;
			case 1:
				edge2 = ground->listEdges->at(j + ((i - 1) * 3 * numberSudivisionZ + i) + j * 3);
				break;
			default:
				edge2 = ground->listEdges->at(numberSudivisionZ + ((i - 1) * 3 * numberSudivisionZ + i) + j * 3);
				break;
			}
			ground->listFaces->push_back(new Face(edge1, edge2, edge3));
		}
	}
	Face::VerifyFaces(ground->listVertices, ground->listFaces, vec3(0.0f, 1.0f, 0.0f));
	ground->CalculateIndexes();
	gom.listObjects->push_back(ground);
}

bool Setup()
{
	camera.position = vec3(0.f, 30.f, 1.f);
	camera.orientation = vec3(0.f, 0.f, 0.f);
	camera.target = vec3(0.f, 0.f, 0.f);

	gom.Setup("basic.vert", "basic.frag");
	
	GenerationGround();
	DestructorManager::LaunchDestruction(gom, ground, vec3(0.0f, 0.0f, 0.0f), -camera.position);
	camera.position = vec3(0.f, 20.f, 20.f);
	
	return true;
}

//
// Libere la memoire occupee
//
void Clean()
{
	gom.Clean();
}

//
// Fonction d'event de la souris
//
void ActiveMouse(int mousex, int mousey)
{
	//float tempX = mousex - sizeWindowX/2, tempY = (mousey - sizeWindowY/2) * -1;
}
void PassiveMouse(int mousex, int mousey)
{
}

double orientedAngle(const vec3 & p1, const vec3 & p2, const vec3 & p3)
{
   vec3 v1 = p2 - p1;
   vec3 v2 = p3 - p1;

   double angle = (atan2f(v1.y, v1.x) - atan2f(v2.y, v2.x)) * 180 / M_PI;
   if (angle < 0)
	   angle += 360;
   return angle / 360;
}

void Keyboard(unsigned char key, int mx, int my)
{
	switch(key)
	{
	case 's':
		camera.orientation.y -= 45;
		break;
	case 'q':
		camera.orientation.y += 45;
		break;
	case 'o':
		++currentShrapnel;
		if (currentShrapnel >= gom.listObjects->size())
			currentShrapnel = 0;
		break;
	case 'p':
		--currentShrapnel;
		if (currentShrapnel < 0)
			currentShrapnel = gom.listObjects->size() - 1;
		break;
	case 'b':
		gom.listObjects->at(currentShrapnel)->Move(vec3(1,0,0));
		break;
	case 'n':
		gom.listObjects->at(currentShrapnel)->Move(vec3(-1,0,0));
		break;
	case 'h':
		gom.listObjects->at(currentShrapnel)->Move(vec3(0,1,0));
		break;
	case 'j':
		gom.listObjects->at(currentShrapnel)->Move(vec3(0,-1,0));
		break;
	case 'y':
		gom.listObjects->at(currentShrapnel)->Move(vec3(0,0,1));
		break;
	case 'u':
		gom.listObjects->at(currentShrapnel)->Move(vec3(0,0,-1));
		break;
	}
}

// 
//
//
int main(int argc, char *argv[])
{
	EsgiGLApplication esgi;
    
	esgi.InitWindowPosition(0, 0);
	esgi.InitWindowSize(sizeWindowX, sizeWindowY);
	esgi.InitDisplayMode(ESGI_WINDOW_RGBA|ESGI_WINDOW_DEPTH|ESGI_WINDOW_DOUBLEBUFFER);
	esgi.CreateWindow("AnnualProject", ESGI_WINDOW_CENTERED);
	
    esgi.InitFunc(&Setup);
    esgi.CleanFunc(&Clean);
    esgi.IdleFunc(&Update);
	esgi.DisplayFunc(&Draw);
	esgi.MotionFunc(&ActiveMouse);
	esgi.PassiveMotionFunc(&PassiveMouse);
	esgi.KeyboardFunction(&Keyboard);
    
	esgi.MainLoop();
    
    return 0;
}