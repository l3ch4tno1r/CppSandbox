#define MEM_TRACKING 1

#if MEM_TRACKING == 1
#include <mutex>
#include <iostream>
#include <atomic>

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
	static MemTracker& Get() noexcept;

	void BeginSession();
	void EndSession();

	size_t NumAlloc()   const;
	size_t NumDeAlloc() const;

	class ScopeBasedSession
	{
	private:
		ScopeBasedSession();

	public:
		~ScopeBasedSession();

	private:
		size_t m_AllocAtBegining;
		size_t m_DeallocAtBegining;

		friend class MemTracker;
	};

	ScopeBasedSession BeginScopeBasedSession();

private:
	std::mutex m_AllocatedMut;
	size_t     m_Allocated;
	size_t     m_NumAlloc;

	std::mutex m_DeallocatedMut;
	size_t     m_Deallocated;
	size_t     m_NumDealloc;

	std::atomic_bool m_SessionRun = false;

	MemTracker() :
		m_Allocated(0),
		m_Deallocated(0),
		m_NumAlloc(0),
		m_NumDealloc(0)
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
		if (!m_SessionRun)
			return;

		std::unique_lock<std::mutex> lock(m_AllocatedMut);

		MEM_LOG(std::cout << "Allocated " << size << " bytes." << std::endl);

		m_Allocated += size;
		++m_NumAlloc;
	}

	void Deallocate(size_t size)
	{
		if (!m_SessionRun)
			return;

		std::unique_lock<std::mutex> lock(m_DeallocatedMut);

		MEM_LOG(std::cout << "Deallocated " << size << " bytes." << std::endl);

		m_Deallocated += size;
		++m_NumDealloc;
	}

	friend void* ::operator new(size_t);
	friend void* ::operator new[](size_t);
	friend void  ::operator delete(void*);
	friend void  ::operator delete(void*, size_t);
	friend void  ::operator delete[](void*);
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

void operator delete(void* ptr)
{
	ASSERT(ptr != nullptr);

	MemTracker::Get().Deallocate(0);

	free(ptr);
}

void operator delete(void* ptr, size_t size)
{
	ASSERT(ptr != nullptr);

	MemTracker::Get().Deallocate(size);

	free(ptr);
}

void operator delete[](void* ptr)
{
	ASSERT(ptr != nullptr);

	MemTracker::Get().Deallocate(0);

	free(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	ASSERT(ptr != nullptr);

	MemTracker::Get().Deallocate(size);

	free(ptr);
}

MemTracker& MemTracker::Get() noexcept
{
	static MemTracker instance;
	return instance;
}

void MemTracker::BeginSession()
{
	m_SessionRun = true;
}

void MemTracker::EndSession()
{
	m_SessionRun = false;
}

size_t MemTracker::NumAlloc() const
{
	return m_NumAlloc;
}

size_t MemTracker::NumDeAlloc() const
{
	return m_NumDealloc;
}

MemTracker::ScopeBasedSession::ScopeBasedSession()
{
	m_AllocAtBegining   = MemTracker::Get().NumAlloc();
	m_DeallocAtBegining = MemTracker::Get().NumDeAlloc();

	MemTracker::Get().BeginSession();

	std::cout << "Memory tracking session started...\n" << std::endl;
}

MemTracker::ScopeBasedSession::~ScopeBasedSession()
{
	size_t deltaalloc   = MemTracker::Get().NumAlloc()   - m_AllocAtBegining;
	size_t deltadealloc = MemTracker::Get().NumDeAlloc() - m_DeallocAtBegining;

	MemTracker::Get().EndSession();

	std::cout << "\nMemory tracking session ended : " << std::endl;
	std::cout << " - Number of allocations   : " << deltaalloc << std::endl;
	std::cout << " - Number of deallocations : " << deltadealloc << '\n' << std::endl;
}

MemTracker::ScopeBasedSession MemTracker::BeginScopeBasedSession()
{
	return ScopeBasedSession();
}
#endif
