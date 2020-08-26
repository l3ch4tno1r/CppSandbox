#pragma once

#include "Utilities/ErrorHandling.h"

namespace LCN {

	template<class RandomIt>
	void MakeHeap(RandomIt first, RandomIt last)
	{
		int size = last.GetRawPtr() - first.GetRawPtr();

		ASSERT(size > 0);

		for (int i = size - 1; i >= 0; --i)
		{
			int parentId = (i - 1) / 2;

			while (parentId >= 0 && first[parentId] < first[i])
			{
				auto temp = first[parentId];

				first[parentId] = first[i];
				first[i] = temp;

				parentId = (parentId - 1) / 2;
			}
		}
	}
}