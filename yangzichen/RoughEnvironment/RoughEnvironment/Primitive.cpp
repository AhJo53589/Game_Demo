#include "Primitive.h"

namespace pool
{
	namespace Primitive
	{
		bool DrawSphere(LPDIRECT3DDEVICE9 pDevice, float radius, unsigned int slices, unsigned int stacks, LPD3DXMESH *ppMesh, bool ccw)
		{
			if (pDevice == NULL || radius <= 0 )
				return false;

			if (slices < 1)
				slices = 1;

			if (stacks < 4)
				stacks = 4;

			/*
				创建mesh
				NumFaces : mesh将拥有的面数。该值必须大于0
				NumVertices : mesh将拥有的顶点数。该值必须大于0
				Options : 用来创建mesh的一个或多个创建标志、以下为常用部分
					D3DXMESH_32BIT — mesh使用32位索引。
					D3DXMESH_MANAGED — mesh数据将被放在受控的内存中。
					D3DXMESH_WRITEONLY — mesh数据只能执行写操作，不能执行读操作。
					D3DXMESH_DYNAMIC — mesh缓存将是动态的。
				FVF : mesh的顶点格式
				pDevice : 与mesh相关的设备
			*/
			if (FAILED(D3DXCreateMeshFVF(stacks * slices * 2, (stacks+1) * (slices+2) - 2, D3DXMESH_MANAGED, DxCommon::Vertex_TEX1::FVF, pDevice, ppMesh)))
				return false;

			DxCommon::Vertex_TEX1 *vertex = NULL;

			(*ppMesh)->LockVertexBuffer(0, (void**)&vertex);

			const float wTex(1.0f / stacks), hTex(2.0f / (slices + 1));
			for (int n=0 ; n!=stacks ; ++n)
			{
				vertex[n] = DxCommon::Vertex_TEX1(0, radius, 0, 0 , -radius, 0, (n+ 0.5f) * wTex, 0);
			}

			for (int n=0 ; n!=stacks ; ++n)
			{
				vertex[stacks + n] = DxCommon::Vertex_TEX1(0, -radius, 0, 0 , radius, 0, (n + 0.5f) * wTex, (slices + 1) * hTex);
			}

			const float a(2 * DxCommon::Pi / stacks), b(DxCommon::Pi / (slices +1));

			for (int m=0 ; m !=slices ; ++m)
			{
				for (int n=0 ; n!=(stacks + 1) ; ++n)
				{
					vertex[2*stacks + m*(stacks + 1) + n] = DxCommon::Vertex_TEX1(
						cos(a*n)*cos(DxCommon::Pi/2 - (m+1)*b) * radius, 
						sin(DxCommon::Pi/2 - (m+1)*b) * radius,
						sin(a*n)*cos(DxCommon::Pi/2 - (m+1)*b) * radius, 
						- cos(a*n)*cos(DxCommon::Pi/2 - (m+1)*b) * radius, 
						- sin(DxCommon::Pi/2 - (m+1)*b) * radius,
						- sin(a*n)*cos(DxCommon::Pi/2 - (m+1)*b) * radius, 
						n * wTex, (m+1) * hTex);
				}
			}

			(*ppMesh)->UnlockVertexBuffer();

			unsigned short *index = NULL;

			(*ppMesh)->LockIndexBuffer(0, (void **)&index);

			int key = 0;
			for (int m=0 ; m !=(slices - 1) ; ++m)
			{
				for (int n=0 ; n!=stacks ; ++n)
				{
						index[key] = 2*stacks + m*(stacks + 1) + n;
						++key;
						index[key] = 2*stacks + (m + 1)*(stacks + 1) + n;
						++key;
						index[key] = 2*stacks + m*(stacks + 1) + (n + 1);
						++key;
						index[key] = 2*stacks + m*(stacks + 1) + (n + 1);
						++key;
						index[key] = 2*stacks + (m + 1)*(stacks + 1) + n;
						++key;
						index[key] = 2*stacks + (m + 1)*(stacks + 1) + (n + 1);
						++key;
				}
			}

			for (int n=0 ; n!=stacks ; ++n)
			{
				index[key] = n;
				++key;
				index[key] = 2*stacks + n;
				++key;
				index[key] = 2*stacks + (n + 1);
				++key;
			}

			for (int n=0 ; n!=stacks ; ++n)
			{
				index[key] = 2*stacks + (slices - 1)*(stacks + 1) + n;
				++key;
				index[key] = stacks + n;
				++key;
				index[key] = 2*stacks + (slices - 1)*(stacks + 1) + (n + 1);
				++key;
			}

			(*ppMesh)->UnlockIndexBuffer();

			unsigned long *attribute = NULL;

			(*ppMesh)->LockAttributeBuffer(0, &attribute);

			for (int n=0 ; n!=stacks*slices*2 ; ++n)
				attribute[n] = 0;

			(*ppMesh)->UnlockAttributeBuffer();

			return true;
		}
	}
}

