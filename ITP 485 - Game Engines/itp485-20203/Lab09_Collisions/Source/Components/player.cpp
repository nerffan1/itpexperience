#include "stdafx.h"
#include "player.h"
#include "animation.h"
#include "game.h"
#include "jsonUtil.h"
#include "skinnedObj.h"
#include "stringUtil.h"


static const float s_accel = 900.0f;
static const float s_moveSpeed = 300.0f;
static const float s_turnSpeed = Math::ToRadians(720.0f);
static const float s_gravity = 500.0f;
static const float s_jumpStart = 300.0f;


Player::Player(Game* pGame, SkinnedObj *pObj)
    : Character(pGame, pObj)
	, mMoveSpeed(0.0f)
    , mState(State::STATE_UNKNOWN)
    , mVel(Vector3::Zero)
    , mPrevAnimTime(0.0f)
    ,mCheck(Vector3::Zero, Vector3::Zero)
{
	Matrix4 mat = mObj->mObjConsts.modelToWorld;
    mPos = mat.GetTranslation();
    Vector3 fwd = mat.GetXAxis();
	mHeading = atan2f(fwd.y, fwd.x);
}

void Player::Update(float deltaTime)
{
	Vector3 move = Vector3::Zero;
	Game* pGame = mGame;
	if (pGame->IsKeyHeld(VK_UP))
		move.x += 1.0f;
	if (pGame->IsKeyHeld(VK_DOWN))
		move.x -= 1.0f;
	if (pGame->IsKeyHeld(VK_RIGHT))
		move.y += 1.0f;
	if (pGame->IsKeyHeld(VK_LEFT))
		move.y -= 1.0f;

	float throttle = move.Length();
	if (throttle > 1.0f)
		throttle = 1.0f;

	if (throttle > 0.0f)
	{	// turn to heading
		float heading = atan2f(move.y, move.x);
		float angDelta = heading - mHeading;
		if (angDelta > Math::Pi)		//mrwTODO wrap angle
			angDelta -= Math::TwoPi;
		if (angDelta < -Math::Pi)
			angDelta += Math::TwoPi;
		float angSpd = s_turnSpeed * deltaTime;
		angDelta = Math::Clamp(angDelta, -angSpd, angSpd);
		mHeading += angDelta;
		if (mHeading > Math::Pi)		//mrwTODO wrap angle
			mHeading -= Math::TwoPi;
		if (mHeading < -Math::Pi)
			mHeading += Math::TwoPi;
	}

	{	// accelerate
		float speed = throttle * s_moveSpeed;
		float spdDelta = speed - mMoveSpeed;
		float accel = s_accel * deltaTime;
		spdDelta = Math::Clamp(spdDelta, -accel, accel);
		mMoveSpeed += spdDelta;
	}

    mPrevAnimTime = mAnimTime;
    Character::Update(deltaTime);

    ChangeState();
    UpdateState(deltaTime);
}

bool Player::CheckGround(float* groundHeight)
{
    mCheck.start = mPos + Vector3(0.0f, 0.0f, 10.0f);
    mCheck.end = mPos + Vector3(0.0f, 0.0f, -10.0f);
    Vector3 newHeight;
    auto platforms = Physics::getPlatforms();
    for (auto plat : platforms)
    {
        if (Physics::Intersect(mCheck,plat->GetAABB(),&newHeight))
        {
            *groundHeight = newHeight.z;
            return true;
        }
    }
    return false;
}

void Player::SetState(State newState)
{
    switch (newState)
    {
    case State::STATE_IDLE:
        SetAnim("idle");
        mVel.z = 0.0f;
        break;
    case State::STATE_FALL:
        SetAnim("fall");
        break;
    case State::STATE_WALK:
        SetAnim("run");
        mVel.z = 0.0f;
        break;
    case State::STATE_JUMP:
        SetAnim("jumpStart");
        mVel.z = s_jumpStart;
        break;
    case State::STATE_LAND:
        SetAnim("land");
        mVel.z = 0.0f;
        break;
    }
    mState = newState;
}

void Player::ChangeState()
{
    float groundHeight = 0.0f;
    bool isOverGround = CheckGround(&groundHeight);
    bool isOnGround = isOverGround && mPos.z <= groundHeight;

    switch (mState)
    {
    case State::STATE_UNKNOWN:
        SetState(State::STATE_IDLE);
        break;
    case State::STATE_LAND:
        if (mAnimTime < mPrevAnimTime)
            SetState(State::STATE_IDLE);
    case State::STATE_IDLE:
        if (mGame->IsKeyHeld(VK_SPACE))
            SetState(State::STATE_JUMP);
        else if (false == isOnGround)
            SetState(State::STATE_FALL);
        else if (mVel.LengthSq() > 0.0001f)
            SetState(State::STATE_WALK);
        break;
    case State::STATE_WALK:
        if (mGame->IsKeyHeld(VK_SPACE))
            SetState(State::STATE_JUMP);
        else if (false == isOnGround)
            SetState(State::STATE_FALL);
        else if (mVel.LengthSq() < 0.0001f)
            SetState(State::STATE_IDLE);
        break;
    case State::STATE_JUMP:
        if (mAnimTime < mPrevAnimTime)
            SetAnim("jump");
    case State::STATE_FALL:
        if (mVel.z < 0.0f && isOnGround)
        {
            mPos.z = groundHeight;
            SetState(State::STATE_LAND);
        }
        break;
    }
}

void Player::UpdateState(float deltaTime)
{
    Matrix4 mat = Matrix4::CreateRotationZ(mHeading);
    Vector3 vel = mMoveSpeed * mat.GetXAxis();
    mVel.x = vel.x;
    mVel.y = vel.y;

    switch (mState)
    {
    case State::STATE_IDLE:
    case State::STATE_LAND:
        break;
    case State::STATE_WALK:
        break;
    case State::STATE_FALL:
    case State::STATE_JUMP:
        mVel.z -= s_gravity * deltaTime;
        break;
    }

    mPos += mVel * deltaTime;
    mObj->mObjConsts.modelToWorld = mat
        * Matrix4::CreateTranslation(mPos);
}
