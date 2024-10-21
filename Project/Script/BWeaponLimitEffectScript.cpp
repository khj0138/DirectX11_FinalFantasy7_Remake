#include "pch.h"

#include "BWeaponLimitEffectScript.h"
namespace ff7r
{
	BWeaponLimitEffectScript::BWeaponLimitEffectScript()
		: Script(SCRIPT_TYPE::BWEAPONLIMITEFFECTSCRIPT)
		, is_init(true)
	{
	}

	BWeaponLimitEffectScript::~BWeaponLimitEffectScript()
	{
	}

	void BWeaponLimitEffectScript::Awake()
	{
		ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		//GetParticleSystem()->SetCreateParticleCnt(300);

		module_data.particle_cnt_max = 3000;

		module_data.module_check[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
		module_data.spawn_rate = 90;
		module_data.spawn_color = vec3(1.0f, 0.7f, 0.2f);
		module_data.spawn_scale_min = vec3(0.08f, 0.02f, 0.02f);
		module_data.spawn_scale_max = vec3(0.08f, 0.02f, 0.02f);

		//module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::MESH;
		module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::POINT;
		//module_data.shape_scale = vec3(0.01f, 3.141592 / 4.0f, 8.0f);
		//module_data.shape_scale = vec3(5.f, 1.f, 1.0f);

		module_data.space = 1; // ½Ã¹Ä·¹ÀÌ¼Ç ÁÂÇ¥°è

		module_data.life_time_min = 2.f;
		module_data.life_time_max = 2.51f;

		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
		module_data.start_scale = 1.0f;
		module_data.end_scale = 0.0f;

		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
		module_data.start_color = vec3(1.0f, 0.6f, 0.0f);
		module_data.end_color = vec3(1.0f, 0.3f, 0.0f);

		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		//module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::CUSTOM_2;
		module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::CIRCLE;
		module_data.speed = 2.f;
		//module_data.velocity_dir = vec3(0.f, 1.f, 0.f);
		module_data.offset_angle = 0.f;
		//module_data.speed_max = 
		module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = false;
		module_data.start_drag = 50.f;
		module_data.end_drag = -50.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
		module_data.noise_term = 0.5f;
		module_data.noise_force = 100.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		module_data.velocity_alignment = true;
		module_data.velocity_scale = false;
		module_data.velocity_scale_max = vec3(5.f, 1.f, 1.f);

		vec3 _dir = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		module_data.velocity_dir = vec4(_dir.x, _dir.y, _dir.z, 1.0f);
		module_data.speed_max = 20.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::INIT] = true;
		GetParticleSystem()->SetIntensity(10.0f);
		//module_data.velocity_dir = _dir.Normalize();
		time = 0.0f;
		is_init = true;
	}

	void BWeaponLimitEffectScript::Tick()
	{
		time += DT;
		if (time >= 2.05f)
		{
			GetOwner()->SetActive(false);
		}

		if (is_init)
		{
			is_init = false;
		}
		else
		{
			ParticleModule& module_data = GetParticleSystem()->GetModuleData();
			module_data.module_check[(UINT)PARTICLE_MODULE::INIT] = false;
		}
		//time = fmodf(time + DT, 12.0f);
		//ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		//module_data.offset_angle = (time / 12.0f * XM_PI * 2);
		//module_data.velocity_padding = time / 4.0f;

	}

	void BWeaponLimitEffectScript::OnEnable()
	{
		Awake();
	}

	void BWeaponLimitEffectScript::SetDir(vec3 _dir)
	{
		vec3 _origin = vec3(1.0f, 0.0f, 0.0f);

		vec3 _rot = CalcEulerAngleFromVectors(_origin, _dir);
		GetTransform()->SetRelativeRot(_rot);
	}
}