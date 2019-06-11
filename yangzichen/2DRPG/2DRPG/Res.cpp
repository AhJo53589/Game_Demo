#include "Res.h"
#include "Define2DRPG.h"
#include "Camera2d.h"

Model2d::Model2d() : mVB(NULL), mIB(NULL)
{
	d3dDevice->CreateVertexBuffer(sizeof(TexVertex)*4, D3DUSAGE_WRITEONLY, FVF_TEX, D3DPOOL_MANAGED, &this->mVB, 0);
	d3dDevice->CreateIndexBuffer(sizeof(WORD)*6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &this->mIB, 0);

	TexVertex *v;
	this->mVB->Lock(0, 0, (void **)&v, 0);

	float side = TILESIDE / 2;

	v[0] = TexVertex(-side, -side, 0.0f, 0.0f, 1.0f);
	v[1] = TexVertex(-side, side, 0.0f, 0.0f, 0.0f);
	v[2] = TexVertex(side, side, 0.0f, 1.0f, 0.0f);
	v[3] = TexVertex(side, -side, 0.0f, 1.0f, 1.0f);

	this->mVB->Unlock();

	WORD *i;
	this->mIB->Lock(0, 0, (void **)&i, 0);

	i[0] = 0;
	i[1] = 1;
	i[2] = 2;
	i[3] = 0;
	i[4] = 2;
	i[5] = 3;

	this->mIB->Unlock();
}

Model2d::~Model2d()
{
	ReleaseCOM(this->mVB);
	ReleaseCOM(this->mIB);
}

void Model2d::draw(int texEnum, D3DXMATRIX m, Camera *c)
{
	d3dDevice->BeginScene();

	c->enableCamera();
	d3dDevice->SetTexture(0, game->getRes()->getTexture(texEnum));
	d3dDevice->SetTransform(D3DTS_WORLD, &m);

	d3dDevice->SetStreamSource(0, this->mVB, 0, sizeof(TexVertex));
	d3dDevice->SetIndices(this->mIB);
	d3dDevice->SetFVF(FVF_TEX);

	d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	d3dDevice->EndScene();
}

Res::Res() : mModel(NULL), mTexList(TEXTURESIZE)
{
	this->mModel = new Model2d();

	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/map_y.png"), &this->mTexList[MAP_0]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/map_g.png"), &this->mTexList[MAP_1]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/map_b1.png"), &this->mTexList[MAP_2_0]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/map_b2.png"), &this->mTexList[MAP_2_1]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/map_b3.png"), &this->mTexList[MAP_2_2]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/player.png"), &this->mTexList[PLAYER]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/button1.png"), &this->mTexList[BUTTON1]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/button2.png"), &this->mTexList[BUTTON2]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/button3.png"), &this->mTexList[BUTTON3]);
	D3DXCreateTextureFromFile(d3dDevice, TEXT("../texture/form.png"), &this->mTexList[FORM]);

	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 18;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    strcpy(fontDesc.FaceName, TEXT("Times New Roman"));

	HR(D3DXCreateFontIndirect(d3dDevice, &fontDesc, &this->mFont));

}

Res::~Res()
{
	Delete(this->mModel);
	ReleaseCOM(this->mFont);
	for(std::vector<LPDIRECT3DTEXTURE9>::iterator it=this->mTexList.begin(); it!=this->mTexList.end(); it++) 
	{
		ReleaseCOM((*it));
	}
}


