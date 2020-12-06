#pragma once
#include "SpriteComponent.h"
#include <string>	
#include <vector>

class TiledBGComponent : public SpriteComponent
{
public:
	TiledBGComponent(class Actor* mOwner, int drawOrder = 50);
	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
	void Draw(SDL_Renderer* renderer) override;
	SDL_Rect* GetSourceRect(int tile) const;
protected:
	SDL_Rect* r = new SDL_Rect;
	int mTileWidth;
	int mTileHeight;
	std::vector<std::vector<int>> mCSVInfo;
};