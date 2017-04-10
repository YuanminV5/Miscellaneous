#include <iostream>
#include <string>
#include <set>
using namespace std;

typedef unsigned char uint8;

struct CakeRow
{
	string S;
	int K;
};

bool doFlip(string& sides, int count)
{
	if (sides.length() < count)
	{
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		sides[i] == '+' ? sides[i] = '-' : sides[i] = '+';
	}

	return true;
}

int getFlipCount(const CakeRow& row)
{
	string sides = row.S;
	int size = row.K;

	int flipCount = 0;
	bool notPossible = false;

	while (sides.size() >= size)
	{
		int i = 0;
		while (sides[i] == '+' && i < sides.length())
		{
			i++;
		}
		sides.erase(0, i);

		if (sides.size() == 0) 
		{
			break;
		}

		if (doFlip(sides, size))
		{
			flipCount++;
		}
		else
		{
			notPossible = true;
			break;
		}
	}

	if (notPossible)
	{
		return -1;
	}

	return flipCount;
}

int main()
{
	int rowCount = 0;
	CakeRow rows[100];

	cin >> rowCount;
	for (int i = 0; i < rowCount; i++)
	{
		cin >> rows[i].S >> rows[i].K;
	}

	for (int i = 0; i < rowCount; i++)
	{
		cout << "Case #" << i + 1 << ": ";
		auto t = getFlipCount(rows[i]);
		if (t >= 0)
		{
			cout << t;
		}
		else
		{
			cout << "IMPOSSIBLE";
		}

		cout << endl;
	}

	return -1;
}


