// CountingSheep.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <set>
using namespace std;

typedef unsigned char uint8;
const std::set<unsigned char> all = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

void getDigitSet(unsigned long number, set<uint8>& digits)
{
	while (number > 0)
	{
		digits.insert(number % 10);
		number = number / 10;
	}
}
int getLastBeforeSleep(long seed)
{
	if (seed == 0)
	{
		return -1;
	}

	set<uint8> digits;
	int i = 0;
	bool insomnia = false;
	int oldSize = 0;
	int newSize = 0;
	do
	{
		getDigitSet(seed*i, digits);
		i++;
	} while (oldSize = digits.size() < 10);

	if (insomnia)
	{
		return -1;
	}

	if (i > 0)
	{
		return seed*(i - 1);
	}

	return 0;
}

int main()
{
	int count = 0;
	long pickedNumbers[100];

	cin >> count;
	for (int i = 0; i < count; i++)
	{
		cin >> pickedNumbers[i];
	}

	for (int i = 0; i < 100; i++)
	{
		cout << "Case #" << i + 1 << ": ";
		auto t = getLastBeforeSleep(pickedNumbers[i]);
		if (t > 0)
		{
			cout << t;
		}
		else
		{
			cout << "INSOMNIA";
		}

		cout << endl;
	}

	return -1;
}


