#include "pch.h"

#include "ButtonEffectScript.h"
namespace ff7r
{
	ButtonEffectScript::ButtonEffectScript()
		: UIScript(SCRIPT_TYPE::BUTTONEFFECTSCRIPT)
		, length(0.0f)
		, highlight_time(0.0f)
		, cur_time(0.0f)
	{	
	}

	ButtonEffectScript::~ButtonEffectScript()
	{
	}

	void ButtonEffectScript::Awake()
	{
		GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));

		SetSize(vec2(67.f, 40.f), vec2(1.0f, 1.0f));
		SetLeftTop(vec2(800.f, 1852.f));

		UIScript::Awake();
	}

	void ButtonEffectScript::Tick()
	{
		cur_time += DT;

		if (cur_time >= highlight_time)
		{
			GetOwner()->SetActive(false);
			return;
		}

		float _pos_x = lerp(0.0f, length, cur_time / highlight_time);
		_pos_x -= length / 2.f;

		GetTransform()->SetRelativePosX(_pos_x);
	}

	void ButtonEffectScript::OnEnable()
	{
		cur_time = 0.0f;
	}

	void ButtonEffectScript::OnDisable()
	{
	}
}