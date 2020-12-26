#pragma once

#include <utility>

#include "Iterators/RandomIterator.h"

namespace LCN {

	template<typename T, size_t N>
	class StaticArray
	{
	public:
		using ValType = T;
		using PtrType = ValType*;
		using RefType = ValType&;

		using Iterator = RandomIterator<StaticArray>;

		RefType operator[](size_t i) { return tab[i]; }
		const RefType operator[](size_t i) const { return tab[i]; }

		constexpr size_t Size() const { return N; }

		Iterator Begin()
		{
			return Iterator(tab);
		}

		Iterator End()
		{
			return Iterator(tab + N);
		}

	private:
		T tab[N];
	};
}