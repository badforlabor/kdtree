#pragma once

/**,
 * Auth :   liubo
 * Date :   2021/07/05 11:48
 * Comment: kdtree  
 */

#define KDTREE_DEBUG 1
 
#include <vector>
#include <array>
#include <algorithm>
#if KDTREE_DEBUG
#include <assert.h>
#endif
#include <math.h>


namespace kdtree
{
	namespace kdtree_inside
	{
		class KdNode
		{
		public:
			class KdNode* Leaves[2];
			int DataIdx;
			int Axis;
			KdNode() : DataIdx(-1), Axis(-1) { Leaves[0] = nullptr; Leaves[1] = nullptr; }
		};

		using KdQueueValue = std::pair<double, int>;

		class KdQueue
		{
		public:
			KdQueue(int Count) : MaxCount(Count)
			{
				QueueData.reserve(MaxCount + 1);
			}

			void Push(double InDistSqr, int DataIdx)
			{
				auto it = std::find_if(std::begin(QueueData), std::end(QueueData), [InDistSqr](const KdQueueValue& V)
					{
						return InDistSqr < V.first;
					});
				QueueData.insert(it, std::make_pair(InDistSqr, DataIdx));
				if (QueueData.size() > MaxCount)
				{
					// QueueData.erase(QueueData.begin() + MaxCount, QueueData.end());
					QueueData.resize(MaxCount);
				}
			}
			KdQueueValue Back() const
			{
				return QueueData.back();
			}

			// 存储排序好的数据：按照距离由近及远
			std::vector<KdQueueValue> QueueData;
			size_t MaxCount;
		};
	}

	template<int KdDim>
	class KdTree
	{
	public:
		KdTree() = default;
	private:
		// noncopyable
		KdTree(const KdTree&) = delete;
		KdTree& operator=(const KdTree&) = delete;
	public:
		using KdValue = std::array<double, KdDim>;

	private:
		using KdNode = kdtree_inside::KdNode;
		using KdQueueValue = kdtree_inside::KdQueueValue;
		using KdQueue = kdtree_inside::KdQueue;

		KdNode* Root = nullptr;
		std::vector<KdValue> ValueList;

		static void Assert(bool b)
		{
#if KDTREE_DEBUG
			assert(b);
#endif
		}
		static double MaxDouble()
		{
			return std::numeric_limits<double>::max();
		}

	public:

		inline static double Sqr(double A)
		{
			return A * A;
		}

		inline static double Abs(double A)
		{
			return A > 0 ? A : -A;
		}

		inline static int AnotherLeaf(int Leaf)
		{
			return (Leaf + 1) % 2;
		}

		static double DistSqrt(const KdValue& A, const KdValue& B)
		{
			double d = 0;
			for (int i = 0; i < KdDim; i++)
			{
				d += Sqr(A[i] - B[i]);
			}
			return sqrt(d);
		}
		static double DistSqrt(const KdValue& A)
		{
			double d = 0;
			for (int i = 0; i < KdDim; i++)
			{
				d += Sqr(A[i]);
			}
			return sqrt(d);
		}

	public:
		~KdTree()
		{
			LoopDestroy(Root);
		}
		void Push(const KdValue& V)
		{
			auto Node = new KdNode();
			ValueList.push_back(V);
			Node->DataIdx = ValueList.size() - 1;
			
			PushImpl(Root, Node, 0);
		}
		// 获取最近的
		int GetNeareast(const KdValue& V) const 
		{
			KdQueueValue Q = std::make_pair(-1, -1);
			GetNeareastImpl(Q, Root, V);
			return Q.second;
		}
		// 获取距离最近的K个值(Nearest Neighbour)
		std::vector<int> GetNnk(const KdValue& V, int K) const
		{
			KdQueue Q(K);
			GetNnkImpl(Q, Root, V);

			std::vector<int> List;
			for (auto It : Q.QueueData)
			{
				List.push_back(It.second);
			}
			return List;
		}
		// 获取一定范围内的
		std::vector<int> FindInRaidus(const KdValue& Center, double Radius) const
		{
			std::vector<int> List;
			FindInRaidusImpl(List, Root, Center, Radius);
			return List;
		}

		const KdValue& Get(int DataIdx) const
		{
			return ValueList[DataIdx];
		}
		const std::vector<KdValue>& Get(const std::vector<int>& DataIdx) const
		{
			std::vector<KdValue> Ret;
			for (auto It : DataIdx)
			{
				Ret.push_back(ValueList[It]);
			}
			return Ret;
		}
		const std::vector<KdValue>& GetValueList() const
		{
			return ValueList;
		}
		
	public:
			void Validate() const
			{
#if KDTREE_DEBUG
				ValidImpl(Root, 0);
#endif
			}

