#include <iostream>
#include <vector>

template<typename _ValType>
class BinaryTree
{
public:
	using ValType = _ValType;

	template<typename ... Args>
	BinaryTree(Args&& ... args) :
		m_Data{ std::forward<Args>(args)... }
	{}

	class Node;

	Node Root() const;

private:
	std::vector<ValType> m_Data;
};

template<typename _ValType>
class BinaryTree<_ValType>::Node
{
	using BinTree = BinaryTree<_ValType>;
	friend BinTree;

public:
	using ValType      = _ValType;
	using ConstRefType = const ValType&;
	using ConstPtrType = const ValType*;

public:
	ConstRefType operator*()  const { return m_Target->m_Data[m_Index]; }

	ConstPtrType operator->() const { return &(m_Target->m_Data[m_Index]); }

	Node& operator=(const Node& other)
	{
		m_Target = other.m_Target;
		m_Index  = other.m_Index;

		return *this;
	}

	operator bool() const { return m_Index < m_Target->m_Data.size(); }

	Node Left()  const { return { *m_Target, 2 * m_Index + 1 }; }

	Node Right() const { return { *m_Target, 2 * m_Index + 2 }; }

private:
	Node(const BinTree& tree, size_t index = 0) :
		m_Target{ &tree },
		m_Index{ index }
	{}

private:
	const BinTree* m_Target;
	size_t m_Index;
};

template<typename _ValType>
typename BinaryTree<_ValType>::Node BinaryTree<_ValType>::Root() const
{
	return { *this };
}

int main()
{
	BinaryTree<int> tree = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	auto node = tree.Root();

	while (node)
	{
		std::cout << *node << '\n';

		node = node.Left();
	}

	std::cin.get();
}