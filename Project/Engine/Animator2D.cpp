#include "pch.h"
#include "Animator2D.h"

#include "Animation2D.h"
#include "MeshRender.h"

#include "ptr.h"
#include "Material.h"
namespace ff7r
{
	Animator2D::Animator2D()
		: Component(COMPONENT_TYPE::ANIMATOR2D)
		, cur_anim(nullptr)
		, is_repeat(false)
		, is_management_active(false)
	{
	}

	Animator2D::Animator2D(const Animator2D& _other)
		: Component(COMPONENT_TYPE::ANIMATOR2D)
		, cur_anim(nullptr)
		, is_repeat(false)
		, is_management_active(false)
	{
	}

	Animator2D::~Animator2D()
	{
		SafeDelMap(map_anim);
	}

	void Animator2D::FinalTick()
	{
		if (nullptr != cur_anim)
		{
			if (cur_anim->IsFinish())
			{
				if(is_repeat)
				{
					cur_anim->Reset();
				}
				else if (is_management_active)
				{
					GetOwner()->SetActive(false);
				}
			}

			cur_anim->FinalTick();
		}
	}

	void Animator2D::UpdateData()
	{
		Ptr<Material> _mtrl = GetMeshRender()->GetMaterial();

		const Anim2DFrame& frm = cur_anim->GetCurFrame();
		vec2 vBackSize = cur_anim->GetBackSize();

		int iAnimUse = 1;
		_mtrl->SetScalarParam(INT_0, &iAnimUse);
		_mtrl->SetScalarParam(VEC2_0, &frm.left_top_uv);
		_mtrl->SetScalarParam(VEC2_1, &frm.slice_uv);
		_mtrl->SetScalarParam(VEC2_2, &frm.offset);
		_mtrl->SetScalarParam(VEC2_3, &vBackSize);

		_mtrl->SetTexParam(TEX_0, cur_anim->GetAtlasTex());
	}

	void Animator2D::Clear()
	{
		Ptr<Material> pMtrl = GetMeshRender()->GetMaterial();

		int iAnimUse = 0;
		pMtrl->SetScalarParam(INT_0, &iAnimUse);

		Ptr<Texture> pTex = nullptr;
		pMtrl->SetTexParam(TEX_0, pTex);
	}

	void Animator2D::Reset()
	{
		cur_anim->Reset();
	}

	void Animator2D::Play(const wstring& _strName, bool _bRepeat, int _frame)
	{
		Animation2D* pAnim = FindAnim(_strName);
		assert(pAnim);

		cur_anim = pAnim;
		cur_anim->SetFrame(_frame);
		is_repeat = _bRepeat;
	}

	Animation2D* Animator2D::FindAnim(const wstring& _strName)
	{
		map<wstring, Animation2D*>::iterator iter = map_anim.find(_strName);

		if (iter == map_anim.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void Animator2D::CreateAnimation(const wstring& _strAnimName
		, Ptr<Texture> _AtlasTex, vec2 _vLeftTop, vec2 _vSlice, vec2 _vBackSize
		, int _FrameCount, int _FPS)
	{
		Animation2D* pAnim = new Animation2D;
		pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS);

		pAnim->owner = this;
		map_anim.insert(make_pair(_strAnimName, pAnim));
	}

	void Animator2D::CreateAnimation(const wstring& _ani_name, Ptr<Texture> _atlas_tex, vec2 _left_top, vec2 _slice_cnt, float _fps)
	{
		if (map_anim.find(_ani_name) == map_anim.end())
		{
			Animation2D* pAnim = new Animation2D;
			pAnim->Create(_ani_name, _atlas_tex, _left_top, _slice_cnt, _fps);

			pAnim->owner = this;
			map_anim.insert(make_pair(_ani_name, pAnim));
		}
	}

	void Animator2D::SaveToLevelFile(FILE* _File)
	{
		fwrite(&is_repeat, sizeof(bool), 1, _File);

		size_t AnimCount = map_anim.size();
		fwrite(&AnimCount, sizeof(size_t), 1, _File);

		for (const auto& pair : map_anim)
		{
			pair.second->SaveToLevelFile(_File);
		}

		wstring strCurAnimName;
		if (nullptr != cur_anim)
		{
			strCurAnimName = cur_anim->GetName();
		}
		SaveWString(strCurAnimName, _File);
	}

	void Animator2D::LoadFromLevelFile(FILE* _File)
	{
		fread(&is_repeat, sizeof(bool), 1, _File);

		size_t AnimCount = 0;
		fread(&AnimCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < AnimCount; ++i)
		{
			Animation2D* pNewAnim = new Animation2D;
			pNewAnim->LoadFromLevelFile(_File);

			map_anim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
			pNewAnim->owner = this;
		}

		wstring strCurAnimName;
		LoadWString(strCurAnimName, _File);

		cur_anim = FindAnim(strCurAnimName);
	}
}