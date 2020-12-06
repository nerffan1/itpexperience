#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (mAnimName.empty())
	{	}
	else 
	{
		if (!mIsPaused)
		{
			mAnimTimer += mAnimFPS * deltaTime;
			//Wrapping 
			if (mAnimTimer >= mAnims[mAnimName].size())
			{
				mAnimTimer = (float)fmod(mAnimTimer, mAnims[mAnimName].size());
			}
		}
		//Pass the correct texture
		SetTexture(mAnims[mAnimName][(size_t)mAnimTimer]);
	}
}

void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer)
{
	if (mAnimName != name)
	{
		mAnimName = name;
	}
	
	if (resetTimer)
	{
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
	