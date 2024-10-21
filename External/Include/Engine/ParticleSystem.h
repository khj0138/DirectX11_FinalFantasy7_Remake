#pragma once
#include "RenderComponent.h"

#include "ptr.h"
#include "ParticleShader.h"

namespace ff7r
{
    class StructuredBuffer;
    class ParticleSystem : public RenderComponent
    {
    public:
        ParticleSystem();
        ParticleSystem(const ParticleSystem& _origin);
        ~ParticleSystem();

        CLONE(ParticleSystem);

        virtual void Start() override;
        virtual void FinalTick() override;
        virtual void Render() override;
        virtual void Render(UINT _i) override {}

        virtual void OnEnable() override;

        virtual void SaveToLevelFile(FILE* _File) override;
        virtual void LoadFromLevelFile(FILE* _File) override;

        void ActiveModule(PARTICLE_MODULE _type) { module_data.module_check[(UINT)_type] = true; }
        void DeactivateModule(PARTICLE_MODULE _type) { module_data.module_check[(UINT)_type] = false; }
        void SetSpawnRate(int _rate) { module_data.spawn_rate = _rate; }
        void SetSpawnInitialColor(vec3 _color) { module_data.spawn_color = _color; }
        void SetIntensity(float _intensity) { intensity = _intensity; }
        void SetCreateParticleCnt(int _cnt) { create_particle_cnt = _cnt; }
        void SetGroundYPos(float _y) { rwbuffer.ground_y = _y; }
        void SetSpawnMesh(Ptr<Mesh> _mesh);

        ParticleModule& GetModuleData() { return module_data; }

    private:
        StructuredBuffer*       particle_buff;
        StructuredBuffer*       rw_buff; // read-write buffer
        StructuredBuffer*       module_buff;
        StructuredBuffer*       mesh_buff;

        ParticleModule          module_data;
        RWParticleBuffer        rwbuffer;
        Ptr<ParticleShader>     particle_shader;

        vec3                    prev_pos;

        float                   intensity;
        float                   acc_time; // accumulated_time

        int                     create_particle_cnt;
        bool                    is_first_frame;
    };

}