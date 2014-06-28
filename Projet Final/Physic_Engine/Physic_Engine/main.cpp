
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include "../EsgiGL/EsgiShader.h"
#include "../EsgiGL/Common/EsgiUtils.h"
#include "../EsgiGL/Common/matrix.h"
#include "../ESGIGL/Common/EsgiTga.h"

#include <vector>
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Generic_Object.h"
#include "ObjectManager.h"
#include "DestructorManager.h"

#include "MathFunctions.h"
#include <cmath>
#include "Common/EsgiTimer.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

using namespace std;

int GameObject::count = 0;

EsgiShader shader;
EsgiShader shaderSkybox;
GLuint skyboxTextureIds[6];

 
//Cube cube2(-14.0f, 900.0f, 0.0f, 10.0f); 
int countSphere = 0;
Cube cube(14.0f, 900.0f, 0.0f, 1.0f);
MathFunctions MF;

Generic_Object* ground = new Generic_Object();

ObjectManager objectManager;
//static vec3 quad[4];
Plane plane;
float scale = 3000.f;
float skyboxSize = scale/2.0f;
Plane skybox[6]={
Plane (vec3(-(skyboxSize),skyboxSize, -(skyboxSize)), vec3(-(skyboxSize), skyboxSize, (skyboxSize)), vec3(-(skyboxSize), -skyboxSize, -(skyboxSize)), vec3(-(skyboxSize), -skyboxSize, (skyboxSize))),
Plane (vec3((skyboxSize), skyboxSize, -(skyboxSize)), vec3(-(skyboxSize), skyboxSize, -(skyboxSize)), vec3((skyboxSize), -skyboxSize, -(skyboxSize)), vec3(-(skyboxSize), -skyboxSize, -(skyboxSize))),
Plane (vec3((skyboxSize), skyboxSize ,(skyboxSize)), vec3((skyboxSize), skyboxSize, -(skyboxSize)), vec3((skyboxSize), -skyboxSize, (skyboxSize)), vec3((skyboxSize), -skyboxSize, -(skyboxSize))),
Plane (vec3(-(skyboxSize),skyboxSize, (skyboxSize)), vec3((skyboxSize), skyboxSize, (skyboxSize)), vec3(-(skyboxSize), -skyboxSize, (skyboxSize)), vec3((skyboxSize), -skyboxSize, (skyboxSize))),
Plane (vec3((skyboxSize), skyboxSize, (skyboxSize)), vec3(-(skyboxSize), skyboxSize, (skyboxSize)), vec3((skyboxSize), skyboxSize, -(skyboxSize)), vec3(-(skyboxSize), skyboxSize, -(skyboxSize))),
Plane (vec3((skyboxSize), -skyboxSize, -(skyboxSize)), vec3(-(skyboxSize), -skyboxSize, -(skyboxSize)), vec3((skyboxSize), -skyboxSize, (skyboxSize)), vec3(-(skyboxSize), -skyboxSize, (skyboxSize))),
};


//Plane plane2(vec3(-30, 0, -30), vec3(-10, 0, -30), vec3(-30, 0, -10), vec3(-10, 0, -10));
//Plane plane3(vec3(10, 0, -30), vec3(30, 0, -30), vec3(10, 0, -10), vec3(30, 0, -10));
//Plane plane4(vec3(-30, 0, 10), vec3(-10, 0, 10), vec3(-30, 0, 30), vec3(-10, 0, 30));
//Plane plane5(vec3(10, 0, 10), vec3(30, 0, 10), vec3(10, 0, 30), vec3(30, 0, 30));
//
//
//
//Plane plane6(vec3(-50, 0, -50), vec3(-30, 0, -50), vec3(-50, 0, -30), vec3(-30, 0, -30));
//Plane plane7(vec3(-10, 0, -50), vec3(10, 0, -50), vec3(-10, 0, -30), vec3(10, 0, -30));
//Plane plane8(vec3(30, 0, -50), vec3(50, 0, -50), vec3(30, 0, -30), vec3(50, 0, -30));
//
//Plane plane9(vec3(-50, 0, -10), vec3(-30, 0, -10), vec3(-50, 0, 10), vec3(-30, 0, 10));
//Plane plane10(vec3(30, 0, -10), vec3(50, 0, -10), vec3(30, 0, 10), vec3(50, 0, 10));
//
//Plane plane11(vec3(-50, 0, 30), vec3(-30, 0, 30), vec3(-50, 0, 50), vec3(-30, 0, 50));
//Plane plane12(vec3(-10, 0, 30), vec3(10, 0, 30), vec3(-10, 0, 50), vec3(10, 0, 50));
//Plane plane13(vec3(30, 0, 30), vec3(50, 0, 30), vec3(30, 0, 50), vec3(50, 0, 50));
//
//Plane plane14(vec3(-300, 1.0f, -200), vec3(300, 1.0f, -200), vec3(-300, 1.0f, -60), vec3(300, 1.0f, -60));
//Plane plane15(vec3(-300, 1.0f, 60), vec3(300, 1.0f, 60), vec3(-300, 1.0f, 200), vec3(300, 1.0f, 200));


