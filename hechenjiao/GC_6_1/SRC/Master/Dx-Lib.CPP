//==========================================================================================//
//																							//
//									CI-CLUB Library DX										//
//																							//
//										ver 1.0												//
//																							//
//						  Game Relation Library　For Windows                                //
//							 ( DX Higher Rank Function )									//
//																							//
//								   2008.10.24  murasho										//
//																							//
//==========================================================================================//
#include		<windows.h>
#include		<math.h>
#include		<stdio.h>
#include		<TCHAR.h>

#include		"Ci-Lib.H"


//----------------------------------------------------------------------------------//
//	★	Define
//
//----------------------------------------------------------------------------------//
#define		GsMESHSTART				8
#define		GsMESHMAX				256

#define		GsANMMESHSTART			8				
#define		GsANMMESHMAX			64

#define		GsLIGHTMAX				8


//----------------------------------------------------------------------------------//
// Mesh Management
typedef struct {
	LPD3DXMESH					lpMesh;			//メッシュポインタ
	DWORD						matCount;		//マテリアル数
	D3DMATERIAL9				*pMat;			//マテリアルポインタ
	LPDIRECT3DTEXTURE9			*pTex;			//テクスチャーポインタ
	BOOL						Flg;			//使用しているか？
} MESHHANG;


// SkinnedMesh Management
typedef struct {
	LPD3DXFRAME					pFrameRoot;				//フレームのポインタ
	LPD3DXANIMATIONCONTROLLER	pAnimController;		//アニメーションコントローラ
	BOOL						Flg;					//使用しているか？
} SKINMESHHANG;


//派生メッシュコンテナー構造体
typedef struct MESHCONTAINER :public D3DXMESHCONTAINER
{
	LPD3DXMESH					lpMesh;					//メッシュのポインタ
	LPD3DXATTRIBUTERANGE		pAttributeTable;		//所属頂点情報
    DWORD						NumAttributeGroups;		//所属頂点数

    LPD3DXBUFFER				pBoneCombinationBuf;	
    D3DXMATRIX**				ppBoneMatrixPtrs;
    D3DXMATRIX*					pBoneOffsetMatrices;	//初期位置の行列
	LPDIRECT3DTEXTURE9*			ppTextures;				//テクスチャーポインタ
	UINT						NumBoneMatricesMax;		//ボーンの数
	D3DXMATRIXA16*				pBoneMatrices;			//ボーン行列

} MESHCONTAINER;


//派生フレーム構造体
typedef struct FRAME: public D3DXFRAME
{
    D3DXMATRIX					CombinedTransformationMatrix;	//演算後の行列
	bool						DrawF;							//そのフレームを描画するかどうか

} FRAME;


//ID3DXAllocateHierarchy
class HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	HIERARCHY(){}
    STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
    STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );	
};


//カメラ情報構造体
typedef struct{
	VEC3				Pos;			//現在の位置(x,y,z)
	VEC3				OldPos;			//1つ前の位置(x,y,z)
	VEC3				LookAt;			//注視点の位置
	VEC3				OldLookAt;		//1つ前の注視点の位置
	VEC3				Dlt;			//移動量
	DIR3				Dir;			//現在の方向(Yaw,Pitch,Roll)
	VEC3				Posture;		//　カメラの姿勢
	
	float				Proj;
	float				Near;
	float				Far;
	float				Aspect;

	MAT					mRot;			//方向行列
	MAT					mView;			//ビュー行列
	MAT					mProj;			//射影行列
}GSCAM;

//　ライト構造体
typedef struct {
	bool				Flg;
	VEC3				Pos;
	short				col;
}GSLIGHT;

//　ポイントライト構造体
typedef struct {
	bool				Flg;
	VEC3				Pos;
	float				r,g,b;
}GSPOILIGHT;

// フォグ
typedef struct {
	bool				Flg;
	short				Sw;
	GsCOLOR				argb;
	float				Near;
	float				Far;
}GSFOG;

// ポイント・ライン
typedef struct {
	D3DXVECTOR3 vecCoord;
	DWORD dwColor;
} COLOR_POINT;


//----------------------------------------------------------------------------------//
//
static		MESHHANG				GsMeshHang[GsMESHMAX];
static		SKINMESHHANG			GsANMMESHHang[GsANMMESHMAX];
static		HIERARCHY				AllocateHierarchy;

GSCAM		GsCam;					// カメラ情報
GSLIGHT		GsLgt[GsLIGHTMAX];
GSPOILIGHT	GsPointLgt[GsLIGHTMAX];
GSFOG		GsFog;

GsMESH		SYS_BILB;


MAT			ViewMatrix;
MAT			ProjMatrix;

char		paths[64];				// ディレクトリ保存用


//----------------------------------------------------------------------------------//
//	★	プロトタイプ宣言
//
//----------------------------------------------------------------------------------//
void	DoNullClear(MESHHANG* pMesh);
void	DoNullClear(SKINMESHHANG* pSkinMesh);
void	DrawFrame(SKINMESHHANG* pSkinMesh, LPD3DXFRAME pD3DXFrame, char *FrameName, float Trans, ID3DXEffect* effect);
void	DrawMeshContainer(SKINMESHHANG* pSkinMesh, LPD3DXMESHCONTAINER pD3DXMeshContainer, LPD3DXFRAME pD3DXFrame, float Trans, ID3DXEffect* effect);
HRESULT GenerateSkinMesh( MESHCONTAINER *pMeshContainer );
HRESULT LoadSkinMesh(char *fName, SKINMESHHANG* pSkinMeshHang, char path[]);
HRESULT	LoadXFile(char *xfileName,MESHHANG *pMyMesh,char path[]);
void	ReleaseSkinMesh(SKINMESHHANG *pSkinMesh);
void	ReleaseXMesh(MESHHANG *pMesh);
BOOL	SetupBoneMatrixPointers(SKINMESHHANG *pSkinMeshMang, LPD3DXFRAME pFrame );
BOOL	SetupBoneMatrixPointersOnMesh(SKINMESHHANG *pSkinMeshHang, LPD3DXMESHCONTAINER pD3DXMeshContainer);
void	Update(SKINMESHHANG* pSkinMesh, D3DXMATRIX mat , ID3DXEffect* effect);
void	UpdateFrameMatrices( LPD3DXFRAME pD3DXFrame, LPD3DXMATRIX pParentMatrix );


//―――――――――――――――――――――――――――――――――――――――――//
//
//		ＤＸ　Ｘ　ＭＯＤＥＬ Function
//
//―――――――――――――――――――――――――――――――――――――――――//

/////////////////////////////////////////////////////////////////////////
//  MyMeshの描画
/////////////////////////////////////////////////////////////////////////
void DrawMyMesh(MESHHANG *pMyMesh)
{
	for(DWORD cnt=0;cnt<pMyMesh->matCount;cnt++){
		if(pMyMesh->pMat!=NULL)	lpD3DDev->SetMaterial(&pMyMesh->pMat[cnt]);
//		else			lpD3DDev->SetMaterial(NULL);
		if(pMyMesh->pTex!=NULL)	lpD3DDev->SetTexture(0,pMyMesh->pTex[cnt]);
		else			lpD3DDev->SetTexture(0,NULL);
		pMyMesh->lpMesh->DrawSubset(cnt);
	}
}

/////////////////////////////////////////////////////////////////////////
//  3DModelの描画
/////////////////////////////////////////////////////////////////////////
void DrawModel(MESHHANG *pMyMesh,
	float scaleX,	float scaleY,	float scaleZ,
	float yaw,		float pitch,	float roll,
	D3DXVECTOR3 pos)
{
	D3DXMATRIX		matScale,matRot,matPos,mat;

	D3DXMatrixScaling(&matScale,scaleX,scaleY,scaleZ);
	D3DXMatrixRotationYawPitchRoll(&matRot,yaw,pitch,roll);
	D3DXMatrixTranslation(&matPos,pos.x,pos.y,pos.z);
	D3DXMatrixMultiply(&mat,&matScale,&matRot);
	D3DXMatrixMultiply(&mat,&mat,&matPos);

	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);
	DrawMyMesh(pMyMesh);
}

//　ARGB指定つき
void DrawModel(MESHHANG *pMyMesh,
	float scaleX,	float scaleY,	float scaleZ,
	float yaw,		float pitch,	float roll,
	D3DXVECTOR3 pos,GsCOLOR argb)
{
	D3DXMATRIX		matScale,matRot,matPos,mat;

	D3DXMatrixScaling(&matScale,scaleX,scaleY,scaleZ);
	D3DXMatrixRotationYawPitchRoll(&matRot,yaw,pitch,roll);
	D3DXMatrixTranslation(&matPos,pos.x,pos.y,pos.z);
	D3DXMatrixMultiply(&mat,&matScale,&matRot);
	D3DXMatrixMultiply(&mat,&mat,&matPos);

	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);

	lpD3DDev->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );		
	lpD3DDev->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	
	lpD3DDev->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	 

	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE); //ARG1とARG2のカラー値を乗算してカラー値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE); //テクスチャのカラー値
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR); //動的のカラー値

	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); //ARG1とARG2のα値を乗算してα値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); //テクスチャのα値
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR); //動的のα値

	DrawMyMesh(pMyMesh);

	argb = 0xffffffff;
	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE); //ARG1とARG2のカラー値を乗算してカラー値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE); //テクスチャのカラー値
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR); //動的のカラー値

	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); //ARG1とARG2のα値を乗算してα値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); //テクスチャのα値
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR); //動的のα値

}


/////////////////////////////////////////////////////////////////////////
//  XFileのロード
/////////////////////////////////////////////////////////////////////////
HRESULT LoadXFile(char *xfileName,MESHHANG *pMyMesh,char path[])
{
	LPD3DXBUFFER		lpD3DBuffer;
	D3DXMATERIAL		*d3dxMat;
	DWORD				i;
	char				str[256]="";
	_TCHAR				wstr[256]=_T("");

	strcat_s(str,path);
	strcat_s(str,xfileName);
	if(FAILED(D3DXLoadMeshFromX(str,D3DXMESH_SYSTEMMEM,lpD3DDev,
			NULL,&lpD3DBuffer,NULL,&pMyMesh->matCount,&pMyMesh->lpMesh))){
		return E_FAIL;
	}
	d3dxMat=(D3DXMATERIAL*)lpD3DBuffer->GetBufferPointer();
	pMyMesh->pMat=new D3DMATERIAL9[pMyMesh->matCount];
	pMyMesh->pTex=new LPDIRECT3DTEXTURE9[pMyMesh->matCount];
	for(i=0;i<pMyMesh->matCount;i++){
		pMyMesh->pMat[i]=d3dxMat[i].MatD3D;
		if (d3dxMat[i].pTextureFilename != NULL)
		{
			lstrcpy(wstr, _T(path));
			lstrcat(wstr, d3dxMat[i].pTextureFilename);
			if (FAILED(D3DXCreateTextureFromFile(lpD3DDev, wstr, &pMyMesh->pTex[i]))) {
				pMyMesh->pTex[i] = NULL;
			}
		}
		else
		{
			pMyMesh->pTex[i] = NULL;
		}
	}
	lpD3DBuffer->Release();
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
//  メッシュデータのリリース
/////////////////////////////////////////////////////////////////////////
void ReleaseXMesh(MESHHANG *pMyMesh)
{
	if(pMyMesh->pTex!=NULL){
		for(DWORD i=0;i<pMyMesh->matCount;i++){
			if(pMyMesh->pTex[i]!=NULL){
				pMyMesh->pTex[i]->Release();
				pMyMesh->pTex[i]=NULL;
			}
		}
		delete[] pMyMesh->pTex;
		pMyMesh->pTex=NULL;
	}
	if(pMyMesh->pMat!=NULL){
		delete[] pMyMesh->pMat;
		pMyMesh->pMat=NULL;
	}
	if(pMyMesh->lpMesh!=NULL){
		pMyMesh->lpMesh->Release();
		pMyMesh->lpMesh=NULL;
	}
}

/////////////////////////////////////////////////////////////////////////
//  ディレクショナルライトの設定
//	color[0] ===> Diffuse
//	color[1] ===> Specular
//	color[2] ===> Ambient
/////////////////////////////////////////////////////////////////////////
void SetupDirectionalLight(D3DCOLORVALUE color[],D3DXVECTOR3 vec,DWORD num)
{
	D3DLIGHT9		light;

	ZeroMemory(&light,sizeof(light));
	light.Type=D3DLIGHT_DIRECTIONAL;
	light.Diffuse=	color[0];
	light.Specular=	color[1];
	light.Ambient=	color[2];
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vec);
	lpD3DDev->SetLight(num,&light);
	lpD3DDev->LightEnable(num,TRUE);
}

