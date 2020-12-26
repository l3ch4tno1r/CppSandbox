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

		class LinkedListElement
		{
		public:
			template<typename ...Args>
			LinkedListElement(Args&&...args) :
				m_Next(nullptr),
				m_Prev(nullptr),
				m_Data(std::forward<Args>(args)...)
			{}

			inline LinkedListElement* GetNext() { return m_Next; }
			inline LinkedListElement* GetPrev() { return m_Prev; }

			inline RefType GetVal() { return  m_Data; }
			inline PtrType GetPtr() { return &m_Data; }

		private:
			LinkedListElement* m_Next = nullptr;
			LinkedListElement* m_Prev = nullptr;

			ValType m_Data;

			friend class LinkedList;
		};

		using ElementType = LinkedListElement;

	public:
		LinkedList() = default;

		~LinkedList()
		{
			LinkedListElement* current = m_First;

			while (current)
			{
				LinkedListElement* next = current->m_Next;

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
				m_First = new LinkedListElement(std::forward<Args>(args)...);
				m_Last  = m_First;
			}
			else
			{
				m_Last->m_Next = new LinkedListElement(std::forward<Args>(args)...);
				m_Last->m_Next->m_Prev = m_Last;
				m_Last = m_Last->m_Next;
			}

			++m_Size;

			return m_Last->m_Data;
		}

		template<typename ...Args>
		Iterator Emplace(Iterator pos, Args&&...args)
		{
			LinkedListElement* curr = pos.GetRawPtr();
			LinkedListElement* prev = curr->m_Prev;

			LinkedListElement* toinsert = new LinkedListElement(std::forward<Args>(args)...);

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

			LinkedListElement* next = m_First->m_Next;

			delete m_First;

			m_First         = next;
			m_First->m_Prev = nullptr;

			--m_Size;
		}

		void PopBack()
		{
			if (!m_Last)
				return;

			LinkedListElement* prev = m_Last->m_Prev;

			delete m_Last;

			m_Last         = prev;
			m_Last->m_Next = nullptr;

			--m_Size;
		}

		Iterator Remove(Iterator pos)
		{
			if (pos == this->End())
				throw std::out_of_range("Iterator out of range.");

			LinkedListElement* curr = pos.GetRawPtr();

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

			LinkedListElement* prev = curr->m_Prev;
			LinkedListElement* next = curr->m_Next;

			delete curr;

			if(prev) prev->m_Next = next;
			if(next) next->m_Prev = prev;

			--m_Size;

			return Iterator(next);
		}

		Iterator Begin() { return Iterator(m_First); }
		Iterator End()   { return Iterator(nullptr); }

	private:
		LinkedListElement* m_First = nullptr;
		LinkedListElement* m_Last  = nullptr;

		size_t m_Size = 0;
	};
}