	protected:
		double Get(int DataIdx, int Axis) const
		{
			return ValueList[DataIdx][Axis];
		}
		void LoopDestroy(KdNode*& Node)
		{
			if (Node == nullptr)
			{
				return;
			}
			LoopDestroy(Node->Leaves[0]);
			LoopDestroy(Node->Leaves[1]);
			delete Node;
			Node = nullptr;
		}
		void PushImpl(KdNode*& ParentNode, KdNode* Node, int Depth)
		{
			if (!ParentNode)
			{
				ParentNode = Node;
				Node->Axis = Depth % KdDim;
				return;
			}

			int Axis = ParentNode->Axis;
			Assert(Axis >= 0 && Axis < KdDim);

			if (Get(Node->DataIdx, Axis) < Get(ParentNode->DataIdx, Axis))
			{
				PushImpl(ParentNode->Leaves[0], Node, Depth + 1);
			}
			else
			{
				PushImpl(ParentNode->Leaves[1], Node, Depth + 1);
			}
		}

		void ValidImpl(KdNode* Node, int Depth) const
		{
			if (!Node)
			{
				return;
			}

			int Axis = Depth % KdDim;
			Assert(Node->Axis == Axis);
			ValidImpl(Node->Leaves[0], Depth + 1);
			ValidImpl(Node->Leaves[1], Depth + 1);
		}

		// 复杂度
		int O() const
		{
			auto v = pow(ValueList.size(), (KdDim - 1) * 1.0f / KdDim);
			v = ceil(v) * KdDim;
			return (int)v;
		}

		void FindInRaidusImpl(std::vector<int>& List, KdNode* Node, const KdValue& Center, double Radius) const
		{
			int Counter = 0;
			FindInRaidusImpl(Counter, List, Node, Center, Radius);
			//Assert(Counter <= O());
		}
		void FindInRaidusImpl(int& Counter, std::vector<int>& List, KdNode* Node, const KdValue& Center, double Radius) const
		{
			if (Node == nullptr)
			{
				return;
			}
			Counter++;

			// 如果在距离范围内，命中
			auto& V = Get(Node->DataIdx);
			if (DistSqrt(V, Center) < Abs(Radius))
			{
				List.push_back(Node->DataIdx);
			}

			// 二分查找：找左节点或者右节点
			auto Axis = Node->Axis;
			int LeafIdx = 1;
			if (Center[Axis] < V[Axis])
			{
				LeafIdx = 0;
			}
			FindInRaidusImpl(Counter, List, Node->Leaves[LeafIdx], Center, Radius);

			// 如果发现此节点在目标范围内，那么也要查一下另一个节点
			if (Abs(Center[Axis] - V[Axis]) < Radius)
			{
				FindInRaidusImpl(Counter, List, Node->Leaves[(LeafIdx + 1) % 2], Center, Radius);
			}
		}
		void GetNeareastImpl(KdQueueValue& Q, KdNode* Node, const KdValue& Center) const
		{
			int Counter = 0;
			GetNeareastImpl(Counter, Q, Node, Center);
			Assert(Counter <= O());
		}
		void GetNeareastImpl(int& Counter, KdQueueValue& Q, KdNode* Node, const KdValue& Center) const
		{
			if (!Node)
			{
				return;
			}
			Counter++;

			// 更新最短距离。
			auto NewDist = DistSqrt(Get(Node->DataIdx), Center);
			if (NewDist < Q.first || Q.first < 0)
			{
				Q.first = NewDist;
				Q.second = Node->DataIdx;
			}

			// 在左边，就查询左子树；否则查询右子树
			int Leaf = 1;
			auto Axis = Node->Axis;
			if (Center[Axis] < Get(Node->DataIdx, Axis))
			{
				Leaf = 0;
			}

			GetNeareastImpl(Counter, Q, Node->Leaves[Leaf], Center);
			
			// 如果发现此节点在优先范围内，那么也要查询下另一个子树
			if (Abs(Get(Node->DataIdx, Axis) - Center[Axis]) < Q.first)
			{
				GetNeareastImpl(Counter, Q, Node->Leaves[AnotherLeaf(Leaf)], Center);
			}
		}
		void GetNnkImpl(KdQueue& Q, KdNode* Node, const KdValue& V) const
		{
			int Counter = 0;
			GetNnkImpl(Counter, Q, Node, V);
			Assert(Counter <= O() * (int)Q.MaxCount);
		}
		void GetNnkImpl(int& Counter, KdQueue& Q, KdNode* Node, const KdValue& V) const
		{
			if (Node == nullptr)
			{
				return;
			}
			Counter++;

			// 记录此数据
			auto Dist = DistSqrt(V, Get(Node->DataIdx));
			Q.Push(Dist, Node->DataIdx);

			// 如果在左，就查左子树；否则查右子树
			auto Axis = Node->Axis;
			int Leaf = 1;
			if (V[Axis] < Get(Node->DataIdx, Axis))
			{
				Leaf = 0;
			}
			GetNnkImpl(Counter, Q, Node->Leaves[Leaf], V);

			// 如果距离当前节点，小于存储的最大值，那么继续查找
			auto MaxDist = Q.Back().first;
			if (Abs(V[Axis] - Get(Node->DataIdx, Axis)) < MaxDist)
			{
				GetNnkImpl(Counter, Q, Node->Leaves[AnotherLeaf(Leaf)], V);
			}
		}
	};

}