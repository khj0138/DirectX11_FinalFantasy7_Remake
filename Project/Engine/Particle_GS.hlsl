#include "Value.hlsli"
#include "Struct.hlsli"

StructuredBuffer<Particle> ParticleBuffer : register(t20);
StructuredBuffer<ParticleModule> ParticleModuleData : register(t21);

#define ModuleData ParticleModuleData[0]


struct VSOut
{
    float3 pos : POSITION;
    uint id : SV_InstanceID;
};

struct GSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    uint id : SV_InstanceID;
};

[maxvertexcount(6)]
void main(point VSOut _in[1], inout TriangleStream<GSOut> outstream)
{
    uint id = _in[0].id;
    
    if (ParticleBuffer[id].active == 0)
    {
        return;
    }

    float3 view_pos = mul(float4(ParticleBuffer[id].world_pos.xyz, 1.f), Mat_View).xyz;
    float2 scale = ParticleBuffer[id].world_scale.xy * ParticleBuffer[id].scale_factor;
   
    float3 new_pos[4] =
    {
        float3(-scale.x / 2.f, +scale.y / 2.f, 0.f),
        float3(+scale.x / 2.f, +scale.y / 2.f, 0.f),
        float3(+scale.x / 2.f, -scale.y / 2.f, 0.f),
        float3(-scale.x / 2.f, -scale.y / 2.f, 0.f)
    };
    
    if (ModuleData.module_render)
    {
        if (ModuleData.vel_scale)
        {
            // ���� ��ƼŬ�� �ӷ��� �˾Ƴ���.
            float3 vVelocity = normalize(ParticleBuffer[id].velocity.xyz);
            vVelocity = mul(float4(vVelocity, 0.f), Mat_View).xyz;
            
            vVelocity = normalize(vVelocity);
            float cur_speed = length(vVelocity.xy);         
            float cur_speed2 = length(ParticleBuffer[id].velocity.xyz);
            
            cur_speed = cur_speed * cur_speed2;
            
            if (ModuleData.speed_max < cur_speed)
            {
                cur_speed = ModuleData.speed_max;
            }
            
            // �ִ�ӵ� ��� ���� �ӵ��� ������ ���Ѵ�.
            float ratio = saturate(cur_speed / ModuleData.speed_max);
          
            // ������ �´� ũ�⺯ȭ���� ���Ѵ�.   
            float3 default_scale = float3(1.f, 1.f, 1.f);
            float3 new_scale = default_scale + (ModuleData.vel_scale_max.xyz - default_scale) * ratio;
                      
            new_pos[0].x = new_pos[0].x * new_scale.x;
            new_pos[1].x = new_pos[1].x * new_scale.x;
            new_pos[2].x = new_pos[2].x * new_scale.x;
            new_pos[3].x = new_pos[3].x * new_scale.x;
        }
        //if (ModuleData.vel_alignment)
        //{
        //    // ��ƼŬ ���� ���� �ӵ��� View �������� ��ȯ
        //    float3 vVelocity = (ParticleBuffer[id].velocity.xyz);
        //    vVelocity = mul(float4(vVelocity, 0.f), Mat_View).xyz;
        //    vVelocity.z = 0;
        //    vVelocity = normalize(vVelocity);
        //    // ��ƼŬ Right ����� �̵� ������ �����ؼ� �� ������ ������ ���Ѵ�.
        //    float3 vRight = float3(1.f, 0.f, 0.f);
        //    float fTheta = acos(dot(vRight, vVelocity));
            
        //    // ������ ����� �ڻ��� ������ �������� �ϱ� ������, 2���� ���� �ݴ�� ������ �ش�.
        //    if (vVelocity.y < vRight.y)
        //    {
        //        fTheta = (2.f * 3.1415926535f) - fTheta;
        //    }
            
        //    // ���� ������ Z �� ȸ�� ����� �����.
        //    float3x3 matRotZ =
        //    {
        //        cos(fTheta), sin(fTheta), 0,
        //        -sin(fTheta), cos(fTheta), 0,
        //                  0, 0, 1.f,
        //    };
            
        //    // 4���� ������ ȸ����Ų��.
        //    for (int i = 0; i < 4; ++i)
        //    {
        //        new_pos[i] = mul(new_pos[i], matRotZ);
        //    }
        //}
        if (ModuleData.vel_alignment)
        {
            float3 velocity = normalize(ParticleBuffer[id].velocity.xyz);
            velocity = mul(float4(velocity, 0.f), Mat_View).xyz;

            
// Z�� ȸ�� ���� ���
            float thetaZ = atan2(velocity.y, velocity.x);

// Y�� ȸ�� ���� ���
            //float thetaY = atan2(velocity.z, sqrt(velocity.x * velocity.x + velocity.y * velocity.y));
            //float thetaY = atan2(velocity.z, velocity.x);

            
// Z�� ȸ�� ��� ����
            float3x3 matRotZ =
            {
                cos(thetaZ), sin(thetaZ), 0,
                -sin(thetaZ), cos(thetaZ), 0,
                0, 0, 1
            };

// Y�� ȸ�� ��� ����
            //float3x3 matRotY =
            //{
            //    cos(thetaY), 0, -sin(thetaY),
            //    0, 1, 0,
            //    sin(thetaY), 0, cos(thetaY)
            //};

// ȸ�� ��� ���� (���� Z�� ȸ�� �� Y�� ȸ�� ����)
           // float3x3 combinedRotation = mul(matRotY, matRotZ);

// 4���� ������ ȸ����Ų��.
            for (int i = 0; i < 4; ++i)
            {
                new_pos[i] = mul(new_pos[i], matRotZ);
            }
            
        }
    }
    
    GSOut output[4] = { (GSOut) 0.f, (GSOut) 0.f, (GSOut) 0.f, (GSOut) 0.f };
    
    output[0].pos = mul(float4(new_pos[0] + view_pos, 1.f), Mat_Proj);
    output[0].uv = float2(0.f, 0.f);
    output[0].id = id;
    
    output[1].pos = mul(float4(new_pos[1] + view_pos, 1.f), Mat_Proj);
    output[1].uv = float2(1.f, 0.f);
    output[1].id = id;
    
    output[2].pos = mul(float4(new_pos[2] + view_pos, 1.f), Mat_Proj);
    output[2].uv = float2(1.f, 1.f);
    output[2].id = id;
    
    output[3].pos = mul(float4(new_pos[3] + view_pos, 1.f), Mat_Proj);
    output[3].uv = float2(0.f, 1.f);
    output[3].id = id;
    
    
    // ���� ����
    outstream.Append(output[0]);
    outstream.Append(output[1]);
    outstream.Append(output[2]);
    outstream.RestartStrip();
    
    outstream.Append(output[0]);
    outstream.Append(output[2]);
    outstream.Append(output[3]);
    outstream.RestartStrip();
}