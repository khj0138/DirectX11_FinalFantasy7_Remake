#include "pch.h"

#include "EffectScript.h"
#include "HealEffectScript.h"
namespace ff7r
{
	HealEffectScript::HealEffectScript()
		: Script((UINT)SCRIPT_TYPE::HEALEFFECTSCRIPT)
		, effect(nullptr)
		, time(0.0f)
	{
	}

	HealEffectScript::~HealEffectScript()
	{
	}

	void HealEffectScript::Awake()
	{
		GetParticleSystem()->SetCreateParticleCnt(100);
		GetParticleSystem()->SetSpawnMesh(
			ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0003_Aerith\\Mesh\\FX_PC0003_Wind_01_M.mesh"));
		ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::MESH;
		module_data.spawn_rate = 50.f;
		module_data.particle_cnt_max = 100;
		module_data.life_time_min = 1.0f;
		module_data.life_time_max = 1.0f;
		module_data.spawn_scale_min = vec3(0.05f, 0.05f, 0.05f);
		module_data.spawn_scale_max = vec3(0.05f, 0.05f, 0.05f);

		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
		module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
		module_data.start_scale = 1.0f;

		module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		module_data.velocity_alignment = false;
		module_data.velocity_scale = false;

		module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = false;
		module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::CUSTOM_3;
		module_data.spawn_color = vec3(129.f / 255.f, 245.f / 255.f, 196.f / 255.f);
		module_data.space = 0;

		GetParticleSystem()->SetIntensity(5.0f);
	}

	void HealEffectScript::Tick()
	{
		time += DT;

		if (time >= 3.2f)
		{
			GetOwner()->SetActive(false);
		}
	}

	void HealEffectScript::OnEnable()
	{
		time = 0.0f;
	}
}