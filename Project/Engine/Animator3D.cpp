#include "pch.h"

#include "Animator3D.h"

#include "TimeMgr.h"
#include "MeshRender.h"
#include "StructuredBuffer.h"
#include "ResourceMgr.h"
#include "Transform.h"

#include "Animation3DShader.h"

#include "KeyMgr.h"

namespace ff7r
{
	Vector4 QuatToVec4(const DirectX::SimpleMath::Quaternion& _q)
	{
		Vector4 vec4;
		vec4.x = _q.x;
		vec4.y = _q.y;
		vec4.z = _q.z;
		vec4.w = _q.w;

		return vec4;
	}
	Animator3D::Animator3D()
		: joint_final_mat_buff(nullptr)
		, is_mat_final_update(false)
		, cur_state(nullptr)
		, wait_state(nullptr)
		, wait_time(0.0f)
		, is_change_clip(false)
		, is_start_change_clip(false)
		, Component(COMPONENT_TYPE::ANIMATOR3D)
		, is_bone_motion(true)
		, cur_root_mat(Matrix())
		, prev_root_mat(Matrix())
		, last_frame_trans(FrameTrans())
		, controller(nullptr)
		, rot_ratio(1.0f)
		, is_move_front(true)
		, any_state(nullptr)
	{
		joint_final_mat_buff = new StructuredBuffer;
		joint_trans_buff = new StructuredBuffer;
		cur_joint_mat_buff = new StructuredBuffer;

		world_dir[0] = { 1.f, 0.f, 0.f };
		world_dir[1] = { 0.f, 1.f, 0.f };
		world_dir[2] = { 0.f, 0.f, 1.f };

		shader = (Animation3DShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(L"Animation3DUpdateCS").Get();
	}

	Animator3D::Animator3D(const Animator3D& _origin)
		: joint_final_mat_buff(nullptr)
		, is_mat_final_update(false)
		, wait_state(nullptr)
		, wait_time(0.0f)
		, is_change_clip(false)
		, is_start_change_clip(false)
		, Component(COMPONENT_TYPE::ANIMATOR3D)
		, is_bone_motion(_origin.is_bone_motion)
		, controller(_origin.controller)
		, cur_state(_origin.cur_state)
		, rot_ratio(_origin.rot_ratio)
		, is_move_front(true)
		, any_state(_origin.any_state)
	{
		joint_final_mat_buff = new StructuredBuffer;
		joint_trans_buff = new StructuredBuffer;
		cur_joint_mat_buff = new StructuredBuffer;

		SetAnimClip(_origin.animations);

		for (auto iter = controller->anim_states.begin(); iter != controller->anim_states.end(); iter++)
		{
			iter->second->SetAnimator(this);
		}
		shader = (Animation3DShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(L"Animation3DUpdateCS").Get();
	}

	Animator3D::~Animator3D()
	{
		if (joint_final_mat_buff)
		{
			delete joint_final_mat_buff;
			joint_final_mat_buff = nullptr;
		}

		if (joint_trans_buff)
		{
			delete joint_trans_buff;
			joint_trans_buff = nullptr;
		}

		if (cur_joint_mat_buff)
		{
			delete cur_joint_mat_buff;
			cur_joint_mat_buff = nullptr;
		}
	}

	void Animator3D::Awake()
	{
		cur_state = controller->anim_states[L"default"];
	}

	void Animator3D::FinalTick()
	{
		if (controller == nullptr)
		{
			return;
		}

		is_mat_final_update = false;

		if (is_start_change_clip)
		{
			cur_state = wait_state;
			if (cur_state->GetAnimation() != nullptr)
			{
				cur_state->Reset();
			}
			cur_state->EnterState(wait_time, start_frame_idx);
			is_change_clip = true;
			is_start_change_clip = false;
		}

		if (any_state)
		{
			AnimationState* _next_state = any_state->CheckTransition();

			if (_next_state)
			{
				_next_state = FindNextState(_next_state);


				if (_next_state)
				{
					ChangeState(_next_state, _next_state->GetAnimation()->GetDuration(), 0);
				}
			}
		}

		if (cur_state)
		{
			cur_state->FinalTick();
			is_change_clip = cur_state->IsWait();
		}

		for (auto& param : controller->parameters)
		{
			if (param.second->type == DATA_TYPE::TRIGGER)
			{
				param.second->data = false;
			}
		}

		Ptr<Mesh> _mesh = GetMeshRender()->GetMesh();
		check_mesh(_mesh);

		if (cur_state->GetAnimation())
		{
			CalcCurRootMatrix();
		}

	}

	void Animator3D::SetAnimClip(const vector<Ptr<Animation3D>>& _animations)
	{
		for (int i = 0; i < _animations.size(); i++)
		{
			Ptr<Animation3D> _anim = _animations[i];
			_anim->SetAnimator(this);
			animations.insert({ _anim->GetKey(), _anim });
		}
	}

	void Animator3D::SetAnimClip(const map<wstring, Ptr<Animation3D>>& _animations)
	{
		for (auto iter = _animations.begin(); iter != _animations.end(); iter++)
		{
			Ptr<Animation3D> _anim = ResourceMgr::GetInst()->FindRes<Animation3D>(iter->first);
			_anim->SetAnimator(this);
			animations.insert({ _anim->GetKey(), _anim });
		}
	}

	void Animator3D::SetAnimatorController(Ptr<AnimatorController> _controller)
	{
		if (_controller != nullptr)
		{
			controller = _controller;

			if (controller->anim_states.find(L"any_state") == controller->anim_states.end())
			{
				AnimationState* _any_state = new AnimationState();
				_any_state->SetName(L"any_state");
				controller->anim_states.insert({ L"any_state", _any_state });
			}

			any_state = controller->anim_states.find(L"any_state")->second;

			for (auto iter = controller->anim_states.begin(); iter != controller->anim_states.end(); iter++)
			{
				iter->second->SetAnimator(this);
			}

			if (!controller->anim_states.empty())
			{
				cur_state = controller->anim_states[L"default"];
			}
		}
	}

	void Animator3D::UpdateData()
	{
		if (!is_mat_final_update)
		{
			Ptr<Mesh> _mesh = GetMeshRender()->GetMesh();
			//check_mesh(_mesh);
			UINT _joint_cnt = (UINT)_mesh->GetJointCount();
			shader->SetOffsetMatBuffer(_mesh->GetJointInverseBuffer());
			shader->SetOutputBuffer(joint_final_mat_buff);
			shader->SetFrameTransBuffer(joint_trans_buff);
			shader->SetCurJointMatBuffer(cur_joint_mat_buff);
			shader->SetBoneCount(_joint_cnt);

			shader->SetChangeAnim(is_change_clip);
			shader->SetStartChangeAnim(is_start_change_clip);

			if (cur_state)
			{
				cur_state->UpdateData();
			}
			// 업데이트 쉐이더 실행
			shader->Execute();

			is_mat_final_update = true;
		}



		// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
		joint_final_mat_buff->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
	}

	void Animator3D::ClearData()
	{
		joint_final_mat_buff->Clear();

		if (cur_state)
		{
			if (cur_state->GetAnimation() != nullptr)
				cur_state->Clear();
		}

		UINT _mtrl_cnt = GetMeshRender()->GetMtrlCount();
		Ptr<Material> _mtrl = nullptr;
		for (UINT i = 0; i < _mtrl_cnt; ++i)
		{
			_mtrl = GetMeshRender()->GetSharedMaterial(i);
			if (nullptr == _mtrl)
				continue;

			_mtrl->SetAnimation3D(false); // Animation Mesh 알리기
			_mtrl->SetJointCount(0);
		}
	}

	AnimationState* Animator3D::FindNextState(AnimationState* _state)
	{
		while (true)
		{
			if (_state && _state->GetAnimation() != nullptr)
			{
				return _state;
			}

			_state = _state->CheckTransition();

			if (!_state)
			{
				return nullptr;
			}
		}
	}

	void Animator3D::SetParameter(wstring _key, Data _data, DATA_TYPE _type)
	{
		auto iter = controller->parameters.find(_key);

		if (iter != controller->parameters.end())
		{
			if (iter->second->type == _type)
			{
				iter->second->data = _data;
			}
		}
	}

	void Animator3D::check_mesh(Ptr<Mesh> _pMesh)
	{
		UINT _joint_cnt = _pMesh->GetJointCount();

		if (joint_final_mat_buff->GetElementCount() != _joint_cnt)
		{
			joint_final_mat_buff->Create(sizeof(Matrix), _joint_cnt, SB_TYPE::READ_WRITE, true, nullptr);
			joint_trans_buff->Create(sizeof(FrameTrans), _joint_cnt, SB_TYPE::READ_WRITE, false, nullptr);
			cur_joint_mat_buff->Create(sizeof(Matrix), _joint_cnt, SB_TYPE::READ_WRITE, true, nullptr);
		}
	}

	function<void()> Animator3D::GetAnimationEvent(wstring _name)
	{
		if (anim_events.find(_name) != anim_events.end())
		{
			return anim_events[_name];
		}

		return nullptr;
	}

	void Animator3D::BoneMotion()
	{
		if (!is_start_clip)
		{
			is_start_clip = true;
			//prev_rot = {};
			prev_rot = MatrixToRotation(cur_root_mat);
			return;
		}
		vec3 cur_root_trans = vec3(cur_root_mat._41, cur_root_mat._42, cur_root_mat._43);
		vec3 prev_root_trans = vec3(prev_root_mat._41, prev_root_mat._42, prev_root_mat._43);


		vec3 change_trans = cur_root_trans - prev_root_trans;
		vec3 trans_rot = MatrixToRotation(cur_root_mat);
		//trans_rot = { trans_rot.x + 90.f * 3.141592f / 180.f, trans_rot.y, trans_rot.z + 90.f * 3.141592f / 180.f };

		vec3 cur_rot = GetTransform()->GetRelativeRot();

		//float rot_y = trans_rot.y - prev_rot.y;
		vec3 rot_ = trans_rot - prev_rot;
		
		/*if (trans_rot.x >= 2.f)
		{
			rot_y *= -1;
		}
		if (cur_rot.x >= 2.f)
		{
			rot_y *= -1;
		}*/
		if (abs(rot_.x) > XM_PI * 0.5f || abs(rot_.z) > XM_PI * 0.5f)
		{
			trans_rot.x = prev_rot.x;
			trans_rot.z = prev_rot.z;
			rot_.y *= -1.0f;
		}
		rot_ *= rot_ratio;
		cur_rot.y += rot_.y;

		GetTransform()->SetRelativeRot(cur_rot);
		prev_rot = trans_rot;


		//Matrix _matRot = XMMatrixIdentity();
		//_matRot = XMMatrixRotationX(cur_rot.x);
		//_matRot *= XMMatrixRotationY(cur_rot.y);
		//_matRot *= XMMatrixRotationZ(cur_rot.z);

		//vec3 vDefaultDir[3] = {
		//	  vec3(1.f, 0.f, 0.f)
		//	, vec3(0.f, 1.f, 0.f)
		//	, vec3(0.f, 0.f, 1.f)
		//};
		//
		//vec3 world_dir[3];
		//for (int i = 0; i < 3; ++i)
		//{
		//	world_dir[i] = XMVector3TransformNormal(vDefaultDir[i], _matRot);
		//}

		if (is_move_front)
		{
			vec3 cur_pos = GetTransform()->GetRelativePos();
			vec3 trans;
			trans += change_trans.x * world_dir[(UINT)DIR_TYPE::RIGHT];
			trans += change_trans.y * world_dir[(UINT)DIR_TYPE::UP];
			trans += change_trans.z * -world_dir[(UINT)DIR_TYPE::FRONT];

			if (rot_ratio != 1.0f)
			{
				trans = RotateVec3YAxis(trans, rot_origin * (rot_ratio - 1.0f));
			}

			GetTransform()->SetRelativePos(cur_pos + trans);
		}
		else
		{
			// vec3 cur_pos = GetTransform()->GetRelativePos();
			// vec3 trans;
			// change_trans.x = change_trans.x > 0.f ? 0.f : change_trans.x;
			// trans += change_trans.x * world_dir[(UINT)DIR_TYPE::RIGHT];
			// trans += change_trans.y * world_dir[(UINT)DIR_TYPE::UP];
			// -trans += change_trans.z * world_dir[(UINT)DIR_TYPE::FRONT];
			// 
			// if (rot_ratio != 1.0f)
			// {
			// 	trans = RotateVec3YAxis(trans, rot_origin * (rot_ratio - 1.0f));
			// }
			// 
			// GetTransform()->SetRelativePos(cur_pos + trans);
		}

		//Matrix cur_mat = GetTransform()->GetWorldMat();
		//cur_mat._41 += trans.x;
		//cur_mat._42 += trans.y;
		//cur_mat._43 += trans.z;
		//GetTransform()->SetWorldMat(cur_mat);
	}

	void Animator3D::SaveToLevelFile(FILE* _file)
	{
		int _cnt = animations.size();
		fwrite(&_cnt, 1, sizeof(int), _file);

		for (auto iter = animations.begin(); iter != animations.end(); iter++)
		{
			SaveWString(iter->first, _file);
			SaveResRef(iter->second.Get(), _file);
		}

		SaveResRef(controller.Get(), _file);

		//int cnt = parameters.size();
		//fwrite(&cnt, sizeof(int), 1, _file);

		//for (auto iter = parameters.begin(); iter != parameters.end(); iter++)
		//{
		//	SaveWString(iter->first, _file);
		//	fwrite(&iter->second.data, sizeof(Data), 1, _file);
		//	fwrite(&iter->second.type, sizeof(DATA_TYPE), 1, _file);
		//}

		//cnt = animations.size();
		//fwrite(&cnt, sizeof(int), 1, _file);

		//for (auto iter = animations.begin(); iter != animations.end(); iter++)
		//{
		//	SaveWString(iter->first, _file);
		//}

		//cnt = anim_states.size();
		//fwrite(&cnt, sizeof(int), 1, _file);

		//for (auto iter = anim_states.begin(); iter != anim_states.end(); iter++)
		//{
		//	SaveWString(iter->first, _file);
		//}

		//for (auto iter = anim_states.begin(); iter != anim_states.end(); iter++)
		//{
		//	iter->second->Save(_file);
		//}
	}

	void Animator3D::LoadFromLevelFile(FILE* _file)
	{
		int _cnt = animations.size();
		fread(&_cnt, 1, sizeof(int), _file);

		for (int i = 0; i < _cnt; i++)
		{
			Ptr<Animation3D> _anim;
			wstring _name;
			LoadWString(_name, _file);
			LoadResRef(_anim, _file);

			animations.insert({ _name, _anim });
		}

		LoadResRef(controller, _file);

		if (controller != nullptr)
		{
			for (auto iter = controller->anim_states.begin(); iter != controller->anim_states.end(); iter++)
			{
				iter->second->SetAnimator(this);
			}

			if (cur_state == nullptr)
			{
				cur_state = controller->anim_states.begin()->second;
			}
		}
	}

	void Animator3D::ChangeState(AnimationState* _state, float _duration, int _frame_idx)
	{
		if (_state->GetAnimation() == nullptr)
		{
			AnimationState* _next = FindNextState(_state);

			wait_time = _duration;
			start_frame_idx = _frame_idx;

			if (_next)
			{
				if (_next->GetName() == cur_state->GetName())
				{
					cur_state->Reset();
					ResetStartClip();
					return;

				}
				else
				{
					wait_state = _next;
				}
				is_start_change_clip = true;
				return;
			}
			else
			{
				// 빈 껍데기에 다음 애니메이션이 안달려 있음
				assert(nullptr);
			}
		}

		wait_state = _state;
		wait_time = _duration;
		start_frame_idx = _frame_idx;
		is_start_change_clip = true;
	}

	AnimationState* Animator3D::CreateEmptyState(wstring _key)
	{
		static int Test_Name = 0;
		_key = std::to_wstring(Test_Name++);

		AnimationState* state = new AnimationState();
		state->SetAnimator(this);
		state->SetName(_key);

		controller->anim_states.insert({ _key , state });

		return state;
	}

	bool Animator3D::RenameState(wstring _prev_key, wstring _new_key)
	{
		if (controller->anim_states.find(_new_key) == controller->anim_states.end())
		{
			AnimationState* _prev_state = GetAnimState(_prev_key);
			controller->anim_states.erase(_prev_key);

			_prev_state->SetName(_new_key);
			controller->anim_states.insert({ _new_key,_prev_state });
			return true;
		}
		return false;

		/*AnimationState* state = new AnimationState();
		state->SetAnimation(_prev_state->GetAnimation());
		state
		state->SetAnimator(this);
		state->SetName(_key);
		states[]*/

		//controller->anim_states.insert({ _key , state });
	}

	void Animator3D::DeleteState(wstring _key)
	{
		AnimationState* _prev_state = GetAnimState(_key);
		for (auto _states_iter = controller->anim_states.begin(); _states_iter != controller->anim_states.end(); _states_iter++)
		{
			AnimationState* _state = _states_iter->second;
			vector<Transition*>& _transitions = _state->GetTransitions();
			//for (int i = 0; i < _transitions.size(); i++)
			for (auto _transitions_iter = _transitions.begin(); _transitions_iter != _transitions.end();)
			{
				if ((*_transitions_iter)->next_state == _prev_state)
				{
					_transitions_iter = _transitions.erase(_transitions_iter);
				}
				else
				{
					_transitions_iter++;
				}
			}
		}
		controller->anim_states.erase(_key);
		delete _prev_state;
		cur_state = controller->GetAnimState(L"default");
	}

	bool Animator3D::RenameParameter(wstring _prev_key, wstring _new_key)
	{
		if (controller->parameters.find(_new_key) == controller->parameters.end())
		{
			Parameter* _prev_param = GetParameter(_prev_key);
			controller->parameters.erase(_prev_key);
			_prev_param->SetKey(_new_key);
			controller->parameters.insert({ _new_key, _prev_param });


			return true;
		}


		return false;
	}

	void Animator3D::DeleteParameter(wstring _key)
	{
		Parameter* _prev_param = GetParameter(_key);
		for (auto _states_iter = controller->anim_states.begin(); _states_iter != controller->anim_states.end(); _states_iter++)
		{
			AnimationState* _state = _states_iter->second;
			vector<Transition*>& _transitions = _state->GetTransitions();
			//for (int i = 0; i < _transitions.size(); i++)
			for (auto _transitions_iter = _transitions.begin(); _transitions_iter != _transitions.end(); _transitions_iter++)
			{
				vector<Condition>& _conditions = (*_transitions_iter)->GetConditions();
				for (auto _conditions_iter = _conditions.begin(); _conditions_iter != _conditions.end();)
				{
					if ((*_conditions_iter).input_data == _prev_param)
					{
						_conditions_iter = _conditions.erase(_conditions_iter);
					}
					else
					{
						_conditions_iter++;
					}
				}
			}
		}
		controller->parameters.erase(_key);
		//delete _prev_param;
	}

	AnimationState* Animator3D::GetAnimState(wstring _key)
	{
		auto iter = controller->anim_states.find(_key);

		if (iter != controller->anim_states.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	Ptr<Animation3D> Animator3D::GetAnimation(wstring _key)
	{
		auto iter = animations.find(_key);

		if (iter != animations.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	Transition* Animator3D::CreateTransition(AnimationState* _src, AnimationState* _dest)
	{
		Transition* _transition = new Transition;
		_transition->always_chek = false;
		_transition->next_state = _dest;

		_src->SetTransition(_transition);

		return _transition;
	}

	Parameter* Animator3D::GetParameter(wstring _key)
	{
		auto iter = controller->parameters.find(_key);

		if (iter != controller->parameters.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	void Animator3D::SaveAnimator()
	{
		if (controller != nullptr)
		{
			controller->Save(controller->GetKey());
		}
		for (auto iter = animations.begin(); iter != animations.end(); iter++)
		{
			iter->second->Save(iter->second->GetKey());
		}
	}

	void Animator3D::CalcWorldDir()
	{
		vec3 _rot = GetTransform()->GetRelativeRot();

		Matrix _matRot = XMMatrixIdentity();
		_matRot = XMMatrixRotationX(_rot.x);
		_matRot *= XMMatrixRotationY(_rot.y);
		_matRot *= XMMatrixRotationZ(_rot.z);

		vec3 vDefaultDir[3] = { vec3(1.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 1.f) };

		for (int i = 0; i < 3; ++i)
		{
			world_dir[i] = XMVector3TransformNormal(vDefaultDir[i], _matRot);
		}
	}

	void Animator3D::CalcCurRootMatrix()
	{
		/*Ptr<Mesh> _mesh = GetMeshRender()->GetMesh();
		check_mesh(_mesh);
		UINT _joint_cnt = (UINT)_mesh->GetJointCount();
		shader->SetOffsetMatBuffer(_mesh->GetJointInverseBuffer());
		shader->SetOutputBuffer(joint_final_mat_buff);
		shader->SetFrameTransBuffer(joint_trans_buff);
		shader->SetBoneCount(_joint_cnt);*/


		Animation3D* cur_anim = cur_state->GetAnimation();
		int cur_frame_idx = cur_anim->GetFrameIdx();

		FrameTrans* cur_frame_trans = cur_anim->FindCurFrameTrans(cur_frame_idx, 0);
		Matrix _scale = Matrix::CreateScale(Vector3(cur_frame_trans->scale.x, cur_frame_trans->scale.y, cur_frame_trans->scale.z));
		Matrix _translate = Matrix::CreateTranslation(Vector3(cur_frame_trans->translate.x, cur_frame_trans->translate.y, cur_frame_trans->translate.z));
		Quaternion _rot(cur_frame_trans->rotation);
		//Matrix _scale(cur_frame_trans->scale);


		Matrix scale, trans;
		Quaternion quter_rot;
		float ratio = cur_anim->GetRatio();

		if (is_change_clip)
		{
			scale = Matrix::Lerp(Matrix::CreateScale(Vector3(last_frame_trans.scale.x, last_frame_trans.scale.y, last_frame_trans.scale.z)), _scale, ratio);
			trans = Matrix::Lerp(Matrix::CreateTranslation(Vector3(last_frame_trans.translate.x, last_frame_trans.translate.y, last_frame_trans.translate.z)), _translate, ratio);
			quter_rot = Quaternion::Lerp(Quaternion(last_frame_trans.rotation), _rot, ratio);
			//SetBoneMotion(true);
			//trans = Matrix::CreateTranslation(Vector3(0, 0, 0));
		}
		else
		{
			int next_frame_idx = cur_anim->GetNextFrameIdx();
			FrameTrans* next_frame_trans = cur_anim->FindCurFrameTrans(next_frame_idx, 0);
			Matrix next_scale = Matrix::CreateScale(Vector3(next_frame_trans->scale.x, next_frame_trans->scale.y, next_frame_trans->scale.z));
			Matrix next_translate = Matrix::CreateTranslation(Vector3(next_frame_trans->translate.x, next_frame_trans->translate.y, next_frame_trans->translate.z));
			Quaternion next_rot(next_frame_trans->rotation);

			scale = Matrix::Lerp(_scale, next_scale, ratio);
			trans = Matrix::Lerp(_translate, next_translate, ratio);
			quter_rot = Quaternion::Lerp(_rot, next_rot, ratio);
		}

		if (is_start_change_clip)
		{
			last_frame_trans.scale = Vector4(scale._11, scale._22, scale._33, scale._44);
			last_frame_trans.translate = Vector4(trans._41, trans._42, trans._43, trans._44);
			//last_frame_trans.translate = Vector4(0, 0, 0, 1);

			//cur_root_mat._41 = cur_root_mat._42 = cur_root_mat._43 = 0;
			last_frame_trans.rotation = vec4(quter_rot);
		}


		Matrix _root_trans = scale;
		_root_trans._44 = 1.0f;


		vec4 temp_trans = vec4(trans._41, trans._42, trans._43, 1.0f);


		/*float a = quter_rot.y;
		quter_rot.y = quter_rot.x;
		quter_rot.x = -a;*/

		float a = quter_rot.x;
		quter_rot.x = quter_rot.y;
		quter_rot.y = quter_rot.z;
		quter_rot.z = a;

		Matrix temp_rotation = {};
		temp_rotation = temp_rotation.CreateFromQuaternion(quter_rot);
		vec3 _hi = MatrixToRotation(temp_rotation);

		prev_root_mat = cur_root_mat;
		cur_root_mat = _root_trans;

		cur_root_mat *= temp_rotation;


		for (int i = 0; i < 3; i++)
		{
			cur_root_mat.m[3][i] += temp_trans[i];
		}


		if (IsBoneMotion() && !is_change_clip)
		{
			BoneMotion();
		}
	}
}