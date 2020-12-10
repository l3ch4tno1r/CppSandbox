#include "Allocators\SimpleMemoryAllocator.h"

void* operator new(size_t size)
{
	return MemManager::Get().Allocate(size);
}

void* operator new[](size_t size)
{
	return MemManager::Get().Allocate(size);
}

void operator delete(void* ptr)
{
	MemManager::Get().Deallocate(ptr);
}

void operator delete(void* ptr, size_t size)
{
	MemManager::Get().Deallocate(ptr, size);
}

void operator delete[](void* ptr)
{
	MemManager::Get().Deallocate(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	MemManager::Get().Deallocate(ptr, size);
}