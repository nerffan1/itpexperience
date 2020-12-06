#pragma once
#include "assetCache.h"
#include "Shader.h"
#include "texture.h"
class Graphics;

class AssetManager
{
public:
	AssetManager(Graphics* pGraphics);
	~AssetManager();

	//AssetManagement
	Shader* GetShader(const std::wstring& shaderName);
	void SetShader(const std::wstring& shaderName, Shader* pShader);
	
	//TextureManagement
	Texture* LoadTexture(const std::wstring& fileName);
	void SetTexture(const std::wstring& texName, Texture* pTex);
	
	void Clear();

	//MESH
	void SetMesh(const std::wstring& meshName, class Mesh* pMesh);
	Mesh* GetMesh(const std::wstring& meshName);
	Mesh* LoadMesh(const std::wstring& fileName);

	Graphics* GetGraphics() { return mGraphics; }

private:
	Graphics* mGraphics;
	AssetCache<Shader> mShadCache;
	AssetCache<Texture> mTexCache;
	AssetCache<Mesh> mMeshCache;
};
