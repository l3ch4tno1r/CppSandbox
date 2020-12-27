#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

#include "Containers/BinarySearchTree.h"

#define SEPARATOR(X) std::cout << "\n----------- " << X << " -----------" << std::endl

int main()
{
	{
		std::vector<int> vec;

		vec.reserve(20);

		for (int i = 0; i < vec.capacity(); ++i)
			vec.emplace_back(i + 1);

		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(vec.begin(), vec.end(), g);

		for (int i : vec)
			std::cout << i << std::endl;

		SEPARATOR(1);

		LCN::BinarySearchTree<int> tree;

		for (auto i : vec)
			tree.Insert(i);

		for (auto it = tree.Begin(); it != tree.End(); ++it)
			std::cout << *it << std::endl;

		SEPARATOR(2);

		auto it2 = tree.Find(50);

		if (it2 != tree.End())
			std::cout << *it2 << std::endl;
		else
			std::cout << "Element not found." << std::endl;

		SEPARATOR(3);

		auto it3 = tree.Find(10);

		while (it3 != tree.End())
			std::cout << *(it3--) << std::endl;
	}

	std::cin.get();
}