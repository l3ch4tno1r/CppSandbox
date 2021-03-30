#pragma once

#include <utility>
#include <initializer_list>

#include "Iterators/RandomIterator.h"

namespace LCN {

	template<typename T>
	class DynamicArray
	{
	public:
		using ValType = T;
		using PtrType = ValType*;
		using RefType = ValType&;

		using Iterator = RandomIterator<DynamicArray>;

	public:
		DynamicArray()
		{
			Realloc(2);
		}

		DynamicArray(size_t size)
		{
			Realloc(size);
		}

		DynamicArray(const std::initializer_list<ValType>& list)
		{
			Realloc(list.size());

			for (const ValType& i : list)
				this->EmplaceBack(i);
		}

		~DynamicArray()
		{
			Clear();
			::operator delete(m_Data, m_Capacity * sizeof(ValType));

			m_Capacity = 0;
			m_Size = 0;

			m_Data = nullptr;
		}

		size_t Size() const
		{
			return m_Size;
		}

		RefType operator[](size_t i)
		{
			return m_Data[i];
		}

		const RefType operator[](size_t i) const
		{
			return m_Data[i];
		}

		RefType PushBack(const RefType value)
		{
			if (m_Size >= m_Capacity)
				Realloc(m_Capacity + m_Capacity / 2);

			m_Data[m_Size] = value;

			return m_Data[m_Size++];
		}

		template<typename ...Args>
		RefType EmplaceBack(Args&& ...args)
		{
			if (m_Size >= m_Capacity)
				Realloc(m_Capacity + m_Capacity / 2);

			new(m_Data + m_Size) ValType(std::forward<Args>(args)...);

			return m_Data[m_Size++];
		}

		void PopBack()
		{
			if (m_Size > 0)
				m_Data[--m_Size].~ValType();
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~ValType();

			m_Size = 0;
		}

		Iterator Begin() { return Iterator(m_Data); }
		Iterator End() { return Iterator(m_Data + m_Size); }

		void Erase(const Iterator& it)
		{
			MoveMemBlockBackward(it.m_Ptr, it.m_Ptr + 1);
		}

		void Erase(const Iterator& first, const Iterator& last)
		{
			MoveMemBlockBackward(first.m_Ptr, last.m_Ptr);
		}

		void Insert(const Iterator& it, const RefType value)
		{
			MoveMemBlockForward(it.m_Ptr + 1, it.m_Ptr);

			*(it.m_Ptr) = value;
		}

		void Insert(const Iterator& it, ValType&& value)
		{
			MoveMemBlockForward(it.m_Ptr + 1, it.m_Ptr);

			*(it.m_Ptr) = std::move(value);
		}

		void Insert(const Iterator& it, const std::initializer_list<ValType>& list)
		{
			PtrType ptr = MoveMemBlockForward(it.m_Ptr + list.size(), it.m_Ptr);

			for (const auto& e : list)
				new(ptr++) ValType(e);
		}

	private:
		void MoveMemBlockBackward(PtrType dest, PtrType src)
		{
			ASSERT(dest >= m_Data && dest < m_Data + m_Size);
			ASSERT(src >= m_Data && src < m_Data + m_Size);
			ASSERT(dest < src);

			for (PtrType ptr = dest; ptr < src; ++ptr)
				ptr->~ValType();

			size_t delta = src - dest;
			m_Size -= delta;

			for (PtrType ptr = dest; ptr < m_Data + m_Size; ptr++)
				*ptr = std::move(*(ptr + delta));
		}

		PtrType MoveMemBlockForward(PtrType dest, PtrType src)
		{
			// TODO : Finish this part !
			//ASSERT(dest >= m_Data && dest < m_Data + m_Size);
			ASSERT(dest >= m_Data);
			ASSERT(src >= m_Data && src <= m_Data + m_Size);
			ASSERT(dest > src);

			size_t delta = dest - src;

			if (m_Size + delta > m_Capacity)
			{
				size_t firsttodest = dest - m_Data;
				size_t firsttosrc = src - m_Data;

				Realloc(m_Size + delta);

				dest = m_Data + firsttodest;
				src = m_Data + firsttosrc;
			}

			PtrType first = m_Data;
			PtrType last  = m_Data + m_Size - 1;
			PtrType end   = m_Data + m_Size;

			m_Size += delta;

			PtrType temp = (dest >= end ? dest : end);

			for (PtrType ptr = m_Data + m_Capacity - 1; ptr >= temp; --ptr)
				new(ptr) ValType(std::move(*(ptr - delta)));

			for (PtrType ptr = last; ptr >= dest; --ptr)
				*ptr = std::move(*(ptr - delta));

			temp = std::min(dest, end);

			for (PtrType ptr = src; ptr < temp; ++ptr)
				ptr->~ValType();

			return src;
		}

	private:
		PtrType m_Data = nullptr;
		size_t  m_Size = 0;
		size_t  m_Capacity = 0;

		void Realloc(size_t capacity)
		{
			PtrType temp = (PtrType)::operator new(capacity * sizeof(ValType));

			if (capacity < m_Size)
				m_Size = capacity;

			for (size_t i = 0; i < m_Size; ++i)
				new(temp + i) ValType(std::move(m_Data[i]));

			for (size_t i = 0; i < m_Size; ++i)
				m_Data[i].~ValType();

			::operator delete(m_Data, m_Capacity * sizeof(ValType));

			m_Data = temp;
			m_Capacity = capacity;
		}
	};
}
