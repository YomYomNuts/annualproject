#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(void)
{
	this->listObjects = new std::vector<GameObject*>();
}

GameObjectManager::~GameObjectManager(void)
{
	delete this->listObjects;
}

void GameObjectManager::Setup(const char * vertexShader, const char * fragmentShader)
{
	this->shader.LoadVertexShader(vertexShader);
	this->shader.LoadFragmentShader(fragmentShader);
	this->shader.Create();
}

void GameObjectManager::Clean()
{
	this->shader.Destroy();
}

void GameObjectManager::RenderObjects(mat4 projectionMatrix, mat4 viewMatrix, mat4 worldMatrix)
{
	this->BeforeRender(projectionMatrix, viewMatrix, worldMatrix);
	
	for (unsigned int i = 0; i < this->listObjects->size(); ++i)
		this->listObjects->at(i)->RenderObject(this->position_attrib, this->color_attrib);
	
	this->AfterRender();
}

void GameObjectManager::BeforeRender(mat4 projectionMatrix, mat4 viewMatrix, mat4 worldMatrix)
{
	GLuint programObject = this->shader.GetProgram();
	glUseProgram(programObject);
	
	this->color_attrib = glGetUniformLocation(programObject, "u_Color");

	GLint projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, false, &projectionMatrix.I.x);
	
	GLuint viewUniform = glGetUniformLocation(programObject, "u_ViewMatrix");
	glUniformMatrix4fv(viewUniform, 1, false, &viewMatrix.I.x);

	GLint worldUniform = glGetUniformLocation(programObject, "u_WorldMatrix");
	glUniformMatrix4fv(worldUniform, 1, 0, &worldMatrix.I.x);

	this->position_attrib = glGetAttribLocation(programObject, "a_Position");
	
	glEnableVertexAttribArray(this->position_attrib);
}

void GameObjectManager::AfterRender()
{
	glDisableVertexAttribArray(this->position_attrib);
}