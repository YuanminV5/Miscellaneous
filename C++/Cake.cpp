#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cmath>
#include <set>
using namespace std;

typedef unsigned char uint8;
typedef unsigned long long int uint64;

struct Cake
{
	int row;
	int column;
	char data[25][25] = {'\0'};
	
};

void divideCake(Cake& cake)
{
	int tiles = cake.row*cake.column;
	int tile = 0;

	set<char> initials;
	while (tile < tiles)
	{
		uint8 row = tile / cake.column;
		uint8 column = tile % cake.column;

		if (cake.data[row][column] == '?' || initials.find(cake.data[row][column]) != initials.end())
		{
			tile++;
			continue;
		}

		initials.insert(cake.data[row][column]);

		// Flood left
		uint8 leftmostPos = column;
		uint8 rightmostPos = column;
		for (int leftColumn = column - 1; leftColumn >= 0; leftColumn--)
		{
			if (cake.data[row][leftColumn] == '?')
			{
				leftmostPos = leftColumn;
				cake.data[row][leftColumn] = cake.data[row][column];
			}
			else
			{
				break;
			}
		}

		// Flood right
		for (int rightColumn = column +1 ; rightColumn < cake.column; rightColumn++)
		{
			if (cake.data[row][rightColumn] == '?')
			{
				rightmostPos = rightColumn;
				cake.data[row][rightColumn] = cake.data[row][column];
				tile = row*cake.column + rightmostPos;
			}
			else
			{
				break;
			}
		}

		// Flood up
		for (int upRow = row - 1; upRow >= 0; upRow--)
		{
			bool OK = true;
			for (uint8 i = leftmostPos; i <= rightmostPos;i++)
			{
				if (cake.data[upRow][i] != '?')
				{
					OK = false;
					break;
				}
			}

			if (!OK)
			{
				break;
			}

			for (int i = leftmostPos; i <= rightmostPos;i++)
			{
				cake.data[upRow][i] = cake.data[row][column];
			}
		}

		// Flood down
		for (uint8 downRow = row + 1; downRow < cake.row; downRow++)
		{
			bool OK = true;
			for (uint8 i = leftmostPos; i <= rightmostPos;i++)
			{
				if (cake.data[downRow][i] != '?')
				{
					OK = false;
					break;
				}
			}

			if (!OK)
			{
				break;
			}

			for (uint8 i = leftmostPos; i <= rightmostPos;i++)
			{
				cake.data[downRow][i] = cake.data[row][column];
				//tile = downRow*cake.column + rightmostPos;
			}
		}
	
		tile++;
	}
}

void printCake(const Cake& cake)
{
	for (uint8 p = 0; p < cake.row; p++)
	{
		for (uint8 q = 0; q < cake.column; q++)
		{
			cout << cake.data[p][q];
		}
		cout << endl;
	}
}
int main()
{
	int testCount = 0;
	Cake cakes[100];

	cin >> testCount;
	for (uint8 i = 0; i < testCount; i++)
	{
		cin >> cakes[i].row >> cakes[i].column;

		int rowCount = cakes[i].row;
		for (uint8 p = 0; p < rowCount; p++)
		{
			cin >> cakes[i].data[p];
		}
	}

	for (uint8 i = 0; i < testCount; i++)
	{
		divideCake(cakes[i]);
		cout << "Case #" << i + 1 << ": "<<endl;
		printCake(cakes[i]);
		cout << endl;
	}

	return -1;
}
