#pragma once

namespace LCN {

template<class ContiguousContainer>
	class RandomIterator
	{
	public:
		using ValueType     = typename ContiguousContainer::ValueType;
		using PointerType   = ValueType* ;
		using ReferenceType = ValueType& ;

	public:
		PointerType m_Ptr;

		RandomIterator(PointerType ptr) :
			m_Ptr(ptr)
		{}

		RandomIterator() :
			m_Ptr(nullptr)
		{}

		RandomIterator(const RandomIterator& other) :
			m_Ptr(other.m_Ptr)
		{}

		PointerType   operator->() { return m_Ptr; }
		ReferenceType operator*() { return *m_Ptr; }
		ReferenceType operator[](size_t i) { return *(m_Ptr + i); }

		PointerType GetRawPtr() { return m_Ptr; }

		RandomIterator& operator++()
		{
			++m_Ptr;

			return *this;
		}

		RandomIterator operator++(int) { return RandomIterator(m_Ptr++); }

		RandomIterator& operator--()
		{
			--m_Ptr;

			return *this;
		}

		RandomIterator operator--(int) { return RandomIterator(m_Ptr--); }

		RandomIterator operator+(size_t i) { return RandomIterator(m_Ptr + i); }
		RandomIterator operator-(size_t i) { return RandomIterator(m_Ptr - i); }

		bool operator==(const RandomIterator& other) const { return m_Ptr == other.m_Ptr; }
		bool operator!=(const RandomIterator& other) const { return m_Ptr != other.m_Ptr; }
		bool operator< (const RandomIterator& other) const { return m_Ptr < other.m_Ptr; }
		bool operator<=(const RandomIterator& other) const { return m_Ptr <= other.m_Ptr; }
		bool operator> (const RandomIterator& other) const { return m_Ptr > other.m_Ptr; }
		bool operator>=(const RandomIterator& other) const { return m_Ptr >= other.m_Ptr; }
	};
}