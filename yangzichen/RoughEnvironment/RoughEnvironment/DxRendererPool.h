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
		//MultiSampleLevel �ɶ�������(enum ��define���� Ĭ�ϵ�̫����)
		eMSL::MultiSampleLevel mMSL;
		//�ƹ�
		bool mLightEnable;
		//Z����
		bool mZEnable;
		//ģ��
		bool mStencil;
		//���������ε��޳���ʽ D3DCULL_NONE : ��Ҫ�޳�����, D3DCULL_CW : ����˳ʱ��ķ���Ķ����޳�����, D3DCULL_CCW : ������ʱ�뷽���޳����档
		_D3DCULL mCULL;

		//����ɫ
		D3DCOLOR mClearColor;

	private:
		//���Ӳ���Ƿ�֧�ֵ�ǰ���ö��ز����ȼ�
		D3DMULTISAMPLE_TYPE GetMultiSampleLevel(D3DFORMAT format);

	private:
		LPDIRECT3D9 mDirect3D;
		LPDIRECT3DDEVICE9 mDevice;
	};
}

