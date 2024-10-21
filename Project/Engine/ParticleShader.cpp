#include "pch.h"

#include "StructuredBuffer.h"

#include "ParticleShader.h"
namespace ff7r
{
	ParticleShader::ParticleShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group)
		: particle_buff(nullptr)
		, module_data_buff(nullptr)
		, rw_buff(nullptr)
	{
		thread_x_per_group = _thread_x_per_group;
		thread_y_per_group = _thread_y_per_group;
		thread_z_per_group = _thread_z_per_group;
	}

	ParticleShader::~ParticleShader()
	{

	}

	void ParticleShader::SetParticleBuffer(StructuredBuffer* _buffer)
	{
		particle_buff = _buffer;
		mtrl_const_data.arr_int[0] = particle_buff->GetElementCount();
	}

	void ParticleShader::UpdateData()
	{
		particle_buff->UpdateDataCS(0, false);
		rw_buff->UpdateDataCS(1, false);
		module_data_buff->UpdateDataCS(20, true);
		noise_tex->UpdateDataCS(21, true);

		// ±×·ì ¼ö
		group_x_cnt = (particle_buff->GetElementCount() / thread_x_per_group) + 1;
	}

	void ParticleShader::Clear()
	{
		particle_buff->ClearCS(false);
		rw_buff->ClearCS(false);
		module_data_buff->ClearCS(true);
		noise_tex->ClearCS(true);
	}

	void ParticleShader::SetNoiseTexture(Ptr<Texture> _tex)
	{
		noise_tex = _tex;
		mtrl_const_data.arr_vec2[0] = vec2(noise_tex->Width(), noise_tex->Height());
	}

}