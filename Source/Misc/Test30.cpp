#include <iostream>
#include <limits>

void FindClosestSum(int* arr1, size_t size1, int* arr2, size_t size2, int target, int& idx1, int& idx2)
{
	idx1 = idx2 = 0;

	int deltamin = std::numeric_limits<int>::max();

	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < size2; j++)
		{
			int delta = std::abs(target - (arr1[i] + arr2[j]));

			if (delta < deltamin)
			{
				deltamin = delta;

				idx1 = i;
				idx2 = j;
			}
		}
	}
}

int main()
{
	int arr1[] = { -1, 3, 8,  2, 9,  5 };
	int arr2[] = {  4, 1, 2, 10, 5, 20 };

	int idx1, idx2;

	FindClosestSum(arr1, 6, arr2, 6, 24, idx1, idx2);

	std::cout << '(' << idx1 << ", " << idx2 << ") -> " << arr1[idx1] + arr2[idx2] << std::endl;

	std::cin.get();
}