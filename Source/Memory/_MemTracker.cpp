#include <mutex>
#include <iostream>

#define DEBUG_MEM 0

#if DEBUG_MEM == 1
#define LOG(X) X
#else
#define LOG(X)
#endif

class MemTracker
{
private:
	std::mutex allocated_mut;
	size_t allocated;

	std::mutex deallocated_mut;
	size_t deallocated;

public:
	MemTracker() :
		allocated(0),
		deallocated(0)
	{}

	~MemTracker()
	{
		std::cout << std::endl;

		std::cout << "Total Memory allocated   : " << allocated << " bytes." << std::endl;
		std::cout << "Total Memory deallocated : " << deallocated << " bytes." << std::endl;

		std::cout << "\nDelta = " << allocated - deallocated << std::endl;

		std::cin.get();
	}

	void Allocate(size_t size)
	{
		std::unique_lock<std::mutex> lock(allocated_mut);

		LOG(std::cout << "Allocated " << size << " bytes." << std::endl);

		allocated += size;
	}

	void Deallocate(size_t size)
	{
		std::unique_lock<std::mutex> lock(deallocated_mut);

		LOG(std::cout << "Deallocated " << size << " bytes." << std::endl);

		deallocated += size;
	}
};

MemTracker tracker;

void* operator new(size_t size)
{
	tracker.Allocate(size);

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	tracker.Deallocate(size);

	free(ptr);
}