#pragma once

#include <initializer_list>

#include "RandomIterator.h"

namespace LCN {

	template<typename T>
	class DynamicArray
	{
	private:
		T*       m_Data = nullptr;
		uint32_t m_Size = 0;
		uint32_t m_Capacity = 0;

		void Realloc(size_t capacity)
		{
			T* temp = (T*)::operator new(capacity * sizeof(T));

			if (capacity < m_Size)
				m_Size = capacity;

			for (size_t i = 0; i < m_Size; ++i)
				new(temp + i) T(std::move(m_Data[i]));

			for (size_t i = 0; i < m_Size; ++i)
				m_Data[i].~T();

			::operator delete(m_Data, m_Capacity * sizeof(T));

			m_Data = temp;
			m_Capacity = capacity;
		}

	public:
		using ValueType = T;
		using Iterator = RandomIterator<DynamicArray>;

		DynamicArray()
		{
			Realloc(2);
		}

		DynamicArray(size_t size)
		{
			Realloc(size);
		}

		DynamicArray(const std::initializer_list<T> list)
		{
			Realloc(list.size());

			for (const T& i : list)
				this->EmplaceBack(i);
		}

		~DynamicArray()
		{
			Clear();
			::operator delete(m_Data, m_Capacity * sizeof(T));

			m_Capacity = 0;
			m_Size = 0;

			m_Data = nullptr;
		}

		size_t Size() const
		{
			return m_Size;
		}

		T& operator[](size_t i)
		{
			return m_Data[i];
		}

		const T& operator[](size_t i) const
		{
			return m_Data[i];
		}

		T& PushBack(const T& value)
		{
			if (m_Size >= m_Capacity)
				Realloc(m_Capacity + m_Capacity / 2);

			m_Data[m_Size] = value;

			return m_Data[m_Size++];
		}

		template<typename ...Args>
		T& EmplaceBack(Args&& ...args)
		{
			if (m_Size >= m_Capacity)
				Realloc(m_Capacity + m_Capacity / 2);

			new(m_Data + m_Size) T(std::forward<Args>(args)...);

			return m_Data[m_Size++];
		}

		void PopBack()
		{
			if (m_Size > 0)
				m_Data[--m_Size].~T();
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~T();

			m_Size = 0;
		}

		Iterator Begin()
		{
			return Iterator(m_Data);
		}

		Iterator End()
		{
			return Iterator(m_Data + m_Size);
		}

	private:
		void MoveMemBlockBackward(T* dest, T* src)
		{
			ASSERT(dest >= m_Data && dest < m_Data + m_Size);
			ASSERT(src >= m_Data && src < m_Data + m_Size);
			ASSERT(dest < src);

			for (T* ptr = dest; ptr < src; ++ptr)
				ptr->~T();

			size_t delta = src - dest;
			m_Size -= delta;

			for (T* ptr = dest; ptr < m_Data + m_Size; ptr++)
				*ptr = std::move(*(ptr + delta));
		}

		T* MoveMemBlockForward(T* dest, T* src)
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

			T* first = m_Data;
			T* last = m_Data + m_Size - 1;
			T* end = m_Data + m_Size;

			m_Size += delta;

			T* temp = (dest >= end ? dest : end);

			for (T* ptr = m_Data + m_Capacity - 1; ptr >= temp; --ptr)
				new(ptr) T(std::move(*(ptr - delta)));

			for (T* ptr = last; ptr >= dest; --ptr)
				*ptr = std::move(*(ptr - delta));

			temp = std::min(dest, end);

			for (T* ptr = src; ptr < temp; ++ptr)
				ptr->~T();

			return src;
		}

	public:
		void Erase(const Iterator& it)
		{
			MoveMemBlockBackward(it.m_Ptr, it.m_Ptr + 1);
		}

		void Erase(const Iterator& first, const Iterator& last)
		{
			MoveMemBlockBackward(first.m_Ptr, last.m_Ptr);
		}

		void Insert(const Iterator& it, const T& value)
		{
			MoveMemBlockForward(it.m_Ptr + 1, it.m_Ptr);

			*(it.m_Ptr) = value;
		}

		void Insert(const Iterator& it, T&& value)
		{
			MoveMemBlockForward(it.m_Ptr + 1, it.m_Ptr);

			*(it.m_Ptr) = std::move(value);
		}

		void Insert(const Iterator& it, const std::initializer_list<T>& list)
		{
			T* ptr = MoveMemBlockForward(it.m_Ptr + list.size(), it.m_Ptr);

			for (const auto& e : list)
				new(ptr++) T(e);
		}
	};
}
