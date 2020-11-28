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
	public:
		~ScopeBasedSession() { Get().EndSession(); }

	private:
		ScopeBasedSession() { Get().BeginSession(); }

		friend class MemTracker;
	};

	ScopeBasedSession BeginScopeBasedSession() { return ScopeBasedSession(); }
};