#pragma once

#include <utility>

#include "Source/ErrorHandling.h"

namespace LCN {

	template<class RandomIt>
	void MakeHeap(RandomIt first, RandomIt last)
	{
		int size = last.GetRawPtr() - first.GetRawPtr();

		ASSERT(size > 0);

		for (int parentId = (size - 2) / 2; parentId >= 0; --parentId)
		{
			size_t leftChildId  = 2 * parentId + 1;
			size_t rightChildId = 2 * parentId + 2;
			size_t nodeToSwapId;

			if (rightChildId >= size)
				nodeToSwapId = leftChildId;
			else
				nodeToSwapId = (first[leftChildId] > first[rightChildId] ? leftChildId : rightChildId);

			if (first[nodeToSwapId] > first[parentId])
				std::swap(first[parentId], first[nodeToSwapId]);
		}

	}
}