#pragma once

#include "Iterators/BidirIterator.h"

namespace LCN {

	template<typename T>
	class LinkedList
	{
	public:
		using ValType = T;
		using PtrType = ValType*;
		using RefType = ValType&;

		using Iterator = BidirIterator<LinkedList>;

		class ListNode
		{
		public:
			template<typename ...Args>
			ListNode(Args&&...args) :
				m_Next(nullptr),
				m_Prev(nullptr),
				m_Data(std::forward<Args>(args)...)
			{}

			inline ListNode* GetNext() { return m_Next; }
			inline ListNode* GetPrev() { return m_Prev; }

			inline RefType GetVal() { return  m_Data; }
			inline PtrType GetPtr() { return &m_Data; }

		private:
			ListNode* m_Next = nullptr;
			ListNode* m_Prev = nullptr;

			ValType m_Data;

			friend class LinkedList;
		};

		using ElementType = ListNode;

	public:
		LinkedList() = default;

		~LinkedList()
		{
			ListNode* current = m_First;

			while (current)
			{
				ListNode* next = current->m_Next;

				delete current;

				current = next;
			}
		}

		size_t Size() const { return m_Size; }

		template<typename ...Args>
		RefType EmplaceBack(Args&&...args)
		{
			if (!m_First && !m_Last)
			{
				m_First = new ListNode(std::forward<Args>(args)...);
				m_Last  = m_First;
			}
			else
			{
				m_Last->m_Next = new ListNode(std::forward<Args>(args)...);
				m_Last->m_Next->m_Prev = m_Last;
				m_Last = m_Last->m_Next;
			}

			++m_Size;

			return m_Last->m_Data;
		}

		template<typename ...Args>
		Iterator Emplace(Iterator pos, Args&&...args)
		{
			ListNode* curr = pos.GetRawPtr();
			ListNode* prev = curr->m_Prev;

			ListNode* toinsert = new ListNode(std::forward<Args>(args)...);

			prev->m_Next     = toinsert;
			toinsert->m_Next = curr;

			curr->m_Prev     = toinsert;
			toinsert->m_Prev = prev;

			++m_Size;

			return Iterator(toinsert);
		}

		void PopFront()
		{
			if (!m_First)
				return;

			ListNode* next = m_First->m_Next;

			delete m_First;

			m_First         = next;
			m_First->m_Prev = nullptr;

			--m_Size;
		}

		void PopBack()
		{
			if (!m_Last)
				return;

			ListNode* prev = m_Last->m_Prev;

			delete m_Last;

			m_Last         = prev;
			m_Last->m_Next = nullptr;

			--m_Size;
		}

		Iterator Remove(Iterator pos)
		{
			if (pos == this->End())
				throw std::out_of_range("Iterator out of range.");

			ListNode* curr = pos.GetRawPtr();

			if (curr == m_First)
			{
				this->PopFront();
				return Iterator(m_First);
			}

			if (curr == m_Last)
			{
				this->PopBack();
				return this->End();
			}

			ListNode* prev = curr->m_Prev;
			ListNode* next = curr->m_Next;

			delete curr;

			if(prev) prev->m_Next = next;
			if(next) next->m_Prev = prev;

			--m_Size;

			return Iterator(next);
		}

		Iterator Begin() { return Iterator(m_First); }
		Iterator End()   { return Iterator(nullptr); }

	private:
		ListNode* m_First = nullptr;
		ListNode* m_Last  = nullptr;

		size_t m_Size = 0;
	};
}