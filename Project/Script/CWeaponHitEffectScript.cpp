#include "pch.h"

#include "CWeaponHitEffectScript.h"
namespace ff7r
{
	CWeaponHitEffectScript::CWeaponHitEffectScript()
		: Script(SCRIPT_TYPE::CWEAPONHITEFFECTSCRIPT)
	{
	}

	CWeaponHitEffectScript::~CWeaponHitEffectScript()
	{
	}

	void CWeaponHitEffectScript::Awake()	
	{
		GetOwner()->SetName(L"Hit Effect");

		GetParticleSystem()->SetCreateParticleCnt(150);

		ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::POINT;
		module_data.spawn_rate = 1300.f;
		//module_data.shape_scale = vec3(0.01f, Degree2Rad(80.f), 5.0f);
		module_data.particle_cnt_max = 150;
		module_data.life_time_min = 1.5f;
		module_data.life_time_max = 3.0f;
		module_data.spawn_scale_min = vec3(0.05f, 0.05f, 0.05f);
		module_data.spawn_scale_max = vec3(0.3f, 0.05f, 0.05f);

		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
		module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
		module_data.start_scale = 1.0f;
		module_data.end_scale = 0.15f;
			
		module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		module_data.velocity_alignment = true;
		module_data.velocity_scale = true;
		module_data.velocity_scale_max = vec3(2.f, 1.f, 1.f);
		module_data.speed_max = 3.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = true;
		module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::CUSTOM_1;
		module_data.speed = 5.5f;
		module_data.spawn_color = vec3(235.f / 255.f, 47.f / 255.f, 27.f / 255.f);
		module_data.space = 0;

		GetParticleSystem()->SetIntensity(5.0f);
		//module_data.velocity_dir = _dir.Normalize();
	}

	void CWeaponHitEffectScript::Tick()
	{
		time += DT;

		if (time > 3.0f)
		{
			GetOwner()->SetActive(false);
		}
	}

	void CWeaponHitEffectScript::OnEnable()
	{
		time = 0.0f;
	}

	void CWeaponHitEffectScript::SetDir(vec3 _dir)
	{
		vec3 _origin = vec3(1.0f, 0.0f, 0.0f);

		vec3 _rot = CalcEulerAngleFromVectors(_origin, _dir);
		GetTransform()->SetRelativeRot(_rot);
	}
}