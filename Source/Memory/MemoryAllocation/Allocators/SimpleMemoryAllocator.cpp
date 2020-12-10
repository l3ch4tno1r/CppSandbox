#include "SimpleMemoryAllocator.h"

#include <cstdlib>

void* SimpleMemoryAllocator::Allocate(size_t size)
{
	return std::malloc(size);
}

void SimpleMemoryAllocator::Deallocate(void* ptr)
{
	std::free(ptr);
}

void SimpleMemoryAllocator::Deallocate(void * ptr, size_t size)
{
	std::free(ptr);
}