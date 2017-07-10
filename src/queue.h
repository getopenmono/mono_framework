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
    
    /**
     * @brief An interface for object that can be put into a @ref Queue
     *
     * This interface defines the *next queue item* pointer on the sub-classes.
     * This pointer is used by the @ref Queue class to realize the queue data
     * structure.
     *
     * Any object you wish to insert into a queue must inherit from this
     * interface.
     *
     * @see GenericQueue
     */
    class IQueueItem
    {
        friend class Queue;
        friend class GenericQueue<IQueueItem>;
    private:
        static const int GENERIC_QUEUE_ITEMS_MUST_INHERIT_FROM_QUEUEITEM = 0;
    public:
        
        IQueueItem *_queueNextPointer;
        
        IQueueItem() : _queueNextPointer(0) {}
    };

    /**
     * @brief A pointer based FIFO style Queue
     *
     * **Note: You should avoid using this Queue class, and consider
     * its template based counter part: @ref GenericQueue**
     *
     * This is a basic single linked FIFO queue structure. All items in the
     * queue *must* implement the @ref QueueItem interface.
     *
     * In theory you can add different types into the queue, as long as they all
     * inherit @ref QueueItem. However, you should *not* do this. Mixing
     * different types in the queue, removes type information - such that the
     * objects original type can not be restored later.
     *
     * Best practice is therefore to use only one type in the queue. To help
     * with this you should really use the @ref GenericQueue class. This class
     * uses C++ templating to keep type information and you do not need to
     * manual type casting.
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

        /**
         * @brief Construct an empty queue
         */
        Queue();

        /**
         * @brief Add a new element to the back of the queue
         * Insert a pointer to an element on the back of the queue.
         */
        void enqueue(IQueueItem *item);
        void Enqueue(IQueueItem *item) __DEPRECATED("Please use the lower case variant","enqueue") { enqueue(item); }

        /**
         * @brief Returns and removes the oldest element in the queue
         */
        IQueueItem* dequeue();
        IQueueItem* Dequeue() __DEPRECATED("Please use the lower case variant","dequeue") { return dequeue(); }

        /**
         * @brief Return the oldest element in the queue, without removing it
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
        
        /**
         * @brief Return the length of the queue
         *
         * The length is the number of item currently present in the queue.
         *
         * This method runs in O(n) (linear time)
         */
        uint16_t Length();

    };


    /**
     * @brief A templated Queue, where template defines the queue element type
     *
     * This class is identical to @ref Queue, but it uses templating to preserve
     * type information.
     *
     * This is a basic single linked FIFO queue structure. All items in the
     * queue *must* implement the @ref QueueItem interface.
     *
     * ### Item data types
     *
     * Items added to the queue *must* inherit from the interface @ref QueueItem.
     * This is required because this interface defined the queues *next pointer*.
     *
     * Should you try to construct a GenericQueue with non-QueueItem based
     * template, you will get a compiler warning.
     *
     * ### Complexity
     *
     * As with standard queue data types, enqueueing and dequeueing items are
     * constant time ( O(1) ). However removing an element inside the queue is
     * linear ( O(n) ), same for getting the current length or size of the queue.
     *
     * @see QueueItem
     */
    template <typename Item>
    class GenericQueue : public Queue
    {
    private:
        static const int ITEMS_MUST_INHERIT_QUEUEITEM = Item::GENERIC_QUEUE_ITEMS_MUST_INHERIT_FROM_QUEUEITEM;
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