/////////////////////////////////////////////////////////////////////////
//	ポイントライトの設定
//	D3DCOLORVALUE		color[3];
//		color[0]	===> Diffuse
//		color[1]	===> Specular
//		color[2]	===> Ambient
//	D3DXVECTOR3			vec;
//		vec			===> Position
//	float				val[4];
//		val[0]		===> Range
//		val[1]		===> Attenuation0
//		val[2]		===> Attenuation1
//		val[3]		===> Attenuation2
/////////////////////////////////////////////////////////////////////////
void SetupPointLight(D3DCOLORVALUE color[],D3DXVECTOR3 vec,float val[],DWORD num)
{
	D3DLIGHT9		light;

	ZeroMemory(&light,sizeof(light));
	light.Type=D3DLIGHT_POINT;
	light.Diffuse		=color[0];
	light.Specular		=color[1];
	light.Ambient		=color[2];
	light.Position		=vec;
	light.Range			=val[0];
	light.Attenuation0	=val[1];
	light.Attenuation1	=val[2];
	light.Attenuation2	=val[3];
	lpD3DDev->SetLight(num,&light);
	lpD3DDev->LightEnable(num,TRUE);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	3D描画環境の設定																//																				//
//―――――――――――――――――――――――――――――――――――――――――//
void	Setup3DEnv(void)
{
	//Zバッファの設定	
	lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);	//Zバッファを使うか
	lpD3DDev->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);		//使うとき書き込むかどうか

	//カルモード(背面の可視設定)
	lpD3DDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);	//左手(背面を左回りにカリングする)
//	lpD3DDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);	//両面

	//ライトの設定(ライトを使うためライティングモードへ移行)
	lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//FOG(霧)の設定
