#include <iostream>
#include <filesystem>
#include <functional>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "../Memory/_MemTracker.h"

namespace fs = std::experimental::filesystem;

const int tabsize = 2;

void DisplayDirTree(const fs::path& filepath, const std::function<void(const fs::directory_entry&)>& action, int _indent = 0)
{
	if (!(fs::exists(filepath) && fs::is_directory(filepath)))
		return;

	std::string indent(tabsize * _indent, ' ');

	for (const auto& entry : fs::directory_iterator(filepath))
	{
		auto filename = entry.path().filename();

		//std::cout << indent << (fs::is_directory(entry.status()) ? "[+] " : "|-> ") << filename << std::endl;

		action(entry);

		if (fs::is_directory(entry.status()))
			DisplayDirTree(entry, action, _indent + 1);
	}
}

int main()
{
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		fs::path path = fs::current_path();

		std::unordered_map<std::string, int> map;

		DisplayDirTree(path, [&](const fs::directory_entry entry)
		{
			if(fs::is_regular_file(entry.status()))
				map[entry.path().extension().string()]++;
		});

		std::cout << "\n---------- Unsorted ----------\n" << std::endl;

		for (const auto& e : map)
			std::cout << e.first << ", " << e.second << std::endl;

		std::cout << "\n---------- Sorted ----------\n" << std::endl;

		using HashMapIterator = std::unordered_map<std::string, int>::iterator;

		std::vector<HashMapIterator> vec;

		vec.reserve(map.size());

		for (auto it = map.begin(); it != map.end(); ++it)
			vec.emplace_back(it);

		std::sort(vec.begin(), vec.end(), [](const HashMapIterator& a, const HashMapIterator& b)
		{
			return a->second > b->second;
		});

		for (const auto& e : vec)
			std::cout << e->first << ", " << e->second << std::endl;

		std::cout << "\n---------- Done ----------" << std::endl;
	}

	std::cin.get();
}