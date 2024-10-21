#include "pch.h"
#include "ParticleSystem.h"

#include "Device.h"
#include "StructuredBuffer.h"

#include "ResourceMgr.h"
#include "Transform.h"
#include "TimeMgr.h"

// =========== Test Header =============
#include "KeyMgr.h"

namespace ff7r
{
	ParticleSystem::ParticleSystem()
		: RenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
		, particle_buff(nullptr)
		, mesh_buff(nullptr)
		, rw_buff(nullptr)
		, module_data{}
		, acc_time(0.f)
		, intensity(1.0f)
		, is_first_frame(true)
		, create_particle_cnt(0)
	{
		rwbuffer.total_spawn_cnt = 0;
		rwbuffer.spawn_cnt = 0;
		rwbuffer.vtx_cnt = 0;
		rwbuffer.ground_y = 0.0f;

		module_data.particle_cnt_max = 400;

		module_data.life_time_min = 2.5f;
		module_data.life_time_max = 2.8f;

		module_data.module_check[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
		module_data.spawn_rate = 200;
		module_data.spawn_color = vec3(1.0f, 0.7f, 0.2f);
		module_data.spawn_scale_min = vec3(0.05f, 0.05f, 0.05f);
		module_data.spawn_scale_max = vec3(0.05f, 0.05f, 0.05f);

		module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::MESH;
		//module_data.shape_scale = vec3(0.01f, 3.141592 / 4.0f, 8.0f);
		module_data.shape_scale = vec3(10.f, 10.f, 1.0f);

		module_data.space = 1; // 시뮬레이션 좌표계

		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
		module_data.start_scale = 1.5f;
		module_data.end_scale = 0.2f;

		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
		module_data.start_color = vec3(1.0f, 0.0f, 0.0f);
		module_data.end_color = vec3(0.5f, 0.5f, 0.0f);

		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
		module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::FROM_CENTER;
		module_data.speed = 1.f;
		//module_data.velocity_dir = vec3(0.f, 1.f, 0.f);
		module_data.offset_angle;
		//module_data.speed_max = 
		module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = false;
		module_data.start_drag = 50.f;
		module_data.end_drag = -50.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
		module_data.noise_term = 0.5f;
		module_data.noise_force = 100.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		module_data.velocity_alignment = false;
		module_data.velocity_scale = false;
		module_data.velocity_scale_max = vec3(5.f, 1.f, 1.f);
		module_data.speed_max = 2.f;

		// 입자 메쉬
		SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"PointMesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"mesh\\WE0000_02.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"mesh\\FX_Awake01_Cylinder_01_M.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"mesh\\FX_Barrier01_CHex_01_M.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Ability\\Barrier01\\Mesh\\FX_Barrier01_CHex_01_M.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0001_Barret\\Mesh\\FX_PC0001_TwistLine_02_M.mesh"));

		// 파티클 전용 재질
		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"ParticleMtrl"));

