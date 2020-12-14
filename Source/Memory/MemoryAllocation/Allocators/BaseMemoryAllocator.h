#pragma once

#include "..\MemoryManager.h"

class BaseMemoryAllocator
{
protected:
	BaseMemoryAllocator() = default;
	~BaseMemoryAllocator() = default;

	BaseMemoryAllocator(const BaseMemoryAllocator&) = delete;
	BaseMemoryAllocator& operator=(const BaseMemoryAllocator&) = delete;

	void* Allocate(size_t size);

	void Deallocate(void* ptr);

	void Deallocate(void* ptr, size_t size);
};