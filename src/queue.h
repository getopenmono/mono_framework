// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef heap_queue_h
#define heap_queue_h

#include <stdio.h>
#include <stdint.h>
#include <deprecated.h>

namespace mono {


    class Queue;

    template <typename Item>
    class GenericQueue;

    class IQueueItem
    {
        friend class Queue;
        friend class GenericQueue<IQueueItem>;
    public:
        IQueueItem *_queueNextPointer;
    };

    /**
     * @brief A pointer based FIFO style Queue
     *
     *
     *
     * @see GenericQueue
     */
    class Queue
    {
    protected:

        IQueueItem *topOfQueue;
        IQueueItem *endOfQueue;

        void sanitizeEndOfQueue();

    public:


        Queue();

        /**
         * @brief Add a new element to the back of the queue
         * Insert a pointer to an element on the back of the queue.
         */
        void enqueue(IQueueItem *item);
        void Enqueue(IQueueItem *item) __DEPRECATED("Please use the lower case variant","enqueue") { enqueue(item); }

        /**
         * @brief Returns and removes the oldest element in the queue
         *
         */
        IQueueItem* dequeue();
        IQueueItem* Dequeue() __DEPRECATED("Please use the lower case variant","dequeue") { return dequeue(); }

        /**
         * @brief Return the oldest element in the queue, without removing it
         *
         */
        IQueueItem* peek();
        IQueueItem* Peek() __DEPRECATED("Please use the lower case variant","peek") { return peek(); }

        /**
         * @brief Get the next element in the queue, after the one you provide.
         *
         * **NOTE**: There is no check if the item belongs in the parent queue
         * at all!
         *
         * @param item A pointer to an item in the queue
         * @return The next element in the queue, after the item you provided.
         */
        IQueueItem* next(IQueueItem *item);
        IQueueItem* Next(IQueueItem *item) __DEPRECATED("Please use the lower case variant","next") { return next(item); }

        /**
         * @brief Check that an object already exists in the queue.
         * Because of the stack based nature of this queue, all objects can only
         * exist one replace in the queue. You cannot add the same object to two
         * different positions in the queue.
         *
         * @param item The element to search for in the queue
         */
        bool exists(IQueueItem *item);
        bool Exists(IQueueItem *item) __DEPRECATED("Please use the lower case variant","exists") { return exists(item); }

        bool remove(IQueueItem *item);
        bool Remove(IQueueItem *item) __DEPRECATED("Please use the lower case variant","remove") { return remove(item); };

        uint16_t Length();

    };


    /**
     * @brief A templated Queue, where template defines the queue element type
     *
     * This class is identical to @ref Queue, but it uses templating to preserve
     * type information.
     *
     * @see Queue
     */
    template <typename Item>
    class GenericQueue : public Queue
    {
    public:

        GenericQueue() : Queue() {}

        void enqueue(Item *i)
        {
            Queue::enqueue((IQueueItem*)i);
        }
        
        void Enqueue(Item *i) __DEPRECATED("Please use the lower case variant","enqueue") { enqueue(i); }

        Item* dequeue()
        {
            return (Item*) Queue::dequeue();
        }
        
        Item* Dequeue() __DEPRECATED("Please use the lower case variant","dequeue") { return dequeue(); }

        Item* peek()
        {
            return (Item*) Queue::peek();
        }
        
        Item* Peek() __DEPRECATED("Please use the lower case variant","peek") { return peek(); }

        Item* next(Item *i)
        {
            return (Item*) Queue::next((IQueueItem*)i);
        }
        
        Item* Next(Item *i) __DEPRECATED("Please use the lower case variant","next") { return next(i); }

        bool exists(Item *i)
        {
            return Queue::exists((IQueueItem*) i);
        }
        
        bool Exists(Item *i) __DEPRECATED("Please use the lower case variant","exists") { return exists(i); }
        
        bool remove(Item *i)
        {
            return Queue::remove((IQueueItem*) i);
        }
        
        bool Remove(Item *i) __DEPRECATED("Please use the lower case variant","remove") { return remove(i); }

    };

}


#endif /* heap_queue_h */
