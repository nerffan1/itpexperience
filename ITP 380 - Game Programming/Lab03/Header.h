#pragma once

class Paddle : public Actor
{
public:
	Paddle(class Game* game);
	~Paddle();
private:
	class SpriteComponent* mSprite = nullptr;
};