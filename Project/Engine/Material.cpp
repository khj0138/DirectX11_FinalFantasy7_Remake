#include "pch.h"
#include "Material.h"

#include "Device.h"
#include "ConstBuffer.h"

#include "ResourceMgr.h"
#include "PathMgr.h"

namespace ff7r
{
	Material::Material(bool _is_engine_res)
		: Resource(RES_TYPE::MATERIAL, _is_engine_res)
		, const_data{}
		, textures{}
	{
	}

	Material::~Material()
	{
	}

	void Material::UpdateData()
	{
		if (nullptr == shader)
			return;

		shader->UpdateData();


		// Texture Update
		for (UINT i = 0; i < TEX_END; ++i)
		{
			if (nullptr == textures[i])
			{
				const_data.arr_tex[i] = 0;
				Texture::Clear(i);
				continue;
			}
			else
			{
				const_data.arr_tex[i] = 1;
				textures[i]->UpdateData(i, PIPELINE_STAGE::PS_ALL);
			}
		}

		// Constant Update
		ConstBuffer* pMtrlBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
		pMtrlBuffer->SetData(&const_data);
		pMtrlBuffer->UpdateData();
	}

	void Material::UpdateDataInst()
	{
		if (nullptr == shader)
			return;

		shader->UpdateDataInst();


		// Texture Update
		for (UINT i = 0; i < TEX_END; ++i)
		{
			if (nullptr == textures[i])
			{
				const_data.arr_tex[i] = 0;
				Texture::Clear(i);
				continue;
			}
			else
			{
				const_data.arr_tex[i] = 1;
				textures[i]->UpdateData(i, PIPELINE_STAGE::PS_ALL);
			}
		}

		// Constant Update
		ConstBuffer* pMtrlBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
		pMtrlBuffer->SetData(&const_data);
		pMtrlBuffer->UpdateData();
	}

	void Material::SetScalarParam(SCALAR_PARAM _Param, const void* _Src)
	{
		switch (_Param)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
			const_data.arr_int[_Param] = *((int*)_Src);
			break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
			const_data.arr_float[_Param - FLOAT_0] = *((float*)_Src);
			break;

		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
			const_data.arr_vec2[_Param - VEC2_0] = *((vec2*)_Src);
			break;

		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
			const_data.arr_vec4[_Param - VEC4_0] = *((vec4*)_Src);
			break;

		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
			const_data.arr_mat[_Param - MAT_0] = *((Matrix*)_Src);
			break;
		}
	}

	void Material::SetTexParam(TEX_PARAM _Param, const Ptr<Texture>& _Tex)
	{
		textures[_Param] = _Tex;
	}

	void Material::GetScalarParam(SCALAR_PARAM _param, void* _pData)
	{
		switch (_param)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int idx = (UINT)_param - (UINT)INT_0;
			*((int*)_pData) = const_data.arr_int[idx];
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			int idx = (UINT)_param - (UINT)FLOAT_0;
			*((float*)_pData) = const_data.arr_float[idx];
		}
		break;

		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			int idx = (UINT)_param - (UINT)VEC2_0;
			*((vec2*)_pData) = const_data.arr_vec2[idx];
		}
		break;

		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			int idx = (UINT)_param - (UINT)VEC4_0;
			*((vec4*)_pData) = const_data.arr_vec4[idx];
		}
		break;

		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{
			int idx = (UINT)_param - (UINT)MAT_0;
			*((Matrix*)_pData) = const_data.arr_mat[idx];
		}
		break;
		}
	}

	// ================
	// File Save / Load
	// ================
	int Material::Save(const wstring& _strRelativePath)
	{
		if (IsEngineRes())
			return E_FAIL;

		wstring strFilePath = PathMgr::GetInst()->GetContentPath();
		strFilePath += _strRelativePath;

		path dirPath = path(strFilePath).parent_path();
		create_directories(dirPath);

		FILE* pFile = nullptr;
		_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

		// Entity
		SaveWString(GetName(), pFile);

		// Res
		SaveWString(GetKey(), pFile);

		// Shader
		SaveResRef(shader.Get(), pFile);

		// Constant
		fwrite(&const_data, sizeof(MtrlConst), 1, pFile);

		// Texture
		for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
		{
			SaveResRef(textures[i].Get(), pFile);
		}

		fclose(pFile);

		return S_OK;
	}


	int Material::Load(const wstring& _strFilePath)
	{
		FILE* pFile = nullptr;
		_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

		// Entity
		wstring strName;
		LoadWString(strName, pFile);
		SetName(strName);

		// Res
		wstring strKey;
		LoadWString(strKey, pFile);

		// Shader
		LoadResRef(shader, pFile);

		// Constant
		fread(&const_data, sizeof(MtrlConst), 1, pFile);

		// Texture
		for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
		{
			LoadResRef(textures[i], pFile);
		}

		fclose(pFile);

		return S_OK;
	}
}