#pragma once
#include "Singleton.h"

namespace ff7r
{
	class Layer;
	class Collider2D;
	union CollisionID
	{
		struct
		{
			UINT left_id;
			UINT right_id;
		};

		UINT_PTR id;
	};


	class CollisionMgr : public Singleton<CollisionMgr>
	{
	public:
		void Tick();

		void LayerCheck(UINT _left, UINT _right);
		void LayerCheck(const wstring& _left_layer, const wstring& _right_layer);

		void Clear() { memset(matrix, 0, sizeof(UINT) * MAX_LAYER); }

	private:
		SINGLE(CollisionMgr);

		void CollisionBtwLayer(Layer* _left, Layer* _right);
		void CollisionBtwObject(GameObject* _left, GameObject* _right);
		bool CollisionBtwCollider(Collider2D* _left, Collider2D* _right);

		UINT					matrix[MAX_LAYER];
		map<UINT_PTR, bool>		map_colli_id;
	};

}