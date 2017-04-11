#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <iostream>
using namespace std;

namespace DP
{
	class SingletonInDesignPattern
	{
	public:
		static SingletonInDesignPattern* poGetInstance()
		{
			if( poSingletonInstance == NULL )
			{
				poSingletonInstance = new SingletonInDesignPattern();
			}
			return poSingletonInstance;
		}

		void vAction()
		{
			cout<<"This is a Singleton output!"<<endl;
		}

		void vDestroySingleton()
		{
			cout<<"Delete Singleton instance!"<<endl;
			delete poSingletonInstance;
			poSingletonInstance = NULL;
		}
	private:

		SingletonInDesignPattern()
		{
		}
		static SingletonInDesignPattern* poSingletonInstance;
	};

	SingletonInDesignPattern* SingletonInDesignPattern::poSingletonInstance = NULL; // static const can be initialized with declaration. i.e, static const int a = 0;
}
#endif