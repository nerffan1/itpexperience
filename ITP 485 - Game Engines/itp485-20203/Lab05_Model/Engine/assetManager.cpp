#include "stdafx.h"
#include "assetManager.h"
#include "Graphics.h"
#include "mesh.h"

AssetManager::AssetManager(Graphics* pGraphics)
    : mGraphics(pGraphics), 
    mShadCache(this),
    mTexCache(this),
    mMeshCache(this)
{
}


AssetManager::~AssetManager()
{
    
}

Shader* AssetManager::GetShader(const std::wstring& shaderName)
{
    return mShadCache.Get(shaderName);
}

void AssetManager::SetShader(const std::wstring& shaderName, Shader* pShader)
{
    mShadCache.Cache(shaderName, pShader);
}

Texture* AssetManager::LoadTexture(const std::wstring& fileName)
{
    return mTexCache.Load(fileName);
}

void AssetManager::SetTexture(const std::wstring& texName, Texture* pTex)
{
    mTexCache.Cache(texName, pTex);
}

void AssetManager::Clear()
{
    mShadCache.Clear();
    mTexCache.Clear();
    mMeshCache.Clear();
}

void AssetManager::SetMesh(const std::wstring& meshName, Mesh* pMesh)
{
    mMeshCache.Cache(meshName, pMesh);
}

Mesh* AssetManager::GetMesh(const std::wstring& meshName)
{
    return mMeshCache.Get(meshName);
}

Mesh* AssetManager::LoadMesh(const std::wstring& fileName)
{
    return mMeshCache.Load(fileName);
}
