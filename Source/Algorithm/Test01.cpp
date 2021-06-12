#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	const size_t size   = 10000;
	const int    target = 7821;

	std::vector<int> v;
	v.reserve(size);

	for (int i = 0; i < size; ++i)
		v.push_back(i + 1);

	///////////////////////
	//-- Linear search --//
	///////////////////////

	int numComp1 = 0;

	std::find_if(v.begin(), v.end(), [&](int x)
	{
		numComp1++;
		return x == target;
	});

	///////////////////////
	//-- Binary search --//
	///////////////////////

	int numComp2 = 0;

	std::binary_search(v.begin(), v.end(), target, [&](int a, int b)
	{
		numComp2++;
		return a < b;
	});

	std::cout << numComp1 << ' ' << numComp2 << std::endl;

	std::cin.get();
}