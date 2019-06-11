#pragma once

#include "define.h"
#include "GameInterface.h"
#include "CameraInterface.h"
#include "InputInterface.h"
#include "NodeInterface.h"

namespace pool
{
	class CRendererInterface
	{
	public: 
		CRendererInterface() : mGame(NULL)
		{
		}

		~CRendererInterface()
		{
		}

		//��ʼ����Ⱦ 
		virtual bool Initialize(PGAME game) = 0;
		
		//��Ӧ�����¡�����
		virtual void Response(DWORD time, PNODE node, PINPUT input, PCAMERA camera) = 0;
		virtual void Update(PNODE node, DWORD time, PCAMERA camera) = 0;
		virtual void BeforeDraw(PNODE node, PCAMERA camera, PNODE rootNode) = 0;
		virtual void Draw(PNODE node, PCAMERA camera) = 0;

	protected:
		PGAME mGame;

	};

	typedef CRendererInterface *PRENDERER;
}