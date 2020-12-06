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

struct MeshFile
{
    Vector3 position;
    Vector3 normal;
    Graphics::Color4 Color;
    Vector2 texcoord;
};

struct Phong
{
    Vector3 pos;
    Vector3 normal;
    Vector2 uvpos;
};

struct Unlit
{
    Vector3 pos;
    Vector2 uvpos;
};

struct Skinned
{
    Vector3 pos;
    Vector3 normal;
    UINT8 bones[4];
    UINT8 weights[4];
    Vector2 uvpos;
};
