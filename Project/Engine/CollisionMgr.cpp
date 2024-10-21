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

				// iRow ���̾�� iCol ���̾�� ���� �浹�˻縦 �����Ѵ�.
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

		// �浹ü ID ����
		CollisionID _id = {};
		_id.left_id = _left->GetCollider2D()->GetID();
		_id.right_id = _right->GetCollider2D()->GetID();

		// ID �˻�
		map<UINT_PTR, bool>::iterator _iter = map_colli_id.find(_id.id);
		if (_iter == map_colli_id.end())
		{
			map_colli_id.insert(make_pair(_id.id, false));
			_iter = map_colli_id.find(_id.id);
		}

		// �� �� �ϳ��� ���� ���� ���¶��(Dead ����)
		bool _dead = false;
		if (_left->IsDead() || _right->IsDead())
		{
			_dead = true;
		}

		// �� �浹ü�� ���� �浹 ������ Ȯ��
		if (CollisionBtwCollider(_left->GetCollider2D(), _right->GetCollider2D()))
		{
			// ������ �浹�� ���� �ְ�, ���� �ϳ� �̻��� ���� �����̶��
			if (_dead && _iter->second)
			{
				_left->GetCollider2D()->EndOverlap(_right->GetCollider2D());
				_right->GetCollider2D()->EndOverlap(_left->GetCollider2D());
			}
			else if (_iter->second)
			{
				// ��� �浹 ��
				_left->GetCollider2D()->OnOverlap(_right->GetCollider2D());
				_right->GetCollider2D()->OnOverlap(_left->GetCollider2D());
			}
			else
			{
				// �̹� �����ӿ� �浹
				if (!_dead) // ���� �ϳ��� Dead ���¸� �浹�� �����Ѵ�.
				{
					_left->GetCollider2D()->BeginOverlap(_right->GetCollider2D());
					_right->GetCollider2D()->BeginOverlap(_left->GetCollider2D());
					_iter->second = true;
				}
			}
		}

		else
		{
			// �浹 ����
			if (_iter->second)
			{
				_left->GetCollider2D()->EndOverlap(_right->GetCollider2D());
				_right->GetCollider2D()->EndOverlap(_left->GetCollider2D());
				_iter->second = false;
			}
		}


	}

	// �� �浹ü�� �浹 �˻� ����
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

		// �� �浹ü�� �� ǥ�� ���� 2���� ����
		vec3 _proj[4] = {};

		_proj[0] = XMVector3TransformCoord(_local_pos[1], _left->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _left->GetColliderWorldMat());
		_proj[1] = XMVector3TransformCoord(_local_pos[3], _left->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _left->GetColliderWorldMat());

		_proj[2] = XMVector3TransformCoord(_local_pos[1], _right->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _right->GetColliderWorldMat());
		_proj[3] = XMVector3TransformCoord(_local_pos[3], _right->GetColliderWorldMat()) - XMVector3TransformCoord(_local_pos[0], _right->GetColliderWorldMat());

		// �� �浹ü�� �߽����� ����
		vec3 vCenter = XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _right->GetColliderWorldMat()) - XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _left->GetColliderWorldMat());


		// �и��� �׽�Ʈ
		for (int i = 0; i < 4; ++i)
		{
			vec3 vProj = _proj[i];
			vProj.Normalize();

			// 4���� ǥ����͸� ������ ���������� ������Ų �Ÿ��� ���� / 2
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