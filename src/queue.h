// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef heap_queue_h
#define heap_queue_h

#include <stdio.h>
#include <stdint.h>

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
        void Enqueue(IQueueItem *item);

        /**
         * @brief Returns and removes the oldest element in the queue
         *
         */
        IQueueItem* Dequeue();

        /**
         * @brief Return the oldest element in the queue, without removing it
         *
         */
        IQueueItem* Peek();

        /**
         * @brief Get the next element in the queue, after the one you provide.
         *
         * **NOTE**: There is no check if the item belongs in the parent queue
         * at all!
         *
         * @param item A pointer to an item in the queue
         * @return The next element in the queue, after the item you provided.
         */
        IQueueItem* Next(IQueueItem *item);

        /**
         * @brief Check that an object already exists in the queue.
         * Because of the stack based nature of this queue, all objects can only
         * exist one replace in the queue. You cannot add the same object to two
         * different positions in the queue.
         *
         * @param item The element to search for in the queue
         */
        bool Exists(IQueueItem *item);

        bool Remove(IQueueItem *item);

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

        void Enqueue(Item *i)
        {
            Queue::Enqueue((IQueueItem*)i);
        }

        Item* Dequeue()
        {
            return (Item*) Queue::Dequeue();
        }

        Item* Peek()
        {
            return (Item*) Queue::Peek();
        }

        Item* Next(Item *i)
        {
            return (Item*) Queue::Next((IQueueItem*)i);
        }

        bool Exists(Item *i)
        {
            return Queue::Exists((IQueueItem*) i);
        }

    };

}


#endif /* heap_queue_h */
