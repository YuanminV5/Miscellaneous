#include <iostream>
using namespace std;

// define a function pointer type
typedef int(*sum)(int, int);

// define a function pointer type
typedef int(callback)(int, int);

// define a function pointer variable
int(*sum3)(int, int);


int getSum(int a, int b)
{
	return a + b;
};


void my0(sum cb, int a, int b)
{
	
	cout << "my0 return: " << cb(a, b) << endl;
}

//void my1(sum *cb, int a, int b)
//{
//	cb(a, b);
//}

void my2(callback cb, int a, int b)
{
	cout << "my2 return: " << cb(a, b) << endl;
}

void my3(callback* cb, int a, int b)
{
	cout << "my3 return: " << cb(a, b) << endl;
}

int main()
{
	sum a = getSum; // correct, sum is a type
	sum a = &getSum; // correct, sum is a type
	//sum *b = getSum; // wrong

	//callback b = getSum; //wrong, sum2 is not a type
	callback *b = getSum; // correct
	callback *b = &getSum; // correct

	//sum3 a = getSum; // wrong, sum3 is not a type
	sum3 = getSum; // correct, sum3 is a variable
	sum3 = &getSum; // correct, sum3 is a variable

	my0(getSum, 1, 2);
	my0(&getSum, 1, 2);

	my2(getSum, 1, 2);
	my2(&getSum, 1, 2);

	my3(getSum, 1, 2);
	my3(&getSum, 1, 2);
	return 0;
}