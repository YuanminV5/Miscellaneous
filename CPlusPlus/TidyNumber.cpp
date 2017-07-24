#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cmath>
using namespace std;

typedef unsigned char uint8;
typedef unsigned long long int uint64;


bool boIsTidy(uint64 number, uint64& newNumber)
{
	// Single digit number
	if (number < 10)
	{
		return true;
	}

	vector<uint8> digits;
	
	// Get all digits in the number
	while (number > 0)
	{
		digits.push_back(number % 10);
		number = number / 10;
	}

	reverse(digits.begin(), digits.end());

	uint8 temp = digits[0];

	// Find first digit that breaks the ascending order
	auto pos = find_if(digits.begin(), digits.end(), [&](uint8 i) -> bool
	{
		if (i < temp)
		{
			return true;
		}
		else
		{
			temp = i;
			return false;
		}
	});

	if (pos == digits.end())
	{
		// Digits are in ascending order, the number is tidy
		return true;
	}

	// Find first element that's bigger than digit at pos


	uint8 index = pos - digits.begin();

	assert(index > 0);

	uint8 posBigger = index - 1;
	uint8 i = 1;
	while (posBigger - 1 >= 0 && digits[posBigger - 1] >= digits[posBigger])
	{
		posBigger -= 1;
	}
	digits[posBigger] = digits[posBigger] - 1;

	for (uint8 i = posBigger + 1; i < digits.size(); i++)
	{
		digits[i] = 9;
	}

	newNumber = 0;
	uint8 length = digits.size();
	for (uint8 i = 0; i < length; i++)
	{
		newNumber += digits[i] * pow(10, length - 1 - i);
	}

	return false;
}

uint64 getLastTidyNumber(uint64 number)
{
	uint64 i = number;

	while (i > 0)
	{
		uint64 old = number;
		if (boIsTidy(i, number))
		{
			break;
		}

		if (old != number)
		{
			i = number;
		}
		else
		{
			i--;
		}
	}

	return i;
}
int main()
{
	int testCount = 0;
	uint64 numbers[100];

	cin >> testCount;
	for (int i = 0; i < testCount; i++)
	{
		cin >> numbers[i];
	}

	for (int i = 0; i < testCount; i++)
	{
		cout << "Case #" << i + 1 << ": ";
		auto t = getLastTidyNumber(numbers[i]);

		cout << t;

		cout << endl;
	}

	return -1;
}


