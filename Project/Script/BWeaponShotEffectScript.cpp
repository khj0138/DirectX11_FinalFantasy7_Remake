#include "pch.h"

#include "BWeaponShotEffectScript.h"
namespace ff7r
{
	BWeaponShotEffectScript::BWeaponShotEffectScript()
		: Script(SCRIPT_TYPE::BWEAPONSHOTEFFECTSCRIPT)
	{
	}

	BWeaponShotEffectScript::~BWeaponShotEffectScript()
	{
	}

	void BWeaponShotEffectScript::Awake()
	{
		GetOwner()->SetName(L"Shot Effect");

		//GetParticleSystem()->SetCreateParticleCnt(150);

		ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::CUBE;
		module_data.spawn_rate = 1300.f;
		//module_data.shape_scale = vec3(0.01f, Degree2Rad(80.f), 5.0f);
		module_data.particle_cnt_max = 200;
		module_data.life_time_min = 0.5f;
		module_data.life_time_max = 1.0f;
		module_data.shape_scale.x = 1.0f;
		module_data.shape_scale.y = 1.0f;
		
		module_data.spawn_scale_min = vec3(0.04f, 0.04f, 0.03f);
		module_data.spawn_scale_max = vec3(0.05f, 0.05f, 0.03f);

		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
		module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
		module_data.start_scale = 0.8f;
		module_data.end_scale = 0.0f;

		module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		module_data.velocity_alignment = true;
		module_data.velocity_scale = true;
		module_data.velocity_scale_max = vec3(2.0f, 4.0f, 2.0f);
		module_data.speed_max = 1.0f;

		module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = false;
		module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::ROTATED_CENTER;
		module_data.speed = 10.0f;
		module_data.spawn_color = vec3(255.f / 255.f, 100.f / 255.f, 10.f / 255.f);
		module_data.space = 1;

		GetParticleSystem()->SetIntensity(5.0f);



		//ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		////GetParticleSystem()->SetCreateParticleCnt(300);

		//module_data.particle_cnt_max = 3000;

		//module_data.module_check[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
		//module_data.spawn_rate = 30;
		//module_data.spawn_color = vec3(1.0f, 0.7f, 0.2f);
		//module_data.spawn_scale_min = vec3(0.05f, 0.01f, 0.01f);
		//module_data.spawn_scale_max = vec3(0.05f, 0.01f, 0.01f);

		////module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::MESH;
		//module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::POINT;
		////module_data.shape_scale = vec3(0.01f, 3.141592 / 4.0f, 8.0f);
		////module_data.shape_scale = vec3(5.f, 1.f, 1.0f);

		//module_data.space = 1; // ½Ã¹Ä·¹ÀÌ¼Ç ÁÂÇ¥°è

		//module_data.life_time_min = 10.f;
		//module_data.life_time_max = 10.51f;

		//module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
		//module_data.start_scale = 1.0f;
		//module_data.end_scale = 0.0f;

		//module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
		//module_data.start_color = vec3(1.0f, 0.0f, 0.0f);
		//module_data.end_color = vec3(0.5f, 0.5f, 0.0f);

		//module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		////module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::CUSTOM_2;
		//module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::CIRCLE;
		//module_data.speed = 2.f;
		////module_data.velocity_dir = vec3(0.f, 1.f, 0.f);
		//module_data.offset_angle = 0.f;
		////module_data.speed_max = 
		//module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = false;
		//module_data.start_drag = 50.f;
		//module_data.end_drag = -50.f;

		//module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
		//module_data.noise_term = 0.5f;
		//module_data.noise_force = 100.f;

		//module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		//module_data.velocity_alignment = true;
		//module_data.velocity_scale = false;
		//module_data.velocity_scale_max = vec3(5.f, 1.f, 1.f);

		//vec3 _dir = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		//module_data.velocity_dir = vec4(_dir.x, _dir.y, _dir.z, 1.0f);
		//module_data.speed_max = 20.f;
		//GetParticleSystem()->SetIntensity(5.0f);
		////module_data.velocity_dir = _dir.Normalize();
		//time = 0.0f;
	}

	void BWeaponShotEffectScript::Tick()
	{
		//time = fmodf(time + DT, 12.0f);
		//ParticleModule& module_data = GetParticleSystem()->GetModuleData();
		//module_data.offset_angle = (time / 12.0f * XM_PI * 2);
		//module_data.velocity_padding = time / 4.0f;
		time += DT;
		if (time >= 20.0f)
		{
			time = 0.0f;
			GetOwner()->SetActive(false);
		}
	}

	void BWeaponShotEffectScript::SetDir(vec3 _dir)
	{
		vec3 _origin = vec3(1.0f, 0.0f, 0.0f);

		vec3 _rot = CalcEulerAngleFromVectors(_origin, _dir);
		GetTransform()->SetRelativeRot(_rot);
	}
}