#include "Sea.h"

void CSea::InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	this->mVertex[0] = pool::DxCommon::Vertex_TEX1(-1000, 0, 1000, 0, 1, 0, 0, 10);
	this->mVertex[1] = pool::DxCommon::Vertex_TEX1(1000, 0, 1000, 0, 1, 0, 10, 10);
	this->mVertex[2] = pool::DxCommon::Vertex_TEX1(-1000, 0, -1000, 0, 1, 0, 0, 0);
	this->mVertex[3] = pool::DxCommon::Vertex_TEX1(1000, 0, -1000, 0, 1, 0, 10, 0);
	
	D3DXCreateMeshFVF(2, 4, D3DXMESH_DYNAMIC, pool::DxCommon::Vertex_TEX1::FVF, device, &this->mMesh);

	pool::DxCommon::Vertex_TEX1 *vertex = NULL;
	this->mMesh->LockVertexBuffer(0, (void**)&vertex);

	for (int n=0 ; n!=4 ; ++n)
	{
		vertex[n] = this->mVertex[n];
	}

	this->mMesh->UnlockVertexBuffer();

	unsigned short *index = NULL;

	this->mMesh->LockIndexBuffer(0,(void**)&index);

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;

	this->mMesh->UnlockIndexBuffer();

	pool::DxCommon::XCOLOR mcolor1(0.9, 0.2, 0.2, 1.0f);
	pool::DxCommon::XCOLOR mcolor2(1.0, 1.0, 1.0, 1.0f);
	this->mMaterial = pool::Light::InitMtrl(mcolor1, mcolor1, mcolor1, mcolor2, 10);

	this->mTexture = pool::CreateTexture(TEXT("Sea"), device);
	this->mSpecialTexture = pool::LoadTexture(TEXT("Texture\\Sea.jpg"), device);

	this->mVS.InitializeVertexShader(device, TEXT("VertexShader\\Sea_VS.txt"), "Main", "vs_2_0");
	this->mPS.InitializePixelShader(device, TEXT("VertexShader\\Sea_PS.txt"), "Main", "ps_2_0");

	this->mRTTFlag = true;
	this->mSpace.mPatentRotationFlag = false;

}

void  CSea::UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	static DWORD timeForWave = 0;
	static DWORD timeForWave0 = 0;
	timeForWave0 = timeForWave;
	timeForWave += time;
	timeForWave %= 10000;
	
	D3DXHANDLE timeHandlethis = 
		this->mPS.GetTransformConstantTable()->GetConstantByName(NULL, "timeForWave");

	this->mPS.GetTransformConstantTable()->SetFloat(device, timeHandlethis, timeForWave);
}

void CSea::BeforeDrawEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera, pool::PNODE rootNode, pool::PGAME game)
{
	if (this->mTexture->mWidth != game->mBackBufferWidth || this->mTexture->mHeight != game->mBackBufferHeight)
		this->mTexture->CreateTextureForRender(device, game->mBackBufferWidth, game->mBackBufferHeight);

	pool::CCameraPool fcamera;

	pool::DxCommon::V3 pos(camera->GetPosition());
	pool::DxCommon::V3 look(camera->GetLook());
	pool::DxCommon::V3 up(camera->GetUp());

	pos.y *= -1;
	look.y *= -1;
	up.y *= -1;

	fcamera.Initialize(pos, look, up);

	this->RendererToTexture(rootNode, device, &fcamera);
}

void CSea::DrawEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	D3DXHANDLE projMatrixHandlethis = 
		this->mVS.GetTransformConstantTable()->GetConstantByName(NULL, "ViewProjMatrix");

	pool::DxCommon::M4X4 view, proj, viewproj;
	device->GetTransform(D3DTS_PROJECTION, &proj);
	device->GetTransform(D3DTS_VIEW, &view);
	viewproj = view * proj;
	this->mVS.GetTransformConstantTable()->SetMatrix(device, projMatrixHandlethis ,&viewproj);

	D3DXHANDLE worldMatrixHandlethis = 
		this->mVS.GetTransformConstantTable()->GetConstantByName(NULL, "WorldMatrix");
	this->mVS.GetTransformConstantTable()->SetMatrix(device, worldMatrixHandlethis ,&this->mSpace.GetMatrix());
	
	device->SetVertexShader(this->mVS.GetVertexShader());

	
	D3DXHANDLE cameraPositionHandlethis = 
		this->mPS.GetTransformConstantTable()->GetConstantByName(NULL, "cameraPosition");
	pool::DxCommon::V4 cameraV4(camera->GetPosition(), 1.0f);
	this->mPS.GetTransformConstantTable()->SetVector(device, cameraPositionHandlethis, &cameraV4);

	D3DXCONSTANT_DESC textureDesc0, textureDesc1;
	UINT count0, count1;
	
	D3DXHANDLE textureHandlethis = 
		this->mPS.GetTransformConstantTable()->GetConstantByName(NULL, "rttMap");
	this->mPS.GetTransformConstantTable()->GetConstantDesc(textureHandlethis, &textureDesc0, &count0);
	device->SetTexture(textureDesc0.RegisterIndex, this->mTexture->mTexture);
	device->SetSamplerState(textureDesc0.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(textureDesc0.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(textureDesc0.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	D3DXHANDLE textureWaveHandlethis = 
		this->mPS.GetTransformConstantTable()->GetConstantByName(NULL, "waveMap");
	this->mPS.GetTransformConstantTable()->GetConstantDesc(textureWaveHandlethis, &textureDesc1, &count1);
	device->SetTexture(textureDesc1.RegisterIndex, this->mSpecialTexture->mTexture);
	device->SetSamplerState(textureDesc1.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(textureDesc1.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(textureDesc1.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	device->SetPixelShader(this->mPS.GetPixelShader());

}
