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

static float MyRand()
{
	//return (rand() - RAND_MAX);
	return ((1.0f * rand() / RAND_MAX) - 1) * 10;
}

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

	// compile error.
	//KdTree tree2 = tree;

	srand(1);
	for (int i = 0; i < 1000; i++)
	{
		KdTree::KdValue v;
		v[0] = MyRand();
		v[1] = MyRand();
		tree.Push(v);
	}
	tree.Validate();

	{
		KdTree::KdValue Center;
		Center[0] = MyRand();
		Center[1] = MyRand();
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
				return tree.DistSqrt(ValueList[Left], Center) < tree.DistSqrt(ValueList[Right], Center);
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
		v[0] = MyRand();
		v[1] = MyRand();
		v[2] = MyRand();
		tree.Push(v);
	}
	tree.Validate();

	{
		KdTree::KdValue Center;
		Center[0] = MyRand();
		Center[1] = MyRand();
		Center[2] = MyRand();
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
				return tree.DistSqrt(ValueList[Left], Center) < tree.DistSqrt(ValueList[Right], Center);
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
			v[j] = MyRand();
		}
		tree.Push(v);
	}
	tree.Validate();

	{
		typename KdTree::KdValue Center;
		for (int i = 0; i < N; i++)
		{
			Center[i] = MyRand();
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
				return tree.DistSqrt(ValueList[Left], Center) < tree.DistSqrt(ValueList[Right], Center);
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
template <typename T, int N>
char(&ArrayCountHelper(const T(&)[N]))[N];
// Number of elements in an array.
#define ARRAY_COUNT( array ) (sizeof(ArrayCountHelper(array))+0) 

void SmokeTest4()
{
	float data[] = { -8.75,-8.75,-12.00,-8.75,-8.75,-7.00,-8.75,-8.75,-2.00,-8.75,-8.75,3.00,-8.75,-8.75,8.00,-8.75,-8.75,13.00,-8.75,-3.75,-17.00,-8.75,-3.75,-12.00,-8.75,-3.75,-7.00,-8.75,-3.75,-2.00,-8.75,-3.75,3.00,-8.75,-3.75,8.00,-8.75,-3.75,13.00,-8.75,-3.75,18.00,-8.75,1.25,-17.00,-8.75,1.25,-12.00,-8.75,1.25,-7.00,-8.75,1.25,-2.00,-8.75,1.25,3.00,-8.75,1.25,8.00,-8.75,1.25,13.00,-8.75,1.25,18.00,-8.75,6.25,-12.00,-8.75,6.25,-7.00,-8.75,6.25,-2.00,-8.75,6.25,3.00,-8.75,6.25,8.00,-8.75,6.25,13.00,-3.75,-8.75,-17.00,-3.75,-8.75,-12.00,-3.75,-8.75,-7.00,-3.75,-8.75,-2.00,-3.75,-8.75,3.00,-3.75,-8.75,8.00,-3.75,-8.75,13.00,-3.75,-8.75,18.00,-3.75,-3.75,-17.00,-3.75,-3.75,-12.00,-3.75,-3.75,-7.00,-3.75,-3.75,-2.00,-3.75,-3.75,3.00,-3.75,-3.75,8.00,-3.75,-3.75,13.00,-3.75,-3.75,18.00,-3.75,1.25,-17.00,-3.75,1.25,-12.00,-3.75,1.25,-7.00,-3.75,1.25,-2.00,-3.75,1.25,3.00,-3.75,1.25,8.00,-3.75,1.25,13.00,-3.75,1.25,18.00,-3.75,6.25,-17.00,-3.75,6.25,-12.00,-3.75,6.25,-7.00,-3.75,6.25,-2.00,-3.75,6.25,3.00,-3.75,6.25,8.00,-3.75,6.25,13.00,-3.75,6.25,18.00,-3.75,11.25,-12.00,-3.75,11.25,-7.00,-3.75,11.25,-2.00,-3.75,11.25,3.00,-3.75,11.25,8.00,-3.75,11.25,13.00,1.25,-8.75,-17.00,1.25,-8.75,-12.00,1.25,-8.75,-7.00,1.25,-8.75,-2.00,1.25,-8.75,3.00,1.25,-8.75,8.00,1.25,-8.75,13.00,1.25,-8.75,18.00,1.25,-3.75,-17.00,1.25,-3.75,-12.00,1.25,-3.75,-7.00,1.25,-3.75,-2.00,1.25,-3.75,3.00,1.25,-3.75,8.00,1.25,-3.75,13.00,1.25,-3.75,18.00,1.25,1.25,-17.00,1.25,1.25,-12.00,1.25,1.25,-7.00,1.25,1.25,-2.00,1.25,1.25,3.00,1.25,1.25,8.00,1.25,1.25,13.00,1.25,1.25,18.00,1.25,6.25,-17.00,1.25,6.25,-12.00,1.25,6.25,-7.00,1.25,6.25,-2.00,1.25,6.25,3.00,1.25,6.25,8.00,1.25,6.25,13.00,1.25,6.25,18.00,1.25,11.25,-12.00,1.25,11.25,-7.00,1.25,11.25,-2.00,1.25,11.25,3.00,1.25,11.25,8.00,1.25,11.25,13.00,6.25,-8.75,-12.00,6.25,-8.75,-7.00,6.25,-8.75,-2.00,6.25,-8.75,3.00,6.25,-8.75,8.00,6.25,-8.75,13.00,6.25,-3.75,-17.00,6.25,-3.75,-12.00,6.25,-3.75,-7.00,6.25,-3.75,-2.00,6.25,-3.75,3.00,6.25,-3.75,8.00,6.25,-3.75,13.00,6.25,-3.75,18.00,6.25,1.25,-17.00,6.25,1.25,-12.00,6.25,1.25,-7.00,6.25,1.25,-2.00,6.25,1.25,3.00,6.25,1.25,8.00,6.25,1.25,13.00,6.25,1.25,18.00,6.25,6.25,-17.00,6.25,6.25,-12.00,6.25,6.25,-7.00,6.25,6.25,-2.00,6.25,6.25,3.00,6.25,6.25,8.00,6.25,6.25,13.00,6.25,6.25,18.00,6.25,11.25,-12.00,6.25,11.25,-7.00,6.25,11.25,-2.00,6.25,11.25,3.00,6.25,11.25,8.00,6.25,11.25,13.00,11.25,-3.75,-12.00,11.25,-3.75,-7.00,11.25,-3.75,-2.00,11.25,-3.75,3.00,11.25,-3.75,8.00,11.25,-3.75,13.00,11.25,1.25,-12.00,11.25,1.25,-7.00,11.25,1.25,-2.00,11.25,1.25,3.00,11.25,1.25,8.00,11.25,1.25,13.00,11.25,6.25,-12.00,11.25,6.25,-7.00,11.25,6.25,-2.00,11.25,6.25,3.00,11.25,6.25,8.00,11.25,6.25,13.00 };

	auto Tag = std::string("template. SmokeTest4");
	SmokeTestTag(Tag.c_str());

	using KdTree = kdtree::KdTree<3>;
	KdTree tree;
	KdTree::KdValue Center;
	Center[0] = -14.964;
	Center[1] = -7.759;
	Center[2] = 24.399;

	for (int i = 0; i < ARRAY_COUNT(data); i += 3)
	{
		KdTree::KdValue V;
		V[0] = data[i + 0];
		V[1] = data[i + 1];
		V[2] = data[i + 2];
		tree.Push(V);
	}
	int id = tree.GetNeareast(Center);
	assert(id >= 0);

	auto& ValueList = tree.GetValueList();
	std::vector<int> SortedList(ValueList.size());

	std::iota(std::begin(SortedList), std::end(SortedList), 0);
	std::sort(std::begin(SortedList), std::end(SortedList), [&](int Left, int Right)
		{
			return tree.DistSqrt(ValueList[Left], Center) < tree.DistSqrt(ValueList[Right], Center);
		});
	assert(id == SortedList[0]);

}

void SmokeTest()
{
	SmokeTest2();
	SmokeTest3();
	SmokeTest4();

	SmokeTestN<2>();
	SmokeTestN<3>();
	SmokeTestN<4>();
	SmokeTestN<5>();
	SmokeTestN<6>();
}