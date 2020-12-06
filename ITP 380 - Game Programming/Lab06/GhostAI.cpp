#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"
#include <unordered_map>
#include "Math.h"


GhostAI::GhostAI(class Actor* owner)
	:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateAnimation();
	UpdateChaseScatt(deltaTime);
}
void GhostAI::Frighten()
{
	if (mState != Dead && mState != Frightened)
	{
		mState = State::Frightened;
		AnimatedSprite* sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("scared0");
		FrightTime = 7.0f;

		//Change nodes 
		reverseNode = mNextNode;
		mNextNode = mPrevNode;
		mPrevNode = reverseNode;
	}
}
void GhostAI::Start(PathNode* startNode)
{
	mGhost->SetPosition(startNode->GetPosition());
	mState = State::Scatter;
	mPrevNode = nullptr;
	mNextNode = nullptr;
	mTargetNode = nullptr;
	if (AStarAlgorithm(startNode, mGhost->GetScatterNode()))
	{
	}
	else
	{
		SDL_Log("There is no path");
	}
}
void GhostAI::Die()
{
	mState = State::Dead;
	mDead = true;
	mPath.clear();
	PathNode* saveCurrentNode = mPrevNode;
	AStarAlgorithm(mNextNode, mGhost->GetGame()->mGhostPen);
	mPrevNode = saveCurrentNode;
}
void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	// Line from next node to subsequent on path
	SDL_RenderDrawLine(render,
		static_cast<int>(mNextNode->GetPosition().x),
		static_cast<int>(mNextNode->GetPosition().y),
		static_cast<int>(mPath.back()->GetPosition().x),
		static_cast<int>(mPath.back()->GetPosition().y));

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}

//Extra Functions
void GhostAI::UpdateChaseScatt(float deltatime)
{
	if (mState != Frightened && mState != Dead)
	{
		if (ScatterTime > 0)
		{
			ScatterTime -= deltatime;
		}
		else
		{
			if (ChaseTime <= 0.0f)
			{
				mState = Scatter;
				mScatter = true;
				ScatterTime = 7.0f;
			}
		}
		if (ChaseTime > 0)
		{
			ChaseTime -= deltatime;
		}
		else
		{
			if (ScatterTime < 0.0f)
			{
				mState = Chase;
				mChase = true;
				ChaseTime = 4.0f;
			}
		}
	}
}

