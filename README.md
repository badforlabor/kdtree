# kdtree
kdtree c++实现



### 详细测试

参见smoke.cpp中



### 使用方法

**只有一个头文件**，引入kdtree.h即可。

```
	#include "kdtree.h"


	using KdTree = kdtree::KdTree<3>;
	KdTree tree;
	
	srand(1);		
	for (int i = 0; i < 1000; i++)
	{
		KdTree::KdValue v;
		v[0] = rand();
		v[1] = rand();
		v[2] = rand();
		
		// 构建
		tree.Push(v);
	}
	
	KdTree::KdValue Center;
	Center[0] = rand();
	Center[1] = rand();
	Center[2] = rand();
	double Radius = 1000;
	
	// 查找
    auto R1 = tree.FindInRaidus(Center, Radius);
    auto R2 = tree.GetNeareast(Center);
    auto R3 = tree.GetNnk(Center, 10);
```

