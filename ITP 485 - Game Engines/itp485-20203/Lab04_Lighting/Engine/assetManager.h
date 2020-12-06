#pragma once
#include "assetCache.h"

class Graphics;

class AssetManager
{
public:
    AssetManager(Graphics* pGraphics);
    ~AssetManager();

    void Clear();

    Graphics* GetGraphics() { return mGraphics; }

private:
    Graphics* mGraphics;
};