		// 파티클 업데이트 컴퓨트 쉐이더	
		particle_shader = (ParticleShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(L"ParticleCS").Get();

		particle_buff = new StructuredBuffer;
		particle_buff->Create(sizeof(Particle), module_data.particle_cnt_max, SB_TYPE::READ_WRITE, false);

		// 파티클 스폰 개수 전달용 버퍼
		rw_buff = new StructuredBuffer;
		rw_buff->Create(sizeof(RWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

		// 모듈 활성화 및 모듈 설정 정보 버퍼
		module_buff = new StructuredBuffer;
		module_buff->Create(sizeof(ParticleModule), 1, SB_TYPE::READ_ONLY, true);
	}

	ParticleSystem::ParticleSystem(const ParticleSystem& _origin)
		: RenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
		, particle_buff(nullptr)
		, mesh_buff(nullptr)
		, rw_buff(nullptr)
		, module_data{}
		, acc_time(0.f)
		, intensity(1.0f)
		, is_first_frame(true)
	{
		rwbuffer.total_spawn_cnt = 0;
		rwbuffer.spawn_cnt = 0;
		rwbuffer.vtx_cnt = 0;
		rwbuffer.ground_y = 0.0f;

		module_data.particle_cnt_max = 800;

		module_data.life_time_min = 2.5f;
		module_data.life_time_max = 2.8f;

		module_data.module_check[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
		module_data.spawn_rate = 200;
		module_data.spawn_color = vec3(1.0f, 0.7f, 0.2f);
		module_data.spawn_scale_min = vec3(0.05f, 0.05f, 0.05f);
		module_data.spawn_scale_max = vec3(0.05f, 0.05f, 0.05f);

		module_data.spawn_shape_type = PARTICLE_SPAWN_SHAPE_TYPE::MESH;
		//module_data.shape_scale = vec3(0.01f, 3.141592 / 4.0f, 8.0f);
		module_data.shape_scale = vec3(10.f, 10.f, 1.0f);

		module_data.space = 1; // 시뮬레이션 좌표계

		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
		module_data.start_scale = 1.5f;
		module_data.end_scale = 0.2f;

		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
		module_data.start_color = vec3(1.0f, 0.0f, 0.0f);
		module_data.end_color = vec3(0.5f, 0.5f, 0.0f);

		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
		module_data.add_velocity_type = PARTICLE_ADD_VEL_TYPE::FROM_CENTER;
		module_data.speed = 1.f;
		//module_data.velocity_dir = vec3(0.f, 1.f, 0.f);
		module_data.offset_angle;
		//module_data.speed_max = 
		module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = false;
		module_data.start_drag = 50.f;
		module_data.end_drag = -50.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
		module_data.noise_term = 0.5f;
		module_data.noise_force = 100.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		module_data.velocity_alignment = false;
		module_data.velocity_scale = false;
		module_data.velocity_scale_max = vec3(5.f, 1.f, 1.f);
		module_data.speed_max = 2.f;

		// 입자 메쉬
		SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"PointMesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"mesh\\WE0000_02.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"mesh\\FX_Awake01_Cylinder_01_M.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"mesh\\FX_Barrier01_CHex_01_M.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Ability\\Barrier01\\Mesh\\FX_Barrier01_CHex_01_M.mesh"));
		//SetSpawnMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"User Resource\\GameContents\\Effect\\Character\\Player\\PC0001_Barret\\Mesh\\FX_PC0001_TwistLine_02_M.mesh"));

		// 파티클 전용 재질
		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"ParticleMtrl"));

		// 파티클 업데이트 컴퓨트 쉐이더	
		particle_shader = (ParticleShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(L"ParticleCS").Get();

		particle_buff = new StructuredBuffer;
		particle_buff->Create(sizeof(Particle), module_data.particle_cnt_max, SB_TYPE::READ_WRITE, false);

		// 파티클 스폰 개수 전달용 버퍼
		rw_buff = new StructuredBuffer;
		rw_buff->Create(sizeof(RWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

		// 모듈 활성화 및 모듈 설정 정보 버퍼
		module_buff = new StructuredBuffer;
		module_buff->Create(sizeof(ParticleModule), 1, SB_TYPE::READ_ONLY, true);
	}

	ParticleSystem::~ParticleSystem()
	{
		if (particle_buff)
		{
			delete particle_buff;
			particle_buff = nullptr;
		}

		if (rw_buff)
		{
			delete rw_buff;
			rw_buff = nullptr;
		}

		if (module_buff)
		{
			delete module_buff;
			module_buff = nullptr;
		}

		if (mesh_buff)
		{
			delete mesh_buff;
			mesh_buff = nullptr;
		}
	}

	void ParticleSystem::Start()
	{
	/*	particle_buff = new StructuredBuffer;
		particle_buff->Create(sizeof(Particle), module_data.particle_cnt_max, SB_TYPE::READ_WRITE, false);*/
	}

	void ParticleSystem::FinalTick()
	{
		if (module_data.spawn_shape_type == PARTICLE_SPAWN_SHAPE_TYPE::TRAIL)
		{
			//vec3 _cur_pos = GetTransform()->GetWorldPos();
			vec3 _cur_pos = GetTransform()->GetRelativePos();

			if (is_first_frame)
			{
				prev_pos = _cur_pos;
				is_first_frame = false;
				rwbuffer.spawn_cnt = 0;
				module_data.module_check[(UINT)PARTICLE_MODULE::INIT] = true;
			}
			else
			{
				module_data.module_check[(UINT)PARTICLE_MODULE::INIT] = false;

				float _len = fabs((_cur_pos - prev_pos).Length());
				float _scale = module_data.spawn_scale_min.x * 0.2f;
				if (_len >= _scale)
				{
					vec3 _dir = (_cur_pos - prev_pos).Normalize() * 0.2f;
					module_data.velocity_dir = _dir;

					rwbuffer.spawn_cnt = (int)(_len / _scale);

					//prev_pos = prev_pos + _dir * rwbuffer.spawn_cnt;
					prev_pos = _cur_pos;
					rwbuffer.total_spawn_cnt += rwbuffer.spawn_cnt;
				}
				else
				{
					rwbuffer.spawn_cnt = 0;
				}
			}


			rw_buff->SetData(&rwbuffer);
		}
		else if (module_data.spawn_rate == 0.0f)
		{
			if (is_first_frame)
			{
				rwbuffer.spawn_cnt = module_data.particle_cnt_max;
				is_first_frame = false;
			}
			else
			{
				rwbuffer.spawn_cnt = 0;
			}
			rw_buff->SetData(&rwbuffer);
		}
		else if (create_particle_cnt != 0)
		{
			if (rwbuffer.total_spawn_cnt >= create_particle_cnt)
			{
				rwbuffer.spawn_cnt = 0;
				rw_buff->SetData(&rwbuffer);
			}
			else
			{
				float fTimePerCount = 1.f / (float)module_data.spawn_rate;
				acc_time += DT;

				// 누적시간이 개당 생성시간을 넘어서면
				if (fTimePerCount < acc_time)
				{
					// 초과 배율 ==> 생성 개수
					float fData = acc_time / fTimePerCount;

					// 나머지는 남은 시간
					acc_time = fTimePerCount * (fData - floor(fData));

					// 버퍼에 스폰 카운트 전달
					rwbuffer.total_spawn_cnt += rwbuffer.spawn_cnt;
					rwbuffer.spawn_cnt = (int)fData;
					rwbuffer.ground_y;
					rw_buff->SetData(&rwbuffer);
				}
			}
		}
		else
		{
			float fTimePerCount = 1.f / (float)module_data.spawn_rate;
			acc_time += DT;

			// 누적시간이 개당 생성시간을 넘어서면
			if (fTimePerCount < acc_time)
			{
				 //초과 배율 ==> 생성 개수
				float fData = acc_time / fTimePerCount;

				 //나머지는 남은 시간
				acc_time = fTimePerCount * (fData - floor(fData));

				// 버퍼에 스폰 카운트 전달
				rwbuffer.spawn_cnt = (int)fData;
				rwbuffer.total_spawn_cnt += rwbuffer.spawn_cnt;

				rw_buff->SetData(&rwbuffer);
			}
		}
		


		GetMaterial()->SetScalarParam(FLOAT_0, &intensity);

		// 파티클 업데이트 컴퓨트 쉐이더
		module_buff->SetData(&module_data);

		particle_shader->SetParticleBuffer(particle_buff);
		particle_shader->SetRWParticleBuffer(rw_buff);
		particle_shader->SetModuleData(module_buff);
		particle_shader->SetNoiseTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\noise\\noise_03.jpg"));
		//particle_shader->SetNoiseTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\noise\\noise_02.png"));
		particle_shader->SetObjectPos(GetTransform()->GetWorldPos());

		vec3 _rot = GetTransform()->GetRelativeRot();

		Matrix _rot_mat = XMMatrixIdentity();
		_rot_mat = XMMatrixRotationX(-_rot.x);
		_rot_mat *= XMMatrixRotationY(-_rot.y);
		_rot_mat *= XMMatrixRotationZ(-_rot.z);

		particle_shader->SetObjectRotation(_rot_mat);

		if (mesh_buff)
		{
			mesh_buff->UpdateDataCS(22, true);

		}
		particle_shader->Execute();
	}

	void ParticleSystem::Render()
	{
		GetTransform()->UpdateData();

		// 파티클버퍼 t20 에 바인딩
		particle_buff->UpdateData(20, PIPELINE_STAGE::PS_ALL);

		// 모듈 데이터 t21 에 바인딩
		module_buff->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);


		// Particle Render	
		Ptr<Texture> pParticleTex = ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\particle\\AlphaCircle.png");
		GetMaterial()->SetTexParam(TEX_0, pParticleTex);

	//Ptr<Texture> pParticleTex2 = ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\FF0000FF.png");
	//GetMaterial()->SetTexParam(TEX_1, pParticleTex2);

		GetMaterial()->UpdateData();
		GetMesh()->RenderParticle(module_data.particle_cnt_max);

		// 파티클 버퍼 바인딩 해제
		particle_buff->Clear();
		module_buff->Clear();
	}

	void ParticleSystem::OnEnable()
	{
		is_first_frame = true;
		rwbuffer.total_spawn_cnt = 0.0f;
		rwbuffer.spawn_cnt = 0.0f;
	}

	void ParticleSystem::SaveToLevelFile(FILE* _File)
	{
		RenderComponent::SaveToLevelFile(_File);

		fwrite(&module_data, sizeof(ParticleModule), 1, _File);
		SaveResRef(particle_shader.Get(), _File);
	}

	void ParticleSystem::LoadFromLevelFile(FILE* _File)
	{
		RenderComponent::LoadFromLevelFile(_File);

		fread(&module_data, sizeof(ParticleModule), 1, _File);

		int i = 0;
		fread(&i, sizeof(i), 1, _File);

		if (i)
		{
			wstring strKey, strRelativePath;
			LoadWString(strKey, _File);
			LoadWString(strRelativePath, _File);

			particle_shader = (ParticleShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(strKey).Get();
		}
	}
	void ParticleSystem::SetSpawnMesh(Ptr<Mesh> _mesh)
	{
		auto& subset = _mesh->GetSubSet();

		
		mesh_buff = new StructuredBuffer;
		mesh_buff->Create(sizeof(Vtx), subset[0].vertex_cnt, SB_TYPE::READ_WRITE, true);
		mesh_buff->SetData(subset[0].vertex_sys_mem, 0);

		rwbuffer.vtx_cnt = (int)subset[0].vertex_cnt;
		
		mesh_buff->UpdateDataCS(22, true);
	}
}