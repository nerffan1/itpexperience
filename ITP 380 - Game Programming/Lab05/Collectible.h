#pragma once
#include <functional>
#include "Component.h"

class Collectible : public Component
{
public:
	Collectible(class Actor* owner);
	void SetOnCollect(std::function<void()> OnCollect);
	void Update(float deltaTime) override;
protected:
	std::function<void()> mOnCollect;
};