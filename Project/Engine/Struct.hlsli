#ifndef _STRUCT
#define _STRUCT

//struct DirectionalLight
//{
//    float3 color;
//    float4 world_dir;
    
//    float intensity;
//};

//struct PointLight
//{
//    float3 color;
//    float4 world_pos; 
//    float4 world_dir; 
    
//    float intensity;
//    float attenuation_radius; 
//};

//struct SpotLight
//{
//    float3 color;
//    float4 world_pos;
//    float4 world_dir;
    
//    float inner_cone_angle;
//    float outer_cone_angle;
    
//    float intensity;
//    float attenuation_radius;
//};

struct LightInfo
{
    float4 color;
    float4 world_pos;
    float4 world_dir;

    uint light_type;

    float intensity;
    float attenuation_radius;
    float angle;
};

struct CaptureInfo
{
    float4  world_pos;
    float   radius;
    uint    level;
    
    float2 padding;
};

struct tSkinningInfo
{
    float3 vPos;
    float3 vTangent;
    float3 vBinormal;
    float3 vNormal;
};


// Particle
struct Particle
{
    float4 local_pos;
    float4 world_pos; // ��ƼŬ ��ġ
    float4 world_scale; // ��ƼŬ ũ��
    float4 color; // ��ƼŬ ����
    float4 velocity; // ��ƼŬ ���� �ӵ�
    float4 force; // ��ƼŬ�� �־��� ��
    float4 random_force; // ��ƼŬ�� ����Ǵ� ���� ��

    float age; // ���� �ð�
    float prev_age; // ���� ������ �����ð�
    float normalize_age; // ������ �����ð��� 0~1�� ����ȭ �� ��
    float life_time; // ����
    float mass; // ����
    float scale_factor; // �߰� ũ�� ����
    
    int active;
    int padding;
};
struct ParticleModule
{
	// ���� ���
    int spawn_shape_type; 
    float4 spawn_color;
    float4 spawn_scale_min;
    float4 spawn_scale_max;
    float3 shape_scale;
    int spawn_rate;
    int space; // 0 World, 1 Local    
    float life_time_min;
    float life_time_max;

	// Color Change ���
    float4 start_color; // �ʱ� ����
    float4 end_color; // ���� ����

	// Scale Change ���
    float start_scale; // �ʱ� ũ��
    float end_scale; // ���� ũ��	

    int particle_cnt_max;
    int particle_cnt;
    
    // Add Velocity ���
    int add_vel_type; // 0 : From Center, 1 : Fixed Direction	
    float4 vel_dir;
    float offset_angle;
    float speed;
    float vel_padding;
    
    // Drag ���
    float start_drag;
    float end_drag;
    
    // NoiseForce ���
    float noise_term;
    float noise_force;
        
    // Render ���
    int vel_alignment; // 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
    int vel_scale; // 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
    float speed_max; // �ִ� ũ�⿡ �����ϴ� �ӷ�
    float4 vel_scale_max; // �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
    int render_padding;

    
    // Module Check
    int module_spawn;
    int module_color_change;
    int module_scale_change;
    int module_add_vel;
         
    int module_drag;
    int module_noise_force;
    int module_render;
    int module_init;
    // int module_init;
};

struct Vertex
{
    float3 pos;
    float4 color;
    float2 uv;
    
    float3 normal;
    float3 tangent;
    float3 binormal;
    
    float4 weights;
    float4 indices;
};
struct Font
{
    float3 text_uv;
    float2 text_size;
};
struct GlobalFont
{
    Font Fonts[63];
    float2 padding;
};

struct DamageFont
{
    Font Fonts[10];
    float2 padding;
};

struct Texts
{
    int sentence[17];
    float2 box_size;
    int first;
    int is_text;
};
#endif