//D3DFOG_NONE
//D3DFOG_EXP = 1,
//D3DFOG_EXP2 = 2,
//D3DFOG_LINEAR = 3,
void	SetupFog(void)
{
	D3DRENDERSTATETYPE	f_stt;

	if (GsFog.Flg) {
		if (GsFog.Sw==OFF)	f_stt	=	D3DRS_FOGTABLEMODE;
		else				f_stt	=	D3DRS_FOGVERTEXMODE;

		lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE);					//FOGをかけるか
		lpD3DDev->SetRenderState(D3DRS_FOGCOLOR, GsFog.argb);				//FOGの色
		lpD3DDev->SetRenderState(f_stt, D3DFOG_LINEAR);						//FOGの種類
		lpD3DDev->SetRenderState(D3DRS_FOGSTART,*(DWORD *)(&GsFog.Near));	//FOG開始点
		lpD3DDev->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&GsFog.Far));		//FOG終了地点
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//	ライトの設定
//		６方向のライトをカメラの方向から当てる																				//
//―――――――――――――――――――――――――――――――――――――――――//
void SetupLightX(void)
{
	short	i;

	D3DCOLORVALUE		color[6][3]={
		//ディフューズ, スペキュラー, アンビエント
		{ {0.4f, 0.35f, 0.35f, 0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},	//正面
		{ {0.5f, 0.45f, 0.45f, 0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},	//正面上 
		{ {0.2f, 0.15f, 0.15f ,0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},	//左右
		{ {0.1f, 0.05f, 0.05f, 0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},  //裏
	};

	//各ライトのカメラからの方向を設定
	VEC3 vDir[6] = { 
		VEC3( 0.0f,  0.0f,  1.0f),	//正面
		VEC3( 0.0f, -1.0f,  1.0f),	//正面斜め上
		VEC3(-1.0f, -1.0f,  0.0f),  //前方右斜め上
		VEC3( 1.0f, -1.0f,  0.0f),	//前方左斜め上
		VEC3( 0.0f, -1.0f, -1.0f),  //後方斜め上
		VEC3( 0.0f,  1.0f,  0.0f),  //真下
	};

	//カメラの方向へ回転させる
	for(i=0; i<6; i++){
		VTFC(&vDir[i], &vDir[i], &GsCam.mRot);
	}

	//ライトを設定
	SetupDirectionalLight(color[0], vDir[0], 1);
	SetupDirectionalLight(color[1], vDir[1], 2);
	SetupDirectionalLight(color[2], vDir[2], 3);
	SetupDirectionalLight(color[2], vDir[3], 4);
	SetupDirectionalLight(color[3], vDir[4], 5);
	SetupDirectionalLight(color[3], vDir[5], 6);

	//環境光
	lpD3DDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}


void	SetupPointLight(void)
{
	D3DCOLORVALUE		Tcol[3]={
		{1.0f,1.0f,1.0f,0.0f},
		{1.0f,1.0f,1.0f,0.0f},
		{1.0f,1.0f,1.0f,0.0f}
	};

//	static float	Tval[4] = {10.0f,0.5f,0.1f,0.5f};
	static float	Tval[4] = {20.0f,0.5f,0.1f,0.5f};

	for (int i=0;i<GsLIGHTMAX;i++) {

		lpD3DDev->LightEnable(i+8,FALSE);

		if (GsPointLgt[i].Flg) {
			Tcol[0].r	= GsPointLgt[i].r;
			Tcol[0].g	= GsPointLgt[i].g;
			Tcol[0].b	= GsPointLgt[i].b;
			SetupPointLight(Tcol,GsPointLgt[i].Pos,&Tval[0],i+8);
			GsPointLgt[i].Flg = OFF;
		}
	}
}

void SetupLight(void)
{
	//D3DCOLORVALUE		color[4][3]={
	//	{{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},
	//	{{1.0f,1.0f,1.0f,1.0f},{0.5f,0.5f,0.5f,0.5f},{0.5f,0.5f,0.5f,0.5f}},
	//	{{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},
	//	{{0.5f,0.5f,0.5f,1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}}
	//};
	D3DCOLORVALUE		color[4][3]={
		//ディフューズ, スペキュラー, アンビエント
		{ {0.4f, 0.35f, 0.35f, 0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},	//正面
		{ {0.5f, 0.45f, 0.45f, 0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},	//正面上 
		{ {0.2f, 0.15f, 0.15f ,0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},	//左右
		{ {0.1f, 0.05f, 0.05f, 0.0f}, {1.0f,1.0f,1.0f, 0.0f}, {1.0f,1.0f,1.0f,0.0f}},   //裏
	};


	for (int i=0;i<GsLIGHTMAX;i++) {

		lpD3DDev->LightEnable(i,FALSE);

		if (GsLgt[i].Flg) {
			SetupDirectionalLight(color[GsLgt[i].col],GsLgt[i].Pos,i);

			GsLgt[i].Flg = OFF;
		}
	}

//test
	SetupPointLight( );

	//環境光
	lpD3DDev->SetRenderState(D3DRS_AMBIENT,0x00202020);
}



//―――――――――――――――――――――――――――――――――――――――――//
//　フォグ　ON/OFF　＆ステータスセット
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_FogSet(bool	sw,float s_near,float e_far,GsCOLOR argb)
{
	GsFog.Flg		= sw;
	GsFog.Sw 		= OFF;
	GsFog.Near		= s_near;
	GsFog.Far		= e_far;
	GsFog.argb 		= argb;
}

//―――――――――――――――――――――――――――――――――――――――――//
//　ライトの位置＆点灯セット
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_LightPosSet(short id,bool sw,float x,float y, float z)
{
	GsLgt[id].Flg	= sw;
	GsLgt[id].Pos 	= VEC3(-x,-y,-z);
	GsLgt[id].col 	= 1;
}

//―――――――――――――――――――――――――――――――――――――――――//
//　ポイントライトの位置＆点灯セット
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_PointLightSet(short id,float x,float y, float z,GsCOLOR argb)
{
	GsPointLgt[id].Flg	= ON;
	GsPointLgt[id].Pos 	= VEC3(x,y,z);
	GsPointLgt[id].r 	= F(GET_R(argb))/1;
	GsPointLgt[id].g 	= F(GET_G(argb))/1;
	GsPointLgt[id].b 	= F(GET_B(argb))/1;
}


//―――――――――――――――――――――――――――――――――――――――――//
//	カメラの設定
//		ビュートランスフォーム、プロジェクショントランスフォームを行う																				//
//―――――――――――――――――――――――――――――――――――――――――//
void	SetupCamera(void)
{
		
	//カメラの設定
	D3DXMatrixLookAtLH(&GsCam.mView,
		&GsCam.Pos,								//カメラ位置
		&GsCam.LookAt,							//注視点
		&GsCam.Posture);						//カメラの上向き

	//ビュー行列の登録
	lpD3DDev->SetTransform(D3DTS_VIEW, &GsCam.mView);
//	Gs_SetViewMatrix(&GsCam.mView);

	//画角、アスペクト比、深度の設定 = クリッピング（Near & Far)
//	D3DXMatrixPerspectiveFovLH(&GsCam.mProj,D3DXToRadian(90.0f), 640.0f/480.0f, 0.1f, 1000.0f);
	D3DXMatrixPerspectiveFovLH(&GsCam.mProj,D3DXToRadian(GsCam.Proj), GsCam.Aspect, GsCam.Near,GsCam.Far);

	lpD3DDev->SetTransform(D3DTS_PROJECTION,&GsCam.mProj);
//	Gs_SetProjMatrix(&GsCam.mProj);
}

//―――――――――――――――――――――――――――――――――――――――――//
//　カメラの方向セット
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_LightDirSet(float x,float y, float z)
{
	GsCam.Dir.Yaw		=y;
	GsCam.Dir.Pit		=x;
	GsCam.Dir.Rol		=z;
	MRYPR(&GsCam.mRot, F(RAD(GsCam.Dir.Yaw)),  F(RAD(GsCam.Dir.Pit)),  F(RAD(GsCam.Dir.Rol)));
}

//―――――――――――――――――――――――――――――――――――――――――//
//　カメラプロジェクションデータセット
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_CamPrjSet(float prj,float asp, float nr,float fr)
{
	GsCam.Proj 		= prj;
	GsCam.Aspect	= asp;
	GsCam.Near  	= nr;
	GsCam.Far		= fr;
}

//―――――――――――――――――――――――――――――――――――――――――//
//　カメラの位置データセット
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_CamPosSet(float x,float y, float z)
{
	GsCam.Pos		= VEC3(x,y,z);
}

//―――――――――――――――――――――――――――――――――――――――――//
//　カメラの被写体位置データセット
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_CamLookAt(float x,float y, float z)
{
	GsCam.LookAt 	= VEC3(x,y,z);
}

	
//―――――――――――――――――――――――――――――――――――――――――//
//　カメラの姿勢･構え
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_CamPosture(float x,float y, float z)
{
	GsCam.Posture 	= VEC3(x,y,z);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	Ｘファイル形式データ　ロード
//―――――――――――――――――――――――――――――――――――――――――//
GsMESH		Gs_LoadXMesh(char *fName,char *path)
{

	int		i,ret=NG;

	for (i=0; i<GsMESHMAX; i++){
		if (!GsMeshHang[i].Flg) {
			ret	= i;
			GsMeshHang[i].Flg = ON;
			if (FAILED(LoadXFile(fName,&GsMeshHang[i],path))) {
				System_Message(GsHwnd,"ロードに失敗しました : %s",fName);
				GsMeshHang[i].Flg = OFF;
				ret = NG;
			}
			break;

		}
	}
	return	(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	メッシュ　リリース
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_ReleaseMesh(int i)
{
	if (GsMeshHang[i].Flg) {
		GsMeshHang[i].Flg = OFF;
		ReleaseXMesh(&GsMeshHang[i]);
	}
	return	(OK);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	メッシュ　描画
//―――――――――――――――――――――――――――――――――――――――――//
// 簡易
void	Gs_DrawDirectModel(float x,float y,float z,GsMESH id)
{
	D3DXVECTOR3		pos;

	pos	= D3DXVECTOR3(x,y,z);
	DrawModel(&GsMeshHang[id],1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,pos);
}

// スケール（x､y､z)
void	Gs_DrawScaleModel(float x,float y,float z,GsMESH id,float scx,float scy,float scz)
{
	D3DXVECTOR3		pos;

	pos	= D3DXVECTOR3(x,y,z);
	DrawModel(&GsMeshHang[id],scx,scy,scz,0.0f,0.0f,0.0f,pos);
}

// スケール（x､y､z)・角度(y､p､r)
void	Gs_DrawModel(float x,float y,float z,GsMESH id,float scx,float scy,float scz,float yaw,float pitch,float roll)
{
	D3DXVECTOR3		pos;

	pos	= D3DXVECTOR3(x,y,z);
	DrawModel(&GsMeshHang[id],scx,scy,scz,F(RAD(yaw)),F(RAD(pitch)),F(RAD(roll)),pos);
}

// VEC3 DIR3版
void	Gs_DrawModel(VEC3 pos,GsMESH id,float scx,float scy,float scz,DIR3 rot)
{
	DrawModel(&GsMeshHang[id],scx,scy,scz,F(RAD(rot.Yaw)),F(RAD(rot.Pit)),F(RAD(rot.Rol)),pos);
}

// モデルα
void	Gs_DrawModel(VEC3 pos,GsMESH id,float scx,float scy,float scz,DIR3 rot,GsCOLOR argb)
{
	DrawModel(&GsMeshHang[id],scx,scy,scz,F(RAD(rot.Yaw)),F(RAD(rot.Pit)),F(RAD(rot.Rol)),pos,argb);
}

void	Gs_DrawModel(float x,float y,float z,GsMESH id,float scx,float scy,float scz,float yaw,float pit,float rol,GsCOLOR argb)
{
	D3DXVECTOR3		pos;

	pos	= D3DXVECTOR3(x,y,z);
	DrawModel(&GsMeshHang[id],scx,scy,scz,F(RAD(yaw)),F(RAD(pit)),F(RAD(rol)),pos,argb);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	メッシュ親子　描画　
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_DrawModelDx(MODEL_PARAM mod,GsMESH id)
{

	D3DXMATRIX		matRot,matPos,mat;

	D3DXMatrixRotationYawPitchRoll(&matRot,
		D3DXToRadian(mod.Rot.Yaw),
		D3DXToRadian(mod.Rot.Pit),
		D3DXToRadian(mod.Rot.Rol) );

	D3DXMatrixTranslation(&matPos,
		mod.Pos.x,
		mod.Pos.y,
		mod.Pos.z);

	D3DXMatrixMultiply(&mat,&matRot,&mod.Mat);
	D3DXMatrixMultiply(&mat,&mat,&matPos);
	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);

	DrawMyMesh(&GsMeshHang[id]);
}


/////////////////////////////////////////////////////////////////////////
//  AXIS 3DModelの描画
/////////////////////////////////////////////////////////////////////////
void DrawModelAxis(MESHHANG		*pMyMesh,
	float scaleX,	float scaleY,	float scaleZ,
	float yaw,		float pitch,	float roll,
	D3DXVECTOR3 pos,float Cnt,float xa,float ya,float za)
{
	D3DXMATRIX		modMat;
	D3DXMATRIX		matRotAxis,matRotHead,matPos,mat,matScale;
	D3DXVECTOR3		axis(xa,ya,za);

	D3DXMatrixRotationYawPitchRoll(&modMat,
		D3DXToRadian(yaw),
		D3DXToRadian(pitch),
		D3DXToRadian(roll));

	D3DXMatrixTranslation(&matPos,pos.x,pos.y,pos.z);
	D3DXMatrixScaling(&matScale,scaleX,scaleY,scaleZ);

	D3DXVec3TransformCoord(&axis,&axis,&modMat);
	D3DXMatrixRotationAxis(&matRotAxis,&axis,D3DXToRadian(Cnt));
	D3DXMatrixRotationX(&matRotHead,D3DXToRadian(0));

	D3DXMatrixMultiply(&mat,&matRotHead,&modMat);
	D3DXMatrixMultiply(&mat,&mat,&matRotAxis);
	D3DXMatrixMultiply(&mat,&mat,&matPos);
	D3DXMatrixMultiply(&mat,&mat,&matScale);



	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);
	DrawMyMesh(pMyMesh);

}

void	Gs_DrawModelAxis(float x,float y,float z,GsMESH id,float scx,float scy,float scz,float yaw,float pitch,float roll,float cnt,float ax,float ay,float az)
{
	D3DXVECTOR3		pos;
	pos	= D3DXVECTOR3(x,y,z);

	DrawModelAxis(&GsMeshHang[id],scx,scy,scz,yaw,pitch,roll,pos,cnt,ax,ay,az);
}



//―――――――――――――――――――――――――――――――――――――――――//
//	SPRITEテクスチャー　3D
//―――――――――――――――――――――――――――――――――――――――――//
void DrawTexture3D(TEX9 pTex,D3DXVECTOR3 *pPos,RECT *pRect,float degree,POINT	*pCenter,float scaleX,float scaleY,GsCOLOR argb)
{
	D3DXMATRIX		mat,matID,matPos,matRot,matScale;
	D3DXVECTOR3		center;

	//単位行列の作成
	D3DXMatrixIdentity(&matID);

	//位置行列の作成
	D3DXMatrixTranslation(&matPos,pPos->x,pPos->y,pPos->z);


	//拡大縮小行列の作成
	D3DXMatrixScaling(&matScale,scaleX,scaleY,1.0f);
//	D3DXMatrixScaling(&matScale,0.12f,0.12f,1.0f);
//	D3DXMatrixScaling(&matScale,1.0f,1.0f,1.0f);


	//行列の合成
	D3DXMatrixMultiply(&mat,&matID,&matScale);
	D3DXMatrixMultiply(&mat,&mat,&matPos);
	//D3DXMatrixMultiply(&mat,&matID,&matPos);
	//D3DXMatrixMultiply(&mat,&mat,&matScale);

	lpSprite->SetWorldViewLH(&mat,&GsCam.mView);

	//回転行列の作成
	D3DXMatrixRotationZ(&matRot,D3DXToRadian(180));
	D3DXMatrixMultiply(&mat,&mat,&matRot);

	//スプライトエンジンに行列を登録
	lpSprite->SetTransform(&mat);

	//センターの作成
	center=D3DXVECTOR3((float)pCenter->x,(float)pCenter->y,0.0f);


	lpSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_BILLBOARD);//
//	lpSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);//D3DXSPRITE_BILLBOARD

	//描画
	lpSprite->Draw(pTex,pRect,&center,NULL,argb);

	//ワールドのリセット
	D3DXMatrixIdentity(&mat);
	lpSprite->SetTransform(&mat);

	lpSprite->End( );
}

void	Gs_DrawLayer3D(float px,float py,float pz,GsTEXTURE no,int x1,int y1,int w,int h,bool rev,GsCOLOR argb,short gen,short rot,float scx,float scy)
{
	D3DXVECTOR3	pos(px,py,pz);
	RECT		rect	= {x1,y1,x1+w,y1+h};
	POINT		rotp	= {0,0};

	if (gen==1)	{	rotp.x = int(w/2);	rotp.y = int(h/2);	}	// center
	if (gen==2)	{	rotp.x = int(w/2);	rotp.y = int(0);	}	// Up center
	if (gen==3)	{	rotp.x = int(w/2);	rotp.y = int(h);	}	// Down center
	if (gen==4)	{	rotp.x = int(0);	rotp.y = int(h/2);	}	// Left center
	if (gen==5)	{	rotp.x = int(w);	rotp.y = int(h/2);	}	// Right center

	DrawTexture3D(GsBmpHang[no].Bmp ,&pos,&rect,F(-rot),&rotp,scx,scy,argb);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	３Ｄ空間内　テクスチャ描画　（ビルボード）
//―――――――――――――――――――――――――――――――――――――――――//
#define		AW		256.0f
#define		AH		256.0f
#define		BSW		64.0f
#define		BSH		64.0f

float		Bil_Aw	= AW;
float		Bil_Ah	= AH;
float		Bil_Bw	= BSW;
float		Bil_Bh	= BSH;

void	Gs_BillboardTexSize(int w,int h)
{
	Bil_Aw		= F(w);
	Bil_Ah		= F(h);
}


void	DrawBillboard(MESHHANG *pMyMesh,TEX9 lpTex,D3DXVECTOR3 pos,
		float scaleX,	float scaleY,	float scaleZ,
		float yaw,		float pitch,	float roll,
		short sx,short sy,short w,short h,GsCOLOR argb)
{
	D3DXMATRIX		matScale,matRot,matPos,mat;
	float			scx,scy,scz;

	scx	= scaleX * (F(w)/F(BSW));
	scz	= scaleZ * (F(h)/F(BSH));
	scy	= scaleY;

	D3DXMatrixScaling(&matScale,scx,scy,scz);
	D3DXMatrixRotationYawPitchRoll(&matRot,yaw,pitch,roll);
	D3DXMatrixTranslation(&matPos,pos.x,pos.y,pos.z);
	D3DXMatrixMultiply(&mat,&matScale,&matRot);
	D3DXMatrixMultiply(&mat,&mat,&matPos);


	//lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	//lpD3DDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	//lpD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	lpD3DDev->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);

	lpD3DDev->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );		
	lpD3DDev->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	
	lpD3DDev->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	 
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);	 
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE); //ARG1とARG2のカラー値を乗算してカラー値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE); //テクスチャのカラー値
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR); //動的のカラー値

	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); //ARG1とARG2のα値を乗算してα値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); //テクスチャのα値
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR); //動的のα値


	//lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); //α値を乗算します。
	//lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); //テクスチャ0のα値
	//lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE); //頂点のα値

	//lpD3DDev->SetTextureStageState(1,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); //テクスチャ1のα値
	//lpD3DDev->SetTextureStageState(1,D3DTSS_ALPHAARG2,D3DTA_CURRENT); //第一ステージステートの出力の値のα値
	//lpD3DDev->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_MODULATE); //乗算


	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);

	if(pMyMesh->pMat!=NULL)	lpD3DDev->SetMaterial(&pMyMesh->pMat[0]);

	D3DXMatrixIdentity(&mat);
	mat._31 = F(sx)/Bil_Aw;			// u
	mat._32 = F(sy)/Bil_Ah;			// v
	mat._11 = F(w)/Bil_Aw;			// w
	mat._22 = F(h)/Bil_Ah;			// h
	lpD3DDev->SetTransform(D3DTS_TEXTURE0,&mat);


	lpD3DDev->SetTexture(0,lpTex);
	pMyMesh->lpMesh->DrawSubset(0);

	lpD3DDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	lpD3DDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);

	lpD3DDev->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);	 
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	D3DXMatrixIdentity(&mat);
	lpD3DDev->SetTransform(D3DTS_TEXTURE0,&mat);
	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);

}

