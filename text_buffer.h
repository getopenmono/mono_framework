//
//  text_buffer.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 12/07/15.
//
//

#ifndef __displaySimTest__text_buffer__
#define __displaySimTest__text_buffer__

#include <string.h>

namespace mono {
    
    
    /**
     * A fixed size buffer that holds text. You append text to the buffer and
     * the buffer stores the text until it is full. When full the buffer starts
     * to discard the oldest text.
     *
     * To allow the class to allocate its buffer on the stack, this class is
     * templated.
     *
     * This class is utilized by the @ref ConsoleView class, for the console
     * text buffer. This buffer holds the text that is visible in the console.
     *
     * @brief Fixed sized buffer to hold text
     */
    template <int lineLength, int lineCount>
    class TextBuffer
    {
    protected:
        char buffer[lineCount][lineLength];
        int writePtr;
        int linePtr;
        bool looped;
        
    public:
        
        TextBuffer()
        {
            looped = false;
            linePtr = 0;
        }
        
        void insertToCurrentLine(int index, char symbol)
        {
            if (index >= lineLength)
                return;
            
            buffer[linePtr][index] = symbol;
        }
        
        
        void incrementLine()
        {
            linePtr++;
            
            if (linePtr >= lineCount)
            {
                linePtr = 0;
                looped = true;
            }
        }
        
        //Getters
        
        char* getLine(int lineNo)
        {
            if (lineNo < lineCount)
                return buffer[lineNo];
            else
                return NULL;
        }
        
        char* rawData()
        {
            return buffer;
        }
        
        int LinePosition()
        {
            return linePtr;
        }
        
        int OldestLinePosition()
        {
            if (looped)
                return LinePosition()+1;
            else
                return 0;
        }
        
        int LineCount()
        {
            return lineCount;
        }
        
        int LineLength()
        {
            return lineLength;
        }
    };
    
}

#endif /* defined(__displaySimTest__text_buffer__) */
