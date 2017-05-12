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


struct TestGroup
{
    int N;
    int R;
    int O;
    int Y;
    int G;
    int B;
    int V;
};

bool isImpossible(TestGroup group)
{
    if (group.R != 0 && (group.N / group.R) < 2)
    {
        return true;
    }

    if (group.O != 0 && (group.N / group.O) < 2)
    {
        return true;
    }

    if (group.Y != 0 && (group.N / group.Y) < 2)
    {
        return true;
    }

    if (group.G != 0 && (group.N / group.G) < 2)
    {
        return true;
    }

    if (group.B != 0 && (group.N / group.B) < 2)
    {
        return true;
    }

    if (group.V != 0 && (group.N / group.V) < 2)
    {
        return true;
    }

    return false;
}
vector<char> getArrangement(TestGroup group)
{
    if (isImpossible(group))
    {
        return vector<char>();
    }

    vector<char> result(group.N, '\0');

    bool first = true;
    int distance = 0;
    int lastPose = 0;


    first = true;
    distance = group.N / group.R;
    lastPose = 0;
    for (int i = 0; i < group.R; i++)
    {
        int skipCount = 0;
        for (int j = 0; j < result.size();j++)
        {
            if (result[j] != '\0')
            {
                continue;
            }

            if (first)
            {
                lastPose = j;
                result[lastPose] = 'R';
                first = false;
                skipCount = 0;
            }
            else if (!first && result[lastPose + distance + skipCount] != '\0')
            {
                skipCount++;
                continue;
            }
            else
            {
                lastPose = lastPose + distance + skipCount;
                result[lastPose] = 'R';
                first = false;
                skipCount = 0;
            }

            break;
        }
    }

    first = true;
    distance = group.N / group.Y;
    lastPose = 0;
    for (int i = 0; i < group.Y; i++)
    {
        int skipCount = 0;
        for (int j = 0; j < result.size();j++)
        {
            if (result[j] != '\0')
            {
                continue;
            }

            if (first)
            {
                lastPose = j;
                result[j] = 'Y';
                first = false;
                skipCount = 0;
            }
            else if (!first && result[lastPose + distance + skipCount] != '\0')
            {
                skipCount++;
                continue;
            }
            else
            {
                lastPose = lastPose + distance + skipCount;
                result[lastPose] = 'Y';
                first = false;
                skipCount = 0;
            }

            break;
        }
    }

    first = true;
    distance = group.N / group.B;
    lastPose = 0;
    for (int i = 0; i < group.B; i++)
    {
        int skipCount = 0;
        for (int j = 0; j < result.size();j++)
        {
            if (result[j] != '\0')
            {
                continue;
            }

            if (first)
            {
                lastPose = j;
                result[j] = 'B';
                first = false;
                skipCount = 0;
            }
            else if (!first && result[lastPose + distance + skipCount] != '\0')
            {
                skipCount++;
                continue;
            }
            else
            {
                lastPose = lastPose + distance + skipCount;
                result[lastPose] = 'B';
                first = false;
                skipCount = 0;
            }

            break;
        }
    }

    return result;
}

int main()
{
    int testCount = 0;
    TestGroup groups[100];

    cin >> testCount;
    for (int i = 0; i < testCount; i++)
    {
        // R Y B single color
        cin >> groups[i].N;
        cin >> groups[i].R;
        cin >> groups[i].O;
        cin >> groups[i].Y;
        cin >> groups[i].G;
        cin >> groups[i].B;
        cin >> groups[i].V;
    }

    for (int i = 0; i < testCount; i++)
    {
        cout << "Case #" << i + 1 << ": ";
        auto t = getArrangement(groups[i]);

        if (t.size() == 0)
        {
            cout << "IMPOSSIBLE";
        }
        else
        {
            for (auto c : t)
            {
                cout << c;
            }
        }

        cout << endl;
    }

    return -1;
}


