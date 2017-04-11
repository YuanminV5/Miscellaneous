//
//  main.cpp
//  SelfLearning
//
//  Created by Yuanmin Wei on 29/9/13.
//  Copyright (c) 2013 Yuanmin Wei. All rights reserved.
//

#include <iostream>
#include "SingletonInDP.h"
#include "DecoratorInDP.h"
#include "ObserverInDP.h"
using namespace std;

int main(int argc, const char * argv[])
{
    // Singleton Pattern
	DP::SingletonInDesignPattern* poSingleton;
	poSingleton = DP::SingletonInDesignPattern::poGetInstance();
	poSingleton->vAction();
	poSingleton->vDestroySingleton();
    
	DP::SingletonInDesignPattern::poGetInstance()->vDestroySingleton();
    
	// Decorator Pattern
    
    
    cout<<"This is a Decorator output"<<endl;
	DP::Beverage* poBeverage = new DP::Coffee();
    
	// Add milk to coffee
	poBeverage = new DP::Milk(poBeverage);
	// Add sugar to coffee
	poBeverage = new DP::Sugar(poBeverage);
	// Add milk to coffee again
	poBeverage = new DP::Milk(poBeverage);
    
	cout<<poBeverage->strGetDescription()<<endl;
	cout<<poBeverage->dbGetCost()<<endl;
    
    // To consider: make the destrutor to be virtual
    delete poBeverage;
    
    // Observer Pattern
    
    cout<<"This is a Observer output"<<endl;
    
    DP::WeatherData* poTodayWeather =  new DP::WeatherData;
    DP::HuminityDisplay* poDisplayHuminity = new DP::HuminityDisplay(poTodayWeather);
    poTodayWeather->vRegisterObserver(poDisplayHuminity);
    
    poTodayWeather->vSetWeatherDate(50, 55, 60);
    poDisplayHuminity->vDeRegisterMyself();
    
    poTodayWeather->vDeRegisterObserver(poDisplayHuminity);
    delete poTodayWeather;
    delete poDisplayHuminity;
    
    
    return 0;
}

