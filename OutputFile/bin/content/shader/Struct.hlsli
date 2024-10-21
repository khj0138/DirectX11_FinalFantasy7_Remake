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
    float4 world_pos; // 파티클 위치
    float4 world_scale; // 파티클 크기
    float4 color; // 파티클 색상
    float4 velocity; // 파티클 현재 속도
    float4 force; // 파티클에 주어진 힘
    float4 random_force; // 파티클에 적용되는 랜덤 힘

    float age; // 생존 시간
    float prev_age; // 이전 프레임 생존시간
    float normalize_age; // 수명대비 생존시간을 0~1로 정규화 한 값
    float life_time; // 수명
    float mass; // 질량
    float scale_factor; // 추가 크기 배율
    
    int active;
    int padding;
};
struct ParticleModule
{
	// 스폰 모듈
    int spawn_shape_type; 
    float4 spawn_color;
    float4 spawn_scale_min;
    float4 spawn_scale_max;
    float3 shape_scale;
    int spawn_rate;
    int space; // 0 World, 1 Local    
    float life_time_min;
    float life_time_max;

	// Color Change 모듈
    float4 start_color; // 초기 색상
    float4 end_color; // 최종 색상

	// Scale Change 모듈
    float start_scale; // 초기 크기
    float end_scale; // 최종 크기	

    int particle_cnt_max;
    int particle_cnt;
    
    // Add Velocity 모듈
    int add_vel_type; // 0 : From Center, 1 : Fixed Direction	
    float4 vel_dir;
    float offset_angle;
    float speed;
    float vel_padding;
    
    // Drag 모듈
    float start_drag;
    float end_drag;
    
    // NoiseForce 모듈
    float noise_term;
    float noise_force;
        
    // Render 모듈
    int vel_alignment; // 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
    int vel_scale; // 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
    float speed_max; // 최대 크기에 도달하는 속력
    float4 vel_scale_max; // 속력에 따른 크기 변화량 최대치
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