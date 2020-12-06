#pragma once
#include "engineMath.h"
#include "Graphics.h"

struct VertexPosColor
{
    Vector3 pos;
    Graphics::Color4 color;
};

struct VertexForTexture
{
    Vector3 pos;
    Graphics::Color4 color;
    Vector2 uvpos;
    Vector3 normal;
};
