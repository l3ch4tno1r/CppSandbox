#pragma once

#include <exception>

template<class MemoryAllocationPolicy>
class MemoryManager : public MemoryAllocationPolicy
{
public:
	using Base = MemoryAllocationPolicy;

	static MemoryManager& Get() noexcept
	{
		static MemoryManager instance;
		return instance;
	}

private:
	MemoryManager() = default;
	~MemoryManager() = default;

	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;

	inline void* Allocate(size_t size)
	{
		if (!size)
			throw std::exception("Cannot allocate 0 byte.");

		void* ptr = Base::Allocate(size);

		if (!ptr)
			throw std::exception("Allocation failled.");

		return ptr;
	}

	inline void Deallocate(void* ptr)
	{
		Base::Deallocate(ptr);
	}

	inline void Deallocate(void* ptr, size_t size)
	{
		Base::Deallocate(ptr, size);
	}

	friend void* ::operator new(size_t);
	friend void* ::operator new[](size_t);
	friend void  ::operator delete(void*);
	friend void  ::operator delete(void*, size_t);
	friend void  ::operator delete[](void*);
	friend void  ::operator delete[](void*, size_t);
};