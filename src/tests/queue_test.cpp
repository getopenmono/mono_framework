// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "catch.hpp"
#include "../queue.h"

using namespace mono;

class Number : public IQueueItem
{
public:
    int n;
    
    Number(int num)
    {
        n = num;
    }
};

SCENARIO("Heap based Queue works","[queue]")
{
    GIVEN("An empty queue")
    {
        GenericQueue<Number> queue;
        
        
        WHEN("one item is added")
        {
            
            queue.Enqueue(new Number(45));
            
            THEN("the length must be 1")
            {
                REQUIRE(queue.Length() == 1);
            }
            
            
            THEN("we should dequeue that same object")
            {
                Number *num = (Number*) queue.Dequeue();
                
                REQUIRE(num != NULL);
                REQUIRE(num->n == 45);
            }
        }
        
        WHEN("4 items are added")
        {
            queue.Enqueue(new Number(1));
            queue.Enqueue(new Number(2));
            queue.Enqueue(new Number(3));
            queue.Enqueue(new Number(4));
            
            THEN("length should be 4")
            {
                REQUIRE(queue.Length() == 4);
            }
            
            WHEN("we dequeue 2")
            {
                Number *n1 = (Number*) queue.Dequeue();
                Number *n2 = (Number*) queue.Dequeue();
                
                THEN("the dequeued must must first added")
                {
                    REQUIRE(n1 != NULL);
                    REQUIRE(n1->n == 1);
                    
                    REQUIRE(n2 != NULL);
                    REQUIRE(n2->n == 2);
                }
                
                THEN("length must be 2")
                {
                    REQUIRE(queue.Length() == 2);
                }
                
                WHEN("we enqueue 1 more")
                {
                    queue.Enqueue(new Number(5));
                    
                    THEN("length must be 3")
                    {
                        REQUIRE(queue.Length() == 3);
                    }
                    
                    WHEN("we dequeue all 3 remaining")
                    {
                        Number *n1 = (Number*) queue.Dequeue();
                        Number *n2 = (Number*) queue.Dequeue();
                        Number *n3 = (Number*) queue.Dequeue();
                        
                        THEN("objects must match")
                        {
                            REQUIRE(n1 != NULL);
                            REQUIRE(n1->n == 3);
                            
                            REQUIRE(n2 != NULL);
                            REQUIRE(n2->n == 4);
                            
                            REQUIRE(n3 != NULL);
                            REQUIRE(n3->n == 5);
                        }
                        
                        THEN("length must be 0")
                        {
                            REQUIRE(queue.Length() == 0);
                        }
                    }
                }
                
            }
            
            
        }
        
    }
}