void	Gs_DrawBillboard(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy)
{
	D3DXVECTOR3		pos;
	pos	= D3DXVECTOR3(x,y,z);

	float	rotx = DIR_AtoB(y,z,GsCam.Pos.y,GsCam.Pos.z);

	DrawBillboard(&GsMeshHang[SYS_BILB],GsBmpHang[Tid].Bmp,pos,
			1.0f*scx,1.0f,1.0f*scy,
			D3DXToRadian(180),D3DXToRadian(-90),D3DXToRadian(0),
			sx,sy,w,h,BETA_COLOR);
}

void	Gs_DrawBillboardYPR(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy,float yaw,float pit,float rol)
{
	D3DXVECTOR3		pos;
	pos	= D3DXVECTOR3(x,y,z);

	float	rotx = DIR_AtoB(y,z,GsCam.Pos.y,GsCam.Pos.z);

	DrawBillboard(&GsMeshHang[SYS_BILB],GsBmpHang[Tid].Bmp,pos,
			1.0f*scx,1.0f,1.0f*scy,
			D3DXToRadian(180+yaw),D3DXToRadian(-90+pit),D3DXToRadian(0+rol),
			sx,sy,w,h,BETA_COLOR);
}

void	Gs_DrawMapboard(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy,float yaw,float pit,float rol)
{
	D3DXVECTOR3		pos;
	pos	= D3DXVECTOR3(x,y,z);

	float	rotx = DIR_AtoB(y,z,GsCam.Pos.y,GsCam.Pos.z);

	DrawBillboard(&GsMeshHang[SYS_BILB],GsBmpHang[Tid].Bmp,pos,
			1.0f*scx,1.0f,1.0f*scy,
			D3DXToRadian(0+yaw),D3DXToRadian(0+pit),D3DXToRadian(0+rol),
			sx,sy,w,h,BETA_COLOR);
}

void	Gs_DrawMapboard(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy,float yaw,float pit,float rol,GsCOLOR argb)
{
	D3DXVECTOR3		pos;
	pos	= D3DXVECTOR3(x,y,z);

	DrawBillboard(&GsMeshHang[SYS_BILB],GsBmpHang[Tid].Bmp,pos,
			1.0f*scx,1.0f,1.0f*scy,
			D3DXToRadian(0+yaw),D3DXToRadian(0+pit),D3DXToRadian(0+rol),
			sx,sy,w,h,argb);
}


//----------------------------------------------------------------------
void	DrawMapboard(MESHHANG *pMyMesh,TEX9 lpTex,D3DXVECTOR3 pos,
		float scaleX,	float scaleY,	float scaleZ,
		float yaw,		float pitch,	float roll,
		short sx,short sy,short w,short h,VEC3 zik,float cnt,GsCOLOR argb)
{
	D3DXMATRIX		matScale,matRot,matPos,mat;
	float			scx,scy,scz;

	scx	= scaleX * (F(w)/F(BSW));
	scz	= scaleZ * (F(h)/F(BSH));
	scy	= scaleY;

	D3DXMatrixScaling(&matScale,scx,scy,scz);
	D3DXMatrixTranslation(&matPos,pos.x,pos.y,pos.z);

	// Rotation
	D3DXMATRIX		matRotAxis,matRotHead;
	D3DXVECTOR3		axis(zik.x,zik.y,zik.z);

	D3DXMatrixRotationYawPitchRoll(&matRot,yaw,pitch,roll);
	D3DXVec3TransformCoord(&axis,&axis,&matRot);
	D3DXMatrixRotationAxis(&matRotAxis,&axis,xRAD(cnt));
	D3DXMatrixMultiply(&matRot,&matRot,&matRotAxis);

	// 合成
	D3DXMatrixMultiply(&mat,&matScale,&matRot);
	D3DXMatrixMultiply(&mat,&mat,&matPos);

	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);


	lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
	lpD3DDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	lpD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


	lpD3DDev->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);

	lpD3DDev->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );		
	lpD3DDev->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );	
	lpD3DDev->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	 
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);	 
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE); //ARG1とARG2のカラー値を乗算してカラー値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE); //テクスチャのカラー値
	lpD3DDev->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR); //動的のカラー値

	lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR,argb);
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); //ARG1とARG2のα値を乗算してα値を取得
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); //テクスチャのα値
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR); //動的のα値

	if(pMyMesh->pMat!=NULL)	lpD3DDev->SetMaterial(&pMyMesh->pMat[0]);

	D3DXMatrixIdentity(&mat);
	mat._31 = F(sx)/Bil_Aw;			// u
	mat._32 = F(sy)/Bil_Ah;			// v
	mat._11 = F(w)/Bil_Aw;			// w
	mat._22 = F(h)/Bil_Ah;			// h
	lpD3DDev->SetTransform(D3DTS_TEXTURE0,&mat);

	lpD3DDev->SetTexture(0,lpTex);
	pMyMesh->lpMesh->DrawSubset(0);

	// Base World ReSet
	lpD3DDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	lpD3DDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);

	lpD3DDev->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);	 
	lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	D3DXMatrixIdentity(&mat);
	lpD3DDev->SetTransform(D3DTS_TEXTURE0,&mat);
	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);
}

void	Gs_DrawMapboardAxis(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy,float yaw,float pit,float rol,VEC3 Ixyz,float cnt)
{
	D3DXVECTOR3		pos;
	pos	= D3DXVECTOR3(x,y,z);

	DrawMapboard(&GsMeshHang[SYS_BILB],GsBmpHang[Tid].Bmp,pos,
			1.0f*scx,1.0f,1.0f*scy,
			xRAD(yaw),xRAD(pit),xRAD(rol),
			sx,sy,w,h,Ixyz,cnt,BETA_COLOR);
}

void	Gs_DrawMapboardAxis(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy,float yaw,float pit,float rol,VEC3 Ixyz,float cnt,GsCOLOR argb)
{
	D3DXVECTOR3		pos;
	pos	= D3DXVECTOR3(x,y,z);

	DrawMapboard(&GsMeshHang[SYS_BILB],GsBmpHang[Tid].Bmp,pos,
			1.0f*scx,1.0f,1.0f*scy,
			xRAD(yaw),xRAD(pit),xRAD(rol),
			sx,sy,w,h,Ixyz,cnt,argb);
}


//
void	Gs_DrawLine3D(float sx,float sy,float sz,float ex,float ey,float ez)
{

	D3DXMATRIX		matScale,matRot,mat;

	D3DXMatrixIdentity(&mat);
	D3DXMatrixScaling(&matScale,1,1,1);
	D3DXMatrixRotationYawPitchRoll(&matRot,0,0,0);
	D3DXMatrixMultiply(&mat,&matScale,&matRot);
	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);

	//色付きのライン　色付き頂点を使用
	COLOR_POINT ColorPoint[2]=
	{ 
		D3DXVECTOR3(sx,sy,sz),D3DCOLOR_ARGB(255,255,255,255),
		D3DXVECTOR3(ex,ey,ez),D3DCOLOR_ARGB(255,255,255,255)
	};	

	lpD3DDev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE );
	lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST,1, ColorPoint, sizeof(COLOR_POINT));

	D3DXMatrixIdentity(&mat);
	lpD3DDev->SetTransform(D3DTS_WORLD,&mat);

}



//―――――――――――――――――――――――――――――――――――――――――//
// ３Ｄ　初期化
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_Init3D(void)
{
	int i;

	// Mesh
	for (i=0; i<GsMESHMAX; i++){
		GsMeshHang[i].Flg	= OFF;
		DoNullClear(&GsMeshHang[i]);
	}

	// Skin
	for (i=0;i<GsANMMESHMAX; i++){
		DoNullClear(&GsANMMESHHang[i]);
	}

	// LIGHT
	for (i=0;i<GsLIGHTMAX;i++) {
		GsLgt[i].Flg		= OFF;
		GsPointLgt[i].Flg	= OFF;
	}
	
	// CAMERA
	Gs_CamPosture(0.0f,1.0f,0.0f);

	// FOG
	GsFog.Flg	= OFF;
	GsFog.Sw	= 0;

	// Billboard
	SYS_BILB	= Gs_LoadXMesh("SYS_B.x",	"DATA/X/");

}


//―――――――――――――――――――――――――――――――――――――――――//
// ３Ｄ　終了処理
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_Exit3D(void)
{
	int		i;

	for (i=0; i<GsMESHMAX; i++){
		Gs_ReleaseMesh(i);
	}

	for (i=0; i<GsANMMESHMAX; i++){
		Gs_ReleaseANMMESH(i);
	}
}


//―――――――――――――――――――――――――――――――――――――――――//
// ３Ｄ　環境設定
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_3DSetUP(void)
{
	Setup3DEnv	( );
	SetupFog	( );
//	SetupLight	( );
//	SetupCamera	( );
}

//―――――――――――――――――――――――――――――――――――――――――//
//  MeshのNULLクリア		
//―――――――――――――――――――――――――――――――――――――――――//
void	DoNullClear(MESHHANG* pMesh)
{
	pMesh->lpMesh				=NULL;
	pMesh->matCount				=0;
	pMesh->pMat					=NULL;
	pMesh->pTex					=NULL;
	pMesh->Flg					=OFF;
	
}


//=================================================================================//
//
//			SkinMesh関連
//
//=================================================================================//


