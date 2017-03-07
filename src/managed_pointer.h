// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef managed_pointer_h
#define managed_pointer_h

#include <stdint.h>
#include <stdlib.h>

#include <mbed_debug.h>

namespace mono {

    /**
     * The managed pointer is an object designed to live on the stack, but point
     * to memory cobntent that live on the heap. The ManagedPointer keeps track
     * of memory references, such that the it can be shared across multiple
     * objects in your code.
     *
     * It maintains an internal reference count, to keep track of how many objects
     * holds a reference to itself. If the count reaches zero, then the content
     * is deallocated.
     *
     * With ManagedPointer you can prevent memory leaks, by ensuring
     * un-references memory gets freed.
     *
     * @brief Pointer to a heap object, that keeps track of memory references.
     */
    template <typename ContentClass>
    class ManagedPointer
    {
    protected:

        uint32_t *refCount;
        ContentClass *content;

    public:

        /** Create an empty pointer */
        ManagedPointer() : refCount(NULL), content(NULL)
        {}

        ManagedPointer(ContentClass *contentPtr, uint32_t initialRefCount = 1)
        {
            content = contentPtr;
            refCount = (uint32_t*) malloc(sizeof(uint32_t));
            *refCount = initialRefCount;
        }

        ManagedPointer(const ManagedPointer &other)
        {
            content = other.content;
            refCount = other.refCount;
            *refCount = *refCount + 1;
        }

        ManagedPointer &operator=(const ManagedPointer &other)
        {
            //debug("mgrPtr assigning: 0x%x\r\n",other.content);
            if (this->content != 0)
                this->~ManagedPointer();
            
            if (!other)
            {
                content =  0;
                refCount = 0;
            }
            else
            {
                content = other.content;
                refCount = other.refCount;
                *refCount = *refCount + 1;
            }

            return *this;
        }

        ManagedPointer &operator=(ContentClass *contentPtr)
        {
            //debug("raw ptr to mgrPtr: 0x%x\r\n",contentPtr);
            if (this->content != 0)
                this->~ManagedPointer();

            if (contentPtr == NULL)
                return *this;

            content = contentPtr;
            refCount = (uint32_t*) malloc(sizeof(uint32_t));
            *refCount = 1;

            return *this;
        }

        ContentClass* operator->()
        {
            return content;
        }

        bool operator==(ManagedPointer &other) const
        {
            return content == other.content;
        }

        bool operator==(ContentClass *other) const
        {
            return content == other;
        }

        bool operator!=(ManagedPointer &other) const
        {
            return content != other.content;
        }

        bool operator!=(ContentClass *other) const
        {
            return content != other;
        }

        operator bool() const
        {
            return content != NULL;
        }

        /**
         * Give up this ManagedPointers refrences to the content object, by setting
         * its pointer to NULL.
         *
         * This means that if the Reference count is 1, this pointer is the only
         * existing, and it will not dealloc the content memory upon deletion of
         * the ManagedPointer.
         *
         * We Reference count is > 1, then other ManagedPointers might dealloc
         * the content memory.
         */
        void Surrender()
        {
            content = NULL;
        }

        uint32_t References() const
        {
            return refCount != 0 ? *refCount : 0;
        }

        uint32_t Address() const
        {
            return (uint32_t) content;
        }

        ContentClass* Pointer() const
        {
            return content;
        }

        ContentClass& Reference() const
        {
            return *content;
        }
        
        ~ManagedPointer()
        {
            if (content != NULL) {
                (*refCount) = *refCount-1;
            }
            else
                return;

            if (*refCount <= 0)
            {
                free(refCount);

                if (content != NULL)
                    delete content;
            }

        }

    };

}

#endif /* managed_pointer_h */
