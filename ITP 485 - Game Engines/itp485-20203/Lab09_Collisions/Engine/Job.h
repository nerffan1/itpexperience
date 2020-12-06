#pragma once

class Job
{
public:
	virtual void DoIt() = 0;
};

class Anim : public Job
{
	void DoIt() override
	{
		int a = 0;
		int b = 0;
		int c = 0;
		int d = 0;
		int e = 0;
		e = 1 + b;
		c = a + e;
		e += c;
		b = 1 + a + b;
	}
};

class AnimateUp : public Job
{
public:
	AnimateUp(class Character* inChar);
	void DoIt() override;

private:
	Character* mChar;
};