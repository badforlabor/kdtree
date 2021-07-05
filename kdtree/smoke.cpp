/**,
 * Auth :   liubo
 * Date :   2021/07/05 15:44
 * Comment: 冒烟测试  
 */

#include <iostream>
#include <random>
#include "kdtree.h"
#include <assert.h>
#include <numeric>
#include <string>

class SmokeTestTag
{
public:
	SmokeTestTag(const char* Tag) : TagStr(Tag)
	{
		std::cout << "-------------------" << " " << TagStr.c_str();
		std::cout << " Start" << " " << "-------------------" << std::endl;
	}
	~SmokeTestTag()
	{
		std::cout << "-------------------" << " " << TagStr.c_str();
		std::cout << " End" << " " << "-------------------" << std::endl;
	}

private:
	std::string TagStr;
};

void SmokeTest2()
{
	SmokeTestTag("SmokeTest2");

	using KdTree = kdtree::KdTree<2>;
	KdTree tree;

	srand(1);
	for (int i = 0; i < 1000; i++)
	{
		KdTree::KdValue v;
		v[0] = rand();
		v[1] = rand();
		tree.Push(v);
	}
	tree.Validate();

	{
		KdTree::KdValue Center;
		Center[0] = rand();
		Center[1] = rand();
		double Radius = 1000;

		auto R1 = tree.FindInRaidus(Center, Radius);
		auto R2 = tree.GetNeareast(Center);
		auto R3 = tree.GetNnk(Center, 10);

		assert(R1.size() > 0);
		assert(R2 >= 0);
		assert(R3.size() > 0);

		auto& ValueList = tree.GetValueList();

		std::vector<int> SortedList(ValueList.size());

		std::iota(std::begin(SortedList), std::end(SortedList), 0);
		std::sort(std::begin(SortedList), std::end(SortedList), [&](int Left, int Right) 
			{
				return tree.DistSqr(ValueList[Left], Center) < tree.DistSqr(ValueList[Right], Center);
			});

		for (size_t i = 0; i < R1.size(); i++)
		{
			assert(std::find(R1.begin(), R1.end(), SortedList[i]) != R1.end());
		}

		assert(R2 == SortedList[0]);

		for (size_t i = 0; i < R3.size(); i++)
		{
			assert(std::find(R3.begin(), R3.end(), SortedList[i]) != R3.end());
		}
	}

	{
		auto& ValueList = tree.GetValueList();
		double Radius = 0.1f;

		for (auto It : ValueList)
		{
			auto R1 = tree.FindInRaidus(It, Radius);
			auto R2 = tree.GetNeareast(It);
			auto R3 = tree.GetNnk(It, 10);
			assert(R1.size() > 0);
			assert(R2 >= 0);
			assert(R3.size() > 0);
		}
	}
}

void SmokeTest3()
{
	SmokeTestTag("SmokeTest3");

	using KdTree = kdtree::KdTree<3>;
	KdTree tree;

	srand(1);
	for (int i = 0; i < 1000; i++)
	{
		KdTree::KdValue v;
		v[0] = rand();
		v[1] = rand();
		v[2] = rand();
		tree.Push(v);
	}
	tree.Validate();

	{
		KdTree::KdValue Center;
		Center[0] = rand();
		Center[1] = rand();
		Center[2] = rand();
		double Radius = 1000;

		auto R1 = tree.FindInRaidus(Center, Radius);
		auto R2 = tree.GetNeareast(Center);
		auto R3 = tree.GetNnk(Center, 10);

		assert(R1.size() > 0);
		assert(R2 >= 0);
		assert(R3.size() > 0);

		auto& ValueList = tree.GetValueList();

		std::vector<int> SortedList(ValueList.size());

		std::iota(std::begin(SortedList), std::end(SortedList), 0);
		std::sort(std::begin(SortedList), std::end(SortedList), [&](int Left, int Right)
			{
				return tree.DistSqr(ValueList[Left], Center) < tree.DistSqr(ValueList[Right], Center);
			});

		for (size_t i = 0; i < R1.size(); i++)
		{
			assert(std::find(R1.begin(), R1.end(), SortedList[i]) != R1.end());
		}

		assert(R2 == SortedList[0]);

		for (size_t i = 0; i < R3.size(); i++)
		{
			assert(std::find(R3.begin(), R3.end(), SortedList[i]) != R3.end());
		}
	}

	{
		auto& ValueList = tree.GetValueList();
		double Radius = 0.1f;

		for (auto It : ValueList)
		{
			auto R1 = tree.FindInRaidus(It, Radius);
			auto R2 = tree.GetNeareast(It);
			auto R3 = tree.GetNnk(It, 10);
			assert(R1.size() > 0);
			assert(R2 >= 0);
			assert(R3.size() > 0);
		}
	}
}

template<int N>
void SmokeTestN()
{
	auto Tag = std::string("template. SmokeTest") + std::to_string(N);
	SmokeTestTag(Tag.c_str());

	using KdTree = kdtree::KdTree<N>;
	KdTree tree;

	srand(1);
	for (int i = 0; i < 1000; i++)
	{
		typename KdTree::KdValue v;
		for (int j = 0; j < N; j++)
		{
			v[j] = rand();
		}
		tree.Push(v);
	}
	tree.Validate();

	{
		typename KdTree::KdValue Center;
		for (int i = 0; i < N; i++)
		{
			Center[i] = rand();
		}
		double Radius = 1000;

		auto R1 = tree.FindInRaidus(Center, Radius);
		auto R2 = tree.GetNeareast(Center);
		auto R3 = tree.GetNnk(Center, 10);

		assert(R1.size() > 0);
		assert(R2 >= 0);
		assert(R3.size() > 0);

		auto& ValueList = tree.GetValueList();

		std::vector<int> SortedList(ValueList.size());

		std::iota(std::begin(SortedList), std::end(SortedList), 0);
		std::sort(std::begin(SortedList), std::end(SortedList), [&](int Left, int Right)
			{
				return tree.DistSqr(ValueList[Left], Center) < tree.DistSqr(ValueList[Right], Center);
			});

		for (size_t i = 0; i < R1.size(); i++)
		{
			assert(std::find(R1.begin(), R1.end(), SortedList[i]) != R1.end());
		}

		assert(R2 == SortedList[0]);

		for (size_t i = 0; i < R3.size(); i++)
		{
			assert(std::find(R3.begin(), R3.end(), SortedList[i]) != R3.end());
		}
	}

	{
		auto& ValueList = tree.GetValueList();
		double Radius = 0.1f;

		for (auto It : ValueList)
		{
			auto R1 = tree.FindInRaidus(It, Radius);
			auto R2 = tree.GetNeareast(It);
			auto R3 = tree.GetNnk(It, 10);
			assert(R1.size() > 0);
			assert(R2 >= 0);
			assert(R3.size() > 0);
		}
	}
}

void SmokeTest()
{
	SmokeTest2();
	SmokeTest3();

	SmokeTestN<2>();
	SmokeTestN<3>();
	SmokeTestN<4>();
	SmokeTestN<5>();
	SmokeTestN<6>();
}