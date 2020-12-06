#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>
#include <unordered_map>

class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};
	
	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;
	
	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);
	
	// Get the current state
	State GetState() const { return mState; }
	
	// Called when the ghost should switch to the "Frightened" state
	void Frighten();
	
	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current path
	void DebugDrawPath(struct SDL_Renderer* render);

	//A* Algorithm
	bool AStarAlgorithm(class PathNode* startNode, class PathNode* target);
	bool NodeInSet(class PathNode* node, const std::vector<class PathNode*>& Set);
	float ComputeG(class PathNode* curNod, std::unordered_map<class PathNode*, class PathNode*>& parents);
	float ComputeH(class PathNode* curNod, PathNode* target);
	PathNode* GhostAI::LowestFNode(const std::vector<class PathNode*>& set, std::unordered_map<class PathNode*, float>& f);
	void CloseNode(std::vector<PathNode*>& Open, std::vector<class PathNode*>& Closed, class PathNode* mov);
	std::vector<PathNode*> FindPath(std::unordered_map<class PathNode*, class PathNode*>& Parents, PathNode* Target);

	//Failsafe
	void FailSafe(PathNode* RevNode);

	//Movement
	void UpdateMovement(float deltaTime);
	void CheckNodeCollision(float deltaTime);
	void ScatterMov(float deltaTime);
	Vector2 GetDirVec();
	Vector2 GetVelocity();
	void mPathTransition(PathNode* Target);
	void mNextCheck();

	//Tunnels
	void Teleport();
	void mPathTeleport();
	void frightTeleport();
	
	//Frightened
	void FrightenedMov(float deltaTime);
	void ChooseRandomNode();
	void UpdateFright(float deltatime);
	void UpdateFrightAnimation(class AnimatedSprite* sprite);

	//Death
	void DeathMovement(float deltaTime);
	void UpdateDeathAnimation(class AnimatedSprite* sprite);
	void UpdateDeath();

	//Animation
	void CalculateDir();
	void UpdateAnimation();
	void UpdateScattChaseAnimation(class AnimatedSprite* sprite);

	//Chase State
	void ChaseMov(float deltatime);
	void InkyChase(float deltatime);
	void PinkyChase(float deltatime);
	void ClydeChase(float deltatime);
	void BlinkyChase(float deltatime);
	void UpdateChaseScatt(float deltatime);

private:
	// Member data for pathfinding

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying
	// to get to
	class PathNode* mNextNode = nullptr;

	// This vector always contains the path
	// from "next node" to "target node"
	// (if there is still such a path)
	std::vector<class PathNode*> mPath;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;

	//Helper data
	float SPEED = 90.0f;
	float FrightTime = 0.0f;
	float ChaseTime = 0.0f;
	float ScatterTime = 7.0f;
	PathNode* reverseNode; // Will come in useful during failsafe
	Vector2 mDirection;
	PathNode* LastIntersectedNode = nullptr;

	//Booleans
	bool mDead = false;
	bool mScatter = true;
	bool mChase = false;
};