//A* Algorithm
bool GhostAI::AStarAlgorithm(PathNode* startNode, PathNode* target)
{
	mPath.clear();
	//Create variables that will help in the algorithm
	PathNode* Current = nullptr; // The Current node
	std::vector<PathNode*> OpenSet;
	std::vector<PathNode*> ClosedSet;
	std::unordered_map<PathNode*, float> fMap;
	std::unordered_map<PathNode*, float> hMap;
	std::unordered_map<PathNode*, float> gMap;
	std::unordered_map<PathNode*, PathNode*> ParentNode;

	Current = startNode;
	ParentNode[Current] = nullptr; //Our starting node has no parent node!
	ClosedSet.push_back(Current);

	//Modified A*
	do
	{
		for (PathNode* n : Current->mAdjacent)
		{
			if (n != ParentNode[n] && n != mPrevNode)
			{
				if (NodeInSet(n, ClosedSet))
				{
					continue;
				}
				else if (NodeInSet(n, OpenSet))
				{
					//Check if there's a better parent
					PathNode* originalParent = ParentNode[n];
					ParentNode[n] = Current;
					float newG = ComputeG(n, ParentNode);
					if (newG < gMap[n])
					{
						gMap[n] = newG;
						fMap[n] = gMap[n] + hMap[n];
					}
					else
					{
						ParentNode[n] = originalParent;
					}
				}
				else
				{
					//Add node to OpenSet if not found in either set
					ParentNode[n] = Current;
					hMap[n] = ComputeH(n, target);
					gMap[n] = ComputeG(n, ParentNode);
					fMap[n] = hMap[n] + gMap[n];
					OpenSet.push_back(n);
				}
			}
		}

		// If Openset is empty, then there is no path
		if (OpenSet.empty())
		{
			break;
		}

		//After looking at all the adjacent nodes
		Current = LowestFNode(OpenSet, fMap);
		CloseNode(OpenSet, ClosedSet, Current);

	} while (Current != mTargetNode);

	//Set some parameters 
	mTargetNode = target;
	mPrevNode = startNode;
	mPath = FindPath(ParentNode, target);
	mNextNode = mPath.back();
	return true; // Algorithm is succesful	
}
bool GhostAI::NodeInSet(PathNode* node, const std::vector<PathNode*>& Set)
{
	auto iter = std::find(Set.begin(), Set.end(), node);
	if (iter != Set.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
float GhostAI::ComputeG(PathNode* curr, std::unordered_map<PathNode*, PathNode*>& parents)
{
	if (parents[curr] != nullptr)
	{
		//Local Variables
		float edge = 0.0f;

		//Check if nodes are tunnels, whose edge is 0
		bool NodesAreTunnel = (curr->GetType() == PathNode::Type::Tunnel || parents[curr]->GetType() == PathNode::Type::Tunnel);
		if (NodesAreTunnel)
		{
			edge = 0.0f;
		}
		else
		{
			edge = (curr->GetPosition() - parents[curr]->GetPosition()).Length();
		}

		//Recurse 
		return  edge + ComputeG(parents[curr], parents);
	}
	else
	{
		return 0.0f; //The start node has a null parentnode
	}
}
float GhostAI::ComputeH(PathNode* curNod, PathNode* target)
{
	//The tunnels must be taken into account, so compare the heuristic case
	//of going directly to target, to the case where tunnels are taken
	float DistToTarget = (target->GetPosition() - curNod->GetPosition()).Length();
	float LeftToTarget = (target->GetPosition() - mOwner->GetGame()->mTunnelLeft->GetPosition()).Length();
	float RightToTarget = (target->GetPosition() - mOwner->GetGame()->mTunnelRight->GetPosition()).Length();
	float DistToRightT = (curNod->GetPosition() - mOwner->GetGame()->mTunnelRight->GetPosition()).Length();
	float DistToLeftT = (curNod->GetPosition() - mOwner->GetGame()->mTunnelLeft->GetPosition()).Length();

	//Create the h(x) for the tunnel cases 
	float TunnelLeftH = DistToLeftT + RightToTarget;
	float TunnelRightH = DistToRightT + LeftToTarget;

	return Math::Min(DistToTarget, Math::Min(TunnelLeftH, TunnelRightH));
}
PathNode* GhostAI::LowestFNode(const std::vector<PathNode*>& set, std::unordered_map<PathNode*, float>& f)
{
	//Create variable that will be returned
	PathNode* TempNode = set[0];

	//Loop through the rest of the vector and find the min value
	for (size_t i = 0; i < set.size(); i++)
	{
		if (f[set[i]] < f[TempNode])
		{
			TempNode = set[i];
		}
	}

	return TempNode;
}
void GhostAI::CloseNode(std::vector<PathNode*>& Open, std::vector<PathNode*>& Closed, PathNode* mov)
{
	//Add to Closed Set
	Closed.push_back(mov);

	//Get node iterator
	auto iter = std::find(Open.begin(), Open.end(), mov);

	//Delete node from Open Set
	if (iter != Open.end())
	{
		Open.erase(iter);
	}
	else
	{
		SDL_Log("The node you're trying to close is not in the OpenSet.");
	}

}
std::vector<PathNode*> GhostAI::FindPath(std::unordered_map<PathNode*, PathNode*>& Parents, PathNode* Target)
{
	std::vector<PathNode*> path;
	PathNode* buffer = Target;
	path.push_back(buffer);

	//Create a vector of all the nodes
	while (Parents[buffer] != nullptr)
	{
		buffer = Parents[buffer];
		path.push_back(buffer);
	}
	return path;
}
void GhostAI::FailSafe(PathNode* RevNode)
{
	float dist = 2000; //Initiate random big value so that we can set it to lower value later
	for (PathNode* nod : mPrevNode->mAdjacent)
	{
		if (nod != RevNode)
		{
			float DistNode = (nod->GetPosition() - mTargetNode->GetPosition()).Length();
			if (DistNode < dist)
			{
				dist = DistNode;
				mNextNode = nod;
			}
		}
	}

	if (reverseNode != mTargetNode && mPrevNode != mTargetNode)
	{
		//A* changes the mPrevNode, so I must make sure to save it. 
		PathNode* saveCurrentNode = mPrevNode;
		AStarAlgorithm(mNextNode, mTargetNode);
		mPrevNode = saveCurrentNode;
	}
}

//Movement Functions
void GhostAI::UpdateMovement(float deltaTime)
{
	if (mState == State::Scatter && mPrevNode != nullptr && mNextNode != nullptr)
	{
		SPEED = 90.0f;
		ScatterMov(deltaTime);
	}
	else if (mState == State::Frightened)
	{
		SPEED = 65.0f;
		FrightenedMov(deltaTime);
	}
	else if (mState == Dead)
	{
		SPEED = 125.0f;
		DeathMovement(deltaTime);
	}
	else if (mState == State::Chase)
	{
		SPEED = 90.0f;
		ChaseMov(deltaTime);
	}
}
void GhostAI::CheckNodeCollision(float deltaTime)
{
	Vector2 NewPos = mGhost->GetPosition() + GetVelocity() * deltaTime;
	mGhost->SetPosition(NewPos);
	CollisionComponent* GhostColl = mGhost->GetComponent<CollisionComponent>();
	CollisionComponent* NodeColl = mNextNode->GetComponent<CollisionComponent>();
	Vector2 offset;
	CollSide Node = GhostColl->GetMinOverlap(NodeColl, offset);
	if (Node != CollSide::None)
	{
		LastIntersectedNode = mNextNode;
		reverseNode = mPrevNode;
		mPrevNode = mNextNode;
		mNextCheck();

		if (mState == Chase)
		{
			mPathTransition(mOwner->GetGame()->mPlayer->GetPrevNode());
			if (!mPath.empty())
			{
				mPath.pop_back();
			}
			if (!mPath.empty()) //If there's more nodes, then remove last one
			{
				mNextNode = mPath.back();
			}
		}
		else if (mState != State::Frightened)
		{
			if (!mPath.empty())
			{
				mPath.pop_back();
			}
			if (!mPath.empty()) //If there's more nodes, then remove last one
			{
				mNextNode = mPath.back();
			}
			else // If empty, target has been reached!
			{
				if (mState == Dead)
				{
					mState = State::Scatter;
				}
				else if (mState == Scatter)
				{
					FailSafe(reverseNode);
				}
			}
		}
	}
	CalculateDir();
}
void GhostAI::mPathTransition(PathNode* Target)
{
	PathNode* saveCurrentNode = mPrevNode;
	AStarAlgorithm(mNextNode, Target);
	mPrevNode = saveCurrentNode;
}

//Direction Help
Vector2 GhostAI::GetDirVec()
{
	if (mPrevNode->GetType() == PathNode::Type::Tunnel)
	{
		return mDirection;
	}
	else
	{
		mDirection = mNextNode->GetPosition() - mPrevNode->GetPosition();
		float magnitude = (mPrevNode->GetPosition() - mNextNode->GetPosition()).Length();
		mDirection *= (1 / magnitude);
	}

	return mDirection;
}
Vector2 GhostAI::GetVelocity()
{
	//Find the vector
	return GetDirVec() * SPEED;
}
void GhostAI::CalculateDir()
{
	mDirection = GetDirVec();

	//Set direction and return it 
	if (mDirection.x < 0.0f)
	{
		mGhost->SetDirection(Direction::Left);
	}
	else if (mDirection.x > 0.0f)
	{
		mGhost->SetDirection(Direction::Right);
	}
	else if (mDirection.y > 0.0f)
	{
		mGhost->SetDirection(Direction::Down);
	}
	else if (mDirection.y < 0.0f)
	{
		mGhost->SetDirection(Direction::Up);
	}
	else
	{
		mGhost->SetDirection(Direction::Stationary);
	}
}

//Tunnels 
void GhostAI::Teleport()
{
	if (mState == State::Frightened)
	{
		frightTeleport();
	}
	else
	{
		mPathTeleport();
	}
}
void GhostAI::mNextCheck()
{
	//If Node is of type Tunnel, then teleport the ghost
	if (mPrevNode->GetType() == PathNode::Tunnel)
	{
		Teleport();
	}
	else
	{
		mGhost->SetPosition(mNextNode->GetPosition());
		if (mState == Frightened)
		{
			ChooseRandomNode();
		}
	}
}
void GhostAI::mPathTeleport()
{
	if (mPrevNode == mOwner->GetGame()->mTunnelLeft)
	{
		mGhost->SetPosition(mOwner->GetGame()->mTunnelRight->GetPosition());
		reverseNode = mPrevNode;
		mPrevNode = mOwner->GetGame()->mTunnelRight;
	}
	else
	{
		mGhost->SetPosition(mOwner->GetGame()->mTunnelLeft->GetPosition());
		reverseNode = mPrevNode;
		mPrevNode = mOwner->GetGame()->mTunnelLeft;
	}
	mPath.pop_back();
	mNextNode = mPath.back();
}
void GhostAI::frightTeleport()
{
	if (mPrevNode == mOwner->GetGame()->mTunnelLeft)
	{
		mGhost->SetPosition(mOwner->GetGame()->mTunnelRight->GetPosition());
		reverseNode = mPrevNode;
		mPrevNode = mOwner->GetGame()->mTunnelRight;
		for (size_t i = 0; i < 2; i++)
		{
			if (mPrevNode->mAdjacent[i] != mOwner->GetGame()->mTunnelLeft)
			{
				mNextNode = mPrevNode->mAdjacent[i];
			}
		}
	}
	else
	{
		mGhost->SetPosition(mOwner->GetGame()->mTunnelLeft->GetPosition());
		reverseNode = mPrevNode;
		mPrevNode = mOwner->GetGame()->mTunnelLeft;
		for (size_t i = 0; i < 2; i++)
		{
			if (mPrevNode->mAdjacent[i] != mOwner->GetGame()->mTunnelRight)
			{
				mNextNode = mPrevNode->mAdjacent[i];
			}
		}
	}
}

//Scatter
void GhostAI::ScatterMov(float deltaTime)
{
	//mPath Transition
	if (mDead == true || mChase == true)
	{
		mTargetNode = mGhost->GetScatterNode();
		mPathTransition(mTargetNode);
		mDead = false;
		mChase = false;
	}
	CheckNodeCollision(deltaTime);
}


//Frightened
void GhostAI::FrightenedMov(float deltaTime)
{
	mPath.clear();
	CheckNodeCollision(deltaTime);
	UpdateFright(deltaTime);
}
void GhostAI::ChooseRandomNode()
{
	//For some reason the GetIntRange doesn't work for the GhostPen, so I'll make a case just for it
	if (mPrevNode->mAdjacent.size() == 1)
	{
		mNextNode = mPrevNode->mAdjacent[0];
	}
	else
	{
		do
		{
			size_t rand = Random::GetIntRange(0, mPrevNode->mAdjacent.size() - 1);
			mNextNode = mPrevNode->mAdjacent[rand];
		} while (mNextNode == reverseNode || mNextNode == mPrevNode || mNextNode == mOwner->GetGame()->mGhostPen);
	}
}
void GhostAI::UpdateFright(float deltatime)
{
	if (FrightTime <= 0.0f)
	{
		mState = State::Scatter;
	}
	else
	{
		FrightTime -= deltatime;
	}
}

//Death
void GhostAI::DeathMovement(float deltaTime)
{
	CheckNodeCollision(deltaTime);
}
void GhostAI::UpdateDeath()
{
	if (mPrevNode == mOwner->GetGame()->mGhostPen)
	{
		mState = Scatter;
		reverseNode = mPrevNode;
		AStarAlgorithm(mPrevNode, mGhost->GetScatterNode());
	}
}
void GhostAI::UpdateDeathAnimation(AnimatedSprite* sprite)
{
	if (mOwner->mDirection == Direction::Left)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("deadleft");
	}
	else if (mOwner->mDirection == Direction::Up)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("deadup");
	}
	else if (mOwner->mDirection == Direction::Down)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("deaddown");
	}
	else if (mOwner->mDirection == Direction::Right)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("deadright");
	}
}

