#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cmath>
using namespace std;

typedef unsigned char uint8;
typedef unsigned long long int uint64;

struct stall
{
	uint64 N;
	uint64 K;
};

struct result
{
	uint64 max;
	uint64 min;
};

result* getResult(stall stall)
{
	result t;
	uint64 left = 0;
	uint64 right = stall.N - 1;

	for (uint8 i = 1; i <= stall.K / 2; i++)
	{

	}
	return nullptr;
}



int main()
{
	int testCount = 0;
	stall stalls[100];

	cin >> testCount;
	for (int i = 0; i < testCount; i++)
	{
		cin >> stalls[i].N >> stalls[i].K;
	}

	for (int i = 0; i < testCount; i++)
	{
		cout << "Case #" << i + 1 << ": ";
		auto t = getResult(stalls[i]);

		cout << t->max << " " << t->min;

		cout << endl;
		delete t;
	}

	return -1;
}