struct EsgiCamera
{
	vec3 position;
	vec3 target;
	vec3 orientation;
};
EsgiCamera camera;

void Update(float elapsedTime);
void Draw();
void Clean();
bool Setup();
void Keyboard(unsigned char key, int mx, int my);

int main(int argc, char *argv[])
{
	MF.dotProduct(vec3(1.0f, 2.0f, 3.2f), vec3(4.0f, 5.0f, 6.0f));
	EsgiGLApplication esgi;
    
	esgi.InitWindowPosition(0, 0);
	esgi.InitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	esgi.InitDisplayMode(ESGI_WINDOW_RGBA | ESGI_WINDOW_DEPTH | ESGI_WINDOW_DOUBLEBUFFER);
	esgi.CreateWindow("Physic engine demo", ESGI_WINDOW_CENTERED);
	
    esgi.IdleFunc(&Update);
	esgi.DisplayFunc(&Draw);
    esgi.InitFunc(&Setup);
    esgi.CleanFunc(&Clean);
	esgi.KeyboardFunction(&Keyboard);

	esgi.MainLoop();
}

void Update(float elapsedTime)
{

	//EsgiTimer benchmark;
	//benchmark.Begin();

	objectManager.update(elapsedTime);
	
	float y = ground->getComponents()->getRigidBody()->getPosLowestVertex().y;

	y;

	//benchmark.End();
	printf("duree de l'update : %Lf millisecondes\n", 1/elapsedTime/*(benchmark.GetElapsedTime()*//**1000.0*/);
	//camera.target = sphere.getCenterOfObject();

}

void Draw()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	mat4 cameraMatrix = esgiLookAt(camera.position, camera.target, vec3(0.0, 1.0, 0.0));
	mat4 projectionMatrix = esgiPerspective(45, (float)((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), 0.01f, 10000.f);
	
	mat4 viewMatrix = esgiMultiplyMatrix(cameraMatrix, esgiRotateY(camera.orientation.y));
	mat4 modelviewMatrix;
	mat4 worldMatrix;
	modelviewMatrix.Identity();
	worldMatrix.Identity();
	modelviewMatrix = esgiMultiplyMatrix(viewMatrix, worldMatrix);
	

	esgiUtilsDrawAxes(modelviewMatrix,projectionMatrix,1.f);

	GLuint programObject = shader.GetProgram();
	glUseProgram(programObject);


	GLint position_attrib = glGetAttribLocation(programObject,"a_Position");

	GLint color_uniform = glGetUniformLocation(programObject, "u_Color");

	GLint viewUniform = glGetUniformLocation(programObject, "u_ViewMatrix");
	glUniformMatrix4fv(viewUniform, 1, 0, &viewMatrix.I.x);

	mat4 uniformMatrix = esgiRotateX(0);
	GLint rotationUniform = glGetUniformLocation(programObject, "u_RotationMatrix");
	glUniformMatrix4fv(rotationUniform, 1, 0, &uniformMatrix.I.x);

	GLint projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, 0, &projectionMatrix.I.x);

	worldMatrix.Identity();
	worldMatrix.T.set(0.f, 0.f, 0.f, 1.f); // Translate sur l'axe z
	GLint worldUnifrom = glGetUniformLocation(programObject, "u_WorldMatrix");
	glUniformMatrix4fv(worldUnifrom, 1, 0, &worldMatrix.I.x);



	

	glEnableVertexAttribArray(position_attrib);
	objectManager.render(&position_attrib, &color_uniform);

	glDisableVertexAttribArray(position_attrib);


	//----------= Set the different settings for the shader =---------//
	programObject = shaderSkybox.GetProgram();
	glUseProgram(programObject);

	position_attrib = glGetAttribLocation(programObject,"a_Position");
	

	color_uniform = glGetUniformLocation(programObject, "u_Color");
	glUniform4f(color_uniform,0.f, 0.f, 0.f, 1.f);

	viewUniform = glGetUniformLocation(programObject, "u_ViewMatrix");
	glUniformMatrix4fv(viewUniform, 1, 0, &viewMatrix.I.x);

	//uniformMatrix = esgiRotateX(-90);
	//uniformMatrix.T.set(0.f,10.f,10.f,1.f);
	//uniformMatrix.Identity();
	rotationUniform = glGetUniformLocation(programObject, "u_RotationMatrix");
	glUniformMatrix4fv(rotationUniform, 1, 0, &uniformMatrix.I.x);

	GLuint uniformScale = glGetUniformLocation(programObject, "u_Scale");
	glUniform1f(uniformScale, scale);

	GLuint uniformFace = glGetUniformLocation(programObject, "u_Face");

	projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, 0, &projectionMatrix.I.x);
	

	worldMatrix.Identity();
	worldMatrix.T.set(0.f, 0.f, 0.f, 1.f); // Translate sur l'axe z
	worldUnifrom = glGetUniformLocation(programObject, "u_WorldMatrix");
	glUniformMatrix4fv(worldUnifrom, 1, 0, &worldMatrix.I.x);


	////----------= Texture =----------//

	// Display or not the texture
	
	for(int i = 0; i < 6; ++i)
	{
		glVertexAttribPointer(position_attrib, 3, GL_FLOAT, false, 0, &skybox[i].getListOfVertices()->at(0).x);
		glUniform1i(uniformFace, i);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyboxTextureIds[i]);

		GLint textureUnit0 = glGetUniformLocation(programObject, "u_Texture");
		glUniform1i(textureUnit0, 0);

		glEnableVertexAttribArray(position_attrib);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glDisableVertexAttribArray(position_attrib);


}

