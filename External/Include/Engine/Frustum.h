#pragma once
#include "Entity.h"

namespace ff7r
{
    enum FACE_TYPE
    {
        FT_NEAR,
        FT_FAR,
        FT_UP,
        FT_DOWN,
        FT_LEFT,
        FT_RIGHT,

        FT_END,
    };


    class Camera;

    class Frustum : public Entity
    {
    private:
        Camera* m_pCam;

        // ���� ���������� 8���� ��
        vec3        m_arrProj[8];


        // ����ü�� 6��
        vec4        m_arrFace[FT_END];

    public:
        void FinalTick();


    public:
        bool FrustumCheck(vec3 _vPos);
        bool FrustumCheckBound(vec3 _vPos, float _fRadis);

    public:
        CLONE(Frustum);



    public:
        Frustum() {}
        Frustum(Camera* _Camera);
        ~Frustum();
    };
}
