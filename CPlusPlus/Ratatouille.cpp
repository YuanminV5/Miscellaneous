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

struct Materials
{
	int noOfRequiredIngredients;
	vector<int> neededAmount;

	int noOfPackages;
	vector<vector<int>> packageWeights;
};

int getMaxKit(Materials& m)
{
	return 0;
}
int main()
{
	int testCount = 0;
	Materials materials[100];

	cin >> testCount;
	for (uint8 i = 0; i < testCount; i++)
	{
		cin >> materials[i].noOfRequiredIngredients >> materials[i].noOfPackages;

		for (uint8 no = 0; no < materials[i].noOfRequiredIngredients; no++)
		{
			int amount;
			cin >> amount;
			materials[i].neededAmount.push_back(amount);
		}

		for (uint8 noOfIngredients = 0; noOfIngredients < materials[i].noOfRequiredIngredients; noOfIngredients++)
		{
			vector<int> packageWeight;
			for (uint8 noOfPackages = 0; noOfPackages < materials[i].noOfPackages; noOfPackages++)
			{
				int amount;
				cin >> amount;
				packageWeight.push_back(amount);
			}

			materials[i].packageWeights.push_back(packageWeight);
		}
	}

	for (uint8 i = 0; i < testCount; i++)
	{
		cout << "Case #" << getMaxKit(materials[i]) << ": "<<endl;
	}

	return -1;
}
