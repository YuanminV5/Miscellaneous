// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

class base
{
public:
	virtual void read() = 0;

	virtual void write()
	{
		cout << "I am the write in virtual base class." << endl;
	}
};

class weak : virtual public base
{
public:
	// Without the following block Compiler Warning (level 2) C4250, and final class people not necessarily needed implementation
	// With the following block Compiler Error C2250, and final class people must have implementation
	virtual void read()
	{
		cout << "I am the read in weak derived class." << endl;
	}

	virtual void write()
	{
		cout << "I am the write in weak derived class." << endl;
	}
};

class dominant : virtual public base
{
public:
	virtual void read()
	{
		cout << "I am the read in dominant derived class." << endl;
	}

	virtual void write()
	{
		cout << "I am the write in dominant derived class." << endl;
	}
};

class people : public weak, public dominant
{
public:
	virtual void read()
	{
		cout << "I am the read in people derived class." << endl;
	}

	virtual void write()
	{
		cout << "I am the write in people derived class." << endl;
	}
};



int main()
{
	people* ptr = new people();
	base* base_ptr = ptr;
	weak* weak_ptr = ptr;
	dominant* dominant_ptr = ptr;

	ptr->dominant::read();

	base_ptr->read();
	weak_ptr->write();
	//base_ptr->base::read();	people* ptr = new people();

    return 0;
}

