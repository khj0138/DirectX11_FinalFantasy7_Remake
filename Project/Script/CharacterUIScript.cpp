#include "pch.h"

#include "FillUIScript.h"
#include "PlayerDataScript.h"
#include "TextUIScript.h"

#include "CharacterUIScript.h"
namespace ff7r
{
	CharacterUIScript::CharacterUIScript()
		: UIScript(SCRIPT_TYPE::CHARACTERUISCRIPT)
		, hp(nullptr)
		, mp(nullptr)
		, limit(nullptr)
		, atb(nullptr)
		, target(nullptr)
		, name(nullptr)
		, hp_text(nullptr)
		, mp_text(nullptr)
		, atb_text(nullptr)
		, limit_text(nullptr)
		, max_hp_text(nullptr)
		, cur_hp_text(nullptr)
		, cur_mp_text(nullptr)
	{
	}

	CharacterUIScript::~CharacterUIScript()
	{
	}

	void CharacterUIScript::Awake()
	{
		UIScript::Awake();
		CreateChild();
	}

	void CharacterUIScript::Tick()
	{
		PlayerDataScript* _data = target->GetScript<PlayerDataScript>();
		max_hp_text->SetText(to_string((int)_data->GetMaxHP()));
		cur_hp_text->SetText(to_string((int)_data->GetCurHP()));
		cur_mp_text->SetText(to_string((int)_data->GetCurMP()));
	}

