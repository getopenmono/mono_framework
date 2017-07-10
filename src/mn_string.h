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
     * This string class takes care of all alloc and dealloc of memory. It is a
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
     * @brief High level string class
     */
    class String
    {
    public:
        uint32_t *refCount;
        bool malloced;
        char *stringData;

        void preAllocbytes(uint32_t count);

    public:
        
        /**
         * @brief C lib. style format string
         *
         * Construct a mono string using the *string format* syntax.
         *
         * @param format The formatted string with placeholders
         * @return String The resulting string, interpolated with provided parameters
         */
        static String Format(const char *format, ...);
        
        /**
         * @brief Construct an empty invalid string
         */
        String();
        
        /**
         * @brief Construct an empty string with a pre-allocated size
         *
         * Use this constructor to created a buffer-like string object.
         * @param preAllocBytes The number of bytes to allocate in the string object
         */
        String(uint32_t preAllocBytes);
        
        /**
         * @brief Construct a string from an existing C string, with a fixed length
         *
         * The string data is copied to the mono string, such that @ref String
         * has it own copy of the string data.
         *
         * @param str Pointer to the original C string
         * @param length The length of the provided string, without NULL terminator
         */
        String(char *str, uint32_t length);
        
        /**
         * @brief Construct a mono String from an existing NULL terminated C string
         *
         * The length of the source string is determined by the Clib function
         * `strlen`.
         *
         * @param str A pointer to the C style string
         */
        String(char *str);
        
        /**
         * @brief Construct a mono String from an existing NULL terminated C string
         *
         * The length of the source string is determined by the Clib function
         * `strlen`.
         *
         * @param str A pointer to the C style string
         */
        String(const char *str);
        
        /**
         * @brief Construct a mono String from an existing mono string
         *
         * The length of the source string is determined by the Clib function
         * `strlen`.
         *
         * @param str A reference to the mono string
         */
        String(const String &str);
        
        /**
         * @brief Return the length of the string
         *
         * The length of the actual string is returned, not counting the NULL
         * terminator.
         *
         * This method uses `strlen` which does not support variable byte-length
         * character encoding. (That means UTF8, 16 and alike.)
         */
        uint32_t Length() const;

        String& operator=(const char *str);

        String& operator=(const String &str);

        bool operator==(String const &other) const;

        bool operator!=(String const &other) const;

        bool operator==(const char *other) const;

        bool operator!=(const char *other) const;
        
        /**
         * @brief Access each character (or byte) of the string in an array like fashion
         */
        char operator[](uint32_t pos) const;
        
        /**
         * @brief Short-hand for @ref CString
         *
         * @see CString
         */
        char* operator()() const;
        
        /**
         * @brief Get a pointer to the raw C style string
         *
         * Use this method if need to print a string using `printf` or alike.
         *
         * To obtain a pointer to the raw string data.
         */
        char* CString() const;

        ~String();

    private:
        void CopyFromCString(const char * cstring);

    };
}

#endif /* mn_string_h */
