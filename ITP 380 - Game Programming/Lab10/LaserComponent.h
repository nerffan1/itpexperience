#pragma once
#include "MeshComponent.h"
#include "SegmentCast.h"

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner);
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;

	//Transformation
	class Matrix4 TransformLaser(LineSegment line);

private:
	void CreateReflections(LineSegment& initialLaser, Vector3 LastNormal, class Actor* ignoreAc);
	std::vector<LineSegment> mLineSeg;
	std::vector<class Actor*> MapBlocks;

};
