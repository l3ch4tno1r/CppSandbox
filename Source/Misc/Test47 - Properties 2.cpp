#include <iostream>

template<class OwnerType, class DerivedType>
class Property
{
public:
	friend OwnerType;

protected:
	Property(OwnerType& owner) :
		m_Owner(owner)
	{}

	const OwnerType& Owner() const { return m_Owner; }
	      OwnerType& Owner()       { return m_Owner; }

private:
	OwnerType& m_Owner;
};

#define PROPERTY(OwnerType, Name, ValType, GetterName, SetterName)\
class Name ## Property : public Property<OwnerType, Name ## Property>\
{\
public:\
	using Base = Property<OwnerType, Name ## Property>;\
	friend OwnerType;\
	inline Name ## Property& operator=(const ValType& value)\
	{\
		this->Owner().SetterName(value);\
		return *this;\
	}\
	inline operator ValType() const { return this->Owner().GetterName(); }\
private:\
	Name ## Property(OwnerType& owner) :\
		Base(owner)\
	{}\
};\
Name ## Property Name() { return Name ## Property(*this); }

class Test
{
public:
	int  GetMin() const { return m_Min; }
	void SetMin(int min) { m_Min = min; }
	PROPERTY(Test, Min, int, GetMin, SetMin);
	
	int  GetMax() const { return m_Max; }
	void SetMax(int max) { m_Max = max; }
	PROPERTY(Test, Max, int, GetMax, SetMax);

	int  GetWidth() const { return m_Max - m_Min; }
	void SetWidth(int width) { m_Max = m_Min + width; }
	PROPERTY(Test, Width, int, GetWidth, SetWidth);

private:
	int m_Min, m_Max;
};

int main()
{
	Test test;

	test.Min() = 4;

	test.Width() = 3;

	std::cout << test.Max() << std::endl;

	test.Max() = 10;

	std::cout << test.Width() << std::endl;

	std::cin.get();
}