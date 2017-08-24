#include <iostream>
using namespace std;

// https://isocpp.org/wiki/faq/pointers-to-members#memfnptr-vs-fnptr
// define a function pointer type
typedef int(*callbackptrtype)(int, int);

// define a function pointer type
typedef int(callbacktype)(int, int);

// define a function pointer variable
int(*callbackptr)(int, int);

void my0(callbackptrtype cb, int a, int b)
{
    cout << "my0 return: " << cb(a, b) << endl;
}

void my1(callbacktype cb, int a, int b)
{
    cout << "my2 return: " << cb(a, b) << endl;
}

void my2(callbacktype* cb, int a, int b)
{
    cout << "my3 return: " << cb(a, b) << endl;
}

int getSum(int a, int b)
{
    return a + b;
};

class A
{
public:
    A()
    {
        // Simply cannnot do this: Assign C++ member function to C function pointer
        //my1(getSum, 1, 2);

        // OK to use static method
        my1(getSum2, 1, 2);
    }

    void my0(callbackptrtype cb, int a, int b)
    {
        cout << "my0 return: " << cb(a, b) << endl;
    }

    void my1(callbacktype cb, int a, int b)
    {
        cout << "my2 return: " << cb(a, b) << endl;
    }

    void my2(callbacktype* cb, int a, int b)
    {
        cout << "my3 return: " << cb(a, b) << endl;
    }
private:
    int getSum(int a, int b)
    {
        return a + b;
    };

    static int getSum2(int a, int b)
    {
        return a + b;
    };
};
int main()
{
    callbackptrtype a0 = getSum; // correct, callbackptrtype is a type
    callbackptrtype a1 = &getSum; // correct, callbackptrtype is a type
    //callbackptrtype *b = getSum; // wrong

    //callbackptrtype b = getSum; //wrong
    callbacktype *b = getSum; // correct
    callbacktype *b = &getSum; // correct

    //callbackptr a = getSum; // wrong, callbackptr is not a type
    callbackptr = getSum; // correct, callbackptr is a variable
    callbackptr = &getSum; // correct, callbackptr is a variable

    my0(getSum, 1, 2);
    my0(&getSum, 1, 2);

    my1(getSum, 1, 2);
    my1(&getSum, 1, 2);

    my2(getSum, 1, 2);
    my2(&getSum, 1, 2);
    return 0;
}