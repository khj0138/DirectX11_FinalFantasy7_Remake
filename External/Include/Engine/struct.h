#pragma once



struct Vertex
{
	vec3 pos;
	vec4 color;
	vec2 uv;

	vec3 normal;
	vec3 tangent;
	vec3 binormal;

	vec4 weights;
	vec4 indices;
};

typedef Vertex Vtx;


// Event
struct Event
{
	EVENT_TYPE	type;
	DWORD_PTR	w_param;
	DWORD_PTR	l_param;
};


struct DebugShapeInfo
{
	SHAPE_TYPE	shape;
	Matrix		mat_world;
	vec3		world_pos;
	vec3		world_scale;
	vec3		world_rotation;
	vec4		color;
	float		max_time;
	float		cur_time;
	bool		depth_test;
};

const enum COLLISION_ACTOR_TYPE {
	COLLISION_ACTOR_STATIC,
	COLLISION_ACTOR_DYNAMIC,
	COLLISION_ACTOR_KINEMATIC,
	COLLISION_ACTOR_CCT
};

struct LightInfo
{
	vec4		color;
	vec4		world_pos;
	vec4		world_dir;

	UINT		light_type;

	float		intensity;
	float		attenuation_radius;
	float		angle;
};

struct CaptureInfo
{
	vec4		world_pos;
	float		radius;
	UINT		level;
	vec2		padding;
};

// TileMap
struct Tile
{
	vec2 left_top;
	vec2 slice;
};

// Animator2D
struct Anim2DFrame
{
	vec2	left_top_uv;
	vec2	slice_uv;
	vec2	offset;
	float	duration;
};

// ============
// Animation 3D
// ============
struct FrameTrans
{
	vec4	translate;
	vec4	scale;
	vec4	rotation;
};

struct MTJoint
{
	wstring				name;
	int					depth;
	int					parent_id;
	Matrix				mat_inverse;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix				mat_bone;
};

// Particle
struct Particle
{
	vec4	local_pos;		// 오브젝트로부터 떨어진 거리
	vec4	world_pos;		// 파티클 최종 월드위치
	vec4	world_scale;	// 파티클 크기	
	vec4	color;			// 파티클 색상
	vec4	velcotiy;		// 파티클 현재 속도
	vec4	force;			// 파티클에 주어진 힘
	vec4	random_force;	// 파티클에 적용되는 랜덤방향 힘

	float   age;			// 생존 시간
	float   prev_age;		// 이전 프레임 생존 시간
	float   normalized_age;	// 수명대비 생존시간을 0~1로 정규화 한 값
	float	life_time;		// 수명
	float	mass;			// 질량
	float   scale_factor;	// 추가 크기 배율

	int     active;			// 파티클 활성화 여부
	int     padding;
};

struct RWParticleBuffer
{
	int		spawn_cnt;			// 스폰 시킬 파티클 개수
	int		total_spawn_cnt;
	int		vtx_cnt;
	float	ground_y;
};



struct ParticleModule
{
	// 스폰 모듈
	PARTICLE_SPAWN_SHAPE_TYPE spawn_shape_type;

	vec4    spawn_color;
	vec4	spawn_scale_min;
	vec4	spawn_scale_max;

	vec3	shape_scale;
	int		spawn_rate;			// 초당 생성 개수
	int		space;				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)
	float   life_time_min;		// 최소 수명
	float   life_time_max;		// 최대 수명

	// Color Change 모듈
	vec4	start_color;		// 초기 색상
	vec4	end_color;			// 최종 색상

	// Scale Change 모듈
	float	start_scale;			// 초기 배율
	float	end_scale;			// 최종 배율	

	// 버퍼 최대크기
	int		particle_cnt_max;
	int		particle_cnt;

	// Add Velocity 모듈
	PARTICLE_ADD_VEL_TYPE     add_velocity_type;
	vec4	velocity_dir;
	float	offset_angle;
	float	speed;
	float     velocity_padding;

	// Drag 모듈 - 속도 제한
	float	start_drag;
	float	end_drag;

	// NoiseForce 모듈 - 랜덤 힘 적용	
	float	noise_term;		// 랜덤 힘 변경 간격
	float	noise_force;	// 랜덤 힘 크기

	// Render 모듈
	int		velocity_alignment;	// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
	int		velocity_scale;		// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
	float   speed_max;			// 최대 크기에 도달하는 속력
	vec4	velocity_scale_max;	// 속력에 따른 크기 변화량 최대치
	int		render_padding;

	// Module Check
	int		module_check[(UINT)PARTICLE_MODULE::END];
};



// ===================
// 상수버퍼 대응 구조체
// ===================
struct TransformConst
{
	Matrix mat_world;
	Matrix mat_world_inv;
	Matrix mat_view;
	Matrix mat_view_inv;
	Matrix mat_proj;
	Matrix mat_proj_inv;

	Matrix mat_wv;
	Matrix mat_wvp;
};

extern TransformConst g_transform_data;


struct MtrlFactor
{
	vec4 color_factor;
	vec3 emissive_factor;

	float metalic_factor;
	float roughness_factor;

	// material에 아래의 정보가 들어가 있긴 한데 사용처를 아직 모름
	// float alpha_cut_off
	// float emissive_strength
};

struct MtrlConst
{
	MtrlFactor	factor;
	int			factor_padding[3];

	int			arr_int[4];
	int		arr_bool[4];
	//int temp[3];
	float		arr_float[4];
	vec2		arr_vec2[4];
	vec4		arr_vec4[4];
	Matrix		arr_mat[4];

	int			arr_tex[(UINT)TEX_PARAM::TEX_END];

	int			arr_anim[4];
};

struct CaptureConst
{
	UINT object_cnt;
	UINT min_level;
	UINT max_level;

	int padding;
};


struct GlobalConst
{
	vec2	resolution;
	float	delta_time;
	float	acc_time; // accumulated_time

	vec3	main_camera_pos;
	vec3	ambient_color;
	float	ambient_intensity;

	int	light_cnt;
};

extern GlobalConst g_global_data;

struct Font
{
	vec3	text_uv;
	vec2	text_size;
};

struct GlobalFont
{
	Font fonts[63];
	vec2 padding;
};

struct DamageFont
{
	Font fonts[10];
	vec2 padding;
};
struct Texts
{
	// 문장 개수 달라지면 바꿔야함
	int		sentence[17];
	vec2	font_size;
	int		first = 0;
	int		is_text = 1;
};


union InstID
{
	struct 
	{
		UINT mesh;
		WORD mtrl;
		WORD mtrl_idx;
	};

	ULONG64 llID;
};

namespace ff7r
{
	class GameObject;
	struct InstObj
	{
		GameObject* object;
		UINT					mtrl_idx;
	};
}
struct InstancingData
{
	Matrix mat_world;
	Matrix mat_wv;
	Matrix mat_wvp;

	int    row_idx;
};

//struct LightInfoConst	
//{
//	vec3	ambient_color;
//	float	ambient_intensity;
//
//	UINT	directional_light_cnt;
//	UINT	point_light_cnt;
//	UINT	spot_light_cnt;
//
//	int		padding;
//};
//
//extern LightInfoConst g_light_data;