void Clean()
{
	shader.Destroy();
	shaderSkybox.Destroy();
}

bool Setup()
{
	shader.LoadVertexShader("quad3d.vert");
	shader.LoadFragmentShader("basic.frag");
	shader.Create();

	shaderSkybox.LoadVertexShader("quad3d_texture.vert");
	shaderSkybox.LoadFragmentShader("basic_texture.frag");
	shaderSkybox.Create();

	camera.position = vec3(0.f, 100.f, 80.f); //vec3(3.f, 10.f, 20.f);vec3(0.0f, 3.f, 4.f); (3.f, 10.f, 70.f); (3.f, 20.f, 100.f)
	camera.orientation = vec3(0.f, 0.f, 0.f);
	camera.target = vec3(0.f, 0.f, 0.f);



	int numberSudivisionX = 2;
	int numberSudivisionZ = 2;

	

	vec3 minimalSize(-scale, 0.f, -scale);
	vec3 maximalSize(scale, 0.f, scale);

	float sizeElementX = (maximalSize.x - minimalSize.x) / numberSudivisionX;
	float sizeElementZ = (maximalSize.z - minimalSize.z) / numberSudivisionZ;
	
	for (int i = 0; i < 1 + numberSudivisionX; ++i)
	{
		for (int j = 0; j < 1 + numberSudivisionZ; ++j)
		{
			ground->verticesList->push_back(vec3(minimalSize.x + sizeElementX * i, 0.f, minimalSize.z + sizeElementZ * j));
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

	Face::VerifyFaces(ground->verticesList, ground->listFaces, vec3(0.0f, 1.0f, 0.0f));
	ground->CalculateIndexes();
	objectManager.addObject(ground);

	ground->setWireframeMode(true);

	//camera.position = vec3(0.f, 30.f, 1.f);
	//camera.orientation = vec3(0.f, 0.f, 0.f);
	//camera.target = vec3(0.f, 0.f, 0.f);

	//DestructorManager::LaunchDestruction(objectManager, ground, vec3(0.0f, 0.0f, 0.0f), -camera.position);
	

	//camera.position = vec3(0.f, 10.f, 20.f);
	//camera.orientation = vec3(0.f, 0.f, 0.f);
	//camera.target = vec3(0.f, 0.f, 0.f);


	ground->setRenderObject(true);
	ground->getComponents()->getRigidBody()->addRigidBody(RIGID_GENERIC, *ground->verticesList, *ground->indexesList, vec3(0.0f, 0.0f, 0.0f),false);
	ground->getComponents()->getRigidBody()->setIsStatic(true);
	ground->getComponents()->getRigidBody()->setSize(2000.0f);
	ground->getComponents()->getRigidBody()->setDisplayRigidBody(true);
	ground->getComponents()->getRigidBody()->activateRigidBody(true);
	ground->getComponents()->getRigidBody()->getRigidBodyObjectGeneric()->resetWireframeModeindexes(*ground->listIndexesWireframe);
	ground->setColor(0.84f, 0.79f, 0.69f,1.0f);
	//ground->getComponents()->getGravity()->setUseGravity(true);
	/*Cube* ground;

	ground = new Cube(0.0f, -5000.0f,0.0f, 10000.0f);
	objectManager.addObject(ground);

	ground->setWireframeMode(false);
	ground->getComponents()->getGravity()->setUseGravity(false);
	ground->getComponents()->getRigidBody()->addRigidBody(RIGID_CUBE, ground->getCenterOfObject(), 5000.0f);
	ground->getComponents()->getRigidBody()->setDisplayRigidBody(true);
	ground->getComponents()->getRigidBody()->activateRigidBody(true);
	
	ground->getComponents()->getGravity()->setGravity(9.8f);

	ground->setRenderObject(false);

	ground->getComponents()->getRigidBody()->setIsStatic(true);*/

	// ADD OBJECTS TO LIST

	//objectManager.addObject(&plane);

	/*objectManager.addObject(&plane2);
	objectManager.addObject(&plane3);
	objectManager.addObject(&plane4);
	objectManager.addObject(&plane5);

	objectManager.addObject(&plane6);
	objectManager.addObject(&plane7);
	objectManager.addObject(&plane8);
	objectManager.addObject(&plane9);
	objectManager.addObject(&plane10);
	objectManager.addObject(&plane11);
	objectManager.addObject(&plane12);
	objectManager.addObject(&plane13);*/


	plane.setWireframeMode(true);

	//plane2.setWireframeMode(true);
	//plane3.setWireframeMode(true);
	//plane4.setWireframeMode(true);
	//plane5.setWireframeMode(true);

	//plane6.setWireframeMode(true);
	//plane7.setWireframeMode(true);
	//plane8.setWireframeMode(true);
	//plane9.setWireframeMode(true);
	//plane10.setWireframeMode(true);
	//plane11.setWireframeMode(true);
	//plane12.setWireframeMode(true);
	//plane13.setWireframeMode(true);

	//plane14.setWireframeMode(true);
	//plane15.setWireframeMode(true);

	//cube.setWireframeMode(true);

	// AXES

	plane.setDisplayAxes(true);
	//plane2.setDisplayAxes(true);
	//plane3.setDisplayAxes(true);
	//plane4.setDisplayAxes(true);
	//plane5.setDisplayAxes(true);
	//plane6.setDisplayAxes(true);
	//plane7.setDisplayAxes(true);
	//plane8.setDisplayAxes(true);
	//plane9.setDisplayAxes(true);
	//plane10.setDisplayAxes(true);
	//plane11.setDisplayAxes(true);
	//plane12.setDisplayAxes(true);
	//plane13.setDisplayAxes(true);
	//plane14.setDisplayAxes(true);
	//plane15.setDisplayAxes(true);

	//plane2.setColor(0.5f, 0.5f, 0.5f, 1.0f);
	//plane3.setColor(0.5f, 0.5f, 0.5f, 1.0f);
	//plane4.setColor(0.5f, 0.5f, 0.5f, 1.0f);
	//plane5.setColor(0.5f, 0.5f, 0.5f, 1.0f);

	//plane6.setColor(0.3f, 0.3f, 0.3f, 1.0f);
	//plane7.setColor(0.3f, 0.3f, 0.3f, 1.0f);
	//plane8.setColor(0.3f, 0.3f, 0.3f, 1.0f);
	//plane9.setColor(0.3f, 0.3f, 0.3f, 1.0f);
	//plane10.setColor(0.3f, 0.3f, 0.3f, 1.0f);
	//plane11.setColor(0.3f, 0.3f, 0.3f, 1.0f);
	//plane12.setColor(0.3f, 0.3f, 0.3f, 1.0f);
	//plane13.setColor(0.3f, 0.3f, 0.3f, 1.0f);

	//plane14.setColor(0.46f, 0.77f, 0.93f, 1.0f);
	//plane15.setColor(0.99f, 0.29f, 0.29f, 1.0f);

	plane.setRenderObject(true);

	plane.getComponents()->getRigidBody()->addRigidBody(RIGID_PLANE, vec3(0.0f,0.0f, 0.0f), 20.0f, 20.0f);
	//plane.getComponents()->getRigidBody()->rotate(-45, vec3(0.0f, 0.0f, 1.0f));
	plane.getComponents()->getRigidBody()->setDisplayRigidBody(false);
	plane.getComponents()->getRigidBody()->activateRigidBody(false);
	plane.getComponents()->getGravity()->setUseGravity(false);

	/*plane2.getComponents()->getGravity()->setUseGravity(false);
	plane2.getComponents()->getRigidBody()->activateRigidBody(false);
	plane3.getComponents()->getGravity()->setUseGravity(false);
	plane3.getComponents()->getRigidBody()->activateRigidBody(false);
	plane4.getComponents()->getGravity()->setUseGravity(false);
	plane4.getComponents()->getRigidBody()->activateRigidBody(false);
	plane5.getComponents()->getGravity()->setUseGravity(false);
	plane5.getComponents()->getRigidBody()->activateRigidBody(false);
	plane6.getComponents()->getGravity()->setUseGravity(false);
	plane6.getComponents()->getRigidBody()->activateRigidBody(false);
	plane7.getComponents()->getGravity()->setUseGravity(false);
	plane7.getComponents()->getRigidBody()->activateRigidBody(false);
	plane8.getComponents()->getGravity()->setUseGravity(false);
	plane8.getComponents()->getRigidBody()->activateRigidBody(false);
	plane9.getComponents()->getGravity()->setUseGravity(false);
	plane9.getComponents()->getRigidBody()->activateRigidBody(false);
	plane10.getComponents()->getGravity()->setUseGravity(false);
	plane10.getComponents()->getRigidBody()->activateRigidBody(false);
	plane11.getComponents()->getGravity()->setUseGravity(false);
	plane11.getComponents()->getRigidBody()->activateRigidBody(false);
	plane12.getComponents()->getGravity()->setUseGravity(false);
	plane12.getComponents()->getRigidBody()->activateRigidBody(false);
	plane13.getComponents()->getGravity()->setUseGravity(false);
	plane13.getComponents()->getRigidBody()->activateRigidBody(false);*/

	EsgiTexture * textureLeft = esgiReadTGAFile("img/skybox_left.tga");
	EsgiTexture * textureFront = esgiReadTGAFile("img/skybox_front.tga");
	EsgiTexture * textureRight = esgiReadTGAFile("img/skybox_right.tga");
	EsgiTexture * textureBack = esgiReadTGAFile("img/skybox_back.tga");
	EsgiTexture * textureTop = esgiReadTGAFile("img/skybox_top.tga");
	EsgiTexture * textureBottom = esgiReadTGAFile("img/skybox_bottom.tga");

	glGenTextures(1, &skyboxTextureIds[0]);
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIds[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, textureLeft->internalFormat, textureLeft->width, textureLeft->height, 0, textureLeft->format, textureLeft->datatype, textureLeft->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &skyboxTextureIds[1]);
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIds[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, textureFront->internalFormat, textureFront->width, textureFront->height, 0, textureFront->format, textureFront->datatype, textureFront->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &skyboxTextureIds[2]);
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIds[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, textureRight->internalFormat, textureRight->width, textureRight->height, 0, textureRight->format, textureRight->datatype, textureRight->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &skyboxTextureIds[3]);
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIds[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, textureBack->internalFormat, textureBack->width, textureBack->height, 0, textureBack->format, textureBack->datatype, textureBack->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &skyboxTextureIds[4]);
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIds[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, textureTop->internalFormat, textureTop->width, textureTop->height, 0, textureTop->format, textureTop->datatype, textureTop->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &skyboxTextureIds[5]);
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIds[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, textureBottom->internalFormat, textureBottom->width, textureBottom->height, 0, textureBottom->format, textureBottom->datatype, textureBottom->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	objectManager.setGravity(9.8f);

	return true;
}

Generic_Object* CreateFragments(vec3 position, float size)
{
	vector<vec3>* vertices = new vector<vec3>();
	vector<GLushort>* indexes = new vector<GLushort>();

	size /= 2;

	float LO;
	float HI;
	float x; 
	float z; 
	float y;

	LO = -3.0f;
	HI = 3.0f;

	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	vertices->push_back(vec3(-size + 0.0f + x, -size + 0.0f + y,  size + 0.0f + z));

	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    vertices->push_back(vec3( size + 0.0f + x, -size + 0.0f + y,  size + 0.0f + z));

	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    vertices->push_back(vec3( size + 0.0f + x,  size + 0.0f + y,  size + 0.0f + z));

	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    vertices->push_back(vec3(-size + 0.0f + x,  size + 0.0f + y,  size + 0.0f + z));

	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    vertices->push_back(vec3(-size + 0.0f + x, -size + 0.0f + y, -size + 0.0f + z));

	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    vertices->push_back(vec3( size + 0.0f + x, -size + 0.0f + y, -size + 0.0f + z));

	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    vertices->push_back(vec3( size + 0.0f + x,  size + 0.0f + y, -size + 0.0f + z));
    
	x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	vertices->push_back(vec3(-size + 0.0f + x,  size + 0.0f + y, -size + 0.0f + z));


	indexes->push_back(3);
    indexes->push_back(0);
    indexes->push_back(1);
    indexes->push_back(1);
    indexes->push_back(3);
    indexes->push_back(2);
    indexes->push_back(2);
    indexes->push_back(1);
    indexes->push_back(5);
    indexes->push_back(5);
    indexes->push_back(2);
    indexes->push_back(6);
    indexes->push_back(6);
    indexes->push_back(5);
    indexes->push_back(4);
    indexes->push_back(4);
    indexes->push_back(6);
    indexes->push_back(7);
    indexes->push_back(7);
    indexes->push_back(4);
    indexes->push_back(0);
    indexes->push_back(0);
    indexes->push_back(7);
    indexes->push_back(3);
    indexes->push_back(3);
    indexes->push_back(2);
    indexes->push_back(6);
    indexes->push_back(6);
    indexes->push_back(3);
    indexes->push_back(7);
    indexes->push_back(7);
    indexes->push_back(7);
    indexes->push_back(4);
    indexes->push_back(4);
    indexes->push_back(0);
    indexes->push_back(1);
	indexes->push_back(1);
	indexes->push_back(4);
	indexes->push_back(5);

	return new Generic_Object(*vertices, *indexes, position.x, position.y, position.z, true);
}

void makeExplosion()
{
	float LO;
	float HI;
	float x; 
	float z; 
	float y;
	float r, g, b;
	Generic_Object* frag;
	vec3 oldPos;

	float dirX;
	float dirY;
	float dirZ;


	oldPos = camera.position;

		camera.position = vec3(0.f, 30.f, 1.0f);
		camera.orientation = vec3(0.f, 0.f, 0.f);
		camera.target = vec3(0.f, 0.f, 0.f);

		DestructorManager::LaunchDestruction(objectManager, ground, vec3(0.0f, 0.0f, 0.0f), -camera.position);
		
		ground->getComponents()->getRigidBody()->changeIndexesList(*ground->indexesList);
		ground->getComponents()->getRigidBody()->changeVerticesList(*ground->verticesList);
		ground->getComponents()->getRigidBody()->getRigidBodyObjectGeneric()->resetWireframeModeindexes(*ground->listIndexesWireframe);
		
		
		camera.position = oldPos;

	


	dirX = -2;
		dirY = 4;
		dirZ = -2;


		for(int i = 0; i < 4; ++i)
		{
			for(int j= 0; j < 4; ++j)
			{
				for(int k =0; k < 5; ++k)
				{
					

					//LO = 0.0f;
					//HI = 1.0f;

					//r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					//g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					//b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					//newObject = new Generic_Object(*cube.getListOfVertices(), *cube.getListOfIndexes(), (float)k * 10.0f-10.f, (float)i * 10.0f + 5.0f + 20.0f, (float)j * 10.0f-10.0f, true); // rouge
					//objectManager.addObject(newObject);

					frag = CreateFragments(vec3((float)k * 10.0f-10.f + i*0, (float)i * 10.0f + 0.0f + 20.0f, (float)j * 10.0f-10.0f), 6.0f);

					//cout << ++countSphere << " - x: " << k* 5.0f << "   y: " << i* 5.0f + 5.0f<< "   z: " << j* 5.0f << endl;
					//frag = CreateFragments(vec3(0.0f, 5.0f, 0.0f), 1.0f);
		 
					LO = 0.0f;
					HI = 1.0f;

					r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					frag->setWireframeMode(false);
					frag->getComponents()->getGravity()->setUseGravity(true);
					frag->getComponents()->getRigidBody()->addRigidBody(RIGID_GENERIC, *frag->getListOfVertices(), *frag->getListOfIndexes(), frag->getCenterOfObject(), true);
					frag->getComponents()->getRigidBody()->setDisplayRigidBody(true);
					frag->getComponents()->getRigidBody()->activateRigidBody(true);
					frag->getComponents()->getGravity()->setBounciness(frag->getMass(), frag->getMass()/5550);
					frag->getComponents()->getGravity()->setResistance(frag->getMass(), frag->getMass() / 1.01f);
					frag->setAlternateFacesAndNormals(true);
					frag->getComponents()->getGravity()->setGravity(9.8f);

					switch(j)
					{
					case 0:
						frag->setColor(0.61f, 0.44f, 0.13f, 1.0f);
						break;
					case 1:
						frag->setColor(0.77f, 0.56f, 0.16f, 1.0f);
						break;
					case 2:
						frag->setColor(0.86f, 0.72f, 0.43f, 1.0f);
						break;
					case 3:
						frag->setColor(0.91f, 0.79f, 0.58f, 1.0f);
						break;
					default:
						frag->setColor(r, g, b, 1.0f);
						break;
					}
					
					//frag->getComponents()->getGravity()->addForce(vec3(0.0f, -10.0f, 0.0f), frag->getVelocity(), 1.0);


					LO = 1.0f;
					HI = 2.0f;

					x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					frag->getComponents()->getGravity()->addForce(vec3(dirX * -x, dirY* - y, dirZ *-z), frag->getVelocity(), 1.0);

					++dirX;
					dirX =(dirX ==0?1:dirX);

					objectManager.addObject(frag);
				}

				dirX = -2;
			}

			++dirY;
					++dirZ;
					dirZ =(dirZ ==0?1:dirZ);
		}
}

void Keyboard(unsigned char key, int mx, int my)
{
	float step = 10.0f;

	float LO;
	float HI;
	float x; 
	float z; 
	float y;
	float r, g, b;
	Sphere *tempSphere;
	Generic_Object *newObject;
	Generic_Object* frag;
	vec3 oldPos;

	float dirX;
	float dirY;
	float dirZ;


	switch(key)
	{
	case 'u':
		makeExplosion();
		break;
	case 'g':
		frag = CreateFragments(vec3(0.0f, 5.0f, 0.0f), 1.0f);
		 
		LO = 0.0f;
		HI = 1.0f;

		r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

		b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

		frag->setWireframeMode(false);
		frag->getComponents()->getGravity()->setUseGravity(true);
		frag->getComponents()->getRigidBody()->addRigidBody(RIGID_GENERIC, *frag->getListOfVertices(), *frag->getListOfIndexes(), frag->getCenterOfObject(), true);
		frag->getComponents()->getRigidBody()->setDisplayRigidBody(true);
		frag->getComponents()->getRigidBody()->activateRigidBody(true);
		frag->getComponents()->getGravity()->setBounciness(frag->getMass(), frag->getMass()/3);
		frag->getComponents()->getGravity()->setResistance(frag->getMass(), frag->getMass() / 1.01f);
		frag->setAlternateFacesAndNormals(true);
		frag->getComponents()->getGravity()->setGravity(9.8f);
		frag->setColor(r, g, b, 1.0f);

		objectManager.addObject(frag);

		break;
	case 'v':

		dirX = -2;
		dirY = 4;
		dirZ = -2;


		for(int i = 0; i < 4; ++i)
		{
			for(int j= 0; j < 4; ++j)
			{
				for(int k =0; k < 4; ++k)
				{
					

					//LO = 0.0f;
					//HI = 1.0f;

					//r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					//g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					//b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					//newObject = new Generic_Object(*cube.getListOfVertices(), *cube.getListOfIndexes(), (float)k * 10.0f-10.f, (float)i * 10.0f + 5.0f + 20.0f, (float)j * 10.0f-10.0f, true); // rouge
					//objectManager.addObject(newObject);

					frag = CreateFragments(vec3((float)k * 10.0f-10.f + i*0, (float)i * 10.0f + 5.0f + 20.0f, (float)j * 10.0f-10.0f), 10.0f);

					cout << ++countSphere << " - x: " << k* 5.0f << "   y: " << i* 5.0f + 5.0f<< "   z: " << j* 5.0f << endl;
					//frag = CreateFragments(vec3(0.0f, 5.0f, 0.0f), 1.0f);
		 
					LO = 0.0f;
					HI = 1.0f;

					r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					frag->setWireframeMode(false);
					frag->getComponents()->getGravity()->setUseGravity(true);
					frag->getComponents()->getRigidBody()->addRigidBody(RIGID_GENERIC, *frag->getListOfVertices(), *frag->getListOfIndexes(), frag->getCenterOfObject(), true);
					frag->getComponents()->getRigidBody()->setDisplayRigidBody(true);
					frag->getComponents()->getRigidBody()->activateRigidBody(true);
					frag->getComponents()->getGravity()->setBounciness(frag->getMass(), frag->getMass()/5550);
					frag->getComponents()->getGravity()->setResistance(frag->getMass(), frag->getMass() / 1.01f);
					frag->setAlternateFacesAndNormals(true);
					frag->getComponents()->getGravity()->setGravity(9.8f);
					
					switch(j)
					{
					case 0:
						frag->setColor(0.61f, 0.44f, 0.13f, 1.0f);
						break;
					case 1:
						frag->setColor(0.77f, 0.56f, 0.16f, 1.0f);
						break;
					case 2:
						frag->setColor(0.86f, 0.72f, 0.43f, 1.0f);
						break;
					case 3:
						frag->setColor(0.91f, 0.79f, 0.58f, 1.0f);
						break;
					default:
						frag->setColor(r, g, b, 1.0f);
						break;
					}

					//frag->getComponents()->getGravity()->addForce(vec3(0.0f, -10.0f, 0.0f), frag->getVelocity(), 1.0);


					LO = 1.0f;
					HI = 2.0f;

					x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					frag->getComponents()->getGravity()->addForce(vec3(dirX * -x, dirY* - y, dirZ *-z), frag->getVelocity(), 1.0);

					++dirX;
					dirX =(dirX ==0?1:dirX);

					objectManager.addObject(frag);
				}

				dirX = -2;
			}

			++dirY;
					++dirZ;
					dirZ =(dirZ ==0?1:dirZ);
		}
		
		break;
	case 'a':
		camera.position.y += step;
		break;
	case 'z':
		camera.orientation.y -= step;
		break;
	case 'e':
		camera.position.y -= step;
		break;
	case 'f':

		LO = -10.0f;
		HI = 10.0f;

		x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

		LO = 5.0f;
		HI = 50.0f;
		y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

		LO = 0.0f;
		HI = 1.0f;

		r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		

		cout << ++countSphere << " - x: " << x << "   y: " << y << "   z: " << z << endl;
		tempSphere = new Sphere(x, y, z, 1.0f, 10); // rouge
		objectManager.addObject(tempSphere);

		tempSphere->setWireframeMode(false);
		tempSphere->getComponents()->getGravity()->setUseGravity(true);
		tempSphere->getComponents()->getRigidBody()->addRigidBody(RIGID_SPHERE, vec3(tempSphere->getCenterOfObject().x, tempSphere->getCenterOfObject().y,tempSphere->getCenterOfObject().z), tempSphere->getRadius());
		tempSphere->getComponents()->getRigidBody()->setDisplayRigidBody(true);
		tempSphere->getComponents()->getRigidBody()->activateRigidBody(true);
		tempSphere->getComponents()->getGravity()->setBounciness(tempSphere->getMass(), tempSphere->getMass()/3);
		tempSphere->getComponents()->getGravity()->setResistance(tempSphere->getMass(), tempSphere->getMass() / 1.01f);
		tempSphere->getComponents()->getGravity()->setGravity(9.8f);
		tempSphere->setColor(r, g, b, 1.0f);

		break;
	case 't':

		oldPos = camera.position;

		camera.position = vec3(0.f, 30.f, 1.0f);
		camera.orientation = vec3(0.f, 0.f, 0.f);
		camera.target = vec3(0.f, 0.f, 0.f);

		DestructorManager::LaunchDestruction(objectManager, ground, vec3(0.0f, 0.0f, 0.0f), -camera.position);
		
		ground->getComponents()->getRigidBody()->changeIndexesList(*ground->indexesList);
		ground->getComponents()->getRigidBody()->changeVerticesList(*ground->verticesList);
		ground->getComponents()->getRigidBody()->getRigidBodyObjectGeneric()->resetWireframeModeindexes(*ground->listIndexesWireframe);
		
		
		camera.position = oldPos;

		break;

	case 'h':

		for(int i = 0; i < 3; ++i)
		{
			for(int j= 0; j < 3; ++j)
			{
				for(int k =0; k < 3; ++k)
				{
					

					LO = 0.0f;
					HI = 1.0f;

					r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

					b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
					newObject = new Generic_Object(*cube.getListOfVertices(), *cube.getListOfIndexes(), (float)k * 10.0f-10.f, (float)i * 10.0f + 5.0f + 20.0f, (float)j * 10.0f-10.0f, true); // rouge
					objectManager.addObject(newObject);

					cout << ++countSphere << " - x: " << k* 5.0f << "   y: " << i* 5.0f + 5.0f<< "   z: " << j* 5.0f << endl;
					newObject->setAlternateFacesAndNormals(true);
					newObject->setWireframeMode(true);
					newObject->getComponents()->getGravity()->setUseGravity(true);
					newObject->getComponents()->getRigidBody()->addRigidBody(RIGID_GENERIC, *newObject->getListOfVertices(), *newObject->getListOfIndexes(), newObject->getCenterOfObject(), true);
					newObject->getComponents()->getRigidBody()->setDisplayRigidBody(true);
					newObject->getComponents()->getRigidBody()->activateRigidBody(true);
					newObject->getComponents()->getGravity()->setBounciness(newObject->getMass(), newObject->getMass()/350);
					newObject->getComponents()->getGravity()->setResistance(newObject->getMass(), newObject->getMass() / 1.01f);
					newObject->getComponents()->getGravity()->setGravity(9.8f);
					newObject->setColor(r, g, b, 1.0f);
					newObject->getComponents()->getRigidBody()->setIsStatic(false);
					//newObject->getComponents()->getGravity()->addForce(vec3(-newObject->getCenterOfObject().x/10,
					//						-newObject->getCenterOfObject().y,-newObject->getCenterOfObject().z/7), newObject->getVelocity(), 1);
				}
			}
		}

		break;
	case 'r':

		LO = 0.0f;
					HI = 1.0f;

		r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

		b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		newObject = new Generic_Object(*cube.getListOfVertices(), *cube.getListOfIndexes(), 3.0f, 10.0f, 0.0f, true); // rouge
		objectManager.addObject(newObject);

		newObject->setAlternateFacesAndNormals(true);
		newObject->setWireframeMode(true);
		newObject->getComponents()->getGravity()->setUseGravity(true);
		newObject->getComponents()->getRigidBody()->addRigidBody(RIGID_GENERIC, *newObject->getListOfVertices(), *newObject->getListOfIndexes(), newObject->getCenterOfObject(), true);
		newObject->getComponents()->getRigidBody()->setDisplayRigidBody(true);
		newObject->getComponents()->getRigidBody()->activateRigidBody(true);
		newObject->getComponents()->getGravity()->setBounciness(newObject->getMass(), newObject->getMass()/350);
		newObject->getComponents()->getGravity()->setResistance(newObject->getMass(), newObject->getMass() / 1.01f);
		newObject->getComponents()->getGravity()->setGravity(9.8f);
		newObject->setColor(r, g, b, 1.0f);
		newObject->getComponents()->getRigidBody()->setIsStatic(false);


		//LO = 0.0f;
		//			HI = 1.0f;

		//r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		//g = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

		//b = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		//newObject = new Generic_Object(*cube.getListOfVertices(), *cube.getListOfIndexes(), 0.0f, 0.0f, -5.0f, true); // rouge
		//objectManager.addObject(newObject);

		//newObject->setAlternateFacesAndNormals(true);
		//newObject->setWireframeMode(true);
		//newObject->getComponents()->getGravity()->setUseGravity(false);
		//newObject->getComponents()->getRigidBody()->addRigidBody(RIGID_GENERIC, *newObject->getListOfVertices(), *newObject->getListOfIndexes(), newObject->getCenterOfObject(), true);
		//newObject->getComponents()->getRigidBody()->setDisplayRigidBody(true);
		//newObject->getComponents()->getRigidBody()->activateRigidBody(true);
		//newObject->getComponents()->getGravity()->setBounciness(newObject->getMass(), newObject->getMass()/350);
		//newObject->getComponents()->getGravity()->setResistance(newObject->getMass(), newObject->getMass() / 1.01f);
		//newObject->getComponents()->getGravity()->setGravity(9.8f);
		//newObject->setColor(r, g, b, 1.0f);
		//newObject->getComponents()->getRigidBody()->setIsStatic(false);
		break;
	}

}