//Animation
void GhostAI::UpdateAnimation()
{
	AnimatedSprite* sprite = mGhost->GetComponent<AnimatedSprite>();

	if (mState == State::Scatter || mState == State::Chase)
	{
		UpdateScattChaseAnimation(sprite);
	}
	else if (mState == State::Frightened)
	{
		UpdateFrightAnimation(sprite);
	}
	else if (mState == State::Dead)
	{
		UpdateDeathAnimation(sprite);
	}
}
void GhostAI::UpdateFrightAnimation(AnimatedSprite* sprite)
{
	if (FrightTime > 2.0f)
	{
		sprite->SetAnimation("scared0");
	}
	else
	{
		sprite->SetAnimation("scared1");
	}
}
void GhostAI::UpdateScattChaseAnimation(AnimatedSprite* sprite)
{
	if (mOwner->mDirection == Direction::Left)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("left");
	}
	else if (mOwner->mDirection == Direction::Up)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("up");
	}
	else if (mOwner->mDirection == Direction::Down)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("down");
	}
	else if (mOwner->mDirection == Direction::Right)
	{
		sprite = mGhost->GetComponent<AnimatedSprite>();
		sprite->SetAnimation("right");
	}
}

//Chase State
void GhostAI::ChaseMov(float deltatime)
{
	if (mGhost->GetType() == mGhost->Blinky)
	{
		if (ChaseTime == 4.0f)
		{
			PathNode* saveCurrentNode = mPrevNode;
			AStarAlgorithm(mNextNode, mGhost->GetGame()->mPlayer->GetPrevNode());
			mPrevNode = saveCurrentNode;
		}
		BlinkyChase(deltatime);
	}
	else if (mGhost->GetType() == mGhost->Inky)
	{
		InkyChase(deltatime);
	}
	else if (mGhost->GetType() == mGhost->Pinky)
	{
		PinkyChase(deltatime);
	}
	else if (mGhost->GetType() == mGhost->Clyde)
	{
		ClydeChase(deltatime);
	}
}
void GhostAI::InkyChase(float deltatime)
{
}
void GhostAI::PinkyChase(float deltatime)
{
}
void GhostAI::ClydeChase(float deltatime)
{
}
void GhostAI::BlinkyChase(float deltatime)
{
	if (mDead == true || mScatter == true)
	{
		mPathTransition(mOwner->GetGame()->mPlayer->GetPrevNode());
		mDead = false;
		mScatter = false;
	}

	CheckNodeCollision(deltatime);
}
