// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef regex_h
#define regex_h

#include "mn_string.h"
#include "slre.h"
#include <stdint.h>

namespace mono {
    
    /**
     * This class is a C++ wrapper around the C library called SLRE
     * (Super Lightweight Regular Expressions)
     *
     * ### Pattern syntax
     *
     * `(?i)`    Must be at the beginning of the regex. Makes match case-insensitive
     * `^`       Match beginning of a buffer
     * `$`       Match end of a buffer
     * `()`      Grouping and substring capturing
     * `\s`      Match whitespace
     * `\S`      Match non-whitespace
     * `\d`      Match decimal digit
     * `\n`      Match new line character
     * `\r`      Match line feed character
     * `\f`      Match form feed character
     * `\v`      Match vertical tab character
     * `\t`      Match horizontal tab character
     * `\b`      Match backspace character
     * `+`       Match one or more times (greedy)
     * `+?`      Match one or more times (non-greedy)
     * `*`       Match zero or more times (greedy)
     * `*?`      Match zero or more times (non-greedy)
     * `?`       Match zero or once (non-greedy)
     * `x|y`     Match x or y (alternation operator)
     * `\meta`   Match one of the meta character: ^$().[]*+?|\
     * `\xHH`    Match byte with hex value 0xHH, e.g. \x4a
     * `[...]`   Match any character from set. Ranges like [a-z] are supported
     * `[^...]`  Match any character but ones from set
     *
     *
     * @url https://github.com/cesanta/slre
     */
    class Regex
    {
        String pattern;
        
        uint32_t currentMatchNum;
    public:
        
        /**
         * Regex Match capture object holding the first match capture
         * @see Regex
         */
//        struct Capture
//        {
//            bool Match;             /**< Is there a match at all */
//            uint32_t MatchIndex ;   /**< The capture group string index */
//            String Value;           /**< The capture group string content */
//            Regex *Context;         /**< A pointer to the owning Regex object */
//        };
        
        typedef slre_cap Capture;
        
        Regex();
        
        /**
         * Create a regular expression object from a pattern string
         * 
         */
        Regex(String pattern);
        
        /**
         * Test if a string matches the regex pattern
         * @return `true` on match, `false` otherwise
         */
        bool IsMatch(String matchStr);
        
        
        /**
         * Get a the first capture group match from a string
         *
         * The Regex class does not allocate any capure objects, so you must 
         * supply all needed objects for captures.
         *
         * @code
         * Regex::Capure caps[3];
         * Regex reg("(..) (..) (..)");
         * bool success = reg.Match("test my regex", caps, 3);
         * @endcode
         *
         * @param matchStr The string to match against the regex pattern
         * @param captureArray A pointer to a array of Capture objects
         * @param capArraySize The size of the provided capture array
         * @return `true` on match, `false` otherwise
         */
        bool Match(String matchStr, Capture *captureArray, uint32_t capArraySize);
        
        /**
         * Return the string value from a match capture object
         * 
         */
        String Value(Capture &cap);
        
        
        /**
         * Get the next match capture group from the match string
         * 
         * The next match is written to the provided @ref CaptureGroup object, 
         * in-line.
         *
         * @param match A reference to the previous match group object
         * @param matchStr The string to match against the regex pattern
         * @return `true` if a new match group was captured, `false` otherwise
         */
        //bool NextMatch(String match, StrCapture *captureArray, ,);
    };
    
}

#endif /* regex_h */
