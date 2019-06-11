#pragma once

#include "define.h"
#include "RendererInterface.h"
#include "ErrorInterface.h"
#include "Camera.h"
#include "NodeInterface.h"
#include "ResManager.h"

namespace pool
{
	class CDxRendererPool : public CRendererInterface, public  CDeviceErrorInterface
	{
	public:
		CDxRendererPool();
		~CDxRendererPool();

		bool Initialize(PGAME game);

		void ClearUp();

		void CleanBuffers(bool bColor, bool bDepth, bool bStencil);

		void Response(DWORD time, PNODE node, PINPUT input, PCAMERA camera);
		void Update(PNODE node, DWORD time, PCAMERA camera);
		void BeforeDraw(PNODE node, PCAMERA camera, PNODE rootNode);
		void Draw(PNODE node, PCAMERA camera);

	public:
		//MultiSampleLevel 采多重样率(enum 在define定义 默认的太长了)
		eMSL::MultiSampleLevel mMSL;
		//灯光
		bool mLightEnable;
		//Z缓存
		bool mZEnable;
		//模板
		bool mStencil;
		//背面三角形的剔除方式 D3DCULL_NONE : 不要剔除背面, D3DCULL_CW : 按照顺时针的方向的顶点剔除背面, D3DCULL_CCW : 按照逆时针方向剔除背面。
		_D3DCULL mCULL;

		//清屏色
		D3DCOLOR mClearColor;

	private:
		//检测硬件是否支持当前设置多重采样等级
		D3DMULTISAMPLE_TYPE GetMultiSampleLevel(D3DFORMAT format);

	private:
		LPDIRECT3D9 mDirect3D;
		LPDIRECT3DDEVICE9 mDevice;
	};
}

