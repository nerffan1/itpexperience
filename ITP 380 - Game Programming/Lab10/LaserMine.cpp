#include "LaserMine.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Game.h"
#include "SegmentCast.h"
#include "LaserComponent.h"

LaserMine::LaserMine(Game* game, Actor* parent) : Actor(game, parent)
{
	//Set Mesh
	mMesh = mGame->GetRenderer()->GetMeshComp("Assets/LaserMine.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMeshComp(mMesh);

	//Set Laser
	mLaser = new LaserComponent(this);
}
