#pragma once
#include "assetCache.h"
#include "Shader.h"
#include "texture.h"
#include "Skeleton.h"
#include "Animation.h"

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
	
	//MeshManagement
	void SetMesh(const std::wstring& meshName, class Mesh* pMesh);
	Mesh* GetMesh(const std::wstring& meshName);
	Mesh* LoadMesh(const std::wstring& fileName);

	//SkeletonManagement
	Skeleton* LoadSkeleton(const std::wstring& fileName);
	void SetSkel(const std::wstring& skelName, Skeleton* pSkel);
	Skeleton* GetSkel(const std::wstring& skelName);

	//AnimationManagement
	Animation* LoadAnimation(const std::wstring& fileName);
	void SetAnim(const std::wstring& animName, Animation* pAnim);
	Animation* GetAnim(const std::wstring& animName);

	Graphics* GetGraphics() { return mGraphics; }
	void Clear();

private:
	Graphics* mGraphics;
	AssetCache<Shader> mShadCache;
	AssetCache<Texture> mTexCache;
	AssetCache<Mesh> mMeshCache;
	AssetCache<Skeleton> mSkelCache;
	AssetCache<Animation> mAnimCache;
};