//―――――――――――――――――――――――――――――――――――――――――//
//	ロード																			//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
GsANMMESH	Gs_LoadANMMESH(char* fName, char path[])
{
	int		i,ret;

	for (i=GsANMMESHSTART; i<GsANMMESHMAX; i++){
		if(!GsANMMESHHang[i].Flg) {
			ret	= i;
			GsANMMESHHang[i].Flg = ON;

			if(FAILED(LoadSkinMesh(fName, &GsANMMESHHang[i], path))){
				System_Message(GsHwnd,"ロードに失敗しました : %s",fName);
				GsMeshHang[i].Flg = OFF;
				ret = NG;
			}
			break;
		}
	}
	return	(ret);
}

HRESULT LoadSkinMesh(char *fName, SKINMESHHANG* pSkinMeshHang, char path[])
{
	LPD3DXANIMATIONCONTROLLER	pAnimControllerTmp;
	UINT						i;
	char						str[256]="";
	UINT						TracksNum;
	UINT						AnimSetsNum;
	
	strcat_s(str,path);
	strcat_s(str,fName);

//	strcpy_s(paths,32,path);
	strcpy(paths,path);

	if( FAILED( D3DXLoadMeshHierarchyFromX(
		str, 
		D3DXMESH_MANAGED, 
		lpD3DDev, 
		&AllocateHierarchy, 
		NULL, 
		&pSkinMeshHang->pFrameRoot,
		&pAnimControllerTmp)))
    {
		System_Message(GsHwnd,"Xファイルのロードに失敗しました : %s",fName);
        return E_FAIL;   
    }

	//トラック数をアニメーションセット数に増やす
	TracksNum   = pAnimControllerTmp->GetMaxNumTracks();
	AnimSetsNum = pAnimControllerTmp->GetMaxNumAnimationSets();
	if( TracksNum < AnimSetsNum ) TracksNum = AnimSetsNum;

	if( FAILED( pAnimControllerTmp->CloneAnimationController(
					pAnimControllerTmp->GetMaxNumAnimationOutputs(),
					pAnimControllerTmp->GetMaxNumAnimationSets(),
					TracksNum,
					pAnimControllerTmp->GetMaxNumEvents(),
					&pSkinMeshHang->pAnimController))) 
					return false;

	SAFE_RELEASE(pAnimControllerTmp);

	if( AnimSetsNum > 1 ){
		// トラックに全てのアニメーションセットを読み込む
		for(i = 1; i < AnimSetsNum; i++ ){
			LPD3DXANIMATIONSET pAnimSet = NULL;
			if( FAILED( pSkinMeshHang->pAnimController->GetAnimationSet(i, &pAnimSet))) return false;
			if( FAILED( pSkinMeshHang->pAnimController->SetTrackAnimationSet(i, pAnimSet))) return false;
			SAFE_RELEASE( pAnimSet );

			// 全てのトラックにデフォルトのD3DXTRACK_DESCを設定しておく
			D3DXTRACK_DESC Desc = { D3DXPRIORITY_LOW, 1, 1, 0, FALSE };
			if( FAILED(pSkinMeshHang->pAnimController->SetTrackDesc( i, &Desc))) return false;
		}
	}
	
	if( FAILED((SetupBoneMatrixPointers(pSkinMeshHang, pSkinMeshHang->pFrameRoot)))) return false;

	return S_OK;
}


//―――――――――――――――――――――――――――――――――――――――――//
//	フレームの作成																	//
//		Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる			//																				//
//―――――――――――――――――――――――――――――――――――――――――//

HRESULT HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
    HRESULT hr = S_OK;
    FRAME *pFrame;
    *ppNewFrame = NULL;

    pFrame = new FRAME;
    if ( pFrame == NULL) return E_OUTOFMEMORY;   
 
	pFrame->Name=new TCHAR[lstrlen(Name) + 1];	
    if ( !pFrame->Name) return E_FAIL;
	strcpy( pFrame->Name,Name);

	pFrame->DrawF=ON;
    D3DXMatrixIdentity( &pFrame->TransformationMatrix);
    D3DXMatrixIdentity( &pFrame->CombinedTransformationMatrix);
    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;
    *ppNewFrame = pFrame;
    pFrame = NULL;
    delete pFrame;

    return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//	メッシュコンテナーの作成														//
//																				    //
//―――――――――――――――――――――――――――――――――――――――――//

HRESULT HIERARCHY::CreateMeshContainer( THIS_
					LPCSTR Name,
					CONST D3DXMESHDATA* pMeshData,
                    CONST D3DXMATERIAL* pMaterials, 
					CONST D3DXEFFECTINSTANCE* pEffectInstances,
					DWORD NumMaterials, 
					CONST DWORD *pAdjacency,
					LPD3DXSKININFO pSkinInfo, 
                    LPD3DXMESHCONTAINER *ppMeshContainer) 
{
	HRESULT					hr = S_OK;
	LPD3DXMESH				pMesh = NULL;
	*ppMeshContainer = NULL;

	// patch meshes を扱う事はできない
	if( pMeshData->Type != D3DXMESHTYPE_MESH ) return E_FAIL;
	pMesh=pMeshData->pMesh;

	// FVF で記述されたメッシュ以外は読めない
	if( pMesh->GetFVF() == 0 ) return E_FAIL;
	UINT NumFaces = pMesh->GetNumFaces();

	//メッシュ作成
	DWORD					iFacesAmount;
    DWORD					iMaterial; 
	LPDIRECT3DDEVICE9		pDevice = NULL;
	MESHCONTAINER 			*pMeshContainer = NULL;
    DWORD					dwBoneAmt=0;
   
    pMeshContainer = new MESHCONTAINER;
    if (pMeshContainer == NULL) return E_OUTOFMEMORY;
    ZeroMemory(pMeshContainer, sizeof(MESHCONTAINER));
    
	pMeshContainer->Name=new TCHAR[lstrlen(Name) + 1];	
    if (!pMeshContainer->Name) return E_FAIL;
	strcpy(pMeshContainer->Name,Name);

	if(FAILED(pMesh->GetDevice(&pDevice))){
		hr = E_FAIL;
		SAFE_RELEASE( pDevice );
		if( pMeshContainer ) DestroyMeshContainer( pMeshContainer );
		return hr;
	}
 
    iFacesAmount = pMesh->GetNumFaces();

    // 当該メッシュが法線を持たない場合は法線を追加する
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL)) {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                    pMesh->GetFVF() | D3DFVF_NORMAL, 
                                    pDevice, &pMeshContainer->MeshData.pMesh );
        if (FAILED(hr)) return E_FAIL;

		// 引数で渡されたメッシュへのポインタに新しいメッシュへのポインタをセット
		// pMeshへの参照はこの時点で存在しないので、ここではreleaseをかけない
        pMesh = pMeshContainer->MeshData.pMesh;
        D3DXComputeNormals( pMesh, NULL );

    } else{
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type  = D3DXMESHTYPE_MESH;
        pMesh->AddRef();
    }

	//メッシュのマテリアル設定
    pMeshContainer->NumMaterials	= max(1, NumMaterials);
    pMeshContainer->pMaterials		= new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures		= new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency		= new DWORD[iFacesAmount*3];

    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL)) return E_FAIL;  
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    if (NumMaterials > 0){            
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++){
            if( pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL){
				char		strTexturePath[256] ="DATA\\X\\";
				strcpy(strTexturePath,paths);
				LPSTR		fName	= pMeshContainer->pMaterials[iMaterial].pTextureFilename;

                strcat_s( strTexturePath, fName);

                if( FAILED( D3DXCreateTextureFromFile( 
								pDevice, 
								strTexturePath,
								&pMeshContainer->ppTextures[iMaterial]))){

					pMeshContainer->ppTextures[iMaterial] = NULL;
				}
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }

    }else{
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular  = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

	//当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != NULL) // ワンスキン形式のXファイル
	{
		pMeshContainer->pSkinInfo = pSkinInfo;// pSkinInfo と pMesh をセット
        pSkinInfo->AddRef();

		pMeshContainer->lpMesh = pMesh;
		pMesh->AddRef();

		// Bone Offset Matrices 保存用のメモリを確保
		dwBoneAmt = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneAmt];
		if( !pMeshContainer->pBoneOffsetMatrices ) return E_OUTOFMEMORY;
			
		// Bone Offset Matrices 読み込み
		for (DWORD i= 0; i < dwBoneAmt; i++){
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
					pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),sizeof(D3DMATRIX));	
		}

		//スキンメッシュ作成
		hr = GenerateSkinMesh(pMeshContainer);
		if(FAILED(hr)){
			hr = E_FAIL;
			SAFE_RELEASE( pDevice );
		if( pMeshContainer ) DestroyMeshContainer( pMeshContainer );
			return hr;
		}
	}
	//ローカルに生成したメッシュコンテナーを呼び出し側にコピーする
	*ppMeshContainer = pMeshContainer;

    return S_OK;
}

HRESULT GenerateSkinMesh( MESHCONTAINER *pMeshContainer )
{
	HRESULT hr = S_OK;

	//スキン情報がないなら終了
	if( !pMeshContainer->pSkinInfo ) return hr;

	//初期化しておく
	if(pMeshContainer->MeshData.pMesh)			SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	if(pMeshContainer->pBoneCombinationBuf)		SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

	hr = pMeshContainer->lpMesh->CloneMeshFVF(
			D3DXMESH_MANAGED, pMeshContainer->lpMesh->GetFVF(),
			lpD3DDev , &pMeshContainer->MeshData.pMesh );
	if( FAILED( hr ) ) return hr;

	//メッシュコンテナに所属している頂点数の取得
	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( NULL, &pMeshContainer->NumAttributeGroups );
	if( FAILED( hr ) ) return hr;

	//メッシュコンテナに所属している頂点情報の格納
	delete[] pMeshContainer->pAttributeTable;
	pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
	if( !pMeshContainer->pAttributeTable ){
		hr = E_OUTOFMEMORY;
		return hr;
	}
	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL );
	if( FAILED( hr ) ) return hr;

	// 他のメッシュによってボーン行列用のメモリが確保しきれていない場合は確保
	if( pMeshContainer->NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() ){
		pMeshContainer->NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		delete []pMeshContainer->pBoneMatrices;
		pMeshContainer->pBoneMatrices = new D3DXMATRIXA16[pMeshContainer->NumBoneMatricesMax];
		if( !pMeshContainer->pBoneMatrices ){
			hr = E_OUTOFMEMORY;
			return hr;
		}
    }
    return hr;
}

//―――――――――――――――――――――――――――――――――――――――――//
//	ボーン行列のセット																//
//																				    //
//―――――――――――――――――――――――――――――――――――――――――//
BOOL SetupBoneMatrixPointers(SKINMESHHANG *pSkinMeshMang, LPD3DXFRAME pFrame )
{
	if( pFrame->pMeshContainer){
		if( FAILED( SetupBoneMatrixPointersOnMesh(pSkinMeshMang, pFrame->pMeshContainer) ) ) return false;
	}

	if( pFrame->pFrameSibling ){
		if( FAILED( SetupBoneMatrixPointers(pSkinMeshMang, pFrame->pFrameSibling ) ) ) return false;
	}

	if( pFrame->pFrameFirstChild ){
		if( FAILED( SetupBoneMatrixPointers(pSkinMeshMang, pFrame->pFrameFirstChild ) ) ) return false;
	}

	return true;
}

