// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mn_string_h
#define mn_string_h

#include <stdint.h>
#include <stdarg.h>

namespace mono {

    /**
     * The mono framework has it own string class, that either reside on the HEAP
     * or inside the read-only data segment (`.rodata`).
     *
     * We use this string class to pass string data to async routines like the
     * @ref View 's @ref scheduleRepaint method. Because views might be repainted
     * at any point in time, we cannot have view data reside on the stack. This
     * string class hold its data on the HEAP, but behaves as it would reside on
     * the stack.
     *
     * THis string class takes care of all alloc and dealloc of memory. It is a
     * referenced based string class. You should not pass pointers of C++ references
     * to this class, but instead normal assignment or pass the full class to
     * functions. The efficient copy / assignment operator methods on the class
     * ensure only data references are passed, behind the scenes.
     *
     * For example:
     * @code
     * String str = String::Format("Hello World, number: %i", 1);
     * String str2 = str;
     * String str3 = str2;
     * @endcode
     *
     * In the code only 1 copy of the string data is present in memory. And only
     * references are passed to the objects `str2` and `str3`. Only as the last
     * object is deallocated is the data disposed from the HEAP.
     *
     * These features makes the class very lightweight and safe to pass around
     * functions and objects.
     *
     * @brief High level string class, that is allocated on the HEAP or `rodata`
     */
    class String
    {
    public:
        uint32_t *refCount;
        bool malloced;
        char *stringData;

        void preAllocbytes(uint32_t count);

    public:

        static String Format(const char *format, ...);

        String();

        String(uint32_t preAllocBytes);

        String(char *str, uint32_t length);

        String(const char *str);

        String(const String &str);

        uint32_t Length() const;

        String& operator=(const char *str);

        String& operator=(const String &str);

        char operator[](uint32_t pos) const;

        char* operator()() const;

        ~String();

    private:
        void CopyFromCString(const char * cstring);

    };
}

#endif /* mn_string_h */
