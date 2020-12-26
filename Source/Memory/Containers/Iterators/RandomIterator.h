#pragma once

namespace LCN {

	template<class ContiguousContainer>
	class RandomIterator
	{
	public:
		using ValType = typename ContiguousContainer::ValType;
		using PtrType = ValType* ;
		using RefType = ValType& ;

	public:
		RandomIterator(PtrType ptr) :
			m_Ptr(ptr)
		{}

		RandomIterator() :
			m_Ptr(nullptr)
		{}

		RandomIterator(const RandomIterator& other) :
			m_Ptr(other.m_Ptr)
		{}

		PtrType operator->() { return  m_Ptr; }
		RefType operator*()  { return *m_Ptr; }
		RefType operator[](size_t i) { return *(m_Ptr + i); }

		PtrType GetRawPtr() { return m_Ptr; }

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
		bool operator< (const RandomIterator& other) const { return m_Ptr <  other.m_Ptr; }
		bool operator<=(const RandomIterator& other) const { return m_Ptr <= other.m_Ptr; }
		bool operator> (const RandomIterator& other) const { return m_Ptr >  other.m_Ptr; }
		bool operator>=(const RandomIterator& other) const { return m_Ptr >= other.m_Ptr; }

	private:
		PtrType m_Ptr = nullptr;
	};
}