BOOL SetupBoneMatrixPointersOnMesh(SKINMESHHANG *pSkinMeshHang, LPD3DXMESHCONTAINER pD3DXMeshContainer)
{
	MESHCONTAINER		*pMeshContainer = (MESHCONTAINER*)pD3DXMeshContainer;

	// スキンメッシュのときはボーン行列をセット
	if( pMeshContainer->pSkinInfo ){
		DWORD NumBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[NumBones];
		if( !pMeshContainer->ppBoneMatrixPtrs ) return false;

		for( DWORD i = 0; i < NumBones; i++ ){
			FRAME *pFrame = (FRAME*)D3DXFrameFind(pSkinMeshHang->pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName( i ) );
			if( !pFrame) return false;
			pMeshContainer->ppBoneMatrixPtrs[i] = &pFrame->CombinedTransformationMatrix;
		}
	}
	return true;
}

//―――――――――――――――――――――――――――――――――――――――――//
//	フレームを破棄する																//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
    SAFE_DELETE_ARRAY( pFrameToFree->Name );
    SAFE_DELETE( pFrameToFree );
    return S_OK; 
}

//―――――――――――――――――――――――――――――――――――――――――//
//	メッシュコンテナーを破棄する													//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    DWORD iMaterial;
    MESHCONTAINER *pMeshContainer = (MESHCONTAINER*)pMeshContainerBase;

    SAFE_DELETE_ARRAY( pMeshContainer->Name );
    SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
	SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );


    if (pMeshContainer->ppTextures != NULL)
    {
        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
        }
    }

	SAFE_DELETE_ARRAY( pMeshContainer->ppTextures);
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs);

	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pSkinInfo );
	SAFE_RELEASE( pMeshContainer->lpMesh );

    SAFE_DELETE( pMeshContainer );
    return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//	描画																			//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
void	DrawSkinMesh(
	SKINMESHHANG *pSkinMeshHang, VEC3 pos,  MAT mRot, float Trans, 
	float Scx, float Scy, float Scz, bool isdrawF, char *FrameName, ID3DXEffect* effect)
{
	D3DXMATRIX	matScale,matRot,matPos,mat;

	//拡大縮小をを行列変換
	D3DXMatrixScaling(&matScale,Scx,Scy,Scz);

	//現在の座標を行列変換
	D3DXMatrixTranslation(&matPos,pos.x,pos.y,pos.z);
	//一つの行列へ合成
	D3DXMatrixMultiply(&mat,&matScale,&mRot);
	D3DXMatrixMultiply(&mat,&mat,&matPos);
	
	//座標の更新
	lpD3DDev->SetTransform(D3DTS_WORLD, &mat);


	Update(pSkinMeshHang, mat, effect);
	if(isdrawF==ON)
		DrawFrame(pSkinMeshHang, pSkinMeshHang->pFrameRoot, FrameName, Trans, effect);
	
}

void	Update(SKINMESHHANG* pSkinMesh, D3DXMATRIX mat, ID3DXEffect* effect)
{	
	//行列の更新
	UpdateFrameMatrices( pSkinMesh->pFrameRoot, &mat);
}

