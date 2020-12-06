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
	mConsts.mWorldToCam = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, -100.0f));
	mConsts.viewProj = Matrix4::CreateOrtho(
		mGraphics->GetScreenWidth(), mGraphics->GetScreenHeight(),
		1000.0f, -1000.0f
	);

	//Create Constant Buffer
	mCamBuff = mGraphics->CreateGraphicsBuffer(
		static_cast<void*>(&mConsts),	 //What to put for InitialData?
		sizeof(mConsts),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	//Upload Buffer

	mGraphics->UploadBuffer(   //Last Time I uploaded one matrix  
		mCamBuff,					//at a time?
		static_cast<void*>(&mConsts),
		sizeof(mConsts)
	);

}

void Camera::SetActive()
{
	//Projection
	mConsts.mWorldToCam.Invert();
	mConsts.mPosition = mConsts.mWorldToCam.GetTranslation();
	mConsts.mWorldToCam.Invert();

	//Upload Buffer 
	mGraphics->UploadBuffer(mCamBuff,
		&mConsts,
		sizeof(mConsts)
	);
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(
		static_cast<UINT>(mGraphics->CONSTANT_BUFFER_CAMERA),
		static_cast<UINT>(1),
		&mCamBuff
	);
}