	void CharacterUIScript::CreateChild()
	{
		CreateFillUI();
		CreateTextUI();
	}
	void CharacterUIScript::CreateFillUI()
	{
		
		GameObject* _hp = new GameObject;
		_hp->AddComponent(new Transform);
		_hp->AddComponent(new MeshRender);
		_hp->AddComponent(new FillUIScript);

		_hp->SetLayerIndex(31);
		_hp->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_hp->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"FillUIMtrl"));
		_hp->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_hp->SetName(L"HP HUD");

		hp = _hp->GetScript<FillUIScript>();
		hp->SetHUDType(FillUIScript::HUD_TYPE::HP);
		hp->SetTarget((CharacterDataScript*)(target->GetScript<PlayerDataScript>()));

		GetOwner()->AddChild(_hp);

		GameObject* _mp = new GameObject;
		_mp->AddComponent(new Transform);
		_mp->AddComponent(new MeshRender);
		_mp->AddComponent(new FillUIScript);

		_mp->SetLayerIndex(31);
		_mp->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_mp->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"FillUIMtrl"));
		_mp->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_mp->SetName(L"MP HUD");

		mp = _mp->GetScript<FillUIScript>();
		mp->SetHUDType(FillUIScript::HUD_TYPE::MP);
		mp->SetTarget((CharacterDataScript*)(target->GetScript<PlayerDataScript>()));

		GetOwner()->AddChild(_mp);


		GameObject* _limit = new GameObject;
		_limit->AddComponent(new Transform);
		_limit->AddComponent(new MeshRender);
		_limit->AddComponent(new FillUIScript);

		_limit->SetLayerIndex(31);
		_limit->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_limit->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"FillUIMtrl"));
		_limit->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_limit->SetName(L"LIMIT HUD");

		limit = _limit->GetScript<FillUIScript>();
		limit->SetHUDType(FillUIScript::HUD_TYPE::LIMIT);
		limit->SetTarget((CharacterDataScript*)(target->GetScript<PlayerDataScript>()));

		GetOwner()->AddChild(_limit);


		GameObject* _atb = new GameObject;
		_atb->AddComponent(new Transform);
		_atb->AddComponent(new MeshRender);
		_atb->AddComponent(new FillUIScript);

		_atb->SetLayerIndex(31);
		_atb->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		_atb->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"FillUIMtrl"));
		_atb->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas03.png"));
		_atb->SetName(L"ATB HUD");

		atb = _atb->GetScript<FillUIScript>();
		atb->SetHUDType(FillUIScript::HUD_TYPE::ATB);
		atb->SetTarget((CharacterDataScript*)(target->GetScript<PlayerDataScript>()));

		GetOwner()->AddChild(_atb);
	}

	void CharacterUIScript::CreateTextUI()
	{
		// =============================================
		GameObject* _name = new GameObject;
		_name->AddComponent(new Transform);
		_name->AddComponent(new FontRenderer);
		_name->AddComponent(new TextUIScript);
		
		_name->SetLayerIndex(31);
		_name->SetName(L"Chracter Name");
		_name->GetTransform()->SetRelativePosXY(-169.f, 21.f);
		_name->GetTransform()->SetRelativeScaleXY(0.6f, 0.6f);

		name = _name->GetScript<TextUIScript>();
		name->CreateDynamicMtrl();
		name->SetText(WStrToStr(target->GetName()));

		GetOwner()->AddChild(_name);

		// =============================================
		GameObject* _max_hp = new GameObject;
		_max_hp->AddComponent(new Transform);
		_max_hp->AddComponent(new FontRenderer);
		_max_hp->AddComponent(new TextUIScript);
	
		_max_hp->GetTransform()->SetRelativePosXY(10.f, 19.f);
		_max_hp->GetTransform()->SetRelativeScaleXY(0.55f, 0.55f);
		_max_hp->SetLayerIndex(31);
		_max_hp->SetName(L"Max HP Text");

		max_hp_text = _max_hp->GetScript<TextUIScript>();
		max_hp_text->CreateDynamicMtrl();
		max_hp_text->SetText("100");

		GetOwner()->AddChild(_max_hp);

		// =============================================
		GameObject* _cur_hp = new GameObject;
		_cur_hp->AddComponent(new Transform);
		_cur_hp->AddComponent(new FontRenderer);
		_cur_hp->AddComponent(new TextUIScript);

		_cur_hp->SetLayerIndex(31);
		_cur_hp->SetName(L"Cur HP Text");
		_cur_hp->GetTransform()->SetRelativePosXY(-42.f, 19.f);
		_cur_hp->GetTransform()->SetRelativeScaleXY(0.55f, 0.55f);

		cur_hp_text = _cur_hp->GetScript<TextUIScript>();
		cur_hp_text->CreateDynamicMtrl();
		cur_hp_text->SetText("100");

		GetOwner()->AddChild(_cur_hp);

		// =============================================
		GameObject* _cur_mp = new GameObject;
		_cur_mp->AddComponent(new Transform);
		_cur_mp->AddComponent(new FontRenderer);
		_cur_mp->AddComponent(new TextUIScript);
		
		_cur_mp->SetLayerIndex(31);
		_cur_mp->SetName(L"Cur MP Text");
		_cur_mp->GetTransform()->SetRelativePosXY(130.f, 19.f);
		_cur_mp->GetTransform()->SetRelativeScaleXY(0.55f, 0.55f);

		cur_mp_text = _cur_mp->GetScript<TextUIScript>();
		cur_mp_text->CreateDynamicMtrl();
		cur_mp_text->SetText("100");

		GetOwner()->AddChild(_cur_mp);

		// =============================================
		GameObject* _hp = new GameObject;
		_hp->AddComponent(new Transform);
		_hp->AddComponent(new FontRenderer);
		_hp->AddComponent(new TextUIScript);
		
		_hp->SetLayerIndex(31);
		_hp->SetName(L"HP Text");
		_hp->GetTransform()->SetRelativePosXY(-77.f, 11.f);
		_hp->GetTransform()->SetRelativeScaleXY(0.4f, 0.4f);

		hp_text = _hp->GetScript<TextUIScript>();
		hp_text->CreateDynamicMtrl();
		hp_text->SetText("HP");
		hp_text->SetColor(vec4(80.f / 255.f, 188.f / 255.f, 223.f / 255.f, 1.0f));

		GetOwner()->AddChild(_hp);

		// =============================================
		GameObject* _mp = new GameObject;
		_mp->AddComponent(new Transform);
		_mp->AddComponent(new FontRenderer);
		_mp->AddComponent(new TextUIScript);
		
		_mp->SetLayerIndex(31);
		_mp->SetName(L"MP Text");
		_mp->GetTransform()->SetRelativePosXY(95.f, 11.f);
		_mp->GetTransform()->SetRelativeScaleXY(0.4f, 0.4f);

		mp_text = _mp->GetScript<TextUIScript>();
		mp_text->CreateDynamicMtrl();
		mp_text->SetText("MP");
		mp_text->SetColor(vec4(80.f / 255.f, 188.f / 255.f, 223.f / 255.f, 1.0f));

		GetOwner()->AddChild(_mp);

		// =============================================
		GameObject* _atb = new GameObject;
		_atb->AddComponent(new Transform);
		_atb->AddComponent(new FontRenderer);
		_atb->AddComponent(new TextUIScript);
		
		_atb->SetLayerIndex(31);
		_atb->SetName(L"ATB Text");
		_atb->GetTransform()->SetRelativePosXY(27.f, -18.f);
		_atb->GetTransform()->SetRelativeScaleXY(0.4f, 0.4f);

		atb_text = _atb->GetScript<TextUIScript>();
		atb_text->CreateDynamicMtrl();
		atb_text->SetText("ATB");
		atb_text->SetColor(vec4(80.f / 255.f, 188.f / 255.f, 223.f / 255.f, 1.0f));

		GetOwner()->AddChild(_atb);

		// =============================================
		GameObject* _limit = new GameObject;
		_limit->AddComponent(new Transform);
		_limit->AddComponent(new FontRenderer);
		_limit->AddComponent(new TextUIScript);
		
		_limit->SetLayerIndex(31);
		_limit->SetName(L"Limit Text");
		_limit->GetTransform()->SetRelativePosXY(127.f, -18.f);
		_limit->GetTransform()->SetRelativeScaleXY(0.4f, 0.4f);

		limit_text = _limit->GetScript<TextUIScript>();
		limit_text->CreateDynamicMtrl();
		limit_text->SetText("Limit");
		limit_text->SetColor(vec4(80.f / 255.f, 188.f / 255.f, 223.f / 255.f, 1.0f));

		GetOwner()->AddChild(_limit);
	}
}