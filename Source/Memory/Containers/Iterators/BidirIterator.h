#pragma once

namespace LCN {

	template<class Container>
	class BidirIterator
	{
	public:
		using ValType = typename Container::ValType;
		using PtrType = ValType*;
		using RefType = ValType&;

		using ElementType    = typename Container::ElementType;
		using ElementTypePtr = ElementType*;

	public:
		BidirIterator() = default;

		BidirIterator(const BidirIterator&) = default;

		BidirIterator(ElementTypePtr ptr) :
			m_Ptr(ptr)
		{}

		inline RefType operator*()  { return m_Ptr->GetVal(); }
		inline PtrType operator->() { return m_Ptr->GetPtr(); }

		ElementTypePtr GetRawPtr() { return m_Ptr; }

		bool operator==(const BidirIterator& other) { return other.m_Ptr == m_Ptr; }
		bool operator!=(const BidirIterator& other) { return other.m_Ptr != m_Ptr; }

		BidirIterator& operator++()
		{
			m_Ptr = m_Ptr->GetNext();

			return *this;
		}

		BidirIterator operator++(int)
		{
			ElementTypePtr temp = m_Ptr;

			m_Ptr = m_Ptr->GetNext();

			return BidirIterator(temp);
		}

		BidirIterator& operator--()
		{
			m_Ptr = m_Ptr->GetPrev();

			return *this;
		}

		BidirIterator operator--(int)
		{
			ElementTypePtr temp = m_Ptr;

			m_Ptr = m_Ptr->GetPrev();

			return BidirIterator(temp);
		}

	private:
		ElementTypePtr m_Ptr = nullptr;
	};
}