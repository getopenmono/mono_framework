// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "queue.h"

using namespace mono;

Queue::Queue()
{
    topOfQueue = NULL;
    endOfQueue = NULL;
}

void Queue::Enqueue(mono::IQueueItem *item)
{
    //first check that this object does not already exist in the queue
    if (this->Exists(item))
        return;
    
    // dont allow the new end of the queue to reference rubbish data
    item->_queueNextPointer = NULL;
    
    if (topOfQueue == NULL)
    {
        topOfQueue = endOfQueue = item;
    }
    else
    {
        endOfQueue->_queueNextPointer = item;
        endOfQueue = item;
    }
}

IQueueItem* Queue::Dequeue()
{
    if (topOfQueue == NULL)
        return NULL;
    
    IQueueItem *next = topOfQueue;
    topOfQueue = next->_queueNextPointer;
    
    if (topOfQueue == NULL)
        endOfQueue = NULL;
    
    return next;
}

IQueueItem* Queue::Peek()
{
    return topOfQueue;
}

IQueueItem* Queue::Next(mono::IQueueItem *item)
{
    if (item == NULL)
        return NULL;
    
    return item->_queueNextPointer;
}

bool Queue::Remove(IQueueItem *item)
{
    IQueueItem *prev = NULL;
    IQueueItem *next = topOfQueue;
    while (next != NULL && next != item)
    {
        prev = next;
        next = next->_queueNextPointer;
    }
    
    if (next == item)
    {
        if (prev == NULL)
        {
            topOfQueue = item->_queueNextPointer;
        }
        else
        {
            prev->_queueNextPointer = item->_queueNextPointer;
            
        }
        
        item->_queueNextPointer = NULL;
        sanitizeEndOfQueue();
        
        return true;
    }
    else
        return false;
}

bool Queue::Exists(mono::IQueueItem *item)
{
    IQueueItem *next = topOfQueue;
    
    while (next != NULL) {
        if (next == item)
            return true;
        
        next = next->_queueNextPointer;
    }
    
    return false;
}

uint16_t Queue::Length()
{
    int cnt = 0;
    
    IQueueItem *next = topOfQueue;
    while (next != NULL) {
        cnt++;
        next = next->_queueNextPointer;
    }
    
    return cnt;
}

void Queue::sanitizeEndOfQueue()
{
    IQueueItem *next = topOfQueue;
    IQueueItem *prev = NULL;
    
    while (next != NULL) {
        prev = next;
        next = next->_queueNextPointer;
    }
    
    endOfQueue = prev;
}