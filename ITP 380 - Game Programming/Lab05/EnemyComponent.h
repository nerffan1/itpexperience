#pragma once
#include "Component.h"
#include <functional>

class EnemyComponent : public Component
{
public:
	EnemyComponent(class Actor* owner);
	~EnemyComponent();

	//Set callbacks
	void SetmOnDeath(std::function<void()> OnDeath) { mOnDeath = OnDeath; }
	void SetmOnTakeDamage(std::function<void()> OnTakeD) { mOnTakeDamage = OnTakeD;; }
	void TakeDamage(int dam);

protected:
	int HP;
	int mDamage;
	std::function<void()> mOnDeath;
	std::function<void()> mOnTakeDamage;
};