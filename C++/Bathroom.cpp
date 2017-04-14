#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cmath>
using namespace std;

typedef unsigned char uint8;
typedef unsigned long long int uint64;

struct BathRoom
{
	uint64 N;
	uint64 K;
};

struct Result
{
	uint64 max;
	uint64 min;
};

struct LongestEmptyRange
{
	uint64 startPos;
	uint64 consecutiveSpaceCount;
};

LongestEmptyRange *getEmptyRange(const vector<uint64> &stalls)
{
	uint64 longestConsecutiveSpaceCount = 0;
	uint64 longestConsecutiveStartPos = 0;

	uint64 i = 0;

	while (i < stalls.size())
	{
		if (stalls[i] == 0)
		{
			uint64 pos = i;
			uint64 count = 1;

			uint64 j = i + 1;
			for (; j < stalls.size() && stalls[j] == 0; j++)
			{
				count++;
			}

			if (count > longestConsecutiveSpaceCount)
			{
				longestConsecutiveSpaceCount = count;
				longestConsecutiveStartPos = i;
			}
			i = j;

			continue;
		}

		i++;
	}

	return new LongestEmptyRange
	{
		longestConsecutiveStartPos,
		longestConsecutiveSpaceCount
	};
}

Result *getResultBruteForce(BathRoom bathroom)
{
	vector<uint64> stalls(bathroom.N + 2);
	stalls[0] = 1;
	stalls[stalls.size() - 1] = 1;

	for (uint64 k = 1; k < bathroom.K; k++)
	{
		auto p = getEmptyRange(stalls);

		auto pos = (p->startPos +(p->startPos + p->consecutiveSpaceCount - 1)) / 2 ;

		stalls[pos] = 1;
	}

	auto p = getEmptyRange(stalls);

	auto endPos = p->startPos + p->consecutiveSpaceCount - 1;
	auto pos = (p->startPos + endPos) / 2;

	uint64 ls = pos - p->startPos;
	uint64 rs = endPos - pos;
	return  new Result
	{
		ls < rs ? rs : ls,
		ls < rs ? ls : rs
	};
}

int main()
{
	int testCount = 0;
	BathRoom bathrooms[100];

	cin >> testCount;
	for (uint8 i = 0; i < testCount; i++)
	{
		cin >> bathrooms[i].N >> bathrooms[i].K;
	}

	for (uint8 i = 0; i < testCount; i++)
	{
		cout << "Case #" << i + 1 << ": ";
		auto t = getResultBruteForce(bathrooms[i]);

		cout << t->max << " " << t->min;

		cout << endl;
		delete t;
	}

	return -1;
}