//フレーム内のメッシュ毎にワールド変換行列を更新する
void	UpdateFrameMatrices( LPD3DXFRAME pD3DXFrame, LPD3DXMATRIX pParentMatrix )
{
	FRAME *pFrame = (FRAME*)pD3DXFrame;

	if (pParentMatrix ) {
		D3DXMatrixMultiply( &pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix );
	}
	else { 
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if( pFrame->pFrameSibling ) UpdateFrameMatrices( pFrame->pFrameSibling, pParentMatrix );

	if( pFrame->pFrameFirstChild ) UpdateFrameMatrices( pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix );
}


//フレームをレンダリングする
void	DrawFrame(SKINMESHHANG* pSkinMesh, LPD3DXFRAME pD3DXFrame, char *FrameName, float Trans, ID3DXEffect* effect)
{
	LPD3DXMESHCONTAINER pD3DXMeshContainer;
	FRAME *pFrame = (FRAME*)pD3DXFrame;
	pD3DXMeshContainer = pD3DXFrame->pMeshContainer;

	//名前がないときはフラグで検査　あるときはその名前で検査
	if(pFrame->DrawF==ON && FrameName==NULL ||  FrameName!=NULL && (strcmp(FrameName, pD3DXFrame->Name)==0)){
		while( pD3DXMeshContainer ){
			DrawMeshContainer( pSkinMesh, pD3DXMeshContainer, pD3DXFrame ,Trans, effect);
			pD3DXMeshContainer = pD3DXMeshContainer->pNextMeshContainer;
		}
	}

	if( pD3DXFrame->pFrameSibling ) DrawFrame( pSkinMesh, pD3DXFrame->pFrameSibling , FrameName, Trans, effect);
	if( pD3DXFrame->pFrameFirstChild ) DrawFrame(pSkinMesh, pD3DXFrame->pFrameFirstChild, FrameName, Trans, effect);
}


void	DrawMeshContainer(SKINMESHHANG* pSkinMesh, LPD3DXMESHCONTAINER pD3DXMeshContainer, LPD3DXFRAME pD3DXFrame, float Trans, ID3DXEffect* effect)
{
	MESHCONTAINER   *pMeshContainer = (MESHCONTAINER*)pD3DXMeshContainer;
	D3DXMATRIX		matId;
	PBYTE			pVerticesSrc;
	PBYTE			pVerticesDest;
	D3DMATERIAL9	mat;
	
	if( pMeshContainer->pSkinInfo ){ // スキンメッシュの場合
		DWORD NumBones = pMeshContainer->pSkinInfo->GetNumBones();
		{
			for( DWORD i = 0; i < NumBones; i++ ){
				D3DXMatrixMultiply(
					&pMeshContainer->pBoneMatrices[i],
					&pMeshContainer->pBoneOffsetMatrices[i], 
					pMeshContainer->ppBoneMatrixPtrs[i]
				);
			}
		}

		// ワールド行列をクリア
		D3DXMatrixIdentity(&matId);
		lpD3DDev->SetTransform(D3DTS_WORLD, &matId);
		

		// 頂点バッファをロック
		pMeshContainer->lpMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer( 0, (LPVOID*)&pVerticesDest);

		// スキンメッシュ作成
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh( pMeshContainer->pBoneMatrices, NULL, pVerticesSrc, pVerticesDest);

		// 頂点バッファのロックを解除
		pMeshContainer->lpMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();


		// スキンメッシュ描画
		if(effect==NULL){
			for( UINT i = 0; i < pMeshContainer->NumAttributeGroups; i++ ){
			
				mat = pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[i].AttribId].MatD3D;
				mat.Diffuse.a = Trans;
				lpD3DDev->SetMaterial(&mat);

				lpD3DDev->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[i].AttribId]);
				pMeshContainer->MeshData.pMesh->DrawSubset( pMeshContainer->pAttributeTable[i].AttribId);
			}
		}
		else {
			for( UINT i = 0; i < pMeshContainer->NumAttributeGroups; i++){
				mat = pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[i].AttribId].MatD3D;
				mat.Diffuse.a = Trans;
				effect->SetVector("Diffuse", (D3DXVECTOR4*)&mat.Diffuse);
				effect->SetVector("Ambient", (D3DXVECTOR4*)&mat.Ambient);


				effect->SetTexture("MeshTex", pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[i].AttribId]);

				// サブセットの描画
				UINT p, np;
				effect->Begin(&np, 0);
				for (p=0; p<np; p++) {
					effect->BeginPass(p);
					pMeshContainer->MeshData.pMesh->DrawSubset( pMeshContainer->pAttributeTable[i].AttribId);
					effect->EndPass();
				}
				effect->End();
			}
		}

	}
	else{ // 通常のアニメメッシュの場合
		lpD3DDev->SetTransform( D3DTS_WORLD, 
			&((FRAME *)pD3DXFrame )->CombinedTransformationMatrix );

		// メッシュ描画
		for ( DWORD i = 0; i < pMeshContainer->NumMaterials; i++ ){
			mat=pMeshContainer->pMaterials[i].MatD3D;
			mat.Diffuse.a = Trans;
			lpD3DDev->SetMaterial(&mat);
			lpD3DDev->SetTexture( 0, pMeshContainer->ppTextures[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset( i );
		}
	}
}


void Gs_DrawAnimModel(VEC3 Pos,GsANMMESH no,DIR3 Rot,float Scx, float Scy, float Scz)
{
	MAT		mRot;
	MRYPR(&mRot, F(RAD(Rot.Yaw)), F(RAD(Rot.Pit)), F(RAD(Rot.Rol)));
	DrawSkinMesh(&GsANMMESHHang[no], Pos, mRot, 0, Scx, Scy, Scz , ON, NULL, NULL);
}

void Gs_DrawSKINMESH(GsANMMESH no, VEC3 Pos, MAT mRot, float Trans, float Scx, float Scy, float Scz)
{
	DrawSkinMesh(&GsANMMESHHang[no], Pos, mRot, Trans, Scx, Scy, Scz , ON, NULL, NULL);
}

void Gs_DrawFRAME(GsANMMESH no, VEC3 Pos, MAT mRot, float Trans, float Scx, float Scy, float Scz, char *FrameName)
{
	DrawSkinMesh(&GsANMMESHHang[no], Pos, mRot,Trans, Scx, Scy, Scz , ON, FrameName, NULL);
}

void Gs_DrawSKINMESH(GsANMMESH no, VEC3 Pos, MAT mRot, float Trans, float Scx, float Scy, float Scz, ID3DXEffect* effect)
{
	DrawSkinMesh(&GsANMMESHHang[no], Pos, mRot, Trans, Scx, Scy, Scz , ON, NULL, effect);
}

void Gs_DrawFRAME(GsANMMESH no, VEC3 Pos, MAT mRot, float Trans, float Scx, float Scy, float Scz, char *FrameName, ID3DXEffect* effect)
{
	DrawSkinMesh(&GsANMMESHHang[no], Pos, mRot,Trans, Scx, Scy, Scz , ON, FrameName, effect);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	SkinMeshの破棄																	//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
bool	Gs_ReleaseANMMESH(GsANMMESH no)
{
	if (GsANMMESHHang[no].Flg) {
		GsANMMESHHang[no].Flg = OFF;
		ReleaseSkinMesh(&GsANMMESHHang[no]);
	}
	return	(OK);
}

void	ReleaseSkinMesh(SKINMESHHANG *pSkinMesh)
{
	if(pSkinMesh->pFrameRoot)			SAFE_DELETE(pSkinMesh->pFrameRoot);
	if(pSkinMesh->pAnimController)		SAFE_RELEASE(pSkinMesh->pAnimController);				
}

void	DoNullClear(SKINMESHHANG* pSkinMesh)
{
	pSkinMesh->pFrameRoot           = NULL;
	pSkinMesh->pAnimController      = NULL;
	pSkinMesh->Flg					=OFF;
}

//―――――――――――――――――――――――――――――――――――――――――//
// 　階層走査し指定した名前のFrameを見つけ出す
//―――――――――――――――――――――――――――――――――――――――――//
FRAME*  FindFrame(char *FrameName, LPD3DXFRAME pD3DXFrame)
{
	FRAME *pFrame = (FRAME*)pD3DXFrame;
 
     //名前を検索
     if ((pFrame->Name != NULL) && (strcmp(FrameName, pFrame->Name) == 0)) return pFrame;
 
     //子供、兄弟を検索
     if (pFrame->pFrameFirstChild) {
         pFrame = FindFrame(FrameName, pFrame->pFrameFirstChild);
         if (pFrame != NULL) return pFrame;
     }
     if (pFrame->pFrameSibling) {
         pFrame = FindFrame(FrameName, pFrame->pFrameSibling);
         if (pFrame != NULL) return pFrame;
     }
 
     return NULL;
}

LPD3DXFRAME*   Gs_FindFRAME(GsANMMESH no, char *FrameName)
{
	FRAME*	pFrame;
	pFrame = FindFrame(FrameName, GsANMMESHHang[no].pFrameRoot);

	return (LPD3DXFRAME*)pFrame;
}

//―――――――――――――――――――――――――――――――――――――――――//
// 　Frameの描画のON/OFFを設定
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_SetFrameDF(GsANMMESH no, char *FrameName, bool isdrawF)
{
	FRAME*	pFrame=NULL;

	pFrame = FindFrame(FrameName, GsANMMESHHang[no].pFrameRoot);
	if(pFrame != NULL){
		pFrame->DrawF=isdrawF;
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
// 　Frameの姿勢行列を取り出す
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_GetFrameMat(GsANMMESH no, char *FrameName, MAT* pMat)
{
	FRAME*	pFrame=NULL;
	MESHCONTAINER   *pMC = NULL;

	pFrame = FindFrame(FrameName, GsANMMESHHang[no].pFrameRoot);
	if(pFrame != NULL){
		*pMat=pFrame->CombinedTransformationMatrix;
	}
}

	
//―――――――――――――――――――――――――――――――――――――――――//
// 　Frameの頂点座標を取得
//―――――――――――――――――――――――――――――――――――――――――//
D3DXVECTOR3* Gs_GetVERTEX(LPD3DXFRAME* pframe)
{
	LPD3DXMESH		pMesh;				// メッシュ
	PBYTE			*pVertexBuffer;		// 頂点バッファ
	D3DXVECTOR3		*pVertex;			// 頂点座標
	FRAME			*pFrame		=	(FRAME*)pframe;

	pMesh = pFrame->pMeshContainer->MeshData.pMesh;

	// 頂点バッファのロック
	pMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pVertexBuffer );

	DWORD NumVertices = pMesh->GetNumVertices();    // 頂点の数を取得する。
	DWORD FVF = pMesh->GetFVF();                    // 頂点フォーマットを取得する。
	DWORD VertexSize = D3DXGetFVFVertexSize( FVF ); // 頂点のサイズを取得する。

	//for( DWORD i = 0; i < NumVertices; i++ ){
		pVertex = (D3DXVECTOR3*)pVertexBuffer;		// 頂点座標を取得する。
		//pVertexBuffer += VertexSize;				// 次の頂点にポインタを移動する。
	//}

	// 頂点バッファのロックを解除する。
	pMesh->UnlockVertexBuffer();

	return	(pVertex);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  SkinMeshアニメ管理
//―――――――――――――――――――――――――――――――――――――――――//

//―――――――――――――――――――――――――――――――――――――――――//
// グローバルアニメーションタイムの取得
//―――――――――――――――――――――――――――――――――――――――――//
float	Gs_GetANMTIME(GsANMMESH no) 
{
	float time;
	if (GsANMMESHHang[no].pAnimController)
		time = F(GsANMMESHHang[no].pAnimController->GetTime());
	else 
		time = 0.0f;
	return time;
}

//―――――――――――――――――――――――――――――――――――――――――//
// グローバルアニメーションタイムを進める
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_AdvanceANMTIME(GsANMMESH no, double time) 
{
	if (GsANMMESHHang[no].pAnimController){ 
		GsANMMESHHang[no].pAnimController->AdvanceTime(time, NULL);
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
// グローバルアニメーションタイムをリセットする
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_ResetANMTIME(GsANMMESH no) 
{
	if (GsANMMESHHang[no].pAnimController){ 
		GsANMMESHHang[no].pAnimController->ResetTime();
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//  アニメーショントラックの切り替え
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_SetANMTRACK( GsANMMESH no, DWORD track, bool enable )
{
	if (GsANMMESHHang[no].pAnimController){
		GsANMMESHHang[no].pAnimController->SetTrackEnable(track, enable);
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//  アニメーショントラックの重みの設定
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_SetANMWEIGHT(GsANMMESH no, DWORD track, float weight)
{
	if (GsANMMESHHang[no].pAnimController) 
		GsANMMESHHang[no].pAnimController->SetTrackWeight(track, weight);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  アニメーショントラックの速度の設定
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_SetANMSPEED(GsANMMESH no, DWORD track, float speed)
{
	if (GsANMMESHHang[no].pAnimController) 
		GsANMMESHHang[no].pAnimController->SetTrackSpeed(track, speed);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  アニメーショントラックごとのローカルタイムの設定
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_SetTRACKTIME(GsANMMESH no, DWORD track, double time)
{
	if	(GsANMMESHHang[no].pAnimController)
		GsANMMESHHang[no].pAnimController->SetTrackPosition(track, time);
}



//―――――――――――――――――――――――――――――――――――――――――//
//
//
//			３Ｄ演算関係
//
//
//―――――――――――――――――――――――――――――――――――――――――//

//―――――――――――――――――――――――――――――――――――――――――//
//	Rotation set
//―――――――――――――――――――――――――――――――――――――――――//
DIR3	Dir3(float Y,float P,float R)
{
	DIR3	x;
	x.Yaw	= Y;
	x.Pit	= P;
	x.Rol	= R;
	return	x;
}

//―――――――――――――――――――――――――――――――――――――――――//
//	ベクトルの回転
//―――――――――――――――――――――――――――――――――――――――――//
D3DXVECTOR3	MakeVec(float dx,float dy,float dz,float yaw,float pitch,float roll)
{
	D3DXVECTOR3		pDist = D3DXVECTOR3(dx,dy,dz);
	D3DXMATRIX		mat;
	D3DXVECTOR3		vec;

	D3DXMatrixRotationYawPitchRoll(&mat,D3DXToRadian(yaw),D3DXToRadian(pitch),D3DXToRadian(roll));
	D3DXVec3TransformCoord(&vec,&pDist,&mat);
	return vec;
}

D3DXVECTOR3 MakeVector(D3DXVECTOR3 *pDist,float yaw,float pitch,float roll)
{
	D3DXMATRIX		mat;
	D3DXVECTOR3		vec;

	D3DXMatrixRotationYawPitchRoll(&mat,yaw,pitch,roll);
	D3DXVec3TransformCoord(&vec,pDist,&mat);
	return vec;
}


//―――――――――――――――――――――――――――――――――――――――――//
//	2D用ベクトルの回転
//―――――――――――――――――――――――――――――――――――――――――//
void MakeXY(float len,float rad,float *px,float *py,int xy)
{
	D3DXVECTOR3		vec;
	D3DXMATRIX		mat;

	switch(xy){
		case 0:
			vec=D3DXVECTOR3(len,0.0f,0.0f);
			break;
		case 1:
			vec=D3DXVECTOR3(0.0f,len,0.0f);
			break;
		default:
			MessageBox(NULL,"xyの指定が間違っています","MakeXY()が失敗",MB_OK);
			return;
	}
	D3DXMatrixRotationYawPitchRoll(&mat,0.0f,0.0f,rad);
	D3DXVec3TransformCoord(&vec,&vec,&mat);
	*px=vec.x;
	*py=vec.y;
}

/////////////////////////////////////////////////////////////////////////
//	2点間を結ぶベクトルの正規化したx軸成分とy軸成分を返す
/////////////////////////////////////////////////////////////////////////
void MakeXYNormalize(float x1,float y1,float x2,float y2,float *px,float *py)
{
	D3DXVECTOR3		vec,vec1,vec2;

	vec1=D3DXVECTOR3(x1,y1,0.0f);
	vec2=D3DXVECTOR3(x2,y2,0.0f);
	D3DXVec3Subtract(&vec,&vec2,&vec1);
	D3DXVec3Normalize(&vec,&vec);
	*px=vec.x;
	*py=vec.y;
}


/////////////////////////////////////////////////////////////////////////
//	2点間を結ぶベクトルの正規化したVEC3成分を返す
/////////////////////////////////////////////////////////////////////////
VEC3	MakeVecNormalize(VEC3 vec1,VEC3 vec2)
{
	VEC3		vec;

	D3DXVec3Subtract(&vec,&vec2,&vec1);
	D3DXVec3Normalize(&vec,&vec);
	return	(vec);
}

/////////////////////////////////////////////////////////////////////////
//	2D用ベクトルの長さ計算
//		x1とy1に2Dベクトルの座標１を、x2とy2に2Dベクトルの座標２を
//		パラーメーターとして渡すことで、
//		座標１と座標２の長さを求めて返す。
/////////////////////////////////////////////////////////////////////////
float GetLen(float x1,float y1,float x2,float y2)
{
	float			len=0.0f;
	D3DXVECTOR3		vec,vec1(x1,y1,0.0f),vec2(x2,y2,0.0f);

	D3DXVec3Subtract(&vec,&vec1,&vec2);
	len=D3DXVec3Length(&vec);
	return len;
}

// 3D用
float Get3DLen(VEC3 a,VEC3 b)
{
	float		len=0.0f;
	VEC3		vec;

	D3DXVec3Subtract(&vec,&a,&b);
	len=D3DXVec3Length(&vec);
	return len;
}



//―――――――――――――――――――――――――――――――――――――――――//
//  オブジェクトを回転行列の方向へ移動量だけ進める
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_MoveOBJECT(VEC3* pos, MAT mRot, VEC3 mDist)
{
	MAT  mat;
	VEC3 vec;

	//D3DXMatrixRotationYawPitchRoll(&mat,F(RAD(dir.Yaw)),dir.Pitch,dir.Roll),		
	D3DXVec3TransformCoord(&vec,&mDist,&mRot);
	D3DXVec3Add(pos, pos, &vec);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  クオータニオンを用いて途中の回転行列を取得
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_MatSLERP(MAT* pmat, MAT* pmat1, MAT* pmat2, float ratio)
{
	QUA qua, qua1, qua2;

	//行列をクオータニオンへ変換
	D3DXQuaternionRotationMatrix(&qua1, pmat1);
	D3DXQuaternionRotationMatrix(&qua2, pmat2);
	//途中の回転角度のクオータニオンを取得
	D3DXQuaternionSlerp(&qua, &qua1, &qua2, ratio);
	//クオータニオンを行列へ
	D3DXMatrixRotationQuaternion(pmat, &qua);
}


//―――――――――――――――――――――――――――――――――――――――――//
//  指定したYaw　Pit　Rol　を回転行列へ加算
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_MatAddYPR(MAT *mat, float yaw,float pit,float rol)
{
	MAT		smat;

	D3DXMatrixIdentity(&smat);

	D3DXMatrixRotationYawPitchRoll(&smat,D3DXToRadian(yaw),D3DXToRadian(pit),D3DXToRadian(rol));
	D3DXMatrixMultiply(mat,mat,&smat);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  回転行列からYaw　Pit　Rolを取得
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_MatGetYPR(DIR3 *pDir, MAT mat)
{
	pDir->Rol = atan2f(mat._12, mat._22);
	pDir->Pit = asinf(-mat._32);
	pDir->Yaw = atan2f(mat._31, mat._33);

	if (fabsf(cosf(pDir->Pit)) < 1.0e-6f) {
		if (mat._12 > 0.0f)		pDir->Rol	= pDir->Rol + D3DX_PI;
		else					pDir->Rol	= pDir->Rol - D3DX_PI;
		if (mat._31 > 0.0f)		pDir->Yaw	= pDir->Yaw + D3DX_PI;
		else					pDir->Yaw	= pDir->Yaw - D3DX_PI;
	}

	pDir->Yaw	= D3DXToDegree(pDir->Yaw);
	pDir->Pit	= D3DXToDegree(pDir->Pit);
	pDir->Rol	= D3DXToDegree(pDir->Rol);

	if (pDir->Yaw < 0)	pDir->Yaw = pDir->Yaw + 360;

}



//―――――――――――――――――――――――――――――――――――――――――//
// 親　子　継承
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_LinkChild(MODEL_PARAM *pChild,MODEL_PARAM *pParent)
{
	//child（自分）までの回転行列を作る
	D3DXMatrixRotationYawPitchRoll(&pChild->Mat,
		D3DXToRadian(pParent->Rot.Yaw),
		D3DXToRadian(pParent->Rot.Pit),
		D3DXToRadian(pParent->Rot.Rol));

	//parent（親）までの状態行列を継承する
	D3DXMatrixMultiply(&pChild->Mat,&pChild->Mat,&pParent->Mat);

	//child（自分）の位置を求める
	D3DXVec3TransformCoord(&pChild->Pos,&pChild->Ofs,&pChild->Mat);
	D3DXVec3Add(&pChild->Pos,&pChild->Pos,&pParent->Pos);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	高さの判定
//―――――――――――――――――――――――――――――――――――――――――//
void	CheckHeight(LPD3DXMESH lpMesh,D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3		ray;
	BOOL			isHit=FALSE;
	DWORD			faceCnt;
	float			u,v,dist;
	LPD3DXBUFFER	xBuf;
	DWORD			hitCnt;

	ray=*pVec;
//	ray.y+=1.0f;
	ray.y+=4.0f;
//	D3DXIntersect(lpMesh,&ray,&D3DXVECTOR3(0.0f,-1.0f,0.0f),&isHit,
//		&faceCnt,&u,&v,&dist,&xBuf,&hitCnt);
	D3DXIntersect(lpMesh,&ray,&D3DXVECTOR3(0.0f,-1.0f-0.090f,0.0f),&isHit,
		&faceCnt,&u,&v,&dist,&xBuf,&hitCnt);
	if(isHit)	xBuf->Release();
	if(isHit)	pVec->y=ray.y-dist;
}

//―――――――――――――――――――――――――――――――――――――――――//
//	傾きの判定
//―――――――――――――――――――――――――――――――――――――――――//
void	CheckLean(LPD3DXMESH lpMesh,D3DXVECTOR3 *pPos,
	float yaw,float *pitch,float *roll,float L,float R,float U,float D)
{
	D3DXVECTOR3		ray1,ray2,vec,vec0,vec1,vec2;
	BOOL			isHit1,isHit2;
	DWORD			faceCnt;
	float			u,v,dist1,dist2;
	LPD3DXBUFFER	xBuf;
	DWORD			hitCnt;
	float			val;

	//前
	ray1=MakeVector(&D3DXVECTOR3(0.0f,1.0f,U),yaw,*pitch,*roll);
	D3DXVec3Add(&ray1,pPos,&ray1);
	D3DXIntersect(lpMesh,&ray1,&D3DXVECTOR3(0.0f,-1.0f,0.0f),&isHit1,
		&faceCnt,&u,&v,&dist1,&xBuf,&hitCnt);

	if(isHit1)	xBuf->Release();

	//後ろ
	ray2=MakeVector(&D3DXVECTOR3(0.0f,1.0f,D),yaw,*pitch,*roll);
	D3DXVec3Add(&ray2,pPos,&ray2);
	D3DXIntersect(lpMesh,&ray2,&D3DXVECTOR3(0.0f,-1.0f,0.0f),&isHit2,
		&faceCnt,&u,&v,&dist2,&xBuf,&hitCnt);

	if(isHit2)	xBuf->Release();

	//チェック
	if(isHit1 && isHit2){
		//前のヒットポジション
		vec1=ray1;
		vec1.y-=dist1;
		//後ろのヒットポジション
		vec2=ray2;
		vec2.y-=dist2;
		//傾きベクトルの作成
		D3DXVec3Subtract(&vec,&vec1,&vec2);
		//傾きベクトルから地面に水平のベクトルを作成
		vec0=vec;
		vec0.y=0;
		//ベクトルの正規化
		D3DXVec3Normalize(&vec,&vec);
		D3DXVec3Normalize(&vec0,&vec0);
		//傾きの計算
		val=D3DXVec3Dot(&vec,&vec0);	//ベクトルの内積
		if((val>=-1)&&(val<=1)){
			if(vec1.y>vec2.y)	*pitch=-(float)acos((double)val);
			else				*pitch=(float)acos((double)val);
		}
	}

	//右
	ray1=MakeVector(&D3DXVECTOR3(R,1.0f,0.0f),yaw,*pitch,*roll);
	D3DXVec3Add(&ray1,pPos,&ray1);
	D3DXIntersect(lpMesh,&ray1,&D3DXVECTOR3(0.0f,-1.0f,0.0f),&isHit1,
		&faceCnt,&u,&v,&dist1,&xBuf,&hitCnt);
	if(isHit1)	xBuf->Release();

	//左
	ray2=MakeVector(&D3DXVECTOR3(L,1.0f,0.0f),yaw,*pitch,*roll);
	D3DXVec3Add(&ray2,pPos,&ray2);
	D3DXIntersect(lpMesh,&ray2,&D3DXVECTOR3(0.0f,-1.0f,0.0f),&isHit2,
		&faceCnt,&u,&v,&dist2,&xBuf,&hitCnt);
	if(isHit2)	xBuf->Release();

	//チェック
	if(isHit1 && isHit2){
		//前のヒットポジション
		vec1=ray1;
		vec1.y-=dist1;
		//後ろのヒットポジション
		vec2=ray2;
		vec2.y-=dist2;
		//傾きベクトルの作成
		D3DXVec3Subtract(&vec,&vec1,&vec2);
		//傾きベクトルから地面に水平のベクトルを作成
		vec0=vec;
		vec0.y=0;
		//ベクトルの正規化
		D3DXVec3Normalize(&vec,&vec);
		D3DXVec3Normalize(&vec0,&vec0);
		//傾きの計算
		val=D3DXVec3Dot(&vec,&vec0);	//ベクトルの内積
		if((val>=-1)&&(val<=1)){
			if(vec1.y>vec2.y)	*roll=(float)acos((double)val);
			else				*roll=-(float)acos((double)val);
		}
	}
}


//―――――――――――――――――――――――――――――――――――――――――//
//　メッシュ内の（ｘ、ｙ、ｚ）の高さを取得
//―――――――――――――――――――――――――――――――――――――――――//
float	Gs_CheckHeight(GsMESH id,float x,float y, float z)
{
	D3DXVECTOR3		vec = D3DXVECTOR3(x,y,z);
	CheckHeight(GsMeshHang[id].lpMesh,&vec);

	return(F(vec.y));
}

//―――――――――――――――――――――――――――――――――――――――――//
//　メッシュ内の（ｘ、ｙ、ｚ）の傾き
//―――――――――――――――――――――――――――――――――――――――――//
float	Gs_CheckPitch(GsMESH id,float x,float y, float z,float yaw,float pit,float rol)
{
	D3DXVECTOR3		vec = D3DXVECTOR3(x,y,z);
	yaw	= D3DXToRadian(yaw);
	pit	= D3DXToRadian(pit);
	rol	= D3DXToRadian(rol);
	CheckLean(GsMeshHang[id].lpMesh,&vec,yaw,&pit,&rol,-0.5f,0.5f,0.5f,-0.5f);
	return(D3DXToDegree(pit));
}

float	Gs_CheckRoll(GsMESH id,float x,float y, float z,float yaw,float pit,float rol)
{
	D3DXVECTOR3		vec = D3DXVECTOR3(x,y,z);
	yaw	= D3DXToRadian(yaw);
	pit	= D3DXToRadian(pit);
	rol	= D3DXToRadian(rol);
	CheckLean(GsMeshHang[id].lpMesh,&vec,yaw,&pit,&rol,-0.5f,0.5f,0.5f,-0.5f);
	return(D3DXToDegree(rol));
}



//―――――――――――――――――――――――――――――――――――――――――//
// VIEW
//
//―――――――――――――――――――――――――――――――――――――――――//
typedef struct {
	short		Flg;
	VEC3		viewEye;
	VEC3		viewLook;
	short		vX,vY,vW,vH;
}GSCAMRET;

GSCAMRET	CamRet[3];



void		Gs_SetCamView(short id,VEC3 Eye,VEC3 Look)
{
	CamRet[id].viewEye	= Eye;
}

HRESULT ChangeViewport(DWORD dwX,DWORD dwY,DWORD dwWidth,DWORD dwHeight)
{
	D3DVIEWPORT9 vp;

	vp.X		=dwX;
	vp.Y		=dwY;
	vp.Width	=dwWidth;
	vp.Height	=dwHeight;
	vp.MinZ		=0;
	vp.MaxZ		=1;	

	lpD3DDev->SetViewport(&vp);

	 // プロジェクショントランスフォーム（カメラ座標→スクリーン座標への変換）
	 D3DXMATRIX matProj;
	 D3DXMatrixPerspectiveFovLH( &matProj, GsCam.Proj,
		 (FLOAT)dwWidth/(FLOAT)dwHeight, 0.1f, 1000.0f );
	 lpD3DDev->SetTransform( D3DTS_PROJECTION, &matProj );   

	return S_OK;
}

void	SetViewMatrix(VEC3 eye,VEC3 look)
{

	// ビュートランスフォーム（ワールド座標→カメラ座標への変換）
	D3DXMATRIX		matView;
	D3DXVECTOR3		vecUpVec(0,1,0);//上方ベクトル定義	

	D3DXMatrixLookAtLH( &matView, &eye, &look, &vecUpVec );
	lpD3DDev->SetTransform( D3DTS_VIEW, &matView );

}

void	Gs_DrawView(void	(*draw)(void),int x,int y,int w,int h,VEC3 eye,VEC3 look) 
{

	ChangeViewport(x,y,w,h);
	SetViewMatrix(eye,look);
	SetupLight();
	Gs_DrawClear2( );
	Gs_DrawScene( );
	draw( );

	ChangeViewport(0,0,WINW,WINH);
}



//==========================================================================================================//
//								EOP																			//
//==========================================================================================================//
