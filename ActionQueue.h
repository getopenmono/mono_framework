//
//  ActionQueue.hpp
//  mono framework
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef ActionQueue_h
#define ActionQueue_h

#include <FunctionPointer.h>

namespace mono {
    
    template <int TotalSize>
    class ActionQueue {
    protected:
        
        int queueSize;
        mbed::FunctionPointer *queue[TotalSize];
        
    public:
        
        ActionQueue(): queueSize(0)
        {
            
        }
        
        bool AddHandler(mbed::FunctionPointer *handler)
        {
            if (queueSize < TotalSize)
            {
                queue[queueSize++] = handler;
                return true;
            }
            else
                return false;
        }
        
        bool RemoveHandler(mbed::FunctionPointer *handler)
        {
            bool sanitize = false;
            
            for (int i=0; i<TotalSize; i++) {
                if (queue[i] == handler)
                {
                    //begin move those items with higher index, up one slot
                    sanitize = true;
                }
                
                if (sanitize && i < TotalSize-1)
                {
                    queue[i] = queue[i+1];
                }
            }
            
            if (sanitize)
                queueSize--;
            
            return sanitize; //if we sanitized the queue, an item was removed
        }
        
    };
}

#endif /* ActionQueue_h */
