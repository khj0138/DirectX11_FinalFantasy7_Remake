#include "Value.hlsli"
#include "Struct.hlsli"
#include "Func.hlsli"

struct ParticleRW
{
    int SpawnCnt;
    int TotlaSpawnCnt;
    int VertexCnt;
    float GoundY;
};

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleRW> ParticleSpawnCount : register(u1);

StructuredBuffer<ParticleModule> ParticleModuleData : register(t20);
StructuredBuffer<Vertex> VertexData : register(t22);

//StructuredBuffer<float4> ParticleModuleData : register(t21);
Texture2D NoiseTexture : register(t21);

#define ObjectPos           Vec4_0
#define ObjectRot_Mat       Mat_0

#define NoiseTexResolution  Vec2_0

#define SpawnCount          ParticleSpawnCount[0].SpawnCnt
#define TotalSpawnCount     ParticleSpawnCount[0].TotlaSpawnCnt
#define VertexCount         ParticleSpawnCount[0].VertexCnt
#define GroundY             ParticleSpawnCount[0].GoundY
#define ModuleData          ParticleModuleData[0]
#define ParticleMaxCount    ParticleModuleData[0].particle_cnt_max

#define SpawnModule         ParticleModuleData[0].module_spawn
#define ColorChangeModule   ParticleModuleData[0].module_color_change
#define ScaleChangeModule   ParticleModuleData[0].module_scale_change
#define Initialize          ParticleModuleData[0].module_init

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (ParticleMaxCount <= DTid.x)
    {
        return;
    }
    
    Particle particle = ParticleBuffer[DTid.x];
    
    if (Initialize)
    {
        particle.active = 0;
        ParticleBuffer[DTid.x] = particle;
        return;
    }
    int a = -1;
    if (SpawnModule)
    {
        
        if (particle.active == 0)
        {
            //while (0 < SpawnCount)
            //{
            //    int orgvalue = SpawnCount;
            //    int outvalue = 0;
            //    InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
            //}
            //if (orgvalue == outvalue)
            
            int count;
            InterlockedAdd(SpawnCount, -1, count);
        
            if (count > 0)
            {
                particle.active = 1;
                    
                float3 rand_value_1 = (float3) 0.f;
                float3 rand_value_2 = (float3) 0.f;
                float3 rand_value_3 = (float3) 0.f;
                    
                    // 전체 유효 스레드의 아이디를 0 ~ 1 로 정규화
                float normalize_thread_id = (float) DTid.x / (float) ParticleMaxCount;
                
                GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id, rand_value_1);
                GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id + rand_value_1.x * 0.1f, rand_value_2);
                GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id + rand_value_2.y * 0.2f, rand_value_3);
                    
                if (ModuleData.spawn_shape_type == 0)
                {
                    
                    particle.local_pos.xyz 
                        = float3(ModuleData.shape_scale.x * rand_value_1.x - ModuleData.shape_scale.x * 0.5f
                                , ModuleData.shape_scale.y * rand_value_2.x - ModuleData.shape_scale.y * 0.5f
                                , /*ModuleData.shape_scale.z * rand_value_3.x - ModuleData.shape_scale.z * 0.5f*/0.0f);
                    
                    particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;

                    
                }
                else if (ModuleData.spawn_shape_type == 1)
                {
                    float radius = ModuleData.shape_scale.x;
                    float azimuth_angle = rand_value_1.x * 2 * PI; // 방위각
                    float elevation_angle = rand_value_2.x * PI; // 고도각
                    float distance = radius * pow(rand_value_3.x, 1.0f / 3.0f); // 균일한 분포를 위한 세제곱근
                    
                    particle.local_pos.x = distance * sin(elevation_angle) * cos(azimuth_angle);
                    particle.local_pos.y = distance * sin(elevation_angle) * sin(azimuth_angle);
                    particle.local_pos.z = distance * cos(elevation_angle);
                    
                    particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
                }
                else if (ModuleData.spawn_shape_type == 2)
                {
                    float radius = ModuleData.shape_scale.x;
                    float angle = ModuleData.shape_scale.y;
                    float cone_len = ModuleData.shape_scale.z;
                    
                    float azimuth_angle = rand_value_1.x * 2.0f * PI;
                    float distance = radius * pow(rand_value_2.x, 1.0f / 2.0f);
                    float second_circle_radius = cone_len / tan(angle);
                    float second_circle_distance = second_circle_radius * pow(rand_value_3.x, 1.0f / 2.0f);
                    
                    particle.local_pos.x = distance * cos(azimuth_angle);
                    particle.local_pos.y = distance * sin(azimuth_angle);
                    particle.local_pos.z = 0.0f;
                    
                    particle.velocity.x = second_circle_distance * cos(azimuth_angle) - particle.local_pos.x;
                    particle.velocity.y = second_circle_distance * sin(azimuth_angle) - particle.local_pos.y;
                    particle.velocity.z = cone_len;
                    
                    particle.local_pos.xyz = mul(ObjectRot_Mat, particle.local_pos).xyz;
                    particle.velocity.xyz = mul(ObjectRot_Mat, particle.velocity).xyz;
                    //
                    particle.velocity.xyz *= ModuleData.speed;
                    //particle.velocity.xyz *= 0.2f;
                    
                    particle.local_pos.xyz += particle.velocity.xyz * rand_value_1.y * 0.15f;
                    particle.local_pos.xyz += particle.velocity.xyz * rand_value_2.y * 0.15f;
                    particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
                }
                else if (ModuleData.spawn_shape_type == 3)
                {
                    //particle.local_pos.xyz = mul(particle.local_pos.xyz, rotationX);
                    particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
                    
                    
                    
                    
                    //float3x3 rotationZ = float3x3(
                    //     cos(ModuleData.offset_angle), -sin(ModuleData.offset_angle), 0,
                    //     sin(ModuleData.offset_angle), cos(ModuleData.offset_angle), 0,
                    //     0, 0, 1
                    // );
                    ////float3x3 rotationX = float3x3(
                    ////    1, 0, 0,
                    ////    0, cos(ModuleData.offset_angle), -sin(ModuleData.offset_angle),
                    ////    0, sin(ModuleData.offset_angle), cos(ModuleData.offset_angle)
                    ////);
                    //particle.padding = (int)(ModuleData.vel_padding * 360.f) + DTid.x % 3 * 120 ;
                    //float _angle = (float) (particle.padding) / 180.0f * PI;
                    //particle.padding += 50;
                    //float3x3 rotationX = float3x3(
                    //    1, 0, 0,
                    //    0, cos(_angle), -sin(_angle),
                    //    0, sin(_angle), cos(_angle)
                    //);
                    ////particle.padding = 1;
                    ////particle.mass = 1.f * ModuleData.offset_angle;
                    ////particle.mass;// = 200.f / 180.0f * PI; // * PI + ModuleData.offset_angle;
                    ////particle.local_pos.xyz = VertexData[DTid.x % 3 * 82 + 41].pos;
                    //particle.local_pos.xyz = VertexData[DTid.x % 3 * 0 + 2 * 82 + 80].pos;
                    ////particle.local_pos.x = 0.f;
                    
                    ////particle.local_pos.xyz = VertexData[DTid.x % VertexCount].pos;
                    //particle.local_pos.xyz = mul(particle.local_pos.xyz, rotationX);
                    //particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
                    //// VertexData.
                    
                    
                }
                else if (ModuleData.spawn_shape_type == 4)
                {
                    // particle.local_pos.xyz = VertexData[DTid.x % VertexCount].pos;
                    particle.world_pos.xyz = ObjectPos.xyz - (ModuleData.vel_dir.xyz * count);
                }
                else if (ModuleData.spawn_shape_type == 5)
                {
                    particle.local_pos.xyz 
                        = float3(ModuleData.shape_scale.x * rand_value_1.x - ModuleData.shape_scale.x * 0.5f
                                , ModuleData.shape_scale.y * rand_value_2.x - ModuleData.shape_scale.y * 0.5f
                                , /*ModuleData.shape_scale.z * rand_value_3.x - ModuleData.shape_scale.z * 0.5f*/0.0f);
                    
                    particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
                    
                    //particle.local_pos.x = DTid.x % 512.f;
                    //particle.local_pos.y = DTid.x / 512.f;
                    
                    // particle.test_uv = float4(rand_value_3.xyz, rand_value_2.z);
                    
                    // particle.local_pos.x -= 256.f;
                    // particle.local_pos.y -= 256.f;
                    // 
                    // particle.local_pos.x *= 0.1;
                    // particle.local_pos.y *= 0.1;
                    // 
                    // particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
                }
                else if (ModuleData.spawn_shape_type == 6)
                {
                    particle.world_pos.xyz = ObjectPos.xyz;
                }
                
                particle.world_scale.xyz = ModuleData.spawn_scale_min + (ModuleData.spawn_scale_max - ModuleData.spawn_scale_min) * rand_value_3.y;
                
                particle.life_time = ModuleData.life_time_min + (ModuleData.life_time_max - ModuleData.life_time_min) * rand_value_2.y;
                
                
                if (ModuleData.module_add_vel)
                {
                    if (ModuleData.add_vel_type == 0)  // From Center
                    {
                        float3 velocity = normalize(particle.local_pos.xyz);
                        particle.velocity.xyz = velocity * ModuleData.speed;
                    }
                    else if (ModuleData.add_vel_type == 1)  // To Center
                    {
                        float3 velocity = -normalize(particle.local_pos.xyz);
                        particle.velocity.xyz = velocity * ModuleData.speed;
                    }
                    else if (ModuleData.add_vel_type == 2)
                    {
                        particle.velocity.xyz = normalize(ModuleData.vel_dir.xyz) * ModuleData.speed;
                    }
                    else if (ModuleData.add_vel_type == 3)
                    {
                        float index = TotalSpawnCount + count;
                        index *= 0.07f;
                        particle.velocity.x = rand_value_3.y * ModuleData.speed;
                        particle.velocity.y = sin(index) * ModuleData.speed * 3.0f;
                        particle.velocity.z = cos(index) * ModuleData.speed * 3.0f;
                    }
                    else if (ModuleData.add_vel_type == 4)
                    {
                       // particle.velocity.z = -(rand_value_2.z * 0.5f + 0.5f);
                        particle.velocity.y = (rand_value_3.x - 0.5f) * 1.0472;
                        particle.velocity.z = (rand_value_1.x - 0.5f) * 1.0472;
                        particle.velocity.x = rand_value_2.x * 0.5f + 0.5f;
                        
                        //particle.velocity.z = 1.0f;
                        //particle.velocity.y = 1.0f;
                       // -(rand_value_2.z * 0.5f + 0.5f);
                        
                        //particle.velocity *= 0.5f;
                        //if (rand_value_3.z < 0.3f)
                        //{
                        //    particle.velocity *= (ModuleData.speed * (rand_value_3.z + 0.05f));
                        //}
                        //else
                        //{
                        //    particle.velocity *= ModuleData.speed;
                        //}
                        particle.force.x = rand_value_2.z * 0.2f;
                        float t = ModuleData.speed; /** (rand_value_3.z * 0.5f + 0.5f);*/
                        particle.velocity *= (t);
                        particle.velocity.xyz = mul(ObjectRot_Mat, particle.velocity).xyz;
                        particle.velocity.y = abs(particle.velocity.y);
                        //particle.velocity.y += rand_value_3.z * ModuleData.speed * 0.2f;

                    }
                    else if (ModuleData.add_vel_type == 5)
                    {
                        particle.velocity.y = (rand_value_3.x - 0.5f) * 1.2f + 0.4f;
                        particle.velocity.z = (rand_value_1.x - 0.5f) * 2.0f;
                        particle.velocity.x = (rand_value_2.x * 0.6f) + 0.4f;
                        //particle.velocity.xyz = (float3) 1.0f;
                        //particle.velocity.x = rand_value_2.x * 0.5f + 0.5f;
                        
                        //particle.force.x = rand_value_2.z * 2.0f;
                        float t = ModuleData.speed;
                        particle.velocity *= (t);
                        particle.velocity.xyz = mul(ObjectRot_Mat, particle.velocity).xyz;
                    
                        
                        //float t = particle.age / particle.life_time  ;
                        ////float speed = 1.0f / particle.life_time ;
                        //float idx = t * 30.0f;
                        ////float percent = idx - (float) ((int) (idx));
                        //int cur = DTid.x % 3 * 0 + 2 * 82 + 80 - (int) (idx) * 2;
                        //int next = cur - 2;
                        
                        //// particle.velocity.xyz = (VertexData[cur].pos * (percent) + VertexData[next].pos * (1.0f - percent)) * 40.0f;
                        //particle.velocity.xyz = (VertexData[next].pos - VertexData[cur].pos) * 30.0f;// * speed * 1.0f; // * 1.0f;
                        ////particle.velocity.x *= 0.f;
                        ////particle.velocity.yz *= 1.5f;
                        ////particle.velocity.y = (rand_value_3.x - 0.5f) * 1.2f + 0.4f;
                        ////particle.velocity.z = (rand_value_1.x - 0.5f) * 2.0f;
                        ////particle.velocity.x = (rand_value_2.x * 0.6f) + 0.4f;
                        //////particle.velocity.xyz = (float3) 1.0f;
                        //////particle.velocity.x = rand_value_2.x * 0.5f + 0.5f;
                        
                        //////particle.force.x = rand_value_2.z * 2.0f;
                        ////float t = ModuleData.speed;
                        ////particle.velocity *= (t);
                        ////particle.velocity.xyz = mul(ObjectRot_Mat, particle.velocity).xyz;
                        //float _angle = (float) (particle.padding) / 180.0f * PI;
                        //particle.padding = particle.padding;
                        ////particle.padding++;
                        //float3x3 rotationZ = float3x3(
                        // cos(particle.mass), -sin(particle.mass), 0,
                        // sin(particle.mass), cos(particle.mass), 0,
                        // 0, 0, 1
                        //);
                        ////float3x3 rotationX = float3x3(
                        ////1, 0, 0,
                        ////0, cos(particle.mass), -sin(particle.mass),
                        ////0, sin(particle.mass), cos(particle.mass)
                        ////);
                        //float3x3 rotationX = float3x3(
                        //1, 0, 0,
                        //0, cos(_angle), -sin(_angle),
                        //0, sin(_angle), cos(_angle)
                        //);

                        //particle.velocity.xyz = mul(particle.velocity.xyz, rotationX);
                        ////particle.mass++;

                    }
                    else if (ModuleData.add_vel_type == 6)
                    {
                        
                        float _speed = ModuleData.speed;
                        
                        float _angle = (float) (DTid.x % 8.0f) / 8.0f * PI * 2.0f;
                        
                        float3 _vec = float3(0.0f, 3.0f, 0.0f);
                        _vec = mul(ObjectRot_Mat, float4(_vec, 1.f)).xyz;
                        
                        float3x3 rotationX = float3x3(
                        1, 0, 0,
                        0, cos(_angle), -sin(_angle),
                        0, sin(_angle), cos(_angle)
                        );
                        
                        float4x4 _mat = (float4x4) transpose(ObjectRot_Mat);
                        float3 _vel = mul(_mat, float4(_vec, 1.0f)).xyz;
                        _vel = mul(_vel, rotationX);
                        _vel = mul(ObjectRot_Mat, float4(_vel, 1.0f)).xyz;
                        //float3 _vel = mul(_vec, rotationX);
                        //particle.local_pos.zxy = _vel;
                        //particle.local_pos.yzx = _vel;
                        
                        particle.local_pos.xyz = _vel;
                        
                        //particle.local_pos.xyz = mul(ObjectRot_Mat, particle.local_pos).xyz;
                        //xyz = particle.world_pos.xyz + _vel;
                        
                        //particle.velocity.xyz = mul(ObjectRot_Mat, particle.velocity).xyz;
                        //float _len = length(particle.world_scale.yz);
                        //float3 _scale = float3 (0.0f, _len, 0.0f);
                        
                        //_scale = mul(_scale, rotationX);
                        
                        
                        rotationX = float3x3(
                        1, 0, 0,
                        0, cos(108.f * PI / 180.f), -sin(108.f * PI / 180.f),
                        0, sin(108.f * PI / 180.f), cos(108.f * PI / 180.f)
                        );
                        //_scale = mul(_scale, rotationX);
                        
                        _vel = mul(_mat, float4(_vel, 1.0f)).xyz;
                        _vel = mul(_vel, rotationX);
                        _vel = mul(ObjectRot_Mat, float4(_vel, 1.0f)).xyz;
                        
                        float t = ModuleData.speed;
                        particle.velocity.xyz = _vel * t;
                        //particle.velocity.xyz = particle.velocity.yzx;
                        //particle.velocity.xyz = particle.velocity.zxy;
                        //float _len = length(particle.world_scale.yz);
                        //
                        ////particle.world_scale.y = abs(length(particle.world_scale.xyz)) * 100.0f;
                        //
                        //float3 _dir = _vel.xyz / length(_vel.xyz);
                        //float2 _scale = _dir.yz * length(particle.world_scale.yz);
                        ////particle.world_scale.y = abs(_scale.x);
                        ////particle.world_scale.z = abs(_scale.y);
                        //
                        ////particle.world_scale.y = abs(_scale.y);
                        //particle.world_scale.z = abs(30.0f);                        
                    }
                    else if (ModuleData.add_vel_type == 7)
                    
                    {
                        int index = (TotalSpawnCount - count) * 16;
                        particle.local_pos.xyz = VertexData[index].pos;
                        particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
             
                        particle.velocity.xyz = -normalize(float3(particle.local_pos.x, particle.local_pos.y -1.0f, particle.local_pos.z));

                    }
                    else if (ModuleData.add_vel_type == 8)
                    {
                        
                        
                        float3 velocity = normalize(particle.local_pos.xyz);
                        particle.velocity.xyz = velocity * ModuleData.speed;
                        
                        particle.velocity.xyz = mul(ObjectRot_Mat, float4(particle.velocity.xyz, 1.0f)).xyz;
                        particle.world_pos.xyz = mul(ObjectRot_Mat, float4(particle.world_pos.xyz, 1.0f)).xyz;
                        
                        
                        //float3 _vec = float3(0.0f, 2.5f, 0.0f);
                        //_vec = mul(ObjectRot_Mat, float4(_vec, 1.f)).xyz;
                        
                        //float3x3 rotationX = float3x3(
                        //1, 0, 0,
                        //0, cos(_angle), -sin(_angle),
                        //0, sin(_angle), cos(_angle)
                        //);
                        
                        //float4x4 _mat = (float4x4) transpose(ObjectRot_Mat);
                        //float3 _vel = mul(_mat, float4(_vec, 1.0f)).xyz;
                        //_vel = mul(_vel, rotationX);
                        //_vel = mul(ObjectRot_Mat, float4(_vel, 1.0f)).xyz;
                        ////float3 _vel = mul(_vec, rotationX);
                        ////particle.local_pos.zxy = _vel;
                        ////particle.local_pos.yzx = _vel;
                        
                        //particle.local_pos.xyz = _vel;
                        
                        ////particle.local_pos.xyz = mul(ObjectRot_Mat, particle.local_pos).xyz;
                        ////xyz = particle.world_pos.xyz + _vel;
                        
                        ////particle.velocity.xyz = mul(ObjectRot_Mat, particle.velocity).xyz;
                        ////float _len = length(particle.world_scale.yz);
                        ////float3 _scale = float3 (0.0f, _len, 0.0f);
                        
                        ////_scale = mul(_scale, rotationX);
                        
                        
                        //rotationX = float3x3(
                        //1, 0, 0,
                        //0, cos(105.f * PI / 180.f), -sin(105.f * PI / 180.f),
                        //0, sin(105.f * PI / 180.f), cos(105.f * PI / 180.f)
                        //);
                        ////_scale = mul(_scale, rotationX);
                        
                        //_vel = mul(_mat, float4(_vel, 1.0f)).xyz;
                        //_vel = mul(_vel, rotationX);
                        //_vel = mul(ObjectRot_Mat, float4(_vel, 1.0f)).xyz;
                        
                        //float t = ModuleData.speed;
                        //particle.velocity.xyz = _vel * t;
                        //particle.velocity.xyz = particle.velocity.yzx;
                        //particle.velocity.xyz = particle.velocity.zxy;
                        //float _len = length(particle.world_scale.yz);
                        //
                        ////particle.world_scale.y = abs(length(particle.world_scale.xyz)) * 100.0f;
                        //
                        //float3 _dir = _vel.xyz / length(_vel.xyz);
                        //float2 _scale = _dir.yz * length(particle.world_scale.yz);
                        ////particle.world_scale.y = abs(_scale.x);
                        ////particle.world_scale.z = abs(_scale.y);
                        //
                        ////particle.world_scale.y = abs(_scale.y);
                        //particle.world_scale.z = abs(30.0f);
                        
                        
                    }
                }
                  
                particle.mass = 1.f;
                particle.color = ModuleData.spawn_color;
                particle.age = 0.f;
            }
        }
    }

    if (particle.active)
    {
        // 파티클의 Age 에 시간을 누적시킴
        particle.prev_age = particle.age;
        particle.age += DT;
        particle.normalize_age = saturate(particle.age / particle.life_time);
        //particle.force.xyz = (float3) 0.f;
        
        // 파티클의 수명이 끝나면, 다시 비활성화 상태로 되돌림
        if (particle.life_time <= particle.age)
        {
            particle.active = 0.f;
        }
                
        // NoiseForce 모듈 (랜덤으로 힘) 적용 모듈
        if (ModuleData.noise_force)
        {
            if (particle.prev_age == 0.f)
            {
                 // 랜덤 결과를 받을 변수
                float3 rand_value_1 = (float3) 0.f;
                float3 rand_value_2 = (float3) 0.f;
                float3 rand_value_3 = (float3) 0.f;
                    
                // 전체 유효 스레드의 아이디를 0 ~ 1 로 정규화
                float normalize_thread_id = (float) DTid.x / (float) ParticleMaxCount;
                GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id, rand_value_1);
                GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id + rand_value_1.x * 0.1f, rand_value_2);
                GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id + rand_value_2.y * 0.2f, rand_value_3);
                
                float3 force = normalize(float3(rand_value_1.x, rand_value_2.x, rand_value_3.z));
                particle.random_force.xyz = force * ModuleData.noise_force;
            }
            else
            {
                int age = int(particle.age * (1.f / ModuleData.noise_term));
                int prev_age = int(particle.prev_age * (1.f / ModuleData.noise_term));

                // 지정한 간격을 넘어간 순간, 새로운 랜덤 Force 를 준다.
                if (age != prev_age)
                {
                    float3 rand_value_1 = (float3) 0.f;
                    float3 rand_value_2 = (float3) 0.f;
                    float3 rand_value_3 = (float3) 0.f;
                    
                    float normalize_thread_id = (float) DTid.x / (float) ParticleMaxCount;
                    GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id, rand_value_1);
                    GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id + 0.1f, rand_value_2);
                    GaussianSample(NoiseTexture, NoiseTexResolution, normalize_thread_id + 0.2f, rand_value_3);
                
                    float3 force = normalize(float3(rand_value_1.x, rand_value_2.x, rand_value_3.z));
                   // particle.random_force.xyz = force * ModuleData.noise_force;
                }
            }
            
            
           // particle.force.xyz += particle.random_force.xyz;
        }
        if (ModuleData.add_vel_type == 6)
        {
            
            //float3 _vel = particle.local_pos.yzx;
            //float3 _vel = particle.local_pos.zxy;
            float3 _vel = particle.local_pos.xyz;
            
            
            
            //_vel = particle.velocity.xyz;
            float3x3 rotationX = float3x3(
                        1, 0, 0,
                        0, cos(108.0f / 180.0f * PI), -sin(108.0f / 180.0f * PI),
                        0, sin(108.0f / 180.0f * PI), cos(108.0f / 180.0f * PI)
                        );
            
            
            //float3 _scale = float3(0.0f, _len, 0.0f);
            //
            //_scale = mul(_scale, rotationX);
            //_vel = mul(_vel, rotationX);
            //_vel = mul(ObjectRot_Mat, float4(_vel, 1.0f)).xyz;
            
            float4x4 _mat = (float4x4) transpose(ObjectRot_Mat);
            _vel = mul(_mat, float4(_vel, 1.0f)).xyz;
            _vel = mul(_vel, rotationX);
            _vel = mul(ObjectRot_Mat, float4(_vel, 1.0f)).xyz;
            
            
            float t = ModuleData.speed;
            particle.velocity.xyz = _vel * t;
            
            
        }
        // 파티클에 힘이 적용 된 경우, 힘에 의한 속도의 변화량 계산
        //float3 accel = particle.force.xyz / particle.mass;
        //particle.velocity.xyz += accel * DT;
        
        // 속도 제한(Drag) 모듈
        if (ModuleData.module_drag)
        {
            
            // 파티클의 현재 속력
            //float speed = length(particle.velocity);
            //float drag = ModuleData.start_drag + (ModuleData.end_drag - ModuleData.start_drag) * particle.normalize_age;
            
            //// 속도가 반대로 뒤집히는것 방지
            //if (drag <= 0.f)
            //{
            //    drag = 0.001f;
            //}
            
            //if (drag < speed)
            //{
            //    particle.velocity = normalize(particle.velocity) * drag;
            //}
            
            particle.velocity.y -= 4.f * DT;
            particle.velocity.xz *= pow((1.0f / 2.2f), DT);

        }
        
        // 속도에 따른 파티클위치 이동
        // Sim 좌표계에 따라서 이동방식 분기
        if (ModuleData.space == 0)
        {
            particle.world_pos += particle.velocity * DT;
            if (GroundY != 0.0f)
            {
                if (particle.world_pos.y < GroundY)
                {
                    particle.velocity.y *= -0.8f;
            
                }
                particle.world_pos += particle.velocity * DT ;
            }
        }
        else if (ModuleData.space == 1)
        {
           particle.local_pos += particle.velocity * DT;
           particle.world_pos.xyz = particle.local_pos.xyz + ObjectPos.xyz;
        }
        
        // 크기 변화 모듈이 활성화 되어있으면
        if (ModuleData.module_scale_change)
        {
            particle.scale_factor = ModuleData.start_scale + particle.normalize_age * (ModuleData.end_scale - ModuleData.start_scale);
        }
        else
        {
            particle.scale_factor = 1.f;
        }
        
        // 색상 변화모듈이 활성화 되어있으면
        if (ModuleData.module_color_change)
        {
            particle.color = ModuleData.start_color + particle.normalize_age * (ModuleData.end_color - ModuleData.start_color);
        }
        
    }
    
    // 변경점 적용
    ParticleBuffer[DTid.x] = particle;
}