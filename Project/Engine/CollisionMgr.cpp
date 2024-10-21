#include "pch.h"
#include "CollisionMgr.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "Collider2D.h"

namespace ff7r
{
	CollisionMgr::CollisionMgr()
		: matrix{}
	{

	}


	CollisionMgr::~CollisionMgr()
	{

	}

	void CollisionMgr::Tick()
	{
		Level* _level = LevelMgr::GetInst()->GetCurLevel();

		for (UINT row = 0; row < MAX_LAYER; ++row)
		{
			for (UINT col = row; col < MAX_LAYER; ++col)
			{
				if (!(matrix[row] & (1 << col)))
					continue;

				// iRow 레이어와 iCol 레이어는 서로 충돌검사를 진행한다.
				CollisionBtwLayer(_level->GetLayer(row), _level->GetLayer(col));
			}
		}
	}

	void CollisionMgr::CollisionBtwLayer(Layer* _left, Layer* _right)
	{
		const vector<GameObject*>& _left_objects = _left->GetObjects();
		const vector<GameObject*>& _right_objects = _right->GetObjects();

		if (_left == _right)
		{
			for (size_t i = 0; i < _left_objects.size(); ++i)
			{
				for (size_t j = i + 1; j < _right_objects.size(); ++j)
				{
					CollisionBtwObject(_left_objects[i], _right_objects[j]);
				}
			}
		}
		else
		{
			for (size_t i = 0; i < _left_objects.size(); ++i)
			{
				for (size_t j = 0; j < _right_objects.size(); ++j)
				{
					CollisionBtwObject(_left_objects[i], _right_objects[j]);
				}
			}
		}
	}

	void CollisionMgr::CollisionBtwObject(GameObject* _left, GameObject* _right)
	{
		if (!(_left->GetCollider2D() && _right->GetCollider2D()))
			return;

		// 충돌체 ID 생성
		CollisionID _id = {};
		_id.left_id = _left->GetCollider2D()->GetID();
		_id.right_id = _right->GetCollider2D()->GetID();

		// ID 검색
		map<UINT_PTR, bool>::iterator _iter = map_colli_id.find(_id.id);
		if (_iter == map_colli_id.end())
		{
			map_colli_id.insert(make_pair(_id.id, false));
			_iter = map_colli_id.find(_id.id);
		}

		// 둘 중 하나라도 삭제 예정 상태라면(Dead 상태)
		bool _dead = false;
		if (_left->IsDead() || _right->IsDead())
		{
			_dead = true;
		}

		// 두 충돌체가 지금 충돌 중인지 확인
		if (CollisionBtwCollider(_left->GetCollider2D(), _right->GetCollider2D()))
		{
			// 이전에 충돌한 적이 있고, 둘중 하나 이상이 삭제 예정이라면
			if (_dead && _iter->second)
			{
				_left->GetCollider2D()->EndOverlap(_right->GetCollider2D());
				_right->GetCollider2D()->EndOverlap(_left->GetCollider2D());
			}
			else if (_iter->second)
			{
				// 계속 충돌 중
				_left->GetCollider2D()->OnOverlap(_right->GetCollider2D());
				_right->GetCollider2D()->OnOverlap(_left->GetCollider2D());
			}
			else
			{
				// 이번 프레임에 충돌
				if (!_dead) // 둘중 하나라도 Dead 상태면 충돌을 무시한다.
				{
					_left->GetCollider2D()->BeginOverlap(_right->GetCollider2D());
					_right->GetCollider2D()->BeginOverlap(_left->GetCollider2D());
					_iter->second = true;
				}
			}
		}

		else
		{
			// 충돌 해제
			if (_iter->second)
			{
				_left->GetCollider2D()->EndOverlap(_right->GetCollider2D());
				_right->GetCollider2D()->EndOverlap(_left->GetCollider2D());
				_iter->second = false;
			}
		}


	}

	// 두 충돌체의 충돌 검사 진행
	bool CollisionMgr::CollisionBtwCollider(Collider2D* _left, Collider2D* _right)
	{
		// 0 -- 1
		// |    |
		// 3 -- 2
		vec3 _local_pos[4] =
		{
			vec3(-0.5f, 0.5f, 0.f),
			vec3(0.5f, 0.5f, 0.f),
			vec3(0.5f, -0.5f, 0.f),
			vec3(-0.5f, -0.5f, 0.f),
		};

		// 두 충돌체의 각 표면 벡터 2개씩 구함
		vec3 _proj[4] = {};

		_proj[0] = XMVector3TransformCoord(_local_pos[1], _left->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _left->GetColliderWorldMat());
		_proj[1] = XMVector3TransformCoord(_local_pos[3], _left->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _left->GetColliderWorldMat());

		_proj[2] = XMVector3TransformCoord(_local_pos[1], _right->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _right->GetColliderWorldMat());
		_proj[3] = XMVector3TransformCoord(_local_pos[3], _right->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _right->GetColliderWorldMat());

		// 두 충돌체의 중심점을 구함
		vec3 vCenter = XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _right->GetColliderWorldMat()) - XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _left->GetColliderWorldMat());


		// 분리축 테스트
		for (int i = 0; i < 4; ++i)
		{
			vec3 vProj = _proj[i];
			vProj.Normalize();

			// 4개의 표면백터를 지정된 투영축으로 투영시킨 거리의 총합 / 2
			float fProjDist = 0.f;
			for (int j = 0; j < 4; ++j)
			{
				fProjDist += fabsf(_proj[j].Dot(vProj));
			}
			fProjDist /= 2.f;

			float fCenter = fabsf(vCenter.Dot(vProj));

			if (fProjDist < fCenter)
				return false;
		}


		return true;
	}



	void CollisionMgr::LayerCheck(UINT _left, UINT _right)
	{
		UINT row = (UINT)_left;
		UINT col = (UINT)_right;

		if (row > col)
		{
			UINT iTemp = col;
			col = row;
			row = iTemp;
		}

		matrix[row] |= (1 << col);
	}

	void CollisionMgr::LayerCheck(const wstring& _left_layer, const wstring& right_layer)
	{
		Level* _level = LevelMgr::GetInst()->GetCurLevel();

		Layer* _left = _level->FindLayerByName(_left_layer);
		Layer* _right = _level->FindLayerByName(right_layer);

		LayerCheck(_left->GetLayerIndex(), _right->GetLayerIndex());
	}
}