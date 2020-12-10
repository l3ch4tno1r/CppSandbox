#pragma once

#include "..\MemoryManager.h"

class SimpleMemoryAllocator
{
protected:
	SimpleMemoryAllocator() = default;
	~SimpleMemoryAllocator() = default;

	SimpleMemoryAllocator(const SimpleMemoryAllocator&) = delete;
	SimpleMemoryAllocator& operator=(const SimpleMemoryAllocator&) = delete;

	void* Allocate(size_t size);

	void Deallocate(void* ptr);

	void Deallocate(void* ptr, size_t size);
};

typedef MemoryManager<SimpleMemoryAllocator> MemManager;