//
//  Obeserver.h
//  DesignPattern
//
//  Created by Yuanmin Wei on 29/9/13.
//  Copyright (c) 2013 Yuanmin Wei. All rights reserved.
//

#ifndef __DesignPattern__Obeserver__
#define __DesignPattern__Obeserver__

#include <iostream>

namespace DP
{
    // Create abstract Subject
    class Subject
    {
    public:
        Subject()
        {
            
        }
        virtual void vNotifyObserver() = 0;
        virtual void vRegisterObserver() = 0;
        virtual void vDeRegisterObserver() = 0;

    };
    
}
#endif /* defined(__DesignPattern__Obeserver__) */
