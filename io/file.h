// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef file_h
#define file_h

#include <stdio.h>

#include "../mn_string.h"

namespace mono { namespace io {
    
    class File
    {
    protected:
        // no public contructor
        File();
        
        static bool writeString(String text, FILE *file);
        
    public:
        
        /**
         * @brief Check if a file exists
         *
         * Determine if a file exists at a given path.
         *
         * @param path The file's path (relative or absolute)
         * @returns `true` if file exists, `false`otherwise
         */
        static bool exists(String path);
        
        /**
         * @brief Return a file's size in bytes
         *
         * This is a short hand method, that uses stdlib routines
         * to get a file's size. You provide the path to the file.
         *
         * If the size cannot be determined (file does not exists), this 
         * method will return 0.
         *
         * @param path The relative or absolute path to the file
         * @returns The file size in bytes or 0 on error (or empty file)
         */
        static size_t size(String path);
        
        /**
         * @brief Read and return the first line in a text file
         *
         * Read the first line of text in the file, defined by the given
         * path. **It is very important the file is a text file!** This
         * method will (by default) search for the first occurence of a
         * newline character. All bytes before the newline is returned. (This
         * means the newline character itself is not part of the returned.)
         *
         * **MEMORY USAGE**: Be aware that the returned string is contained
         * in memory. Be causious not to exhaust all RAM, by loading in a lot
         * of text. Limit the maximum memory used by setting the optional
         * `maxLenght` argument.
         *
         * If the file does not exist, an empty string is returned.
         *
         * @param path The path to the text file
         * @param maxLength OPTIONAL: Set to a positive integer, to load only that many bytes.
         * @param endLineDelimiter OPTIONAL: You can provide a custom stop delimiter, if you so not want \n.
         * @returns A string with the first line of text
         */
        static mono::String readFirstLine(String path, int maxLength = -1,char endLineDelimiter = '\n');
        
        /**
         * @brief Write a string to a file (detructive)
         *
         * You provide a text string, that is written to a provided file path.
         * Any exists file at the path gets overwritten. The complete content of
         * the string is written.
         *
         * @param text The string data to write to a file
         * @param path The file's path
         * @returns `true` upon success, `false`otherwise
         */
        static bool writeString(String text, String path);
        

        /**
         * @brief Append a text string to the end of a file
         * 
         * Write a text string to a file (appending), without overwriting any
         * text in the file. Only characters present in the  string are written
         * to the file. This method does not automatically insert newlines.
         * If you want to append a line of text see @ref appendLine
         *
         * If the file does not exist, it is created and the text is written to
         * the beginning of the file.
         *
         * @param text The text string to append to the file
         * @param path The path to the file that is appended
         * @return `true` on success. `false` otherwise.
         */
        static bool appendString(String text, String path);

        /**
         * @brief Append a line of text to the end of a file
         * 
         * Append a text line (string and a newline character), to the end of
         * the defined file. By default the newline sequence is `\\n`, but you
         * can overwrite this behavior using the third optional argument.
         *
         * This method ensure that a subsequent string written to the file,
         * will begin on a new line. If the provided text string already ends 
         * with a newline sequence, the outline will be 2 lines skipped.
         *
         * If the destination file does not exist, it is created.
         *
         * @param text The text string to write to the file
         * @param path The file destination path
         * @param lineDelimiter Optional: Define a custom new line sequence
         * @return `true` on success, `false` otherwise
         */
        static bool appendLine(String text, String path, const char *lineDelimiter = "\n");
    };
    
} }

#endif /* file_h */
