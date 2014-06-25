#ifndef __OBJECTMANAGER__
#define __OBJECTMANAGER__

#include "../../EsgiGL/Common/matrix.h"
#include "../../EsgiGL/EsgiShader.h"
#include "GameObject.h"

#include <vector>

class GameObjectManager
{
	// Attributs
public:
	std::vector<GameObject*> * listObjects;
protected:
private:
	EsgiShader shader;
	GLint position_attrib;
	GLint color_attrib;

	//Methods
public:
	GameObjectManager(void);
	~GameObjectManager(void);
	void Setup(const char * vertexShader, const char * fragmentShader);
	void Clean();
	void RenderObjects(mat4 projectionMatrix, mat4 viewMatrix, mat4 worldMatrix);
protected:
private:
	virtual void BeforeRender(mat4 projectionMatrix, mat4 viewMatrix, mat4 worldMatrix);
	virtual void AfterRender();
};

#endif