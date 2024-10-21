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
	Matrix				mat_inverse;	// Offset ���(�� -> ��Ʈ ������ ���)
	Matrix				mat_bone;
};

// Particle
struct Particle
{
	vec4	local_pos;		// ������Ʈ�κ��� ������ �Ÿ�
	vec4	world_pos;		// ��ƼŬ ���� ������ġ
	vec4	world_scale;	// ��ƼŬ ũ��	
	vec4	color;			// ��ƼŬ ����
	vec4	velcotiy;		// ��ƼŬ ���� �ӵ�
	vec4	force;			// ��ƼŬ�� �־��� ��
	vec4	random_force;	// ��ƼŬ�� ����Ǵ� �������� ��

	float   age;			// ���� �ð�
	float   prev_age;		// ���� ������ ���� �ð�
	float   normalized_age;	// ������ �����ð��� 0~1�� ����ȭ �� ��
	float	life_time;		// ����
	float	mass;			// ����
	float   scale_factor;	// �߰� ũ�� ����

	int     active;			// ��ƼŬ Ȱ��ȭ ����
	int     padding;
};

struct RWParticleBuffer
{
	int		spawn_cnt;			// ���� ��ų ��ƼŬ ����
	int		total_spawn_cnt;
	int		vtx_cnt;
	float	ground_y;
};



struct ParticleModule
{
	// ���� ���
	PARTICLE_SPAWN_SHAPE_TYPE spawn_shape_type;

	vec4    spawn_color;
	vec4	spawn_scale_min;
	vec4	spawn_scale_max;

	vec3	shape_scale;
	int		spawn_rate;			// �ʴ� ���� ����
	int		space;				// ��ƼŬ ������Ʈ ��ǥ�� ( 0 : World,  1 : Local)
	float   life_time_min;		// �ּ� ����
	float   life_time_max;		// �ִ� ����

	// Color Change ���
	vec4	start_color;		// �ʱ� ����
	vec4	end_color;			// ���� ����

	// Scale Change ���
	float	start_scale;			// �ʱ� ����
	float	end_scale;			// ���� ����	

	// ���� �ִ�ũ��
	int		particle_cnt_max;
	int		particle_cnt;

	// Add Velocity ���
	PARTICLE_ADD_VEL_TYPE     add_velocity_type;
	vec4	velocity_dir;
	float	offset_angle;
	float	speed;
	float     velocity_padding;

	// Drag ��� - �ӵ� ����
	float	start_drag;
	float	end_drag;

	// NoiseForce ��� - ���� �� ����	
	float	noise_term;		// ���� �� ���� ����
	float	noise_force;	// ���� �� ũ��

	// Render ���
	int		velocity_alignment;	// 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
	int		velocity_scale;		// 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
	float   speed_max;			// �ִ� ũ�⿡ �����ϴ� �ӷ�
	vec4	velocity_scale_max;	// �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
	int		render_padding;

	// Module Check
	int		module_check[(UINT)PARTICLE_MODULE::END];
};



// ===================
// ������� ���� ����ü
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

	// material�� �Ʒ��� ������ �� �ֱ� �ѵ� ���ó�� ���� ��
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
	// ���� ���� �޶����� �ٲ����
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
