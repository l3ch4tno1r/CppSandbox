#include <iostream>
#include <array>

#include <LCN_Math/Source/Geometry/Geometry.h>

/*
template<typename T, size_t Dim>
class UnitVectors
{
public:
	using HVectorType = LCN::HVectorND<T, Dim>;

	static const std::array<HVectorType, Dim>& Vectors()
	{
		static std::array<HVectorType, Dim> vectors;
		return vectors;
	}
};
*/

//----------------------------//

template <unsigned ...Args>
struct XArrayData
{
	static const int Values[sizeof...(Args)];
};

template<unsigned N, unsigned ...Args>
struct _XArrayGenerator
{
	using Xdata = typename _XArrayGenerator<N - 1, N, Args...>::Xdata;
};

template<unsigned ...Args>
struct _XArrayGenerator<1, Args...>
{
	using Xdata = typename XArrayData<1, Args...>;
};

template<unsigned N>
struct XArray
{
	using Xdata = typename _XArrayGenerator<N>::Xdata;
};

template <unsigned ...Args>
const int XArrayData<Args...>::Values[sizeof...(Args)] = { Args... };

//----------------------------//

template<class VectorType, size_t ... Args>
struct UnitVectorData
{
	static const VectorType UnitVectors[sizeof...(Args)];
};

template<class VectorType, size_t N, size_t ... Args>
struct IdxGenerator
{
	using Data = typename IdxGenerator<VectorType, N - 1, N, Args...>::Data;
};

template<class VectorType, size_t ... Args>
struct IdxGenerator<VectorType, 0, Args...>
{
	using Data = typename UnitVectorData<VectorType, Args...>;
};

template<class VectorType>
struct UnitVectors
{
	using Data = typename IdxGenerator<VectorType, VectorType::Dim>::Data;
};

template<class VectorType>
VectorType f(size_t i)
{
	return VectorType(i);
}

template<class VectorType, size_t ... Args>
const VectorType UnitVectorData<VectorType, Args...>::UnitVectors[sizeof...(Args)] = { f<VectorType>(Args - 1)... };

//UnitVectorData<LCN::HVector3Df>::

int main()
{
	for (int i = 0; i < 3; ++i)
		std::cout << XArray<3>::Xdata::Values[i] << std::endl;

	//IdxGenerator<LCN::HVector3Df, 1, 2>::Data::UnitVectors;

	for(const auto& uv : UnitVectors<LCN::HVector3Df>::Data::UnitVectors)
		std::cout << uv << std::endl;

	for (const auto& uv : UnitVectors<LCN::HVector2Df>::Data::UnitVectors)
		std::cout << uv << std::endl;

	for (const auto& uv : UnitVectors<LCN::Vector3Df>::Data::UnitVectors)
		std::cout << uv << std::endl;

	//IdxGenerator<LCN::HVector3Df, LCN::HVector3Df::Dim>::Data

	//UnitVectors<LCN::HVector3Df>::Data::UnitVectors[0];

	//std::cout << UnitVectorData<LCN::HVector3Df>::UnitVectors[0] << std::endl;

	std::cin.get();
}