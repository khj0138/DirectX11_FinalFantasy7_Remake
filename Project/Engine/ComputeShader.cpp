#include "pch.h"
#include "ComputeShader.h"

#include "PathMgr.h"
#include "Device.h"
#include "ConstBuffer.h"


namespace ff7r
{
	ComputeShader::ComputeShader()
		: Shader(RES_TYPE::COMPUTE_SHADER)
		, group_x_cnt(1)
		, group_y_cnt(1)
		, group_z_cnt(1)
	{
	}

	ComputeShader::~ComputeShader()
	{
	}

	void ComputeShader::CreateComputeShader(const wstring& _strFileName, const string& _strFuncName)
	{
		// Shader 파일 경로
		wstring strShaderFile = PathMgr::GetInst()->GetContentPath();
		strShaderFile += _strFileName;

		// Shader Compile	
		if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "cs_5_0", 0, 0, compute_shader_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Compute Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 Shader 를 만든다.
		DEVICE->CreateComputeShader(compute_shader_blob->GetBufferPointer(), compute_shader_blob->GetBufferSize()
			, nullptr, compute_shader.GetAddressOf());
	}

	void ComputeShader::Execute()
	{
		UpdateData();

		static ConstBuffer* pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
		pCB->SetData(&mtrl_const_data);
		pCB->UpdateData_CS();

		CONTEXT->CSSetShader(compute_shader.Get(), nullptr, 0);
		CONTEXT->Dispatch(group_x_cnt, group_y_cnt, group_z_cnt);

		Clear();
	}
}