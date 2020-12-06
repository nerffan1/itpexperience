#include "LaserComponent.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Block.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Player.h"

LaserComponent::LaserComponent(Actor* owner) : MeshComponent(owner, true)
{
	mMesh = mOwner->GetGame()->GetRenderer()->GetMeshComp("Assets/Laser.gpmesh");
	SetMeshComp(mMesh);
}

void LaserComponent::Update(float deltaTime)
{
	//Clear the laser, and redraw them all in new frame
	mLineSeg.clear();

	//Define Line Segment
	LineSegment FirstLine;
	FirstLine.mStart = mOwner->GetWorldPosition();
	FirstLine.mEnd = FirstLine.mStart + mOwner->GetWorldForward() * 500;
	mLineSeg.push_back(FirstLine);

	//Find Collisions of Laser and its reflections
	CreateReflections(mLineSeg[0], mOwner->GetQuatForward(), mOwner->GetParent());
}

void LaserComponent::Draw(Shader* shader)
{
	for (LineSegment m : mLineSeg)
	{
		if (mMesh)
		{
			// Set the world transform
			shader->SetMatrixUniform("uWorldTransform",
				TransformLaser(m));
			// Set the active texture
			Texture* t = mMesh->GetTexture(mTextureIndex);
			if (t)
			{
				t->SetActive();
			}
			// Set the mesh's vertex array as active
			VertexArray* va = mMesh->GetVertexArray();
			va->SetActive();
			// Draw
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}
	}
}

Matrix4 LaserComponent::TransformLaser(LineSegment line)
{
	//To find rotation axis, then find cross product vector to find rotation vector 
	Vector3 LaserDirection = line.mEnd - line.mStart;
	LaserDirection.Normalize();
	
	Vector3 rotationVector;
	if (Vector3::Dot(LaserDirection, Vector3::UnitX) == 1.0f)
	{
		rotationVector = Vector3::UnitY;
	}
	else if (Vector3::Dot(LaserDirection, Vector3::UnitX) ==  -1.0f)
	{
		rotationVector = -1.0f *Vector3::UnitY;
	}
	else 
	{
		rotationVector = Vector3::Cross(Vector3::UnitX, LaserDirection);
	}

	rotationVector.Normalize();
	float rotationAngle = Math::Acos(Vector3::Dot(Vector3::UnitX, mOwner->GetWorldForward()));
	Matrix4 RotMa = Matrix4::CreateFromQuaternion(Quaternion(rotationVector, rotationAngle));

	//Scale and Transformation
	Matrix4 ScaleMa = Matrix4::CreateScale(line.Length(), 1.0f, 1.0f);
	Matrix4 TransMa = Matrix4::CreateTranslation(Vector3::Lerp(line.mStart, line.mEnd, 0.5f));
	return ScaleMa * RotMa * TransMa;
}

void LaserComponent::CreateReflections(LineSegment& initialLaser, Vector3 LastNormal, Actor* ignoreAc)
{
	MapBlocks = mOwner->GetGame()->GetBlocks();
	Player* mPlayer = mOwner->GetGame()->GetPlayer();
	CastInfo CollisionInfo;
	
	//Check Player first to see if a line doesn't interesect with him
	if (SegmentCast(mPlayer, initialLaser, CollisionInfo))
	{
		initialLaser.mEnd = CollisionInfo.mPoint;
	}
	else if (SegmentCast(MapBlocks, initialLaser, CollisionInfo, ignoreAc))
	{
		initialLaser.mEnd = CollisionInfo.mPoint;
		Block* intersected = dynamic_cast<Block*>(CollisionInfo.mActor);
		if (intersected->GetMirror())
		{
			LineSegment newLine;
			Vector3 RefDir = Vector3::Reflect(LastNormal, CollisionInfo.mNormal);
			newLine.mStart = CollisionInfo.mPoint;
			newLine.mEnd = newLine.mStart + RefDir * 500.0f;
			mLineSeg.push_back(newLine);
			Actor* coll = CollisionInfo.mActor;
			CreateReflections(newLine,RefDir, coll);
		}
	}

}
