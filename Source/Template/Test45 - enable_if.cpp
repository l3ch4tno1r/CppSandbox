#include <iostream>
#include <type_traits>
#include <iterator>
#include <vector>
#include <list>

template<typename _IntegralType>
typename std::enable_if<std::is_integral_v<_IntegralType>>::type
Print(const _IntegralType i)
{
	std::cout << i << '\n';
}

template<typename _IteratorType>
constexpr bool IsAtLeastRandomIt = std::is_base_of_v<
	std::random_access_iterator_tag,
	std::iterator_traits<_IteratorType>::iterator_category>;

template<typename _RandomIt>
typename std::enable_if<IsAtLeastRandomIt<_RandomIt>>::type
PrintMiddleElem(_RandomIt it1, _RandomIt it2)
{
	size_t diff = it2 - it1;

	std::cout << *(it1 + diff / 2) << '\n';
}

template<typename _ForwardIt>
typename std::enable_if<!IsAtLeastRandomIt<_ForwardIt>>::type
PrintMiddleElem(_ForwardIt it1, _ForwardIt it2)
{
	_ForwardIt midIt = it1;
	size_t count = 1;

	while (++it1 != it2)
	{
		if (count++ % 2 == 0)
			++midIt;
	}

	std::cout << *midIt << '\n';
}

int main()
{
	Print(5);
	//Print(5.0f);
	Print('a');

	std::vector<int> vec  = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::list<int>   list = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	PrintMiddleElem(vec.begin(),  vec.end());
	PrintMiddleElem(list.begin(), list.end());

	std::cin.get();
}