//
//  ObserverInDP.h
//  SelfLearning
//
//  Created by Yuanmin Wei on 29/9/13.
//  Copyright (c) 2013 Yuanmin Wei. All rights reserved.
//

#ifndef __SelfLearning__ObserverInDP__
#define __SelfLearning__ObserverInDP__

#include <iostream>
#include "vector"
#include "iterator"

// System Configuration
#define PUSH_MODE 0

using namespace std;

namespace DP {
    
    
    // The FocusData that is wanted by Observers
    typedef struct
    {
        float flTemperature;
        float flHumidity;
        float flPresssure;
    } tstFocusData;
    
    // Pre-declaration
    class Observer;
    
    // Define a Subject abstract class
    class Subject
    {
    public:
        // Use default constructor provided by compiler
        virtual void vRegisterObserver(Observer* ) = 0;
        virtual void vDeRegisterObserver(Observer*) = 0;
        
    protected:
        virtual void vNotifyObserver() = 0;
    };
    
    // Define an Observer abstract class
    class Observer
    {
    public:
        // Use default constructor provided by compiler
        virtual void vUpdate(tstFocusData* pstFocusData = NULL) = 0;
    };
    
    // Define a concrete Subject class
    class WeatherData: public Subject
    {
    public:
        WeatherData():m_boPropogateFocusDataChange(false)
        {
            m_stWeatherData.flHumidity = 0;
            m_stWeatherData.flPresssure = 0;
            m_stWeatherData.flTemperature = 0;
        }
        
        void vSetWeatherDate(float flHumidity, float flPressure, float flTemperature)
        {
            m_stWeatherData.flHumidity = flHumidity;
            m_stWeatherData.flPresssure = flPressure;
            m_stWeatherData.flTemperature = flTemperature;
            vNotifyObserver();
        }
    
        virtual void vRegisterObserver(Observer* poObserver)
        {
            vector<Observer*>::iterator itr;
            bool boObserverRegistered = false;
            
            // To avoid multiple registeration, even a little low efficient
            for (itr = m_poObserverList.begin(); itr != m_poObserverList.end(); itr++)
            {
                if (*itr == poObserver)
                {
                    boObserverRegistered = true;
                    break;
                }
            }
            
            // Only register for once
            if(!boObserverRegistered)
            {
                m_poObserverList.push_back(poObserver);
            }
        }
        
        virtual void vDeRegisterObserver(Observer* poObserver)
        {
            int iSize = m_poObserverList.size();
            bool boObserverRegisteredBefore = false;
            
            for(int iCnt = 0; iCnt < iSize; iCnt++)
            {
                if(m_poObserverList[iCnt] == poObserver)
                {
                    // CAUTION: Length of list is dynamically modified
                    m_poObserverList.erase(m_poObserverList.begin() + iCnt);
                    boObserverRegisteredBefore = true;
                    break;
                }
            }
            
            if (boObserverRegisteredBefore)
            {
                cout<<"Observer deregistered!"<<endl;
            }
            else
            {
                cout<<"No such a registered observer!"<<endl;   
            }
        }
        
        virtual void vNotifyObserver()
        {
            if (m_boPropogateFocusDataChange)
            {
                vector<Observer*>::iterator itr;
                
                // To avoid multiple registeration, even a little low efficient
                for (itr = m_poObserverList.begin(); itr != m_poObserverList.end(); itr++)
                {
                    if (*itr != NULL)
                    {
                        // Observers maybe don't want to process data change immediately
                        if(PUSH_MODE == 1)
                        {
                            (*itr)->vUpdate(&m_stWeatherData);
                        }
                        else
                        {
                            (*itr)->vUpdate();
                        }
                    }
                }
            }

        }
        
    protected:
        // We need this because we may want additional calculation
        void vSetFocusDataHasChanged()
        {
            // To add: pre-process raw data and determine if we shall propogate data change
            m_boPropogateFocusDataChange = true;
            vNotifyObserver();
        }
        
    private:
        std::vector<Observer*> m_poObserverList;
        tstFocusData m_stWeatherData;
        bool m_boPropogateFocusDataChange;
    };
    
    // Define a concrete Observer class
    class HuminityDisplay: public Observer
    {
    public:
        // Through interface, JAVA can refer to a class rather make an own copy
        HuminityDisplay(Subject* poSubjectRef):m_poSubjectRef(poSubjectRef)
        //HuminityDisplay(WeatherData& oSubjectRef):m_oSubjectRef(oSubjectRef)
        {
        
        }
        
        virtual void vUpdate(tstFocusData* pstFocusData = NULL)
        {
            if(NULL != pstFocusData)
            {
                cout<<"Current huminity is "<<pstFocusData->flHumidity<<endl;
            }
            else
            {
                // TBD: behaviour in PULL_MODE
            }
        }
        
        void vDeRegisterMyself()
        {
            m_poSubjectRef->vDeRegisterObserver(this);
            //m_oSubjectRef.vDeRegisterObserver(this);
        }
    private:
        Subject* m_poSubjectRef;
        //WeatherData m_oSubjectRef;
    };
}
#endif /* defined(__SelfLearning__ObserverInDP__) */
