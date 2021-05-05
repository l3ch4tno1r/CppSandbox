#include <iostream>
#include <shared_mutex>
#include <atomic>

#include <LCN_Math/Source/Geometry/Geometry.h>

template<class Owner, typename Type>
class Property
{
public:
	virtual const Type& Get() const = 0;

	virtual void Set(const Type&) {}
	virtual void Set(Type&&) {}

	Property& operator=(const Type& val)
	{
		this->Set(val);

		return *this;
	}

	Property& operator=(Type&& val)
	{
		this->Set(std::move(val));

		return *this;
	}

	operator const Type& ()
	{
		return this->Get();
	}

protected:
	Owner& m_Target;

	friend Owner;

private:
	Property(Owner& target) :
		m_Target(target)
	{}
};

//----------------------//

class Lockable
{
public:
	void lock()          { std::cout << "Locked !" << std::endl; }
	void lock_shared()   { std::cout << "Locked shared !" << std::endl; }
	void unlock()        { std::cout << "Unlocked !" << std::endl; }
	void unlock_shared() { std::cout << "Unlocked shared !" << std::endl; }
};

class Test
{
public:
	Test()            { std::cout << "Default ctor" << std::endl; }
	Test(const Test&) { std::cout << "Copy ctor" << std::endl; }
	Test(Test&&)      { std::cout << "Move ctor" << std::endl; }
	~Test()			  { std::cout << "dtor" << std::endl; }
};

Lockable g_Lock;
Test g_Object;

Test Copy()
{
	std::shared_lock lock(g_Lock);

	return g_Object;
}

//----------------------//

class AABB2Df
{
public:
	using HVectorType = LCN::HVector2Df;
	using RVectorType = LCN::Vector2Df;

	AABB2Df(const RVectorType& min, const RVectorType& max) :
		m_Min(min, 1.0f),
		m_Max(max, 1.0f)
	{}

	const HVectorType& Min() const { return m_Min; }
	      HVectorType& Min()       { return m_Min; }

	const HVectorType& Max() const { return m_Max; }
	      HVectorType& Max()       { return m_Max; }

private:
	class WidthProperty
	{
	public:
		WidthProperty& operator=(float width)
		{
			m_Target.m_Max.x() = m_Target.m_Min.x() + width;

			return *this;
		}

		operator float() const
		{
			return m_Target.m_Max.x() - m_Target.m_Min.x();
		}

	private:
		WidthProperty(AABB2Df& target) :
			m_Target(target)
		{}

		AABB2Df& m_Target;

		friend AABB2Df;
	};

	friend WidthProperty;

public:
	WidthProperty Width() { return WidthProperty(*this); }

private:
	HVectorType m_Min;
	HVectorType m_Max;
};

int main()
{
	AABB2Df aabb({ 0, 0 }, { 3, 2 });

	std::cout << aabb.Min() << std::endl;
	std::cout << aabb.Max() << std::endl;

	std::cout << aabb.Width() << std::endl;

	aabb.Width() = 5;

	std::cout << aabb.Min() << std::endl;
	std::cout << aabb.Max() << std::endl;

	float w = aabb.Width();

	Test copy = Copy();

	std::atomic<int> test;

	int a = test;

	std::cin.get();
}