#pragma once

#include "Iterators/BidirIterator.h"

namespace LCN
{
	template<typename T>
	class BinarySearchTree
	{
	public:
		using ValType = T;
		using PtrType = ValType*;
		using RefType = ValType&;

		using Iterator = BidirIterator<BinarySearchTree>;

		class TreeNode
		{
		public:
			enum
			{
				Left,
				Right
			};

			TreeNode() :
				m_LeftChild(nullptr),
				m_RightChild(nullptr),
				m_Parent(nullptr)
			{}

			TreeNode(const ValType& data) :
				m_LeftChild(nullptr),
				m_RightChild(nullptr),
				m_Parent(nullptr),
				m_Data(data)
			{}

			~TreeNode()
			{
				delete m_Children[Left];
				//std::cout << m_Data << " deleted !" << std::endl;
				delete m_Children[Right];
			}
						
			[[deprecated]]
			void DisplayRecursive(size_t indent = 0) const
			{
				if (m_Children[Left])
					m_Children[Left]->DisplayRecursive(indent + 1);

				std::cout << indent << "\t|" << std::string(1 * indent, '-') << m_Data << std::endl;

				if (m_Children[Right])
					m_Children[Right]->DisplayRecursive(indent + 1);
			}

			bool IsLeaf() const { return !m_LeftChild && !m_RightChild; }

			enum NavDir
			{
				NLeft  = -1,
				NRight = 1
			};

			TreeNode* Navigate(NavDir direction)
			{
				size_t child1 = (direction == NavDir::NRight ? TreeNode::Right : TreeNode::Left);
				size_t child2 = (child1 + 1) % 2;

				if (!m_Children[child1])
				{
					TreeNode* curr = this;
					TreeNode* next = this->m_Parent;

					while (next && next->m_Children[child1] == curr)
					{
						curr = next;
						next = next->m_Parent;
					}

					return next;
				}
				else
				{
					TreeNode* next = this->m_Children[child1];

					while (next && next->m_Children[child2])
						next = next->m_Children[child2];

					return next;
				}
			}

			TreeNode* GetNext() { return Navigate(NavDir::NRight); }
			TreeNode* GetPrev() { return Navigate(NavDir::NLeft); }

			inline RefType GetVal() { return  m_Data; }
			inline PtrType GetPtr() { return &m_Data; }

		private:
			union
			{
				struct
				{
					TreeNode* m_LeftChild;
					TreeNode* m_RightChild;
				};
				TreeNode* m_Children[2];
			};

			TreeNode* m_Parent;

			ValType m_Data;

			friend class BinarySearchTree;
		};

		using ElementType = TreeNode;

	public:
		BinarySearchTree() :
			m_Root(nullptr)
		{}

		~BinarySearchTree()
		{
			delete m_Root;
		}

		void Insert(const ValType& value)
		{
			TreeNode* elementToInsert = new TreeNode(value);

			if (!m_Root)
			{
				m_Root = elementToInsert;
				++m_Size;
				return;
			}

			TreeNode* currentNode = m_Root;

			while (true)
			{
				if (value == currentNode->m_Data)
					break;

				size_t child = (value < currentNode->m_Data ? TreeNode::Left : TreeNode::Right);

				if (!currentNode->m_Children[child])
				{
					currentNode->m_Children[child] = elementToInsert;
					elementToInsert->m_Parent      = currentNode;

					++m_Size;

					break;
				}
				else
					currentNode = currentNode->m_Children[child];
			}
		}

		size_t Size() const { return m_Size; }

		[[deprecated]]
		void Display() const
		{
			if (m_Root)
				m_Root->DisplayRecursive();
		}

		Iterator Begin()
		{
			if (!m_Root)
				return Iterator(nullptr);

			TreeNode* current = m_Root;

			while(current->m_LeftChild)
				current = current->m_LeftChild;

			return Iterator(current);
		}

		Iterator End()
		{
			return Iterator(nullptr);
		}

		Iterator Find(const ValType& value)
		{
			TreeNode* currentNode = m_Root;

			do
			{
				if (!currentNode)
					return this->End();

				if (value == currentNode->m_Data)
					return Iterator(currentNode);

				size_t child = (value < currentNode->m_Data ? TreeNode::Left : TreeNode::Right);

				currentNode = currentNode->m_Children[child];
			}
			while (true);
		}

	private:
		size_t    m_Size;
		TreeNode* m_Root;
	};
}