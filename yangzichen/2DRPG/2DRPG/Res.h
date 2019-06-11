#pragma once
#include "Util.h"

//顶点
struct TexVertex
{
	TexVertex() : ux(0.0f), uy(0.0f), uz(1.0f) {}
	TexVertex(float X, float Y, float Z, float U, float V) : x(X), y(Y), z(Z), ux(0.0f), uy(0.0f), uz(1.0f), u(U), v(V){}

	float x, y, z;
	float ux, uy, uz;
	float u, v;
};

#define FVF_TEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

class Camera;

//2d模型文件
class Model2d
{
public:
	Model2d();
	virtual ~Model2d();

	void draw(int texEnum, D3DXMATRIX m, Camera *c);

protected:
	IDirect3DVertexBuffer9 *mVB;
	IDirect3DIndexBuffer9 *mIB;
};

//纹理枚举
enum 
{
	MAP_0 = 0,
	MAP_1,
	MAP_2_0,
	MAP_2_1,
	MAP_2_2,
	PLAYER,
	BUTTON1,
	BUTTON2,
	BUTTON3,
	FORM,
	TEXTURESIZE
};

//资源管理类
class Res
{
public:
	Res();
	virtual ~Res();

	Model2d *getModel() { return this->mModel; }
	LPDIRECT3DTEXTURE9 getTexture(int texEnum) { return this->mTexList[texEnum]; }
	ID3DXFont* getFont() {return this->mFont;}

protected:
	Model2d *mModel;
	ID3DXFont* mFont;

	std::vector<LPDIRECT3DTEXTURE9> mTexList;
};

