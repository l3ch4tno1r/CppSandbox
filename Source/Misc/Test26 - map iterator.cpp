#include <iostream>
#include <set>
#include <algorithm>
#include <random>
#include <vector>

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

int main()
{
	std::vector<int> v;

	v.reserve(100);

	for (int i = 0; i < v.capacity(); ++i)
		v.emplace_back(i);

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(v.begin(), v.end(), g);

	std::set<int> s;

	SEPARATOR(1);

	for (int i : v)
		s.insert(i);

	SEPARATOR(2);

	for (int i : s)
		std::cout << i << std::endl;

	SEPARATOR(3);

	for (auto it = s.begin(); it != s.end(); ++it)
		std::cout << *it << std::endl;

	std::cin.get();
}