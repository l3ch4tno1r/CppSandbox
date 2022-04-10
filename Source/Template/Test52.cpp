#include <iostream>
#include <vector>

////////////////////////
//-- From statement --//
////////////////////////

template<class _ContainerType>
class FromStmt
{
public:
	using ConstIterator = typename _ContainerType::const_iterator;

	ConstIterator begin() const { return m_Target.begin(); }
	ConstIterator end()   const { return m_Target.end(); }

private:
	FromStmt(const _ContainerType& target) :
		m_Target{ target }
	{}

	template<class _ContainerType>
	friend FromStmt<_ContainerType> From(const _ContainerType&);

private:
	const _ContainerType& m_Target;
};

template<class _ContainerType>
FromStmt<_ContainerType> From(const _ContainerType& target)
{
	return { target };
}

/////////////////////////
//-- Where statement --//
/////////////////////////

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5 };

	auto range = From(vec);

	for (const auto& i : range)
		std::cout << i << '\n';

	std::cin.get();
}