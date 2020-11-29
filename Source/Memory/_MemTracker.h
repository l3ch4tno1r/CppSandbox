#pragma once

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
};