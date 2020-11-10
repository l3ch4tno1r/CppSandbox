#define MEM_TRACKING 0

#if MEM_TRACKING == 1
#include <mutex>
#include <iostream>

#include "Utilities/Source/ErrorHandling.h"

#define DEBUG_MEM 0

#if DEBUG_MEM == 1
#define MEM_LOG(X) std::cout << "Mem debug : "; X
#else
#define MEM_LOG(X)
#endif

class MemTracker
{
public:
	static MemTracker& Get() noexcept
	{
		static MemTracker instance;
		return instance;
	}

private:
	std::mutex m_AllocatedMut;
	size_t     m_Allocated;
	size_t     m_NumAlloc;

	std::mutex m_DeallocatedMut;
	size_t     m_Deallocated;
	size_t     m_NumDealloc;

	MemTracker() :
		m_Allocated(0),
		m_Deallocated(0)
	{}

	~MemTracker()
	{
		std::cout << std::endl;

		std::cout << "Total Memory allocated   : " << m_Allocated << " bytes." << std::endl;
		std::cout << "Total Memory deallocated : " << m_Deallocated << " bytes." << std::endl;

		std::cout << '\n';

		std::cout << "Total number of allocations   : " << m_NumAlloc << std::endl;
		std::cout << "Total number of deallocations : " << m_NumDealloc << std::endl;

		std::cout << "\nDelta = " << m_Allocated - m_Deallocated << std::endl;

		std::cin.get();
	}

	void Allocate(size_t size)
	{
		std::unique_lock<std::mutex> lock(m_AllocatedMut);

		MEM_LOG(std::cout << "Allocated " << size << " bytes." << std::endl);

		m_Allocated += size;
		++m_NumAlloc;
	}

	void Deallocate(size_t size)
	{
		std::unique_lock<std::mutex> lock(m_DeallocatedMut);

		MEM_LOG(std::cout << "Deallocated " << size << " bytes." << std::endl);

		m_Deallocated += size;
		++m_NumDealloc;
	}

	friend void* ::operator new(size_t);
	friend void* ::operator new[](size_t);
	friend void  ::operator delete(void*, size_t);
	friend void  ::operator delete[](void*, size_t);
};

void* operator new(size_t size)
{
	MemTracker::Get().Allocate(size);

	return malloc(size);
}

void* operator new[](size_t size)
{
	MemTracker::Get().Allocate(size);

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	ASSERT(ptr != nullptr);

	MemTracker::Get().Deallocate(size);

	free(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	ASSERT(ptr != nullptr);

	MemTracker::Get().Deallocate(size);

	free(ptr);
}
#endif