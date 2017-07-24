#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cmath>
#include <list>
using namespace std;

typedef unsigned char uint8;
typedef unsigned long long int uint64;

struct HorseState
{
    double pos;
    double maxSpeed;
};

struct Race
{
    double totalDistance;
    int horseNumber;
    vector<HorseState> horses;
};

double getMaxSpeed(const Race& race)
{
    list<double> remainingTime;

    for (auto horse : race.horses)
    {
        double time = (race.totalDistance - horse.pos) / horse.maxSpeed;
        remainingTime.push_back(time);
    }

    remainingTime.sort();

    double maxTime = remainingTime.back();

    return race.totalDistance/ maxTime;
}

int main()
{
	int testCount = 0;
	Race races[100];

	cin >> testCount;
	for (int i = 0; i < testCount; i++)
	{
		cin >> races[i].totalDistance;
        cin >> races[i].horseNumber;
          
        for (int j = 0; j < races[i].horseNumber;j++)
        {   
            HorseState horseState;
            cin >> horseState.pos;
            cin >> horseState.maxSpeed;

            races[i].horses.push_back(horseState);
        }
	}

	for (int i = 0; i < testCount; i++)
	{
		cout << "Case #" << i + 1 << ": ";
		auto t = getMaxSpeed(races[i]);

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(6);
		cout << t;

		cout << endl;
	}

	return -1;
}


