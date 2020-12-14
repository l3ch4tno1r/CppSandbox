#include "BaseMemoryAllocator.h"

#include <cstdlib>

void* BaseMemoryAllocator::Allocate(size_t size)
{
	return std::malloc(size);
}

void BaseMemoryAllocator::Deallocate(void* ptr)
{
	std::free(ptr);
}

void BaseMemoryAllocator::Deallocate(void * ptr, size_t size)
{
	std::free(ptr);
}