#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"


Camera::~Camera()
{
	mCamBuff->Release();
}

Camera::Camera(Graphics* Graphics) : mGraphics(Graphics), mCamBuff(nullptr)
{
	//Set Matrices
	mPerspective = Matrix4::CreateRotationY(-Math::PiOver2)
		* Matrix4::CreateRotationZ(-Math::PiOver2)
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
			mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight(),
			25.0f, 10000.0f);

	//Create Constant Buffer
	mCamBuff = mGraphics->CreateGraphicsBuffer(
		&mConsts,	 //What to put for InitialData?
		sizeof(mConsts),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
}

void Camera::SetActive()
{
	//Projection
	mWorldToCam.Invert();
	mConsts.mposition = mWorldToCam.GetTranslation();
	mWorldToCam.Invert();

	mConsts.viewProj = mWorldToCam * mPerspective;

	//Upload Buffer 
	mGraphics->UploadBuffer(mCamBuff,
		&mConsts,
		sizeof(mConsts)
	);
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(
		static_cast<UINT>(Graphics::CONSTANT_BUFFER_CAMERA),
		static_cast<UINT>(1),
		&mCamBuff
	);

	mGraphics->GetDeviceContext()->PSSetConstantBuffers(
		static_cast<UINT>(Graphics::CONSTANT_BUFFER_CAMERA),
		static_cast<UINT>(1),
		&mCamBuff
	);
}
