#pragma once
#include "engineMath.h"
#include "Graphics.h"
#include <vector>

class RenderObj
{
public:

	struct PerObjConstants {
		Matrix4 modelToWorld;
	} mObjConsts;

	ID3D11Buffer* mObjBuff;

	
	RenderObj(Graphics* graphics, const class Mesh* pMesh);
	virtual ~RenderObj();
	
	//Graphics
	void SetTexture(int slot, const class Texture* texture);
	virtual void Draw();

	//Components
	void AddComponent(class Component* pComp) { mComponents.push_back(pComp); }

	//Update
	virtual void Update(float deltaTime);

	//Getter
	const Mesh* getMesh() { return mMesh; }
	Graphics* getGraphics() { return mGraphics; }

protected:
	Graphics* mGraphics;
	const Mesh* mMesh;
	std::vector<Component*> mComponents;
};